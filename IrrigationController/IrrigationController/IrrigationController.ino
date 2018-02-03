#include <RealTimeClockDS1307.h>
#include <TimeLib.h>
#include <Wire.h>
#include <MyBase64.h>
#include <MySD.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <WiFiEspServer.h>
#include "SerialManager.h"
#include "WifiManager.h"
#include "Program.h"
#include "DateTime.h"
#include "Common.h"
#include "Debug.h"
#include "CString.h"
#include "Timer.h"




#define serialESP8266 Serial1
#define serialHC05 Serial2

CWifiManager WifiManager(&serialESP8266);
CSerialManager SerialManager(WifiManager, program);




//*************************************************************************************************
// Do all the SD card reader initialisation
//*************************************************************************************************
bool initSD()
{
  bool bResult = false;

  debug.logEventOpen('*', F("Initializing SD card..."));
  if (!SD.begin(10, 11, 12, 13)) 
    debug.logEventClose('*', F("failed"));
  else
  {
    debug.logEventClose('*', F("done"));
    bResult = true;
  }
  return bResult;
}

//******************************************************************************************
//* Runs at weekly intervals
//******************************************************************************************
void OnWeeklyTimer()
{
  debug.logEventOpen('*', F("WEEKLY TIMER"));
  debug.log(F("Battery voltage = "), false);
  debug.log(WifiManager.getRTCBatteryVoltage());
  WifiManager.checkLowRTCBatteryVoltage();
  WifiManager.synchClock();
  debug.logEventClose('*', F(""));
}

//**************************************************************************************************
//* Runs at 10 minute intervals (minimum amount of time that an alarm can be triggered on a station
//**************************************************************************************************
void OnAlarmsTimer()
{
  static CTime timeLastAlarm(rtc);
  CTime timeNow(rtc);
  
  rtc.readClock();
  CBuff<512> buffMsg;
  CBuff<32> buffSubject;
  CString strMessage(buffMsg), strSubject(buffSubject);
  debug.logEventOpen('*', F("ALARMS TIMER"), true);
  debug.log(F("Time last alarm: "), false);
  debug.log(timeLastAlarm.toString());
  if (program.checkAlarms(WifiManager, timeNow - timeLastAlarm, strSubject, strMessage))
  {
    timeLastAlarm = timeNow;
  }
  debug.log(F("Time this alarm: "), false);
  debug.log(timeNow .toString(false));
  debug.log(F("Details: "));
  debug.logEventClose('*', strMessage);
}

//******************************************************************************************
//* Runs at 1 minute intervals
//******************************************************************************************
uint8_t g_nNewDayFlag = 0;

void On1MinTimer()
{
  rtc.readClock();
  if ((rtc.getHours() == 0) && (g_nNewDayFlag == 0))
    g_nNewDayFlag = 1;
  else if ((rtc.getHours() > 0) && (g_nNewDayFlag == 2))
    g_nNewDayFlag = 0;
  
  debug.logEventOpen('*', F("1 MINUTE TIMER"), true);

  // Then iterate through all the programs, for all stations and for today's date, and we need to run any of them based on the current time.
  program.run(SerialManager);
  debug.logEventClose('*', F(""));
  
  // A new day has started so we need to read the program.txt and get the irrigations times for the new date so read in the program for each station for the new date.
  if (g_nNewDayFlag == 1)
  {
    #ifdef LOG
      if (!debug.startLogFile())
        debug.logRuntimeError(F("IrrigationController.ino"), __LINE__);
    #endif
    g_nNewDayFlag = 2;

    CTime time(rtc);
    debug.logEventOpen('*', F("NEW DAY"), true);
    debug.log(F("Time: "), false);
    debug.log(time.toString(false));
    debug.logEventClose('*', F("Reading irrigation settings from SD card!"));
    if (!program.read())
      debug.logRuntimeError(F("IrrigationController.ino"), __LINE__);
    
  }
}

//*************************************************************************************************
// Do all the setup tasks.
//*************************************************************************************************
void setup() 
{
  Serial.begin(115200);
  while (!Serial);

  serialHC05.begin(38400);
  serialESP8266.begin(115200);

  // Wait for serial port to connect - needed for native USB port only
  delay(10);
  
  if (initSD() && WifiManager.begin())
  {
    #ifdef LOG
      if (!debug.startLogFile())
        debug.logRuntimeError(F("IrrigationController.ino"), __LINE__);
    #endif
    program.begin(SerialManager, WifiManager.getUDPServer());
    if (program.read())
    {
      timers.add(100, MINUTE, On1MinTimer);
      timers.add(101, WEEKLY, OnWeeklyTimer);
      #ifndef AUTO_SOLENOID
        timers.add(102, FIVE_MINUTES, OnAlarmsTimer);
      #endif
      debug.log(F("Setup() complete..."));
    }
  }
  else
  {
    debug.logRuntimeError(F("IrrigationController.cpp"), __LINE__);
    resetArd();
  }
}
 
void loop()
{
  timers.poll();
    
  if (serialHC05.available() > 0)
    SerialManager.processData(&serialHC05);

  // Listen for incoming clients
  WifiManager.processRequest();
}




