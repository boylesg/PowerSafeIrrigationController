#include <Arduino.h>
#include <MySD.h>
#include <RealTimeClockDS1307.h>
#include <WiFiEsp.h>
#include <TimeLib.h>
#include <SMTPClient.h>
#include <WiFiEspNTPClient.h>
#include "SerialManager.h"
#include "WifiManager.h"
#include "Common.h"
#include "Debug.h"
#include "Program.h"




extern CSerialManager SerialManager;

CWifiManager::CWifiManager(): m_HTTPServer(0), m_fMinRTCBatteryVoltage(2.5)
{
	m_pSerial = NULL;
  m_nSendDelay = 250;
}

CWifiManager::CWifiManager(Stream* pSerial): m_HTTPServer(80), m_strHTTPClockFilename(F("clock.htm")), m_strHTTPAlarmsFilename(F("walarms.htm")), m_strHTTPProgramFilename(F("program.htm")),
													 m_strHTTPEmailFilename(F("email.htm")),m_strFaviconFileName(F("Icon.png")), m_strHTTPManualFilename(F("manual.htm")), m_strTXTWifiFilename(F("wifi.txt")), 
													 m_strTXTEmailFilename(F("email.txt")), m_strTXTTimezoneFilename(F("timezone.txt")), m_fMinRTCBatteryVoltage(2.5), m_strNetworkName(m_buffNetworkName), 
													 m_strNetworkKey(m_buffNetworkKey), m_strFromEmail(m_buffFromEmail), m_strToEmail(m_buffToEmail), m_strMailServer(m_buffMailServer), 
													 m_strUsername(m_buffUsername), m_strPassword(m_buffPassword), m_strControllerID(m_buffControllerID), m_strIPAddr(m_buffIPAddr)
{
	m_pSerial = pSerial;
	assert(m_pSerial);
	m_nBytesRead = 0;
	m_nTimezone = 0;
	m_bAdjustDaylightSaving = false;
	m_strFromEmail = F("GregsIrrigationController@gmail.com");
  m_nSendDelay = 200;
}

CWifiManager::~CWifiManager()
{
}

void CWifiManager::synchClock()
{
  if (rtc.isStopped())
    rtc.start();

  Serial.println(F("================================="));
  Serial.println(F("SYNCHRONISING SYSTEM CLOCK\n"));
  WifiNTPClient NTPClient;
  NTPClient.begin();
  if (NTPClient.getUTCTime())
  {
    CDateTime datetime(NTPClient.getDOM(), NTPClient.getMonth(), NTPClient.getYear(), NTPClient.getHour(), NTPClient.getMinute(), NTPClient.getSecond());

    datetime.addHours(m_nTimezone);
    if (m_bAdjustDaylightSaving)
      datetime.adjustDaylightSavings();
    debug.log(datetime.toString(), false);
    debug.log(F("..."), false);
  
    rtc.setDayOfWeek(datetime.getDOW() - 1);
    rtc.setDate(datetime.getDOM());
    rtc.setMonth(datetime.getMonth());
    rtc.setYear(datetime.getYear());
    rtc.setHours(datetime.getHours());
    rtc.setMinutes(datetime.getMinutes());
    rtc.setSeconds(datetime.getSeconds());
    rtc.setClock();

    debug.log(F("done!"));
    NTPClient.stop();
  }
  Serial.println(F("================================="));
}

float CWifiManager::getRTCBatteryVoltage()
{
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float fVoltage = analogRead(A0) * (5.0 / 1024.0) * 1.8;
  
  debug.log(F("RTC battery voltage: "), false);
  debug.log(fromReal(fVoltage, 1), false);
  debug.log(F(", "), false);
  debug.log(fromUint(analogRead(A0), 10), true);
  
  return fVoltage;
}

#ifndef __SAM3X8E__
  void CWifiManager::sendFileErrorResponse(WiFiEspClient& WifiClient, const __FlashStringHelper* strFileName)
  {
    char strBuff[33];
    CString str(strBuff, 33);
  
    str = strFileName;
    sendFileErrorResponse(WifiClient, str);
  }
#endif

void CWifiManager::writeToClient(WiFiEspClient& WifiClient, const char* strData, const int nLength)
{
  int nWritten = 0;
  
  while (nWritten < nLength)
  {
    nWritten += WifiClient.write(strData + nWritten, nLength - nWritten);
  }
}

void CWifiManager::writeToClient(WiFiEspClient& WifiClient, const __FlashStringHelper* strData, const int nLength)
{
  char *strTemp = new char[nLength];
  if (strTemp)
  {
    memset((void*)strData, 0, nLength);
    strcpy_P(strTemp, (PGM_P)strData);
    writeToClient(WifiClient, strTemp, nLength);
    delete strTemp;
  }
}

void CWifiManager::sendFileErrorResponse(WiFiEspClient& WifiClient, const char* strFileName)
{
  CBuff<128> Buff;
  CString str(Buff);

  str = F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\n\r\n<!DOCTYPE HTML>\r\n");
  writeToClient(WifiClient, str, str.length());

  str = F("<html>\r\n<h1>Can't open '");
  str += strFileName;
  str += F("' on SD card...</h1>\r\n<br>\r\n</html>\r\n");
  writeToClient(WifiClient, str, str.length());
}

bool CWifiManager::sendFile(WiFiEspClient& WifiClient, const char* strFileName)
{
  bool bResult = false;
  CBuff<256> Buff;
  CTextFile file;

  if (SD.exists(strFileName))
  {
    File file = SD.open(strFileName, FILE_READ);

    bResult = file.available() > 0;
    
    while (file.available() > 0)
    {
      m_nBytesRead = file.readBytes(Buff.m_strBuff, Buff.m_nBuffSize);

      writeToClient(WifiClient, Buff.m_strBuff, Buff.m_nBuffSize);
      delay(m_nSendDelay / 4);
    }
    file.close();
  }
  else
    sendFileErrorResponse(WifiClient, strFileName);

  return bResult;
}

bool CWifiManager::sendManualContent(WiFiEspClient& WifiClient, uint8_t nStation, const uint16_t nRuntime, const uint8_t nStatus)
{
  bool bResult = false;
  CBuff<64> Buff64One, Buff64Two;
  CBuff<16> Buff16One, Buff16Two;
  CBuff<256> BuffLarge;
  CString strLine(BuffLarge), strStation(Buff16One), strStationChecked(Buff64One), strTemp(Buff64Two), strFilename(Buff16Two);
  uint16_t nLineNum = 0;
 
  strFilename = m_strHTTPManualFilename;
  if (SD.exists(strFilename))
  {
    CTextFile file;

    if (file.open(strFilename, FILE_READ))
    {
      bResult = true;
      debug.log(F("Sending HTTP response - manual.htm..."));
      sendHTTPHeader(WifiClient);

      strStation = F("value=\"");
      strStation += fromUint(nStation, 10);
      strStation += F("\"");
      strStationChecked = strStation;
      strStationChecked += F(" checked=\"checked\"");

      while (!file.eof())
      {
        bResult = true;
        if (file.readLine(strLine, true))
          nLineNum++;

//debug.log(nLineNum, false);           
//debug.log(F(" => "), false);
//debug.log(strLine);

        if (nLineNum == 7)
          strLine.replace(F("####"), m_strControllerID);
        else if (((nLineNum >= 14) && (nLineNum <= 20)) || ((nLineNum >= 28) && (nLineNum <= 30)))
        {
          strLine.replace(strStation, strStationChecked);
          strLine.replace(F("~1"), program.getStationDesc(1));
          strLine.replace(F("~2"), program.getStationDesc(2));
          strLine.replace(F("~3"), program.getStationDesc(3));
          strLine.replace(F("~4"), program.getStationDesc(4));
          strLine.replace(F("~5"), program.getStationDesc(5));
          strLine.replace(F("~6"), program.getStationDesc(6));
          strLine.replace(F("~7"), program.getStationDesc(7));
          strLine.replace(F("~8"), program.getStationDesc(8));
        }
        else if ((nLineNum == 22) && (nRuntime > 0))
        {
          strTemp = F("value=\"");
          strTemp += fromUint(nRuntime, 10);
          strTemp += F("\"");
          strLine.replace(F("value=\"10\""), strTemp);
        }
        else if (nLineNum == 26)
        {
          strTemp = F("Station ");
          strTemp += fromUint(nStation, 10);
          
          if (nStatus == STATION_OFF)
            strTemp += F(" is tuned off...");
          else if (nStatus == STATION_ON)
            strTemp += F(" is tuned on...");
          else if (nStatus == STATION_LISTED)
            strTemp += F(" has been added to the run queue...");
          else if (nStatus == STATION_DELISTED)
            strTemp += F(" has been removed from the run queue...");
          else if (nStatus == STATION_ALREADY_ON)
            strTemp += F(" is already turned on...");
          else if (nStatus == STATION_ALREADY_OFF)
            strTemp += F(" is already turned off...");
          else if (nStatus == STATION_ALREADY_LISTED)
            strTemp += F(" has already been added to the run queue...");
          else if (nStatus == STATION_ALREADY_DELISTED)
            strTemp += F(" has already been removed from the run queue...");
          else
            strTemp.empty();
          if (strTemp.length() > 0)
            strLine.replace(F("All stations are currently off..."), strTemp);
        }
        writeToClient(WifiClient, strLine, strLine.length());
        delay(m_nSendDelay);
      }
      file.close();
     }
  }
  else
    sendFileErrorResponse(WifiClient, m_strHTTPManualFilename);

  return bResult;
}

