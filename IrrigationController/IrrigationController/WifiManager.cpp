#include <Arduino.h>
#include <MySD.h>
#include <MyBase64.h>
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




#define f_MIN_VOLTAGE 2.5




extern CSerialManager SerialManager;

CWifiManager::CWifiManager(): m_HTTPServer(0)
{
	m_pSerial = NULL;
  m_nSessionTimer = -1;
}

CWifiManager::CWifiManager(Stream* pSerial): m_HTTPServer(80) 
{
	m_pSerial = pSerial;
	assert(m_pSerial);
	m_nTimezone = 0;
	m_bAdjustDaylightSaving = false;
  CString strFromEmail(m_buffFromEmail);
	strFromEmail = F("GregsIrrigationController@gmail.com");
  m_nSessionTimer = -1;
}

CWifiManager::~CWifiManager()
{
}

void CWifiManager::synchClock()
{
  if (rtc.isStopped())
    rtc.start();

  WifiNTPClient NTPClient;
  NTPClient.begin();
  if (NTPClient.getUTCTime())
  {
    CDateTime datetime(NTPClient.getDOM(), NTPClient.getMonth(), NTPClient.getYear(), NTPClient.getHour(), NTPClient.getMinute(), NTPClient.getSecond());

    datetime.addHours(m_nTimezone);
    if (m_bAdjustDaylightSaving)
      datetime.adjustDaylightSavings();
    debug.logEventOpen('#', F("SYNCHRONISING SYSTEM CLOCK"));
    debug.log(F("Date: "), false);
    debug.log(datetime.getDate().toString());
    debug.log(F("Time: "), false);
    debug.log(datetime.getTime().toString());
    debug.logEventOpen('#', F(""));
  
    rtc.setDayOfWeek(datetime.getDOW() - 1);
    rtc.setDate(datetime.getDOM());
    rtc.setMonth(datetime.getMonth());
    rtc.setYear(datetime.getYear());
    rtc.setHours(datetime.getHours());
    rtc.setMinutes(datetime.getMinutes());
    rtc.setSeconds(datetime.getSeconds());
    rtc.setClock();

    NTPClient.stop();
  }
}

float CWifiManager::getRTCBatteryVoltage()
{
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float fVoltage = analogRead(A0) * (5.0 / 1024.0) * 1.8;
  
  return fVoltage;
}

#ifndef __SAM3X8E__
  void CWifiManager::sendFileErrorResponse(WiFiEspClient& WifiClient, const __FlashStringHelper* strFileName)
  {
    CBuff<32> buff;
    CString str(buff);
  
    str = strFileName;
    sendFileErrorResponse(WifiClient, str);
  }
#endif

