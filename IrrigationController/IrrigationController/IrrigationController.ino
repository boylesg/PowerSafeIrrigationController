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
CTime timeLastAlarms;




//*************************************************************************************************
// Do all the SD card reader initialisation
//*************************************************************************************************
bool initSD()
{
  bool bResult = false;

  debug.log(F("Initializing SD card..."), false);
  if (!SD.begin(10, 11, 12, 13)) 
    debug.log(F("failed!"));
  else
  {
    debug.log(F("done!"));
    bResult = true;
  }
  return bResult;
}

//******************************************************************************************
//* Runs at 24 hour intervals
//******************************************************************************************
void OnWeeklyTimer()
{
  debug.log(F("**************************"));
  debug.log(F("WEEKLY TIMER"));  
  debug.log(F("-------------"));
  WifiManager.checkLowRTCBatteryVoltage();
  WifiManager.synchClock();
  debug.log(F("**************************"));
}

//**************************************************************************************************
//* Runs at 10 minute intervals (minimum amount of time that an alarm can be triggered on a station
//**************************************************************************************************
void OnAlarmsTimer()
{
  rtc.readClock();
  CTime time(rtc);
  debug.log(F("********** ALARMS TIMER "), false);  
  debug.log(time.toString(false));  
  debug.log(F(" **********"));  
  debug.dump(F("Time last alarm"), timeLastAlarms.toString());
  timeLastAlarms = program.checkAlarms(WifiManager, timeLastAlarms);
  debug.dump(F("Time last alarm"), timeLastAlarms.toString());
}

//******************************************************************************************
//* Runs at 1 minute intervals
//******************************************************************************************
uint8_t g_nNewDayFlag = 0;

void On1MinTimer()
{
  debug.log(F("********** 1 MINUTE TIMER **********"));  

  rtc.readClock();
  if ((rtc.getHours() == 0) && (g_nNewDayFlag == 0))
    g_nNewDayFlag = 1;
  else if ((rtc.getHours() > 0) && (g_nNewDayFlag == 2))
    g_nNewDayFlag = 0;
  
  // Then iterate through all the programs, for all stations and for today's date, and we need to run any of them based on the current time.
  program.run(SerialManager);

  // A new day has started so we need to read the program.txt and get the irrigations times for the new date so read in the program for each station for the new date.
  if (g_nNewDayFlag == 1)
  {
    g_nNewDayFlag = 2;
    debug.init();
    
    debug.log(F("********** NEW DAY **********"));
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

  if (initSD() && WifiManager.begin() && program.read())
  {
    program.begin(SerialManager, WifiManager.getUDPServer());
    debug.init();
    rtc.readClock();
    timeLastAlarms.set(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
    timers.add(100, MINUTE, On1MinTimer);
    timers.add(101, WEEKLY, OnWeeklyTimer);
    #ifndef AUTO_SOLENOID
      timers.add(102, FIVE_MINUTES, OnAlarmsTimer);
    #endif
    debug.log(F("Setup() complete..."));
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