bool CWifiManager::processAlarmFormData(CString& strFormData, CString& strMsg)
{
  // EXAMPLE
  // -------
  // Upload=Upload alarms to irrigation controller&S1=false#false#0#0#0&S2=false#false#0#0#0&S3=false#false#0#0#0&S4=false#false#0#0#0&S5=false#false#0#0#0&S6=false#false#0#0#0&S7=false#false#0#0#0&S8=false#false#0#0#0

  CBuff<3> buffNum, buffStation;
  CBuff<8> buff;
  CBuff<24> buffAlarmDet;
  CString strNum(buffNum), strAlarmDet(buffAlarmDet), strStation(buffStation), strVal(buff);
  CProgramStation* pStation = NULL;
  int16_t nPos1 = 0, nPos2 = 0, nStationNum = 1;
  bool bResult = false;

  for (uint8_t nI = 1; nI <= MAX_STATIONS; nI++)
  {
    pStation = program.getStationDetails(nI);
    pStation->m_bMoistureProbe = false;
    pStation->m_bProbeAuto = false;
    pStation->m_nAllowedDryTimeMinutes = 0;
    pStation->m_nProbeThreshold = 0;
    pStation->m_nAutoRunMinutes = 0;
  }
  while (!strFormData.isEmpty())
  {
    pStation = program.getStationDetails(nStationNum);
    strStation = F("S");
    strStation += fromUint(nStationNum, 10);
    nPos1 = strFormData.indexOf(strStation) + 3;
    nPos2 = strFormData.indexOf("&", nPos1 + 1);
    if (nPos2 == -1)
      nPos2 = strFormData.length();

    // false#false#0#0#0
    strAlarmDet = strFormData.substring(nPos1, nPos2 - 1);

    // S2=false#false#0#0#0&S3=false#false#0#0#0&S4=false#false#0#0#0&S5=false#false#0#0#0&S6=false#false#0#0#0&S7=false#false#0#0#0&S8=false#false#0#0#0
    strFormData = strFormData.substring(nPos2 + 1, strFormData.length());
    
//debug.dump(F("strAlarmDet"), strAlarmDet);
//debug.log(F("\n"));    

    // false#false#0#0#0
    nPos1 = strAlarmDet.indexOf(F("#"));
    strVal = strAlarmDet.substring(0, nPos1 - 1);
    pStation->m_bMoistureProbe = strVal == F("true");
    strAlarmDet = strAlarmDet.substring(nPos1 + 1, strAlarmDet.length());
//debug.dump(F("m_bMoistureProbe"), strVal);    
//debug.dump(F("strAlarmDet"), strAlarmDet);    

    // false#0#0#0
    nPos1 = strAlarmDet.indexOf(F("#"));  
    strVal = strAlarmDet.substring(0, nPos1 - 1);
    pStation->m_bProbeAuto = strVal == F("true");
    strAlarmDet = strAlarmDet.substring(nPos1 + 1, strAlarmDet.length());
//debug.dump(F("m_bProbeAuto"), strVal);    
//debug.dump(F("strAlarmDet"), strAlarmDet);    

    // 0#0#0
    nPos1 = strAlarmDet.indexOf(F("#"));
    strVal = strAlarmDet.substring(0, nPos1 - 1);
    pStation->m_nAllowedDryTimeMinutes = strVal.toUint();
    strAlarmDet = strAlarmDet.substring(nPos1 + 1, strAlarmDet.length());
//debug.dump(F("m_nAllowedDryTimeMinutes"), strVal);    
//debug.dump(F("strAlarmDet"), strAlarmDet);    
    
    // 0#0
    nPos1 = strAlarmDet.indexOf(F("#"));
    strVal = strAlarmDet.substring(0, nPos1 - 1);
    pStation->m_nProbeThreshold = strVal.toUint();
    strAlarmDet = strAlarmDet.substring(nPos1 + 1, strAlarmDet.length());
//debug.dump(F("m_nProbeThreshold"), strVal);    
//debug.dump(F("strAlarmDet"), strAlarmDet);    

    // 0
    strVal = strAlarmDet;
    pStation->m_nAutoRunMinutes = strVal.toUint();
    strAlarmDet.empty();
//debug.dump(F("strVal"), strVal);    
//debug.dump(F("strAlarmDet"), pStation->m_nRuntime);
//debug.dump(F("strAlarmDet"), strAlarmDet);
//debug.log(F("----------------------"));

    nStationNum++;
  }
  bResult = program.saveAlarmsData(strMsg);
   
  return bResult;
}

bool CWifiManager::processEmailFormData(CString& strFormData, CString& strMsg)
{
  // EXAMPLE
  // -------
  // Email=gregplants@bigpond.com&IP=101.182.128.114&Server=mail.bigpond.com&Username=gregplants@bigpond.com&Password=pulsar10&Upload=Upload+alarms+to+irrigation+controller
  bool bResult = true;
  int16_t nPos1 = 0, nPos2 = 0;
  CTextFile file;
  CBuff<16> Buff16;
  CString strFilename(Buff16);

  nPos1 = strFormData.indexOf(F("Email"));
  nPos1 = strFormData.indexOf(F("="), nPos1) + 1;
  nPos2 = strFormData.indexOf(F("&"), nPos1);
  m_strToEmail = strFormData.substring(nPos1, nPos2 - 1);

  nPos1 = strFormData.indexOf(F("Server"));
  nPos1 = strFormData.indexOf(F("="), nPos1) + 1;
  nPos2 = strFormData.indexOf(F("&"), nPos1);
  m_strMailServer = strFormData.substring(nPos1, nPos2 - 1);

  nPos1 = strFormData.indexOf(F("Username"));
  nPos1 = strFormData.indexOf(F("="), nPos1) + 1;
  nPos2 = strFormData.indexOf(F("&"), nPos1);
  m_strUsername = strFormData.substring(nPos1, nPos2 - 1);

  nPos1 = strFormData.indexOf(F("Password"));
  nPos1 = strFormData.indexOf(F("="), nPos1) + 1;
  nPos2 = strFormData.indexOf(F("&"), nPos1);
  m_strPassword = strFormData.substring(nPos1, nPos2 - 1);

  strFilename = m_strTXTEmailFilename;
  if (SD.exists(strFilename))
  {
    if (file.open(strFilename, REWRITE))
    {
      file.writeLine(m_strToEmail);
      file.writeLine(m_strMailServer);
      file.writeLine(m_strUsername);
      file.writeLine(m_strPassword);
    }
    else
    {
      debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
      strMsg = F("Could not open '");
      strMsg += strFilename;
      strMsg += F("'...");
    }
    file.close();
  }
  else
  {
    debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
    strMsg = F("Could not open '");
    strMsg += strFilename;
    strMsg += F("'...");
  }
  //file.dump(m_strTXTEmailFilename);

  return bResult;
}

