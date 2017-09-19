#include <avr/sleep.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <RealTimeClockDS1307.h>
#include <TimeLib.h>
#include <TouchScreen.h>
#include <MyBase64.h>
#include <MySD.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <WiFiEspServer.h>
#include "PowerControl.h"
#include "Devices.h"
#include "CString.h"
#include "Common.h"
#include "debug.h"
#include "MainMenu.h"
#include "WifiManager.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif




CWifiManager WifiManager(&Serial1);
CMainMenu dlgMainMenu(TFTLCD, &WifiManager);




uint32_t nPowerDownTrigger = 0;
const uint32_t nInactivityTime = (uint32_t)1000 * 60 * 2;
//const uint32_t nInactivityTime = (uint32_t)5000;

typedef enum{eReset, eHttpReq, eAuto, eManual} enumWakeupType;
volatile enumWakeupType eWakeup = eReset;
volatile bool bWakeupDone = true;

void wakeupCommon()
{
  PowerManager.detachInterrupts();
  resetESP();
  Serial.begin(115200);
  bWakeupDone = false;
}

void wakeupHTTPISR()
{
  wakeupCommon();
  eWakeup = eHttpReq;

  TFTLCD.turnOn();
  resetESP();
  if (!WifiManager.begin())
    debug.logRuntimeError(F("WirelessAlarm.cpp"), __LINE__);
}

void wakeupManualISR()
{
  wakeupCommon();
  eWakeup = eManual;

  TFTLCD.turnOn();
  resetESP();
  if (!WifiManager.begin())
    debug.logRuntimeError(F("WirelessAlarm.cpp"), __LINE__);
}

void wakeupAutoISR()
{
  wakeupCommon();
  eWakeup = eAuto;
}

void setup()
{
  Serial.begin(115200);

  if (!TFTLCD.begin())
    debug.logRuntimeError(F("WirelessAlarm.cpp"), __LINE__);
  else
  {
    TFTLCD.setRelayPin(26);
    PowerManager.setPin21ISR(wakeupAutoISR);
    PowerManager.setPin20ISR(wakeupManualISR);
    PowerManager.setPin19ISR(wakeupHTTPISR, RISING);
    PowerManager.addDevice(&TFTLCD);
  }
  if (!WifiManager.begin())
    debug.logRuntimeError(F("WirelessAlarm.cpp"), __LINE__);
  
  dlgMainMenu.setIPAddr(WiFi.localIP());
  dlgMainMenu.begin();
  dlgMainMenu.show();
  doLowBatteryVoltage();

  nPowerDownTrigger = millis() + nInactivityTime;

  Esp8266.setResetPin(24);
  Arduino.setResetPin(22);
}

void displayWakeMode()
{
  if (eWakeup == eAuto)
    debug.log(F("********** AUTO WAKEUP **********"));
  else if (eWakeup == eHttpReq)
    debug.log(F("********** HTTP REQUEST **********"));
  else if (eWakeup == eManual)
    debug.log(F("********** MANUAL WAKEUP **********"));
  else
    debug.log(F("********** RESET **********"));
}

void doLowBatteryVoltage()
{
  float fVoltage = analogRead(A8) * (5.0 / 1023.0);
  if (fVoltage <= 4.5)
    WifiManager.doLowBatteryWarning(fVoltage);
  debug.log(F("Battery: "), false);
  debug.log(fVoltage, false);
  debug.log(F("V..."));
}

void doWakeUp()
{
  Serial.begin(115200);
  displayWakeMode();
  doLowBatteryVoltage();
  WifiManager.checkAlarm(analogRead(A9));
  bWakeupDone = true;
  nPowerDownTrigger = millis() + nInactivityTime;
}

void loop()
{
  if (!bWakeupDone && (eWakeup != eReset))
    doWakeUp();

  if (dlgMainMenu.pollTouch())
  {
    nPowerDownTrigger = millis() + nInactivityTime;
    //debug.log(F("1111111111"));
  }  
  if (WifiManager.processRequest())
  {
    nPowerDownTrigger = millis() + nInactivityTime;
    //debug.log(F("2222222222"));
  }
  if (millis() >= nPowerDownTrigger)
  {
    //debug.log(F("3333333333"));
    PowerManager.powerDown();
  }
}