void CWifiManager::writeToClient(WiFiEspClient& WifiClient, const char* strData, const int nLength)
{
  int nWritten = 0;
  
  /*
  while (nWritten < nLength)
  {
    nWritten += WifiClient.write(strData + nWritten, nLength - nWritten);
  }
  */
  WifiClient.write(strData, nLength);
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
  uint32_t nBytesRead = 0;

  if (SD.exists(strFileName))
  {
    File file = SD.open(strFileName, FILE_READ);

    bResult = file.available() > 0;
    
    while (file.available() > 0)
    {
      nBytesRead = file.readBytes(Buff, Buff.capacity());  
      writeToClient(WifiClient, Buff, Buff.capacity());
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
  CBuff<64> BuffTemp;
  CBuff<16> BuffFilename;
  CBuff<256> BuffLine;
  CString strLine(BuffLine), strTemp(BuffTemp), strFilename(BuffFilename), strControllerID(m_buffControllerID);
  uint16_t nLineNum = 0;
 
  strFilename = fstr_HTTP_MANUAL;
  if (SD.exists(strFilename))
  {
    CTextFile file;

    if (file.open(strFilename, FILE_READ))
    {
      bResult = true;
      debug.log(F("Sending HTTP response - "), false);
      debug.log(strFilename, false);
      debug.log(F("..."));
      sendHTTPHeader(WifiClient);

      while (!file.eof())
      {
        bResult = true;
        if (file.readLine(strLine, true))
          nLineNum++;

//debug.log(nLineNum, false);           
//debug.log(F(" => "), false);
//debug.log(strLine);

        if (nLineNum == 8)
          strLine.replace(F("####"), strControllerID);
        else if (((nLineNum >= 16) && (nLineNum <= 23)) || ((nLineNum >= 33) && (nLineNum <= 40)))
        {
          strLine.replace(F("~1"), program.getStationDesc(1));
          strLine.replace(F("~2"), program.getStationDesc(2));
          strLine.replace(F("~3"), program.getStationDesc(3));
          strLine.replace(F("~4"), program.getStationDesc(4));
          strLine.replace(F("~5"), program.getStationDesc(5));
          strLine.replace(F("~6"), program.getStationDesc(6));
          strLine.replace(F("~7"), program.getStationDesc(7));
          strLine.replace(F("~8"), program.getStationDesc(8));
        }
        else if (nLineNum == 42)
        {         
          if (nStation == 0);
            nStation = 1;
          strLine.replace(F("~"), fromUint(nStation, 10));
        }
        else if ((nLineNum == 26) && (nRuntime > 0))
        {
          strTemp = F("value=\"");
          strTemp += fromUint(nRuntime, 10);
          strTemp += F("\"");
          strLine.replace(F("value=\"10\""), strTemp);
        }
        else if (((nLineNum == 31) && (nStation > 0)) && (nStation > 0))
        {
          strTemp = F("Station ");
          strTemp += fromUint(nStation, 10);

          if (nStatus == STATION_OFF)
            strTemp += F(" is turned off...");
          else if (nStatus == STATION_ON)
            strTemp += F(" is turned on...");
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
          {
            int nPos1 = 71, nPos2 = strLine.indexOf(F("</td>"), nPos1);
            strLine.remove(nPos1, nPos2 - nPos1);
            strLine.insert(strTemp, nPos1);
            //strLine.replace(F("All stations are currently off..."), strTemp);
          }
        }
        writeToClient(WifiClient, strLine, strLine.length());
      }
      file.close();
     }
  }
  else
    sendFileErrorResponse(WifiClient, fstr_HTTP_MANUAL);

  return bResult;
}

bool CWifiManager::processAlarmFormData(CString& strFormData, CString& strMsg)
{
  // EXAMPLE
  // -------
  // S1=false#false#0#0#0&S2=false#false#0#0#0&S3=false#false#0#0#0&S4=false#false#0#0#0&S5=false#false#0#0#0&S6=false#false#0#0#0&S7=false#false#0#0#0&S8=false#false#0#0#0

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
  CString strFilename(Buff16), strToEmail(m_buffToEmail), strMailServer(m_buffMailServer), 
          strUsername(m_buffUsername), strPassword(m_buffPassword);

  nPos1 = strFormData.indexOf(F("Email"));
  nPos1 = strFormData.indexOf(F("="), nPos1) + 1;
  nPos2 = strFormData.indexOf(F("&"), nPos1);
  strToEmail = strFormData.substring(nPos1, nPos2 - 1);

  nPos1 = strFormData.indexOf(F("Server"));
  nPos1 = strFormData.indexOf(F("="), nPos1) + 1;
  nPos2 = strFormData.indexOf(F("&"), nPos1);
  strMailServer = strFormData.substring(nPos1, nPos2 - 1);

  nPos1 = strFormData.indexOf(F("Username"));
  nPos1 = strFormData.indexOf(F("="), nPos1) + 1;
  nPos2 = strFormData.indexOf(F("&"), nPos1);
  strUsername = strFormData.substring(nPos1, nPos2 - 1);

  nPos1 = strFormData.indexOf(F("Password"));
  nPos1 = strFormData.indexOf(F("="), nPos1) + 1;
  nPos2 = strFormData.indexOf(F("&"), nPos1);
  strPassword = strFormData.substring(nPos1, nPos2 - 1);

  strFilename = fstr_TXT_EMAIL;
  if (SD.exists(strFilename))
  {
    if (file.open(strFilename, REWRITE))
    {
      file.writeLine(strToEmail);
      file.writeLine(strMailServer);
      file.writeLine(strUsername);
      file.writeLine(strPassword);
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
  //file.dump(fstr_TXT_EMAIL);

  return bResult;
}

bool CWifiManager::sendAlarmContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg)
{
  bool bResult = false;
  uint8_t nLineNum = 0, nStationNum = 1;
  CBuff<16> Buff16;
  CBuff<256> BuffLarge;
  CString strLine(BuffLarge), strFilename(Buff16), strControllerID(m_buffControllerID);

  strFilename = fstr_HTTP_WALARMS;
  if (SD.exists(strFilename))
  {
    CTextFile file;   
    if (file.open(strFilename, FILE_READ))
    {
      bResult = true;
      debug.log(F("Sending HTTP response - "), false);
      debug.log(strFilename, false);
      debug.log(F("..."));
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
          strLine.replace(F("####"), strControllerID);
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

bool CWifiManager::processLoginFormData(CString& strFormData)
{
  CBuff<32> buffPassword, buffEncoded;
  CString strPassword(buffPassword), strEncoded(buffEncoded), strNetworkKey(m_buffNetworkKey);
  int16_t nPos1 = 0, nPos2 = 0;

  // EXAMPLE
  // -------
  // Password=0AC9703D29&Login=Login

  nPos1 = strFormData.indexOf(F("=")) + 1;
  nPos2 = strFormData.indexOf(F("H"), nPos1) - 2;
  strEncoded = strFormData.substring(nPos1, nPos2);
  debug.dump(F("strEncoded"), strEncoded);
  base64_decode((char*)strPassword, (char*)strEncoded, strEncoded.length());
  debug.dump(F("strPassword"), strPassword);
  return true;
  //return strPassword == strNetworkKey;
}

bool CWifiManager::sendLogFileContent(WiFiEspClient& WifiClient)
{
  CTextFile* pLogFile = NULL;
  CBuff<128> buffLine;
  CBuff<16> buffFilename;
  CString strLine(buffLine), strFilename(buffFilename);
  bool bResult = false;

  strFilename = fstr_TXT_LOG_FILE;
  if (debug.readLogFile(pLogFile))
  {
    while (pLogFile->readLine(strLine))
    {
      writeToClient(WifiClient, strLine.c_str(), strLine.length());
      strLine = F("\n");
      writeToClient(WifiClient, strLine.c_str(), strLine.length());
      bResult = true;
    }
    debug.doneLogFile();
  }
  else
  {
    sendFileErrorResponse(WifiClient, strFilename);
  } 
  return bResult;
}

bool CWifiManager::sendLoginContent(WiFiEspClient& WifiClient, const __FlashStringHelper* strMsg)
{
  bool bResult = false;
  CBuff<16> BuffFilename;
  CBuff<256> BuffLine;
  CString strLine(BuffLine), strFilename(BuffFilename), strControllerID(m_buffControllerID);
  int nLineNum = 0;

  strFilename = F("login.htm");
  if (SD.exists(strFilename))
  {
    CTextFile file(strFilename, FILE_READ);

    if (file)
    {
      debug.log(F("Sending HTTP response - "), false);
      debug.log(strFilename, false);
      debug.log(F("..."));
      sendHTTPHeader(WifiClient);
      while (!file.eof())
      {
        if (file.readLine(strLine, true))
        {
          nLineNum++;

          if (nLineNum == 8)
          {
            strLine.replace(F("####"), strControllerID);
          }
          else if ((nLineNum == 14) && (strlen_P((PGM_P)strMsg) > 0))
            strLine.replace(F("Wifi network password/key..."), strMsg);
            
          writeToClient(WifiClient, strLine.c_str(), strLine.length());
        }
      }
      bResult = true;
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
  CString strLine(LargeBuff), strTemp(MediumBuff), strFilename(SmallBuff), strToEmail(m_buffToEmail), 
          strMailServer(m_buffMailServer), strUsername(m_buffUsername), strPassword(m_buffPassword), strControllerID(m_buffControllerID);
  uint8_t nLineNum = 0;
  CProgramStation* pStation = NULL;

  strFilename = fstr_HTTP_EMAIL;
  if (SD.exists(strFilename))
  {
    CTextFile file(strFilename, FILE_READ);

    if (file)
    {
      bResult = true;
      debug.log(F("Sending HTTP response - "), false);
      debug.log(strFilename, false);
      debug.log(F("..."));
      sendHTTPHeader(WifiClient);

      while (!file.eof())
      {
        bResult = true;
        if (file.readLine(strLine, true))
          nLineNum++;
          
//debug.log(nLineNum, false);           
//debug.log(F(" => "), false);
//debug.log(strLine);

        if (nLineNum == 9)
        {
          strLine.replace(F("####"), strControllerID);
        }
        else if (bFormResponse && strMsg && (nLineNum == 29))
          strLine.replace(F("Email settings have not been uploaded yet"), strMsg);
        else if (nLineNum == 18)
        {
          strTemp = F("value=\"");
          strTemp += strToEmail;
          strTemp += F("\"");
          strLine.replace(F("value=\"\""), strTemp);
        }
        else if (nLineNum == 20)
        {
          strTemp = F("value=\"");
          strTemp += strMailServer;
          strTemp += F("\"");
          strLine.replace(F("value=\"\""), strTemp);
        }
        else if (nLineNum == 22)
        {
          strTemp = F("value=\"");
          strTemp += strUsername;
          strTemp += F("\"");
          strLine.replace(F("value=\"\""), strTemp);
        }
        else if (nLineNum == 24)
        {
          strTemp = F("value=\"");
          strTemp += strPassword;
          strTemp += F("\"");
          strLine.replace(F("value=\"\""), strTemp);
        }
        writeToClient(WifiClient, strLine.c_str(), strLine.length());
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
  if (fVoltage <= f_MIN_VOLTAGE)
  {
    CBuff<65> buffSubject;
    CBuff<128> buffMessage;
    CString strSubject(buffSubject), strMessage(buffMessage);

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
         strFilename(SmallBuff2), strControllerID(m_buffControllerID);
  uint16_t nPos = 0, nMax = 0, nLineNum = 0;
  float fVoltage = getRTCBatteryVoltage();
  bool bResult = false;

  strFilename = fstr_HTTP_CLOCK;
  if (SD.exists(strFilename))
  {
    CTextFile file(strFilename, FILE_READ);
    if (file)
    {
      debug.log(F("Sending HTTP response - "), false);
      debug.log(strFilename, false);
      debug.log(F("..."));
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
          strLine.replace(F("####"), strControllerID);
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
          if (fVoltage > f_MIN_VOLTAGE)
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
          strLine.replace(F("XXXX"), strControllerID);
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
          CBuff<3> buffHr, buffMin, buffSec;
          CBuff<4> buffTemp;
          CBuff<64> buffDateTime;
          CString strHrs(buffHr), strMins(buffMin), strSecs(buffSec), strTemp(buffTemp), strDateTime(buffDateTime);
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
  CBuff<256> LargeBuff1, LargeBuff2;
  CString strFilename(SmallBuff), strLine(LargeBuff1), strTemp(LargeBuff2), strControllerID(m_buffControllerID);
  uint16_t nLineNum = 0;
  CProgramStation* pStationDetails = NULL;

  strFilename = fstr_HTTP_PROGRAM;
  if (SD.exists(strFilename))
  {
    CTextFile file(strFilename, FILE_READ);
    if (file)
    {
      debug.log(F("Sending HTTP response - "), false);
      debug.log(strFilename, false);
      debug.log(F("..."));
      sendHTTPHeader(WifiClient);
      
      while (!file.eof())
      {
        if (file.readLine(strLine, true))
          nLineNum++;
//debug.log(nLineNum, false);           
//debug.log(F(" => "), false);
//debug.log(strLine);

        if (nLineNum == 8)
        {
          strLine.replace(F("####"), strControllerID);
        }
        else if ((nLineNum >= 55) && (nLineNum <= 62))
        {
          pStationDetails = program.getStationDetails(nLineNum - 54);

          if (pStationDetails)
          {
            strLine.replace(F("desc"), pStationDetails->getDescription());        
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
        else if (bFormResponse && strMsg && (nLineNum == 48))
        { 
          strLine.replace(F("Settings not uploaded yet..."), strMsg);
        }
        writeToClient(WifiClient, strLine.c_str(), strLine.length());
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
  // RadioStation=1&TextDesc=Station 1&Runtime=10&Upload=Upload to irrigation controller
  //  or
  // RadioStation=1&CheckboxOff=off&Upload=Upload+to+irrigation+controller
  CBuff<8> buff;
  CString strNum(buff);
  int nPos = 0;

  if ((nPos = strFormData.indexOf(F("RadioStation="))) > -1)
  {
    strNum = strFormData.substring(nPos + 13, strFormData.indexOf(F("&"), nPos) - 1);
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
  //debug.log(F("$$$$$$$$$$$$$$$$$$$$"));
  //debug.dump(F("strFormData"), strFormData);
  //debug.dump(F("nRuntime"), nRuntime);
  //debug.log(F("$$$$$$$$$$$$$$$$$$$$"));
}

bool CWifiManager::processClockFormData(CString& strFormData, CString& strMsg)
{
  // EXAMPLE
  // -------
  // TextID=AlphaMedical&NumberDay=31&NumberMonth=1&NumberYear=2018&NumberHour=0&NumberMinute=21&NumberSecond=59&SelectTimezone=10&
  // CheckboxDaylightSavings=on&ButtonUpload=Upload+time+to+irrigation+controller&HiddenDOW=3
  CBuff<9> buff9;
  CString strNum(buff9);
  CBuff<16> buff16;
  CString strFilename(buff16), strControllerID(m_buffControllerID);
  int8_t nPos = 0, nDay = 0, nMonth = 0, nHour = 0, nMinute = 0, nSecond = 0, nDOW = 0;
  uint16_t nYear = 0;
  bool bResult = false;
  
  nPos = strFormData.indexOf(F("TextID"));
  strControllerID = strFormData.substring(nPos + 7, strFormData.indexOf(F("&"), nPos) - 1);
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
  strFilename = fstr_TXT_TIMEZONE;
  
  if (SD.exists(strFilename))
  {
    if (fileClock.open(strFilename, REWRITE))
    {
      fileClock.writeLine(strControllerID);
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
  CBuff<24> BuffDesc;
  CBuff<64> BuffStatDet;
  CBuff<24> BuffTemp;
  CBuff<8> BuffStartDate, BuffEndDate, BuffStartTime, BuffEndTime;
  CString strDesc(BuffDesc), strStationDetails(BuffStatDet), strTemp(BuffTemp), strSuspendStartDate(BuffStartDate), 
          strSuspendEndDate(BuffEndDate), strStartTime(BuffStartTime), strEndTime(BuffEndTime);
  int16_t nPos1 = 0, nPos2 = 0;
  uint8_t nRadioFreq = 0, nSelectFreq = 0, nNumDays = 0, nRunFreq = 0, nRunMins = 0, nNumHours = 0, nStationNum = 1;
  enum eRadioFreq {eMonthly = 1, eWeekly = 2, eDaily = 3};
  enum eRunFreq{e2Hrs = 0, e3Hrs = 1, e4Hrs = 2, e6Hrs = 3, e8Hrs = 4, e12Hrs = 5, e24Hrs = 6};
  CTextFile fileIrrigProg, fileIrrigProgPageData;
  bool bResult = false;

  if (program.openIrrigProgramFile(fileIrrigProg, strMsg) && program.openIrrigProgramPageDataFile(fileIrrigProgPageData, strMsg))
  {
    while (strFormData.length() > 0)
    {
      bResult = true;
      
      nPos1 = strFormData.indexOf(F("="));
      nPos2 = strFormData.indexOf(F("&"));
/*
debug.dump(F("nPos1"), nPos1);
debug.dump(F("nPos2"), nPos2);
*/
      if ((nPos1 >= 0) && (nPos2 >= 0))
      {
        strStationDetails = strFormData.substring(nPos1 + 1, nPos2 - 1);
        strFormData = strFormData.substring(nPos2 + 1, strFormData.length());
/*
debug.dump(F("strStationDetails"), strStationDetails);
debug.dump(F("strFormData"), strFormData);
*/
      }
      else if (nPos1 >= 0)
      {
        strStationDetails = strFormData.substring(strFormData.indexOf(F("=")) + 1, strFormData.length() - 1);
        
        strFormData.empty();
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
    strMsg = F("Irrigation settings successfully uploaded!");
    program.read();
  }
  else
    debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
}

void CWifiManager::sendHTTPHeader(WiFiEspClient& WifiClient)
{
  CBuff<128> Buff;
  CString strHeader(Buff);
  
  strHeader = F("HTTP/1.1 200 OK\r\n");
  strHeader += F("Content-Type: text/html\r\nConnection: close\r\n\r\n");
  writeToClient(WifiClient, strHeader, strHeader.length());
}

bool CWifiManager::sendIcon(WiFiEspClient& WifiClient)
{
  CBuff<16> SmallBuff;
  CString strFilename(SmallBuff);

  strFilename = fstr_FAV_ICON;
  sendHTTPHeader(WifiClient);
  return sendFile(WifiClient, strFilename);
}

void CWifiManager::debugFileError(const __FlashStringHelper* strFilename, const bool bFind)
{
  debug.log(F("Could not open "), false);
  debug.log(strFilename, false);
  debug.log(F("!"));
}

void CWifiManager::processRequest()
{
  CBuff<5> buffHexCode;
  CBuff<512> buffReq;
  uint16_t nReqI = 0;
  boolean bNewLine = false, bSaveReqData = true;
  char cCh = 0;

  WiFiEspClient WifiClient = getClient();
  if (WifiClient)
  {
    while (WifiClient.connected()) 
    {
      if (WifiClient.available()) 
      {
        cCh = WifiClient.read();
        
        // If you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        // A http request ends with a blank line
        if ((cCh == '\n') && bNewLine) 
        {
          CString strReq(buffReq);
          processHTTPRequest(WifiClient, strReq);
          break;
        }
        if (cCh == '\n') 
        {
          bNewLine = true;
          bSaveReqData = false;
        }
        else if (cCh != '\r') 
        {
          bNewLine = false;
        }
        if (bSaveReqData)
        {
          if (cCh == '%')
          {
            do {} while (!WifiClient.available());
            cCh = WifiClient.read();
            buffHexCode.m_strBuffer[0] = cCh;
            do {} while (!WifiClient.available());
            cCh = WifiClient.read();
            buffHexCode.m_strBuffer[1] = cCh;
            buffReq.m_strBuffer[nReqI++] = char(toUint(buffHexCode.m_strBuffer, 16));
          }
          else if (cCh == '+')
          {
            buffReq.m_strBuffer[nReqI++] = ' ';
          }
          else
          {
            buffReq.m_strBuffer[nReqI++] = cCh;
          }
        }
      }
    }
    // Give the web browser time to receive the data
    delay(10);

    // Close the connection
    WifiClient.stop();
    Serial.println("Client disconnected");
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
  strWord.fromIPAddr(IP);
  SerialManager.setRemote(strWord);
  debug.dump(F("\tRemote IP address"), strWord);
  debug.dump(F("\tRemote port"), pUDPServer->remotePort());
  debug.log(F("***********************************************************************"));

  SerialManager.processData(&m_UDPServer);
}

void CWifiManager::processHTTPRequest(WiFiEspClient &WifiClient, CString &strReq)
{
	CBuff<24> Buff;
	CString str(Buff);
  CBuff<128> BuffMsg;
  CString strMsg(BuffMsg);
	IPAddress IP;

	if (!strReq.isEmpty())
	{
		debug.log(F("***********************************************************************"));
		debug.log(F("New HTTP request\n"));
		IP = WifiClient.remoteIP();
		str.fromIPAddr(IP);
		debug.dump(F("\tRemote IP address"), str);
		debug.dump(F("\tRemote port"), WifiClient.remotePort());
		debug.dump(F("\tContents"), strReq);
		debug.log(F("***********************************************************************"));

    if (millis() > m_nSessionTimer)
      m_nSessionTimer = -1;

    if (m_nSessionTimer == -1)
    {
      if (strReq.find(F("?")))
      {
        if (processLoginFormData(strReq))
        {
          m_nSessionTimer = millis() + ((uint32_t)1000 * 60 * 20);
          sendIrrigProgContent(WifiClient, false, NULL);
        }
        else
        {
          sendLoginContent(WifiClient, F("Wifi network password/key is incorrect!"));
        }
      }
      else if ((strReq.indexOf(F("GET /icon.png HTTP/1.1")) >= 0))
      {
        if (!sendIcon(WifiClient))
          debugFileError(fstr_FAV_ICON, true);
      }
      else
      {
        sendLoginContent(WifiClient, F(""));
      }
    }
    else
    {        
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
  			debugFileError(fstr_HTTP_PROGRAM, true);
      else if ((strReq.indexOf(F("GET /log.txt HTTP/1.1")) >= 0) && !sendLogFileContent(WifiClient))
        debugFileError(fstr_TXT_LOG_FILE, true);
      else if ((strReq.indexOf(F("GET /walarms.htm HTTP/1.1")) >= 0) && !sendAlarmContent(WifiClient, false, NULL))
        debugFileError(fstr_HTTP_WALARMS, true);
  		else if ((strReq.indexOf(F("GET /email.htm HTTP/1.1")) >= 0) && !sendEmailContent(WifiClient, false, NULL))
  			debugFileError(fstr_HTTP_EMAIL, true);
  		else if ((strReq.indexOf(F("GET /clock.htm HTTP/1.1")) >= 0) && !sendSystemClockContent(WifiClient, false, NULL))
  			debugFileError(fstr_HTTP_CLOCK, true);
  		else if ((strReq.indexOf(F("GET /manual.htm HTTP/1.1")) >= 0) && !sendManualContent(WifiClient, 0, 0, 0))
  			debugFileError(fstr_HTTP_MANUAL, true);
  		else if ((strReq.indexOf(F("GET /icon.png HTTP/1.1")) >= 0) && !sendIcon(WifiClient))
  			debugFileError(fstr_FAV_ICON, true);
  		// EXAMPLE
  		// -------
  		// GET /program.htm?Station1=1/4#1/9#3#1#00:00#00:00#10#2&Station2=##1#2#00:00#00:00#10#2&Station3=##1#2#00:00#00:00#10#2&Station4=##1#2#00:00#00:00#10#2&Station5=##1#2#00:00#00:00#10#2&Station6=##1#2#00:00#00:00#10#2&
  		//                  Station7=##1#2#00:00#00:00#10#2&Station8=##1#2#00:00#00:00#10#2 HTTP/1.1
  		// Host: 10.0.0.7
  		else if (strReq.indexOf(F("program.htm?")) >= 0)
    	{
        int nPos1 = strReq.indexOf(F("Station")), nPos2 = strReq.indexOf(F(" HTTP")) - 1;
        strReq = strReq.substring(nPos1, nPos2);

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

        int nPos1 = strReq.indexOf(F("TextID")), nPos2 = strReq.indexOf(F(" HTTP")) - 1;
        strReq = strReq.substring(nPos1, nPos2);
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
        int nPos1 = strReq.indexOf(F("S1")) + 1, nPos2 = strReq.indexOf(F(" HTTP")) - 1;
        strReq = strReq.substring(nPos1, nPos2);
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
        int nPos1 = strReq.indexOf(F("Email")), nPos2 = strReq.indexOf(F(" HTTP")) - 1;
        strReq = strReq.substring(nPos1, nPos2);

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
        int nPos1 = strReq.indexOf(F("RadioStation")), nPos2 = strReq.indexOf(F(" HTTP")) - 1;
        strReq = strReq.substring(nPos1, nPos2);
  			uint8_t nStation = 0, nStatus = 0;
  			uint16_t nRuntime = 0;
  
  			processManualFormData(strReq, nStation, nRuntime);
        
  			if (nRuntime > 0)
  				nStatus = program.stationOn(nStation, nRuntime);
  			else
  				nStatus = program.stationOff(nStation);
  
//debug.log(F("$$$$$$$$$$$$$$$$$$$$"));
//debug.dump(F("nStation"), nStation);
//debug.dump(F("nRuntime"), nRuntime);
//debug.log(F("$$$$$$$$$$$$$$$$$$$$"));
  
  			if (!sendManualContent(WifiClient, nStation, nRuntime, nStatus))
  				debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
  		}
  	}
	}
}

WiFiEspClient CWifiManager::getClient()
{
  return m_HTTPServer.available();
}

bool CWifiManager::readTimeZone()
{
  bool bResult = false;
  CBuff<32> buff, buffFilename;
  CString str(buff), strFilename(buffFilename), strControllerID(m_buffControllerID);

  strFilename = fstr_TXT_TIMEZONE;

  if (SD.exists(strFilename))
  {
    CTextFile fileTimeZone(strFilename, FILE_READ);

    if (fileTimeZone)
    {
      fileTimeZone.readLine(strControllerID);
      fileTimeZone.readLine(str);
      m_nTimezone = str.toInt(10);
      fileTimeZone.readLine(str);
      m_bAdjustDaylightSaving = str == F("yes");
      bResult = true;
      fileTimeZone.close();
    }
  	else
  	{
      debugFileError(fstr_TXT_TIMEZONE, false);
    }
  }
  else
  {
    debugFileError(fstr_TXT_TIMEZONE, true);
  }
  return bResult;
}

bool CWifiManager::readEmailSettings()
{
	bool bResult = false;
	CBuff<16> SmallBuff;
	CString strFilename(SmallBuff), strToEmail(m_buffToEmail), strMailServer(m_buffMailServer), 
          strUsername(m_buffUsername), strPassword(m_buffPassword);

	strFilename = fstr_TXT_EMAIL;
	if (SD.exists(strFilename))
	{
		CTextFile fileEmail;

		if (fileEmail.open(strFilename, FILE_READ))
		{
			fileEmail.readLine(strToEmail);
			fileEmail.readLine(strMailServer);
			fileEmail.readLine(strUsername);
			fileEmail.readLine(strPassword);
			bResult = true;
			fileEmail.close();
		}
		else
		{
      debugFileError(fstr_TXT_EMAIL, false);
		}
	}
	else
	{
    debugFileError(fstr_TXT_EMAIL, true);
	}
	return bResult;
}

bool CWifiManager::readWifiSSIDKey(CString &strSSID, CString &strKey)
{
  CBuff<16> SmallBuff;
  CString strFilename(SmallBuff);
  bool bResult = false;

  strFilename = fstr_TXT_WIFI;
  if (SD.exists(strFilename))
  {
    CTextFile fileWifi(strFilename, FILE_READ);

    if (fileWifi)
    {
      fileWifi.readLine(strSSID);
      fileWifi.readLine(strKey);
      bResult = (strSSID.length() > 0) && (strKey.length() > 0);
    }
  }
  return bResult;
}

void CWifiManager::updateEmailSettings(const char *strToEmail, const char *strMailServer, const char *strUsername, const char *strPassword, CString &strMsg)
{
	CBuff<16> SmallBuff;
	CString strFilename(SmallBuff), strToEmail_(m_buffToEmail), strMailServer_(m_buffMailServer), 
          strUsername_(m_buffUsername), strPassword_(m_buffPassword);


	if ((strlen(strToEmail) > 0) && (strlen(strMailServer) > 0) && (strlen(strUsername) > 0) && (strlen(strPassword) > 0))
	{
    strToEmail_ = strToEmail;
    strMailServer_ = strMailServer;
    strUsername_ = strUsername;
    strPassword_ = strPassword;
    
		strFilename = fstr_TXT_WIFI;
		if (SD.exists(strFilename))
		 {
			  CTextFile file;
			  if (file.open(fstr_TXT_EMAIL, REWRITE))
			  {
				  file.writeLine(strToEmail);
				  file.writeLine(strMailServer);
				  file.writeLine(strUsername);
				  file.writeLine(strPassword);
          file.close();
			  }
			  else
			  {
				  strMsg = F("Could not write to '");
				  strMsg += fstr_TXT_EMAIL;
				  strMsg += F("'!");
			  }
		}
		else
		{
			strMsg = F("Could find '");
			strMsg += fstr_TXT_EMAIL;
			strMsg += F("'!");
		}
	}
}

void CWifiManager::updateSytemTime(CDateTime &datetime, const bool bAdjustDaylighSavings, const uint8_t nTimezoneAdjust, const char *strID, CString &strMsg)
{
	CBuff<16> SmallBuff1, SmallBuff2;
	CString str(SmallBuff1), strFilename(SmallBuff2), strControllerID(m_buffControllerID);

	rtc.setDayOfWeek(datetime.getDOW());
	rtc.setDate(datetime.getDOM());
	rtc.setMonth(datetime.getMonth());
	rtc.setYear(datetime.getYear());
	rtc.setHours(datetime.getHours());
	rtc.setMinutes(datetime.getMinutes());
	rtc.setSeconds(datetime.getSeconds());
	rtc.setClock();
	
	strControllerID = strID;
	m_nTimezone = nTimezoneAdjust;
	m_bAdjustDaylightSaving = bAdjustDaylighSavings;

	strFilename = fstr_TXT_TIMEZONE;
	if (SD.exists(strFilename))
	{
		CTextFile file;
                      
		if (file.open(fstr_TXT_TIMEZONE, REWRITE))
		{
			file.writeLine(strControllerID);
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
			strMsg += fstr_TXT_TIMEZONE;
			strMsg += F("'!");
		}
	}
	else
	{
		strMsg = F("Could find '");
		strMsg += fstr_TXT_TIMEZONE;
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
		strFilename = fstr_TXT_WIFI;
		if (SD.exists(strFilename))
		{
			CTextFile fileProgram;
			if (fileProgram.open(strFilename, REWRITE))
			{
				fileProgram.writeLine(strNetworkName);
				fileProgram.writeLine(strNetworkKey);
				fileProgram.close();
				strMsg = F("Wifi detail updated!");
			}
			else
			{
        debugFileError(fstr_TXT_WIFI, false);
				strMsg = F("Could not write to '");
				strMsg += fstr_TXT_WIFI;
				strMsg += F("'!");
			}
		}
		else
		{
        debugFileError(fstr_TXT_WIFI, true);
        strMsg = F("Could not find '");
        strMsg += fstr_TXT_WIFI;
        strMsg += F("'!");
		}
	}
	else
		strMsg = F("Error reading details!");
}

bool CWifiManager::sendEmail(const char* strSubject, const char* strMsg)
{
  SMTPClient client;
  bool bResult = false;
  CBuff<33> buff;
  CString strDateTime(buff), strFromEmail(m_buffFromEmail), strToEmail(m_buffToEmail), strMailServer(m_buffMailServer), 
          strUsername(m_buffUsername), strPassword(m_buffPassword), strIPAddr(m_buffIPAddr);
  
  if (strlen(strToEmail) > 0)
  {
    client.setTo(strToEmail);
    client.setFrom(strFromEmail);
    client.setUsername(strUsername);
    client.setPassword(strPassword);
    IPAddress ip = WiFi.localIP();
    strIPAddr.fromIPAddr(ip);
    client.setIPAddr(strIPAddr);
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
    client.setSubject(strSubject);
    client.setContent(strMsg);
    if (client.connect(strMailServer))
    {
      bResult = client.sendEmail();
    }
  }
  debug.logEventOpen('=', F("EMAIL NOTIFICATION"), true);
  if (bResult)
  {
    debug.log(F("Subject: "), false);
    debug.log(strSubject);
    debug.log(F("Body: "), false);
    debug.log(strMsg);
    debug.logEventClose('=', F("Sent"));
  }
  else
  {
    debug.logEventClose('=', F("Not sent"));
  }
  return bResult;
}
 
bool CWifiManager::begin()
{
	bool bResult = false;
	uint8_t nRetries = 0;
	CBuff<16> Buff1, Buff2;
	CBuff<65> BuffSubject;
	CString strTemp(Buff1), strFilename(Buff2), strSubject(BuffSubject), strControllerID(m_buffControllerID), 
	        strNetworkName(m_buffNetworkName), strNetworkKey(m_buffNetworkKey);
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

		if (readWifiSSIDKey(strNetworkName, strNetworkKey))
		{
			// Attempt to connect to WiFi network
			while ((nStatus != WL_CONNECTED) && (nRetries < 10))
			{
        debug.logEventOpen('*', F("CONNECTING TO WiFi NETWORK"), true);
        debug.log(F("Attempting to connect to WPA with SSID: "), false);
        debug.log(strNetworkName, false);
        debug.log(F(" and KEY: "), false);
        debug.log(strNetworkKey);
        
				// Connect to WPA/WPA2 network
				nStatus = WiFi.begin(strNetworkName, strNetworkKey);
        debug.log(F("Status = "), false);
        debug.log(fromUint(nStatus, 10));
				nRetries++;
        if (nStatus != WL_CONNECTED)
          debug.logEventClose('*', "");
			}
			if (nStatus == WL_CONNECTED)
			{
				IPAddress IPAddr = WiFi.localIP();
        CBuff<64> buff;
        CString str(buff);
        str = F("You're connected to the network with IP address: ");
        str += fromIP(IPAddr);
        debug.logEventClose('*', str);
        synchClock();

        strSubject = F("Irrigation controller '");
        strSubject += strControllerID;
        strSubject += F("' IP address");
        
				sendEmail(strSubject, fromIP(IPAddr));

				// Start the web server on port 80
        m_HTTPServer.begin();
        if (!m_UDPServer.begin(10002))
          debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);

				bResult = true;
			}
			else
			{
        debug.logEventClose('*', F("Could not log into local wifi network"));
			}
		}
	}
	return bResult;
}