bool CWifiManager::sendAlarmContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg)
{
  bool bResult = false;
  uint8_t nLineNum = 0, nStationNum = 1;
  CBuff<16> Buff16;
  CBuff<256> BuffLarge;
  CString strLine(BuffLarge), strFilename(Buff16);

  strFilename = m_strHTTPAlarmsFilename;
  if (SD.exists(strFilename))
  {
    CTextFile file;   
    if (file.open(strFilename, FILE_READ))
    {
      bResult = true;
      debug.log(F("Sending HTTP response - walarms.htm..."));
      sendHTTPHeader(WifiClient);

      while (!file.eof())
      {
        bResult = true;
        if (file.readLine(strLine, true))
          nLineNum++;
         
//debug.log(nLineNum, false);           
//debug.log(F(" => "), false);
//debug.log(strLine);

        if (nLineNum == 37)
          strLine.replace(F("####"), m_strControllerID);
        else if ((nLineNum >= 8) && (nLineNum <= 15))
        {
/*
debug.dump(F("nStationNum"), nStationNum);
debug.dump(F("inst"), program.getAlarmInstalled(nStationNum));
debug.dump(F("auto"), program.getAlarmAuto(nStationNum));
debug.dump(F("desc"), program.getStationDesc(nStationNum));
debug.dump(F("dry"), program.getAlarmDryTime(nStationNum));
debug.dump(F("thresh"), program.getAlarmThreshold(nStationNum));
debug.dump(F("ontime"), program.getAlarmOnTime(nStationNum));
*/
          strLine.replace(F("inst"), program.getAlarmInstalled(nStationNum) ? "true" : "false");
          strLine.replace(F("auto"), program.getAlarmAuto(nStationNum) ? "true" : "false");
          strLine.replace(F("desc"), program.getStationDesc(nStationNum));
          strLine.replace(F("dry"), fromUint(program.getAlarmDryTime(nStationNum), 10));
          strLine.replace(F("thresh"), fromUint(program.getAlarmThreshold(nStationNum), 10));
          strLine.replace(F("ontime"), fromUint(program.getAlarmOnTime(nStationNum), 10));
          nStationNum++;
        }
        else if ((nLineNum == 54) && bFormResponse && strMsg && (strLine.indexOf(F("Alarm details have not been uploaded yet")) >= 0))
        {
          strLine.replace(F("Alarm details have not been uploaded yet"), strMsg);
        }
        writeToClient(WifiClient, strLine.c_str(), strLine.length());
        delay(m_nSendDelay);
      }   
      file.close();
    }
    else
      sendFileErrorResponse(WifiClient, strFilename);
  }
  else
    sendFileErrorResponse(WifiClient, strFilename);

  return bResult;
}

bool CWifiManager::sendEmailContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg)
{

  bool bResult = false;
  CBuff<16> SmallBuff;
  CBuff<40> MediumBuff;
  CBuff<256> LargeBuff;
  CString strLine(LargeBuff), strTemp(MediumBuff), strFilename(SmallBuff);
  uint8_t nLineNum = 0;
  CProgramStation* pStation = NULL;

  strFilename = m_strHTTPEmailFilename;
  if (SD.exists(strFilename))
  {
    CTextFile file(strFilename, FILE_READ);

    if (file)
    {
      bResult = true;
      debug.log(F("Sending HTTP response - walarms.htm..."));
      sendHTTPHeader(WifiClient);

      while (!file.eof())
      {
        bResult = true;
        if (file.readLine(strLine, true))
          nLineNum++;
          
//debug.log(nLineNum, false);           
//debug.log(F(" => "), false);
//debug.log(strLine);

        if (nLineNum == 7)
        {
          strLine.replace(F("####"), m_strControllerID);
        }
        else if (bFormResponse && strMsg && (nLineNum == 20))
          strLine.replace(F("Email settings have not been uploaded yet"), strMsg);
        else if (nLineNum == 13)
        {
          strTemp = F("value=\"");
          strTemp += m_strToEmail;
          strTemp += F("\"");
          strLine.replace(F("value=\"\""), strTemp);
        }
        else if (nLineNum == 14)
        {
          strTemp = F("value=\"");
          strTemp += m_strMailServer;
          strTemp += F("\"");
          strLine.replace(F("value=\"\""), strTemp);
        }
        else if (nLineNum == 15)
        {
          strTemp = F("value=\"");
          strTemp += m_strUsername;
          strTemp += F("\"");
          strLine.replace(F("value=\"\""), strTemp);
        }
        else if (nLineNum == 16)
        {
          strTemp = F("value=\"");
          strTemp += m_strPassword;
          strTemp += F("\"");
          strLine.replace(F("value=\"\""), strTemp);
        }
        writeToClient(WifiClient, strLine.c_str(), strLine.length());
        delay(m_nSendDelay);
      }
    }
    else
      sendFileErrorResponse(WifiClient, strFilename);
  }
  else
    sendFileErrorResponse(WifiClient, strFilename);

  return bResult;
}

bool CWifiManager::checkLowRTCBatteryVoltage()
{
  float fVoltage = getRTCBatteryVoltage();
  if (fVoltage <= m_fMinRTCBatteryVoltage)
  {
    char strBuffSubject[65], strBuffMessage[129];
    CString strSubject(strBuffSubject, 65), strMessage(strBuffMessage, 129);

	  strSubject = F("RTC battery voltage alarm triggered");
    strMessage = F("The Real Time Clock's battery voltage has fallen to ");
    strMessage += fromReal(fVoltage, 1);
    strMessage += F("V - please change the battery ASAP.");
    sendEmail(strSubject, strMessage);
  }
}

bool CWifiManager::sendSystemClockContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg)
{
  CBuff<16> SmallBuff1, SmallBuff2;
  CBuff<256> LargeBuff1, LargeBuff2;
  char strBuff2[9], strBuff3[161];
  CString strLine(LargeBuff1),
         strMaxValue(SmallBuff1),
         strTemp(LargeBuff2), 
         strFilename(SmallBuff2);
  uint16_t nPos = 0, nMax = 0, nLineNum = 0;
  float fVoltage = getRTCBatteryVoltage();
  bool bResult = false;

  strFilename = m_strHTTPClockFilename;
  if (SD.exists(strFilename))
  {
    CTextFile file(strFilename, FILE_READ);
    if (file)
    {
      debug.log(F("Sending HTTP response - clock.htm..."));
      sendHTTPHeader(WifiClient);
      
      bResult = true;
      while (!file.eof())
      {
        bResult = true;
        if (file.readLine(strLine, true))
          nLineNum++;

//debug.log(nLineNum, false);           
//debug.log(F(" => "), false);
//debug.log(strLine);

        if (nLineNum == 9)
          strLine.replace(F("####"), m_strControllerID);
        else if (nLineNum == 17)
        {
          nPos = strLine.indexOf(F("max"));
          strMaxValue = strLine.substring(nPos + 5, nPos + 8);
          nMax = strMaxValue.toInt();
          nPos = round((fVoltage / 3.0) * nMax);  
          strLine.replace(F("VOLTAGE_SLIDER"), fromUint(nPos, 10));
        }
        else if (nLineNum == 21)
        {
          if (fVoltage > m_fMinRTCBatteryVoltage)
          {
            strTemp = F("Battery is OK (");
            strTemp += fromReal(fVoltage, 1);
            strTemp += F("V)");
          }
          else
          {
            strTemp = F("Battery is needs to be replaced (");
            strTemp += fromReal(fVoltage, 1);
            strTemp += F("V)");
          }
          strLine.replace(F("VOLTAGE_TEXT"), strTemp);
        }
        else if (nLineNum == 25)
        {
          strLine.replace(F("XXXX"), m_strControllerID);
        }
        else if (nLineNum == 29)
        {
          rtc.readClock();
          uint8_t nMins = rtc.getMinutes(), nSecs = rtc.getSeconds() + 8;

          if (nSecs > 60)
          {
            nSecs %= 60;
            nMins++;
          }
          char strBuffHr[3] = {0,0,0}, strBuffMin[3] = {0,0,0}, strBuffSec[3] = {0,0,0}, strTempSec[5] = {0,0,0,0,0}, strBuffDateTime[65] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
          CString strHrs(strBuffHr, 3), strMins(strBuffMin, 3), strSecs(strBuffSec, 3), strTemp(strTempSec, 5), strDateTime(strBuffDateTime, 65);
          strHrs.fromUint(rtc.getHours());
          strHrs.padLeft('0', 2);

          strMins.fromUint(nMins);
          strMins.padLeft('0', 2);

          strSecs.fromUint(nSecs);
          strSecs.padLeft('0', 2);

          strDateTime = getDayName(rtc.getDayOfWeek() + 1, false);
          strDateTime += F(", ");
          strTemp.fromUint(rtc.getDate());
          strDateTime += strTemp;
          strDateTime += F("/");
          strTemp.fromUint(rtc.getMonth());
          strDateTime += strTemp;
          strDateTime += F("/");
          strTemp.fromUint(rtc.getYear());
          strDateTime += strTemp;

          strDateTime += F("    ");
          strDateTime += strHrs;
          strDateTime += F(":");
          strDateTime += strMins;
          strDateTime += F(":");
          strDateTime += strSecs;
          strLine.replace(F("SYSTEM_TIME"), strDateTime);
        }
        else if (bFormResponse && strMsg && (strLine.indexOf(F("Time not uploaded yet...")) >= 0))
        {
          strLine.replace(F("Time not uploaded yet..."), strMsg);
        }
        writeToClient(WifiClient, strLine.c_str(), strLine.length());
        delay(m_nSendDelay);
      }
    }
    else
      sendFileErrorResponse(WifiClient, strFilename);
  }
  else
    sendFileErrorResponse(WifiClient, strFilename);

  return bResult;
}

bool CWifiManager::sendIrrigProgContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg)
{
  bool bResult = false;
  CBuff<16> SmallBuff;
  CBuff<257> LargeBuff1, LargeBuff2;
  CString strFilename(SmallBuff), strLine(LargeBuff1), strTemp(LargeBuff2);
  uint16_t nLineNum = 0;
  CProgramStation* pStationDetails = NULL;

  strFilename = m_strHTTPProgramFilename;
  if (SD.exists(strFilename))
  {
    CTextFile file(strFilename, FILE_READ);
    if (file)
    {
      debug.log(F("Sending HTTP response - program.htm..."));
      sendHTTPHeader(WifiClient);
      
      while (!file.eof())
      {
        if (file.readLine(strLine, true))
          nLineNum++;
//debug.log(nLineNum, false);           
//debug.log(F(" => "), false);
//debug.log(strLine);

        if (nLineNum == 72)
        {
          strLine.replace(F("####"), m_strControllerID);

        }
        else if ((nLineNum >= 11) && (nLineNum <= 18))
        {
          pStationDetails = program.getStationDetails(nLineNum - 10);

          if (pStationDetails)
          {
            strLine.replace(F("desc"), pStationDetails->m_strDescription);        
            strLine.replace(F("radfreq"), fromUint(pStationDetails->m_nRadioFreq, 10));
            strLine.replace(F("selfreq"), fromUint(pStationDetails->m_nSelectFreq, 10));
            strLine.replace(F("startdate"), pStationDetails->m_dateSuspendStart.toString(false));
            strLine.replace(F("enddate"), pStationDetails->m_dateSuspendEnd.toString(false));
            strLine.replace(F("starttime"), pStationDetails->m_timeStart.toString(false));
            strLine.replace(F("endtime"), pStationDetails->m_timeEnd.toString(false));
            strLine.replace(F("runtime"), fromUint(pStationDetails->m_nRuntime, 10));
            strLine.replace(F("runfreq"), fromUint(pStationDetails->m_nRunFreq, 10));
          }
        }
        else if (bFormResponse && strMsg && (nLineNum == 116))
        { 
          strLine.replace(F("Program not uploaded yet"), strMsg);
        }
        writeToClient(WifiClient, strLine.c_str(), strLine.length());
        delay(m_nSendDelay);
      }
      bResult = nLineNum > 0;
    }
    else
      sendFileErrorResponse(WifiClient, strFilename);
  }
  else
    sendFileErrorResponse(WifiClient, strFilename);

  return bResult;
}

bool CWifiManager::isHexDigit(const char cCh)
{
  return ((cCh >= '0') && (cCh <= '9')) || ((cCh >= 'a') && (cCh <= 'f')) || ((cCh >= 'A') && (cCh <= 'F'));
}

bool CWifiManager::processManualFormData(CString& strFormData, uint8_t& nStation, uint16_t& nRuntime)
{
  // EXAMPLE
  // -------
  // RadStat=1&Runtime=10&Upload=Upload+to+irrigation+controller
  //  or
  // RadStat=1&CheckboxOff=off&Upload=Upload+to+irrigation+controller
  char strBuff[9];
  CString strNum(strBuff, 9);
  int nPos = 0;

  if ((nPos = strFormData.indexOf(F("RadStat="))) > -1)
  {
    strNum = strFormData.substring(nPos + 8, strFormData.indexOf(F("&"), nPos) - 1);
    nStation = strNum.toInt();
  }
  if ((nPos = strFormData.indexOf(F("Runtime="))) > -1)
  {
    strNum = strFormData.substring(nPos + 8, strFormData.indexOf(F("&"), nPos) - 1);
    nRuntime = strNum.toInt();
  }
  else if ((nPos = strFormData.indexOf(F("CheckboxOff="))) > -1)
  {
    strNum = strFormData.substring(nPos + 12, strFormData.indexOf(F("&"), nPos) - 1);
    if (strNum == F("off"))
      nRuntime = 0;
  }
}

bool CWifiManager::processClockFormData(CString& strFormData, CString& strMsg)
{
  // EXAMPLE
  // -------
  // NumberDay=9&NumberMonth=9&NumberYear=2016&NumberHour=19&NumberMinute=37&NumberSecond=37&ButtonUpload=Upload+time+to+irrigation+controller&SelectTimezone=10&CheckboxDaylightSavings=on&HiddenDOW=5
  CBuff<9> buff9;
  CString strNum(buff9);
  CBuff<16> buff16;
  CString strFilename(buff16);
  uint8_t nPos = 0, nDay = 0, nMonth = 0, nHour = 0, nMinute = 0, nSecond = 0, nDOW = 0;
  uint16_t nYear = 0;
  bool bResult = false;
  
  nPos = strFormData.indexOf(F("TextID"));
  m_strControllerID = strFormData.substring(nPos + 7, strFormData.indexOf(F("&"), nPos) - 1);
  
  nPos = strFormData.indexOf(F("NumberDay"));
  strNum = strFormData.substring(nPos + 10, strFormData.indexOf(F("&"), nPos) - 1);
  nDay = strNum.toInt();

  nPos = strFormData.indexOf(F("NumberMonth"), nPos);
  strNum = strFormData.substring(nPos + 12, strFormData.indexOf(F("&"), nPos) - 1);
  nMonth = strNum.toInt();

  nPos = strFormData.indexOf(F("NumberYear"), nPos);
  strNum = strFormData.substring(nPos + 11, strFormData.indexOf(F("&"), nPos) - 1);
  nYear = strNum.toInt();
  
  nPos = strFormData.indexOf(F("NumberHour"), nPos);
  strNum = strFormData.substring(nPos + 11, strFormData.indexOf(F("&"), nPos) - 1);
  nHour = strNum.toInt();
  
  nPos = strFormData.indexOf(F("NumberMinute"), nPos);
  strNum = strFormData.substring(nPos + 13, strFormData.indexOf(F("&"), nPos) - 1);
  nMinute = strNum.toInt();
  
  nPos = strFormData.indexOf(F("NumberSecond"), nPos);
  strNum = strFormData.substring(nPos + 13, strFormData.indexOf(F("&"), nPos) - 1);
  nSecond = strNum.toInt();
  if (nSecond >= 60)
  {
    nSecond %= 60;
    nMinute++;
    if (nMinute >= 60)
    {
      nMinute &= 60;
      nHour++;
      if (nHour >= 24)
        nHour %= 24;
    }
  }
  nPos = strFormData.indexOf(F("HiddenDOW"), nPos);
  strNum = strFormData.substring(nPos + 10, nPos + 12);
  nDOW = strNum.toInt();

  rtc.setDayOfWeek(nDOW);
  rtc.setDate(nDay);
  rtc.setMonth(nMonth - 1);
  rtc.setYear(nYear);
  rtc.setHours(nHour);
  rtc.setMinutes(nMinute);
  rtc.setSeconds(nSecond + 1);
  rtc.setClock();

  nPos = strFormData.indexOf(F("SelectTimezone"));
  nPos = strFormData.indexOf(F("="), nPos) + 1;
  strNum = strFormData.substring(nPos, strFormData.indexOf(F("&"), nPos) - 1);
  m_nTimezone = strNum.toInt();

  nPos = strFormData.indexOf(F("CheckboxDaylightSavings"));
  nPos = strFormData.indexOf(F("="), nPos) + 1;
  strNum = strFormData.substring(nPos, strFormData.indexOf(F("&"), nPos) - 1);
  m_bAdjustDaylightSaving = F("on");

  CTextFile fileClock;
  strFilename = m_strTXTTimezoneFilename;
  
  if (SD.exists(strFilename))
  {
    if (fileClock.open(strFilename, REWRITE))
    {
      fileClock.writeLine(m_strControllerID);
      fileClock.writeLine(m_nTimezone);
      if (m_bAdjustDaylightSaving)
        fileClock.writeLine(F("yes"));
      else
        fileClock.writeLine(F("no"));
      fileClock.close();
      bResult = true;
      strMsg = F("Upload successful!");
    }
    else
    {
      debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
      strMsg = F("Could not open '");
      strMsg += strFilename;
      strMsg += F("'...");
    }
  }
  else
  {
    debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
    strMsg = F("Could not find '");
    strMsg += strFilename;
    strMsg += F("'...");
  }
  return bResult;
}

bool CWifiManager::processProgramFormData(CString& strFormData, CString& strMsg)
{
  // EXAMPLE
  // -------
  // strFormData = Station1=Front@@@@@@@@@@@@@@@@@@@#1/5#1/9#2#1#00:00#00:00#1#2&Station2=Side@@@@@@@@@@@@@@@@@@@@#2/5#2/9#3#1#01:00#23:00#2#3&Station3=Back@@@@@@@@@@@@@@@@@@@@#3/5#3/9#3#2#02:00#22:00#3#4&Station4=Pond@@@@@@@@@@@@@@@@@@@@#4/5#4/9#3#3#03:00#21:00#4#6&Station5=5555@@@@@@@@@@@@@@@@@@@@###1#1#00:00#00:00#0#2&Station6=6666@@@@@@@@@@@@@@@@@@@@###1#1#00:00#00:00#0#2&Station7=7777@@@@@@@@@@@@@@@@@@@@###1#1#00:00#00:00#0#2&Station8=8888@@@@@@@@@@@@@@@@@@@@###1#1#00:00#00:00#0#2 
  CBuff<24> Buff24;
  CBuff<24> Buff64;
  CBuff<24> Buff16;
  CBuff<8> Buff8One, Buff8Two, Buff8Three, Buff8Four;
  CString strDesc(Buff24), strStationDetails(Buff64), strTemp(Buff16), strSuspendStartDate(Buff8One), strSuspendEndDate(Buff8Two), strStartTime(Buff8Three), strEndTime(Buff8Four);
  int16_t nPos1 = 0, nPos2 = 0, nStationNum = 1;
  uint8_t nRadioFreq = 0, nSelectFreq = 0, nNumDays = 0, nRunFreq = 0, nRunMins = 0, nNumHours = 0;
  enum eRadioFreq {eMonthly = 1, eWeekly = 2, eDaily = 3};
  enum eRunFreq{e2Hrs = 0, e3Hrs = 1, e4Hrs = 2, e6Hrs = 3, e8Hrs = 4, e12Hrs = 5, e24Hrs = 6};
  CTextFile fileIrrigProg, fileIrrigProgPageData;
  bool bResult = false;

  debug.dump(F("strFormData"), strFormData.c_str());
  
  if (program.openIrrigProgramFile(fileIrrigProg, strMsg) && program.openIrrigProgramPageDataFile(fileIrrigProgPageData, strMsg))
  {
    while (strFormData.length() > 0)
    {
      bResult = true;
      
      nPos1 = strFormData.indexOf(F("="));
      nPos2 = strFormData.indexOf(F("&"));
      if ((nPos1 >= 0) && (nPos2 >= 0))
      {
        strStationDetails = strFormData.substring(nPos1 + 1, nPos2 - 1);
        strFormData = strFormData.substring(nPos2 + 1, strFormData.length());
      }
      else if (nPos1 >= 0)
      {
        strStationDetails = strFormData.substring(nPos1 + 1, strFormData.length());
        strFormData = "";
      }
      else
        debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
      nPos1 = nPos2 = 0;

      // E.G. VEGES#1/4#1/9#3#1#20:30#00:00#10#24
      nPos1 = strStationDetails.indexOf(F("#"));
      strDesc = strStationDetails.substring(0, nPos1 - 1);
      strStationDetails = strStationDetails.substring(nPos1 + 1, strStationDetails.length());

      // E.G. 1/4#1/9#3#1#20:30#00:00#10#24
      nPos1 = strStationDetails.indexOf(F("#"));
      strSuspendStartDate = strStationDetails.substring(0, nPos1 - 1);
      strStationDetails = strStationDetails.substring(nPos1 + 1, strStationDetails.length());

      // E.G. 1/9#3#1#20:30#00:00#10#24
      nPos1 = strStationDetails.indexOf(F("#"));
      strSuspendEndDate = strStationDetails.substring(0, nPos1 - 1);
      strStationDetails = strStationDetails.substring(nPos1 + 1, strStationDetails.length());

      // E.G. 3#1#20:30#00:00#10#24
      nPos1 = strStationDetails.indexOf(F("#"));
      strTemp = strStationDetails.substring(0, nPos1 - 1);
      nRadioFreq = strTemp.toUint(10);
      strStationDetails = strStationDetails.substring(nPos1 + 1, strStationDetails.length());

      // E.G. 1#20:30#00:00#10#24
      nPos1 = strStationDetails.indexOf(F("#"));
      strTemp = strStationDetails.substring(0, nPos1 - 1);
      nSelectFreq = strTemp.toUint(10);
      strStationDetails = strStationDetails.substring(nPos1 + 1, strStationDetails.length());

      // E.G. 20:30#00:00#10#24
      nPos1 = strStationDetails.indexOf(F("#"));
      strStartTime = strStationDetails.substring(0, nPos1 - 1);
      strStationDetails = strStationDetails.substring(nPos1 + 1, strStationDetails.length());

      // E.G. 00:00#10#24
      nPos1 = strStationDetails.indexOf(F("#"));
      strEndTime = strStationDetails.substring(0, nPos1 - 1);
      strStationDetails = strStationDetails.substring(nPos1 + 1, strStationDetails.length());

      // E.G. 10#24
      nPos1 = strStationDetails.indexOf(F("#"));
      strTemp = strStationDetails.substring(0, nPos1 - 1);
      nRunMins = strTemp.toUint(10);
      strStationDetails = strStationDetails.substring(nPos1 + 1, strStationDetails.length());
      
      // E.G. 24
      nPos1 = strStationDetails.indexOf(F("#"));
      if (nPos1 > -1)
      {
        strTemp = strStationDetails.substring(0, nPos1 - 1);
        strStationDetails = strStationDetails.substring(nPos1 + 1, strStationDetails.length());
      }
      else
        strTemp = strStationDetails;
      nRunFreq = strTemp.toUint(10);
  
      nNumDays = program.getDayInterval(nRadioFreq, nSelectFreq);
      nNumHours = program.getHourInterval(nRunFreq);
/*
debug.dump(F("nStationNum"), nStationNum);
debug.dump(F("strDesc"), strDesc);
debug.dump(F("nRadioFreq"), nRadioFreq);
debug.dump(F("nSelectFreq"), nSelectFreq);
debug.dump(F("strStartTime"), strStartTime);
debug.dump(F("strEndTime"), strEndTime);
debug.dump(F("nRunMins"), nRunMins);
debug.dump(F("nRunFreq"), nRunFreq);
debug.dump(F("strSuspendStartDate"), strSuspendStartDate);
debug.dump(F("strSuspendEndDate"), strSuspendEndDate);
debug.dump(F("nNumDays"), nNumDays);
debug.dump(F("nNumHours"), nNumHours);
debug.log(F("-----------------------------------"));
*/
      program.saveIrrigProg(nStationNum, fileIrrigProg, nNumDays, strStartTime, strEndTime, nNumHours, nRunMins, strSuspendStartDate, strSuspendEndDate, strDesc);
      program.saveIrrigProgPageData(nStationNum, fileIrrigProgPageData, strDesc, nRadioFreq, nSelectFreq, nRunMins, nRunFreq, strStartTime, strEndTime, strSuspendStartDate, strSuspendEndDate);
      nStationNum++;
    }
    fileIrrigProg.close();
	  fileIrrigProgPageData.close();
    program.read();
  }
  else
    debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
}

void CWifiManager::sendHTTPHeader(WiFiEspClient& WifiClient)
{
  CBuff<128> Buff;
  CString strHeader(Buff);
  strHeader = F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\n\r\n");
  writeToClient(WifiClient, strHeader, strHeader.length());
}

bool CWifiManager::sendIcon(WiFiEspClient& WifiClient)
{
  CBuff<16> SmallBuff;
  CString strFilename(SmallBuff);
  sendHTTPHeader(WifiClient);

  return sendFile(WifiClient, strFilename = m_strFaviconFileName);
}

void CWifiManager::displayError(const __FlashStringHelper* strFilename)
{
  debug.log(F("Could not open "));
  debug.log(strFilename);
  debug.log(F("!"));
}

void CWifiManager::processRequest()
{
  CBuff<5> Buff5;
  CBuff<512> LargeBuff;
  CString strReq(LargeBuff), strHexCode(Buff5);
  boolean bNewLine = false;
  char cCh = 0;
  const uint16_t nWait = 500;
  uint32_t nTimeout = 0;

  WiFiEspClient WifiClient = getClient();
  if (WifiClient)
  {
    while (WifiClient.connected())
    {
      if (WifiClient.available())
      {
        cCh = WifiClient.read();
        nTimeout = millis() + nWait;
        if (cCh == '%')
        {
          do {} while (!WifiClient.available());
          cCh = WifiClient.read();
          strHexCode += cCh;
          do {} while (!WifiClient.available());
          cCh = WifiClient.read();
          strHexCode += cCh;
          strReq += char(strHexCode.toUint(16));
          strHexCode.empty();
        }
        else if (cCh == '+')
        {
          strReq += ' ';
        }
        else
        {
          strReq += cCh;
        }

        // If you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        // A http request ends with a blank line
        if ((cCh == '\n') && bNewLine)
        {
          if (strReq.indexOf("GET") > -1)
            processHTTPRequest(WifiClient, strReq);
          break;
        }
        // You're starting a new line
        if (cCh == '\n')
        {
          bNewLine = true;
        }
        // You've gotten a character on the current line
        else if (cCh != '\r')
        {
          bNewLine = false;
        }
      }
      if (millis() > nTimeout)
      {
        processHTTPRequest(WifiClient, strReq);
/*
        IPAddress IP;
        CBuff<24> Buff24;
        CString str(Buff24);
        
        debug.log(F("***********************************************************************"));
        debug.log(F("Unknown request\n"));
        IP = WifiClient.remoteIP();
        str = IP;
        debug.dump(F("\tRemote IP address"), str);
        debug.dump(F("\tRemote port"), WifiClient.remotePort());
        debug.dump(F("\tContents"), strReq);
        debug.log(F("***********************************************************************"));
        WifiClient.flush();
*/
        break;
      }
    }
    // give the web browser time to receive the data
    delay(100);

    // Close the connection:
    WifiClient.stop();
    debug.log(F("Client disconnected"));
  }
  else if (m_UDPServer.parsePacket() > 0)
  {
    processUDPRequest(&m_UDPServer);
  }
}

void CWifiManager::processUDPRequest(WiFiEspUDP *pUDPServer)
{
  CBuff<24> Buff24;
  CString strWord(Buff24);
  IPAddress IP;
  
  debug.log(F("***********************************************************************"));
  debug.log(F("New UDP request\n"));
  IP = pUDPServer->remoteIP();
  strWord = IP;
  debug.dump(F("\tRemote IP address"), strWord);
  debug.dump(F("\tRemote port"), pUDPServer->remotePort());
  debug.log(F("***********************************************************************"));

  SerialManager.processData(&m_UDPServer);
}

void CWifiManager::processHTTPRequest(WiFiEspClient &WifiClient, CString &strReq)
{
	CBuff<24> Buff24;
	CString str(Buff24);
  CBuff<128> Buff128, Buff128Msg;
  CString strMsg(Buff128Msg);
	IPAddress IP;

	if (!strReq.isEmpty())
	{

		debug.log(F("***********************************************************************"));
		debug.log(F("New HTTP request\n"));
		IP = WifiClient.remoteIP();
		str = IP;
		debug.dump(F("\tRemote IP address"), str);
		debug.dump(F("\tRemote port"), WifiClient.remotePort());
		debug.dump(F("\tContents"), strReq);
		debug.log(F("***********************************************************************"));

		// EXAMPLES
		// --------
		// GET / HTTP/1.1
		// Host: 10.0.0.79
		// User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0
		// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*//*;q=0.8
		// Accept-Language: en-US,en;q=0.5
		// Accept-Encoding: gzip, deflate
		// Connection: keep-alive
		// Upgrade-Insecure-Requests: 1
		// Cache-Control: max-age=0

		// GET /program.htp HTTP/1.1
		// Host: 10.0.0.79
		// User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0
		// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*//*;q=0.8
		// Accept-Language: en-US,en;q=0.5
		// Accept-Encoding: gzip, deflate
		// Connection: keep-alive
		// Upgrade-Insecure-Requests: 1
		// Cache-Control: max-age=0

		if (((strReq.indexOf(F("GET / HTTP/1.1")) >= 0) || (strReq.indexOf(F("GET /program.htm HTTP/1.1")) >= 0)) && !sendIrrigProgContent(WifiClient, false, NULL))
			displayError(m_strHTTPProgramFilename);
		else if ((strReq.indexOf(F("GET /walarms.htm HTTP/1.1")) >= 0) && !sendAlarmContent(WifiClient, false, NULL))
			displayError(m_strHTTPAlarmsFilename);
		else if ((strReq.indexOf(F("GET /email.htm HTTP/1.1")) >= 0) && !sendEmailContent(WifiClient, false, NULL))
			displayError(m_strHTTPEmailFilename);
		else if ((strReq.indexOf(F("GET /clock.htm HTTP/1.1")) >= 0) && !sendSystemClockContent(WifiClient, false, NULL))
			displayError(m_strHTTPClockFilename);
		else if ((strReq.indexOf(F("GET /manual.htm HTTP/1.1")) >= 0) && !sendManualContent(WifiClient, 0, 0, 0))
			displayError(m_strHTTPManualFilename);
		else if ((strReq.indexOf(F("GET /favicon.ico HTTP/1.1")) >= 0) && !sendIcon(WifiClient))
			displayError(m_strFaviconFileName);
		// EXAMPLE
		// -------
		// GET /program.htm?Station1=1/4#1/9#3#1#00:00#00:00#10#2&Station2=##1#2#00:00#00:00#10#2&Station3=##1#2#00:00#00:00#10#2&Station4=##1#2#00:00#00:00#10#2&Station5=##1#2#00:00#00:00#10#2&Station6=##1#2#00:00#00:00#10#2&
		//                  Station7=##1#2#00:00#00:00#10#2&Station8=##1#2#00:00#00:00#10#2 HTTP/1.1
		// Host: 10.0.0.7
		else if (strReq.indexOf(F("program.htm?")) >= 0)
		{
			strReq = strReq.substring(strReq.indexOf(F("Station1")), strReq.indexOf(F(" HTTP/1.1")) - 1);
			processProgramFormData(strReq, strMsg);

			if (!sendIrrigProgContent(WifiClient, true, strMsg))
				debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
		}
		// EXAMPLE
		// -------
		// HTTP request:
		// GET /clock.htm?NumberDay=23&NumberMonth=7&NumberYear=2016&NumberHour=15&NumberMinute=42&NumberSecond=54&ButtonUpload=Upload+time+to+irrigation+controller&HiddenDOW=2 HTTP/1.1
		// Host: 10.0.0.79
		// User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0
		// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
		// Accept-Language: en-US,en;q=0.5
		// Accept-Encoding: gzip, deflate
		// Referer: http://10.0.0.79/clock.htm
		// Connection: keep-alive
		// Upgrade-Insecure-Requests: 1
		else if (strReq.indexOf(F("clock.htm?")) >= 0)
		{
			strReq = strReq.substring(strReq.indexOf(F("TextID")), strReq.indexOf(F(" HTTP/1.1")) - 1);
			processClockFormData(strReq, strMsg);

			if (!sendSystemClockContent(WifiClient, true, strMsg))
				debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
		}
		// EXAMPLE
		// -------
		// HTTP request:
		// GET /walarms.htm?Probe1=on&DryTime1=2&DryThresh1=400&Probe2=on&DryTime2=2&DryThresh2=400&Probe4=on&DryTime4=2&DryThresh4=400&Probe6=on&DryTime6=2&DryThresh6=400&Probe8=on&DryTime8=2&DryThresh8=10&Upload=Upload+alarms+to+irrigation+controller HTTP/1.1
		// Host: 10.0.0.79
		// Accept-Language: en-us,en;q=0.5
		// Accept-Encoding: *
		// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
		// User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20150101 Firefox/20.0 (Chrome)
		// Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
		// Connection: keep-alive
		else if (strReq.indexOf(F("walarms.htm?")) >= 0)
		{
			strReq = strReq.substring(strReq.indexOf(F("?")) + 1, strReq.indexOf(F(" HTTP/1.1")) - 1);
			if (!processAlarmFormData(strReq, strMsg))
        sendFileErrorResponse(WifiClient, strMsg);
			else if (!sendAlarmContent(WifiClient, true, strMsg))
				debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
		}
		// EXAMPLE
		// -------
		// HTTP request:
		// GET /email.htm?Email=gregplants@bigpond.com&IP=101.182.128.114&Server=mail.bigpond.com&Username=gregplants@bigpond.com&Password=pulsar10&Upload=Upload+alarms+to+irrigation+controller HTTP/1.1
		// Host: 10.0.0.79
		// Accept-Language: en-us,en;q=0.5
		// Accept-Encoding: *
		// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
		// User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20150101 Firefox/20.0 (Chrome)
		// Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7
		// Connection: keep-alive
		else if (strReq.indexOf(F("email.htm?")) >= 0)
		{
			strReq = strReq.substring(strReq.indexOf(F("Email")), strReq.indexOf(F(" HTTP/1.1")) - 1);
			if (!processEmailFormData(strReq, strMsg))
        sendFileErrorResponse(WifiClient, strMsg);
			else if (!sendEmailContent(WifiClient, true, strMsg))
				debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
		}
		// GET /manual.htm?RadStat=1&Runtime=10&Upload=Upload+to+irrigation+controller HTTP/1.1
		// Host: 10.0.0.79
		// Connection: keep-alive
		// Cache-Control: max-age=0
		// Upgrade-Insecure-Requests: 1
		// User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36
		// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
		// Accept-Encoding: gzip, deflate, sdch
		// Accept-Language: en-US,en;q=0.8
		else if (strReq.indexOf(F("manual.htm?")) >= 0)
		{
			strReq = strReq.substring(strReq.indexOf(F("RadStat")), strReq.indexOf(F(" HTTP/1.1")) - 1);
			uint8_t nStation = 0, nStatus = 0;
			uint16_t nRuntime = 0;

			processManualFormData(strReq, nStation, nRuntime);
			if (nRuntime > 0)
				nStatus = program.stationOn(nStation, nRuntime);
			else
				nStatus = program.stationOff(nStation);

			if (!sendManualContent(WifiClient, nStation, nRuntime, nStatus))
				debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
		}
    else if (strReq.indexOf(F("wirelessalarm?")) >= 0)
    {
      // GET / wirelessalarm?ip=10.0.0.7&station=1&description=Back&drytime=10&ontime=2&auto=1& HTTP/1.1
      strReq = strReq.substring(strReq.indexOf(F("ip")), strReq.indexOf(F(" HTTP/1.1")) - 1);
      
      // ip=10.0.0.7&station=1&description=Back&drytime=10&ontime=2&auto=1&
      processWirlessAlarmData(strReq);
    }
	}
}

void CWifiManager::processWirlessAlarmData(CString &strRequest)
{
  CBuff<33> buffPair;
  CBuff<17> buffVal, buffIP, buffDesc;
  CBuff<7> buffNum;
  CString strPair(buffPair), strVal(buffVal), strNum(buffNum), strIP(buffIP), strDesc(buffDesc);
  int8_t nPos1 = 0, nPos2 = 0;
  uint8_t nStation = 0, nDryTime = 0, nOnTime = 0;
  bool bAuto = false;

  // ip=10.0.0.7&station=1&description=Back&drytime=10&ontime=2&auto=1&
  nPos1 = strRequest.indexOf(F("="));
  nPos2 = strRequest.indexOf(F("&"), nPos1 + 1);
  strIP = strRequest.substring(nPos1 + 1, nPos2 - 1);
  strRequest = strRequest.substring(nPos2 + 1, strRequest.length() - 1);
  
  // station=1&description=Back&drytime=10&ontime=2&auto=1&
  nPos1 = strRequest.indexOf(F("="));
  nPos2 = strRequest.indexOf(F("&"), nPos1 + 1);
  strNum = strRequest.substring(nPos1 + 1, nPos2 - 1);
  nStation = strNum.toUint();
  strRequest = strRequest.substring(nPos2 + 1, strRequest.length() - 1);
  
  // description=Back&drytime=10&ontime=2&auto=1&
  nPos1 = strRequest.indexOf(F("="));
  nPos2 = strRequest.indexOf(F("&"), nPos1 + 1);
  strDesc = strRequest.substring(nPos1 + 1, nPos2 - 1);
  strRequest = strRequest.substring(nPos2 + 1, strRequest.length() - 1);
  
  // drytime=10&ontime=2&auto=1&
  nPos1 = strRequest.indexOf(F("="));
  nPos2 = strRequest.indexOf(F("&"), nPos1 + 1);
  strNum = strRequest.substring(nPos1 + 1, nPos2 - 1);
  nDryTime = strNum.toUint();
  strRequest = strRequest.substring(nPos2 + 1, strRequest.length() - 1);
  
  // ontime=2&auto=1&
  nPos1 = strRequest.indexOf(F("="));
  nPos2 = strRequest.indexOf(F("&"), nPos1 + 1);
  strNum = strRequest.substring(nPos1 + 1, nPos2 - 1);
  nOnTime = strNum.toUint();
  strRequest = strRequest.substring(nPos2 + 1, strRequest.length() - 1);
  
  // auto=1&
  nPos1 = strRequest.indexOf(F("="));
  nPos2 = strRequest.indexOf(F("&"), nPos1 + 1);
  strNum = strRequest.substring(nPos1 + 1, nPos2 - 1);
  bAuto = strNum == F("1");






}

WiFiEspClient CWifiManager::getClient()
{
  return m_HTTPServer.available();
}

bool CWifiManager::readTimeZone()
{
  bool bResult = false;
  CBuff<32> SmallBuff1, SmallBuff2;
  CString str(SmallBuff1), strFilename(SmallBuff2);

  strFilename = m_strTXTTimezoneFilename;
  if (SD.exists(strFilename))
  {
    CTextFile fileTimeZone(strFilename, FILE_READ);

    if (fileTimeZone)
    {
      fileTimeZone.readLine(m_strControllerID);
      fileTimeZone.readLine(str);
      m_nTimezone = str.toInt(10);
      fileTimeZone.readLine(str);
      m_bAdjustDaylightSaving = str == F("yes");
      bResult = true;
      fileTimeZone.close();
    }
	else
	{
		debug.log(F("Could not open '"), false);
		debug.log(strFilename, false);
		debug.log(F("'!"));
	}
  }
  else
  {
	  debug.log(F("Could not find '"), false);
	  debug.log(strFilename, false);
	  debug.log(F("'!"));
  }
  return bResult;
}

bool CWifiManager::readEmailSettings()
{
	bool bResult = false;
	CBuff<16> SmallBuff;
	CString strFilename(SmallBuff);

	strFilename = m_strTXTEmailFilename;
	if (SD.exists(strFilename))
	{
		CTextFile fileEmail;

		if (fileEmail.open(strFilename, FILE_READ))
		{
			fileEmail.readLine(m_strToEmail);
			fileEmail.readLine(m_strMailServer);
			fileEmail.readLine(m_strUsername);
			fileEmail.readLine(m_strPassword);
			bResult = true;
			fileEmail.close();
		}
		else
		{
			debug.log(F("Could not open '"), false);
			debug.log(strFilename, false);
			debug.log(F("'!"));
		}
	}
	else
	{
		debug.log(F("Could not find '"), false);
		debug.log(strFilename, false);
		debug.log(F("'!"));
	}
	return bResult;
}

bool CWifiManager::readWifiSSIDKey(CString &strSSID, CString &strKey)
{
  CBuff<16> SmallBuff;
  CString strFilename(SmallBuff);
  bool bResult = false;

  strFilename = m_strTXTWifiFilename;
  if (SD.exists(strFilename))
  {
    CTextFile fileWifi(strFilename, FILE_READ);

    if (fileWifi)
    {
      fileWifi.readLine(strSSID);
      fileWifi.readLine(strKey);
      debug.log(F("From SD card SSID = "), false);
      debug.log(strSSID, false);
      debug.log(F(", KEY = "), false);
      debug.log(strKey, true);
      bResult = (strSSID.length() > 0) && (strKey.length() > 0);
    }
  }
  return bResult;
}

void CWifiManager::updateEmailSettings(const char *m_strToEmail, const char *strMailServer, const char *m_strUsername, const char *m_strPassword, CString &strMsg)
{
	CBuff<16> SmallBuff;
	CString strFilename(SmallBuff);


	if ((strlen(m_strToEmail) > 0) && (strlen(strMailServer) > 0) && (strlen(m_strUsername) > 0) && (strlen(m_strPassword) > 0))
	{
		strFilename = m_strTXTWifiFilename;
		if (SD.exists(strFilename))
		 {
			  CTextFile file;
			  if (file.open(m_strTXTEmailFilename, REWRITE))
			  {
				  file.writeLine(m_strToEmail);
				  file.writeLine(m_strMailServer);
				  file.writeLine(m_strUsername);
				  file.writeLine(m_strPassword);
          file.close();
			  }
			  else
			  {
				  strMsg = F("Could not write to '");
				  strMsg += m_strTXTEmailFilename;
				  strMsg += F("'!");
			  }
		}
		else
		{
			strMsg = F("Could find '");
			strMsg += m_strTXTEmailFilename;
			strMsg += F("'!");
		}
	}
}

void CWifiManager::updateSytemTime(CDateTime &datetime, const bool bAdjustDaylighSavings, const uint8_t nTimezoneAdjust, const char *strID, CString &strMsg)
{
	CBuff<16> SmallBuff1, SmallBuff2;
	CString str(SmallBuff1), strFilename(SmallBuff2);

	rtc.setDayOfWeek(datetime.getDOW());
	rtc.setDate(datetime.getDOM());
	rtc.setMonth(datetime.getMonth());
	rtc.setYear(datetime.getYear());
	rtc.setHours(datetime.getHours());
	rtc.setMinutes(datetime.getMinutes());
	rtc.setSeconds(datetime.getSeconds());
	rtc.setClock();
	
	m_strControllerID = strID;
	m_nTimezone = nTimezoneAdjust;
	m_bAdjustDaylightSaving = bAdjustDaylighSavings;

	strFilename = m_strTXTTimezoneFilename;
	if (SD.exists(strFilename))
	{
		CTextFile file;
                      
		if (file.open(m_strTXTTimezoneFilename, REWRITE))
		{
			file.writeLine(m_strControllerID);
			file.writeLine(m_nTimezone);
			if (m_bAdjustDaylightSaving)
				file.writeLine(F("yes"));
			else
				file.writeLine(F("no"));
			file.close();

			strMsg = F("Upload successful!");
		}
		else
		{
			strMsg = F("Could write to '");
			strMsg += m_strTXTTimezoneFilename;
			strMsg += F("'!");
		}
	}
	else
	{
		strMsg = F("Could find '");
		strMsg += m_strTXTTimezoneFilename;
		strMsg += F("'!");
	}
}

void CWifiManager::updateWifiSSIDKey(const char *strNetworkName, const char *strNetworkKey, CString &strMsg)
{
	CBuff<16> SmallBuff;
	CString strFilename(SmallBuff);

	debug.log(F("Updating Wifi details\n"));

	if ((strlen(strNetworkName) > 0) && (strlen(strNetworkKey) > 0))
	{
		strFilename = m_strTXTWifiFilename;
		if (SD.exists(strFilename))
		{
			CTextFile fileProgram;
			if (fileProgram.open(strFilename, REWRITE))
			{
				fileProgram.writeLine(strNetworkName);
				fileProgram.writeLine(strNetworkKey);
				fileProgram.close();
				strMsg = F("Upload successful!");
				delay(m_nSendDelay);
				resetArd();
			}
			else
			{
				strMsg = F("Could write to '");
				strMsg += m_strTXTWifiFilename;
				strMsg += F("'!");
			}
		}
		else
		{
			strMsg = F("Could find '");
			strMsg += m_strTXTWifiFilename;
			strMsg += F("'!");
		}
	}
	else
		strMsg = F("Error reading details!");
}

bool CWifiManager::sendEmail(CString& strSubject, CString& strMsg)
{
  SMTPClient client;
  bool bResult = false;
  CBuff<33> buff;
  CString strDateTime(buff);

  if (strlen(m_strToEmail) > 0)
  {

    client.setTo(m_strToEmail);
    client.setFrom(m_strFromEmail);
    client.setUsername(m_strUsername);
    client.setPassword(m_strPassword);
    IPAddress ip = WiFi.localIP();
    m_strIPAddr = ip;
    client.setIPAddr(m_strIPAddr);
/*        
    typedef struct  
    { 
      uint8_t Second; 
      uint8_t Minute; 
      uint8_t Hour; 
      uint8_t Wday;   // day of week, sunday is day 1
      uint8_t Day;
      uint8_t Month; 
      uint8_t Year;   // offset from 1970; 
    }
    tmElements_t, TimeElements, *tmElementsPtr_t;
  
    time_t makeTime(tmElements_t &tm);
*/

    TimeElements Time;
    Time.Second = rtc.getSeconds();
    Time.Minute = rtc.getMinutes();
    Time.Hour = rtc.getHours();
    Time.Wday = rtc.getDay();
    Time.Day = rtc.getDate();
    Time.Month = rtc.getMonth();
    Time.Year = (CDate::m_nCentury + rtc.getYear()) - 1970;
    client.setTime(makeTime(Time));
    
    CDate date(rtc);
    CTime time(rtc);

    strDateTime = date.toString(true, true);
    strDateTime += F(" ");
    strDateTime += time.toString();
    client.setDateTime(strDateTime.c_str());
    client.setSubject(strSubject.c_str());
    client.setContent(strMsg.c_str());
    if (client.connect(m_strMailServer))
    {
      bResult = client.sendEmail();
    }
  }
  if (bResult)
  {
    debug.log(F("========================="));
    debug.log(F("EMAIL SENT"));
    debug.log(F("Subject:"), false);
    debug.log(strSubject);
    debug.log(F("Message:"), false);
    debug.log(strMsg);
    debug.log(F("========================="));
  }
  else
  {
    debug.log(F("========================="));
    debug.log(F("EMAIL NOT SENT"));
    client.printErrorMsg();
    debug.log(F("========================="));
  }
  return bResult;
}
 
bool CWifiManager::begin()
{
	bool bResult = false;
	uint8_t nRetries = 0;
	CBuff<16> Buff1, Buff2;
	CBuff<65> BuffSubject;
	CString strTemp(Buff1), strFilename(Buff2), strSubject(BuffSubject);
	uint8_t nStatus = WL_IDLE_STATUS;

	// Initialize ESP module
	WiFi.init(m_pSerial);

	// Check for the presence of the shield
	if ((nStatus = WiFi.status()) == WL_NO_SHIELD)
  {
		debug.log(F("WiFi shield not present!"));
    WiFi.reset();
    resetArd();
  }
	else
	{
		readTimeZone();
		readEmailSettings();

		if (readWifiSSIDKey(m_strNetworkName, m_strNetworkKey))
		{
			// Attempt to connect to WiFi network
			while ((nStatus != WL_CONNECTED) && (nRetries < 10))
			{
				debug.log(F("Attempting to connect to WPA SSID: "), false);
				debug.log(m_strNetworkName, false);
				debug.log(F(", KEY: "), false);
				debug.log(m_strNetworkKey, false);
				debug.log(F("..."), true);

				// Connect to WPA/WPA2 network
				nStatus = WiFi.begin(m_strNetworkName, m_strNetworkKey);
        debug.dump(F("nStatus"), nStatus);
				nRetries++;
			}
			if (nStatus == WL_CONNECTED)
			{
				IPAddress IPAddr = WiFi.localIP();
				debug.log(F("You're connected to the network with IP address: "), false);
				debug.log(IPAddr, true);

        synchClock();

        CBuff<19> buff;
        CString strIP(buff);
        strIP = IPAddr;
        
        strSubject = F("Irrigation controller '");
        strSubject += m_strControllerID;
        strSubject += F("' IP address");
				sendEmail(strSubject, strIP);

				// Start the web server on port 80
        m_HTTPServer.begin();
        if (!m_UDPServer.begin(10002))
          debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);

				bResult = true;
			}
			else
			{
				debug.log(F("Could not log into local wifi network - SSID & KEY are incorrect!"));
				debug.log(F("Please upload your SSID & KEY via bluetooth."));
			}
		}
	}
	return bResult;
}

