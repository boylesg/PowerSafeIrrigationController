#include <Arduino.h>
#include <MySD.h>
#include <WiFiEsp.h>
#include "WifiManager.h"
#include "Common.h"
#include "DateTime.h"
#include "Debug.h"




const char *CWifiManager::m_strDelimiter = "`";

CWifiManager::CWifiManager(): m_HTTPServer(0)
{
	m_pSerial = NULL;
	m_nDryTime = m_nOnTime = 0;
	m_nThresh = 0;
	m_bAuto = false;
	m_nLastMillis = 0;
	m_nStationNum = -1;
}

CWifiManager::CWifiManager(HardwareSerial* pSerial): m_HTTPServer(80), m_strHTTPAlarmFilename(F("alarm.htm")), m_strHTTPControllerFilename(F("control.htm")),
                           m_strFaviconFileName(F("Icon.png")), m_strTXTWifiFilename(F("wifi.txt")), m_strHTTPStationFilename(F("station.htm")),
                           m_strNetworkName(m_buffNetworkName), m_strNetworkKey(m_buffNetworkKey), 
                           m_strStationDesc(m_buffStationDesc), m_strControllerID(m_buffControllerD),
                           m_strTXTAlarmFilename(F("alarm.txt")), m_strTXTControllerFilename(F("control.txt"))
{
  m_pSerial = pSerial;
  assert(m_pSerial);
  m_nDryTime = m_nOnTime = 0;
  m_nThresh = 0;
  m_bAuto = false;
  m_nLastMillis = 0;
  m_nStationNum = -1;
}

CWifiManager::~CWifiManager()
{
}

void CWifiManager::checkAlarm(const uint16_t nReading)
{
  static uint32_t nLastMillis = 0;
  uint32_t nElaspedMillis = millis() - nLastMillis;
  uint8_t nElapsedMin = nElaspedMillis / (60 * 1000);

  if ((nReading <= m_nThresh) && (nElapsedMin >= m_nDryTime))
  {
/* UDP
    strData = m_strControllerID;
    strData += m_strDelimiter;
    strData += fromUint(m_nStationNum, 10);
    strData += m_strStationDesc;
    strData += m_strDelimiter;
    strData += fromUint(m_nOnTime, 10);
    writeUDPWord(strData);
*/
  	WiFiEspClient client;
  	CBuff<129> buff;
  	CString str(buff);
    IPAddress IPAddr = WiFi.localIP();
    CBuff<17> buffIP;
    CString strIP(buffIP);
    
    strIP = IPAddr;
  	if (client.connect(str, 80))
  	{
  		debug.log(F("Connected to '"), false);
  		debug.log(str, false);
  		debug.log(F("'..."));
  
  		// Make a HTTP request
  		str = F("GET / wirelessalarm?");
  		str += F("ip=");
  		str += strIP;
  		str += F("&station=");
  		str += fromUint(m_nStationNum, 10);
  		str += F("&description=");
  		str += m_strStationDesc;
  		str += F("&drytime=");
  		str += fromUint(nElapsedMin, 10);
  		str += F("&ontime=");
  		str += fromUint(m_nOnTime, 10);
  		str += F("&auto=");
  		str += fromUint(m_bAuto, 10);
  		str += F("&");
  		str += F(" HTTP/1.1");
  		client.println(str);
      str = F("Host: ");
      str += strIP;
  		client.println(str);
  		client.println(F("Connection: close"));
  		client.println();
  	}
  }
  nLastMillis = millis();
}

bool CWifiManager::writeUDPWord(const char *strBuff)
{
  int nSize = strlen(strBuff);
  char *strDelimAdded = new char[nSize + 2];

  if (strDelimAdded)
  {
    memset(strDelimAdded, 0, nSize + 2);
    strcpy(strDelimAdded, strBuff);
    strcat(strDelimAdded, m_strDelimiter);

    m_UDPServer.beginPacket(m_ipaddrController, m_nIrrigControllerPort);
    nSize = m_UDPServer.write(strDelimAdded, strlen(strDelimAdded));
    m_UDPServer.endPacket();

    debug.log(F("Sending '"), false);
    debug.log(strBuff, false);
    debug.log(F("', "), false);
    debug.log(nSize, false);
    if (nSize > 1)
      debug.log(F(" bytes."));
    else
      debug.log(F(" byte."));
    delete strDelimAdded;
  }
}

bool CWifiManager::writeUDPWord(const __FlashStringHelper *strBuff)
{
  uint16_t nSize = strlen_P((PGM_P)strBuff);
  char *strDelimAdded = new char[nSize + 2];
   
  if (strDelimAdded)
  {
    memset(strDelimAdded, 0, nSize + 2);
    strcpy_P(strDelimAdded, (PGM_P)strBuff);
    strcat(strDelimAdded, m_strDelimiter);

    m_UDPServer.beginPacket(m_UDPServer.remoteIP(), m_UDPServer.remotePort());
    nSize = m_UDPServer.write(strDelimAdded, strlen(strDelimAdded));
    m_UDPServer.endPacket();

    debug.log(F("Sending '"), false);
    debug.log(strBuff, false);
    debug.log(F("', "), false);
    debug.log(nSize, false);
    debug.log(F(" bytes."));
    delete strDelimAdded;
  }
}

void CWifiManager::doLowBatteryWarning(const float fVoltage)
{
  m_UDPServer.beginPacket(m_ipaddrController, 100002L);
  CBuff<128> buff;
  CString strMsg(buff);
  strMsg = F("wirelessalarm");
  strMsg += m_strDelimiter;
  strMsg += m_strControllerID;
  strMsg += m_strDelimiter;
  strMsg += m_strStationDesc;
  strMsg += m_strDelimiter;
  strMsg = F("batterylow");
  strMsg += m_strDelimiter;
  strMsg = fromReal(fVoltage, 1, false);
  strMsg += m_strDelimiter;
  m_UDPServer.write(strMsg, strMsg.length());
  m_UDPServer.endPacket();
}

#ifndef __SAM3X8E__
  void CWifiManager::sendErrorResponse(WiFiEspClient& WifiClient, const __FlashStringHelper* strFileName)
  {
    char strBuff[33];
    CString str(strBuff, 33);
  
    str = strFileName;
    sendErrorResponse(WifiClient, str);
  }
#endif

void CWifiManager::writeToClient(WiFiEspClient& WifiClient, const char* strData, const int nLength)
{
  int nWritten = WifiClient.write(strData, nLength);
  if (nWritten == 0)
  {
    WifiClient.flush();
    WifiClient.write(strData, nLength);
  }
}

bool CWifiManager::isHexDigit(const char cCh)
{
  return ((cCh >= '0') && (cCh <= '9')) || ((cCh >= 'a') && (cCh <= 'f')) || ((cCh >= 'A') && (cCh <= 'F'));
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

void CWifiManager::sendErrorResponse(WiFiEspClient& WifiClient, const char* strFileName)
{
  CBuff<128> Buff;
  CString str(Buff);

  str = F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: keep-alive\r\n\r\n");
  writeToClient(WifiClient, str, str.length());

  str = F("<!DOCTYPE HTML>\r\n");
  writeToClient(WifiClient, str, str.length());

  str = F("<html>\r\n");
  writeToClient(WifiClient, str, str.length());

  str = F("<h1>Can't open ");
  writeToClient(WifiClient, str, str.length());

  writeToClient(WifiClient, strFileName, strlen(strFileName));

  str = F(" on SD card...</h1>\r\n");
  writeToClient(WifiClient, str, str.length());

  str = F("<br>\r\n");
  writeToClient(WifiClient, str, str.length());

  str = F("</html>\r\n");
  writeToClient(WifiClient, str, str.length());
}

bool CWifiManager::sendFile(WiFiEspClient& WifiClient, const char* strFileName)
{
  bool bResult = false;
  uint16_t nI = 1;
  CBuff<256> Buff;

  if (SD.exists(strFileName))
  {
    File file = SD.open(strFileName, FILE_READ);

    bResult = file.available() > 0;
    
    while (file.available() > 0)
    {
      m_nBytesRead = file.readBytes(Buff.m_strBuff, Buff.m_nBuffSize);

      writeToClient(WifiClient, Buff.m_strBuff, Buff.m_nBuffSize);
    }
    file.close();
  }
  else
    sendErrorResponse(WifiClient, strFileName);

  return bResult;
}


bool CWifiManager::processAlarmFormData(CString& strFormData)
{
  // EXAMPLE
  // -------
  // alarm.htm?Desc=Back&Auto=2&DryTime=10&Thresh=750&OnTime=10&Upload=Save+alarm+details
  bool bResult = true;
  CBuff<25> buff1, buff2;
  CString strKey(buff1), strVal(buff2);
  int16_t nPos1 = 0, nPos2 = 0;

  if (nPos1 = strFormData.indexOf(F("&Upload")) >= 0)
    strFormData = strFormData.substring(0, nPos1 - 1);

  // alarm.htm?Desc=Back&Auto=2&DryTime=10&Thresh=750&OnTime=10
  if (nPos1 = strFormData.indexOf(F("?")) >= 0)
    strFormData = strFormData.substring(nPos1 + 1, strFormData.length() - 1);

  // Desc=Back&Auto=2&DryTime=10&Thresh=750&OnTime=10
  while (!strFormData.isEmpty())
  {
    if ((nPos1 = strFormData.indexOf(F("=")) >= 0) && (nPos2 = strFormData.indexOf(F("&")) >= 0))
    {
      strKey = strFormData.substring(0, nPos1 - 1);
      strVal = strFormData.substring(nPos1 + 1, nPos2 - 1);
      strFormData = strFormData.substring(nPos2 + 1, strFormData.length() - 1);

      if (strKey == F("Desc"))
        m_strStationDesc = strVal;
      else if (strKey == F("Auto"))
        m_bAuto = strVal == F("1");
      else if (strKey == F("DryTime"))
        m_nDryTime = strVal.toUint();
      else if (strKey == F("Thresh"))
        m_nThresh = strVal.toUint();
      else if (strKey == F("OnTime"))
        m_nOnTime = strVal.toUint();
    }
  }
  CBuff<129> buff;
  CString strMsg(buff);
  saveAlarmDetails(strMsg);
}

bool CWifiManager::sendAlarmContent(WiFiEspClient& WifiClient, const bool bFormResponse)
{
  bool bResult = false;
  uint32_t nMillisStart = millis();
  uint8_t nStationNum = 1, nLineNum = 0;
  int8_t nPos = 0;
  CBuff<16> BuffFilename;
  CBuff<256> BuffLine;
  CBuff<17> BuffTemp;
  CString strLine(BuffLine), strFilename(BuffFilename), strTemp(BuffTemp);

  strFilename = m_strHTTPAlarmFilename;
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
        if (file.readLine(strLine, true))
          nLineNum++;

        if (nLineNum == 24)
        {
          strLine.replace(F("XXXX"), m_strStationDesc);
        }
        else if (nLineNum == 26)
        {
          if (m_bAuto)
            strLine.replace(F("value=\"XXXX\""), F("checked=\"checked\""));
          else
            strLine.replace(F("value=\"XXXX\""), F(""));
        }
        else if (nLineNum == 28)
        {
          strTemp = F("value=\"");
          strTemp += fromUint(m_nDryTime, 10);
          strLine.replace(F("value=\"10\""), strTemp);
        }
        else if (nLineNum == 30)
        {
          strTemp = F("value=\"");
          strTemp += fromUint(m_nThresh, 10);
          strLine.replace(F("value=\"750\""), strTemp);
        }
        else if (nLineNum == 32)
        {
          strTemp = F("value=\"");
          strTemp += fromUint(m_nOnTime, 10);
          strTemp += F("\"");
          strLine.replace(F("value=\"10\""), strTemp);
        }
        else if (bFormResponse && (nLineNum == 26))
          strLine.replace(F("Alarm details have not been uploaded yet"), F("Alarm details were uploaded successfully"));
        writeToClient(WifiClient, strLine.c_str(), strLine.length());
      }
      strLine = F("HTTP response took ");
      strLine += fromUint(millis() - nMillisStart, 10);
      strLine += F("ms to service...");
      debug.log(strLine);
    }
    else
      sendErrorResponse(WifiClient, strFilename);
  }
  else
    sendErrorResponse(WifiClient, strFilename);

  return bResult;
}

bool CWifiManager::sendControllerContent(WiFiEspClient& WifiClient, const bool bFormResponse)
{
  bool bResult = false;
  uint32_t nMillisStart = millis();
  uint8_t nStationNum = 1, nLineNum = 0;
  int8_t nPos = 0;
  CBuff<16> BuffFilename;
  CBuff<256> BuffLine;
  CBuff<17> BuffTemp;
  CString strLine(BuffLine), strFilename(BuffFilename), strTemp(BuffTemp);

  strFilename = m_strHTTPControllerFilename;
  if (SD.exists(strFilename))
  {
    CTextFile file;

    if (file.open(strFilename, FILE_READ))
    {
      bResult = true;
      debug.log(F("Sending HTTP response - "), false);
      debug.log(strFilename);
      debug.log(F("..."));
      sendHTTPHeader(WifiClient);

      while (!file.eof())
      {
        bResult = true;

        if (file.readLine(strLine, true))
          nLineNum++;

        if (nLineNum == 22)
          strLine.replace(F("XXXX"), fromUint(m_ipaddrController[0], 10));
        else if (nLineNum == 23)
          strLine.replace(F("XXXX"), fromUint(m_ipaddrController[1], 10));
        else if (nLineNum == 24)
          strLine.replace(F("XXXX"), fromUint(m_ipaddrController[2], 10));
        else if (nLineNum == 25)
          strLine.replace(F("XXXX"), fromUint(m_ipaddrController[3], 10));
        else if (nLineNum == 27)
          strLine.replace(F("XXXX"), m_strControllerID);
        else if (bFormResponse && (nLineNum == 27))
          strLine.replace(F("Controller details have not been uploaded yet"), F("Controller details were uploaded successfully"));

        writeToClient(WifiClient, strLine, strLine.length());
      }
      strLine = F("HTTP response took ");
      strLine += fromUint(millis() - nMillisStart, 10);
      strLine += F("ms to service...");
      debug.log(strLine);
    }
    else
      sendErrorResponse(WifiClient, strFilename);
  }
  else
    sendErrorResponse(WifiClient, strFilename);

  return bResult;
}

bool CWifiManager::processStationFormData(CString& strFormData)
{
  // EXAMPLE
  // -------
  // station.htm?Station=1&Description=Back&Upload=Save+station+details
  bool bResult = true;
  CBuff<25> buff1, buff2;
  CString strKey(buff1), strVal(buff2);
  int16_t nPos1 = 0, nPos2 = 0;

  if (nPos1 = strFormData.indexOf(F("&Upload")) >= 0)
    strFormData = strFormData.substring(0, nPos1 - 1);

  // station.htm?Station=1&Description=Back
  if (nPos1 = strFormData.indexOf(F("?")) >= 0)
    strFormData = strFormData.substring(nPos1 + 1, strFormData.length() - 1);

  // Station=1&Description=Back
  while (!strFormData.isEmpty())
  {
    if ((nPos1 = strFormData.indexOf(F("=")) >= 0) && (nPos2 = strFormData.indexOf(F("&")) >= 0))
    {
      strKey = strFormData.substring(0, nPos1 - 1);
      strVal = strFormData.substring(nPos1 + 1, nPos2 - 1);
      strFormData = strFormData.substring(nPos2 + 1, strFormData.length() - 1);

      if (strKey == F("Station"))
        m_nStationNum = strVal.toUint();
      else if (strKey == F("Description"))
        m_strStationDesc = strVal;
    }
  }
  CBuff<129> buff;
  CString strMsg(buff);
  saveStationDetails(strMsg);
}

bool CWifiManager::sendStationContent(WiFiEspClient& WifiClient, const bool bFormResponse)
{
  bool bResult = false;
  uint32_t nMillisStart = millis();
  uint8_t nStationNum = 1, nLineNum = 0;
  int8_t nPos = 0;
  CBuff<16> BuffFilename;
  CBuff<256> BuffLine;
  CBuff<17> BuffTemp;
  CString strLine(BuffLine), strFilename(BuffFilename), strTemp(BuffTemp);

  strFilename = m_strHTTPStationFilename;
  if (SD.exists(strFilename))
  {
    CTextFile file;

    if (file.open(strFilename, FILE_READ))
    {
      bResult = true;
      debug.log(F("Sending HTTP response - "), false);
      debug.log(strFilename);
      debug.log(F("..."));
      sendHTTPHeader(WifiClient);

      while (!file.eof())
      {
        bResult = true;

        if (file.readLine(strLine, true))
          nLineNum++;

        if (nLineNum == 20)
          strLine.replace(F("XXXX"), fromUint(m_nStationNum, 10));
        else if (nLineNum == 22)
          strLine.replace(F("XXXX"), m_strStationDesc);
        else if (nLineNum == 26)
          strLine.replace(F("Station details have not been uploaded yet..."), F("Station details were successfully uploaded..."));
        writeToClient(WifiClient, strLine, strLine.length());
      }
      strLine = F("HTTP response took ");
      strLine += fromUint(millis() - nMillisStart, 10);
      strLine += F("ms to service...");
      debug.log(strLine);
    }
    else
      sendErrorResponse(WifiClient, strFilename);
  }
  else
    sendErrorResponse(WifiClient, strFilename);

  return bResult;
}

bool CWifiManager::processControllerFormData(CString& strFormData)
{
  // EXAMPLE
  // -------
  // alarm.htm?IP1=255&IP2=&IP3=&IP4=&ID=Controller1&Upload=Save+controller+details
  bool bResult = true;
  CBuff<25> buff1, buff2;
  CString strKey(buff1), strVal(buff2);
  int16_t nPos1 = 0, nPos2 = 0;

  if (nPos1 = strFormData.indexOf(F("&Upload")) >= 0)
    strFormData = strFormData.substring(0, nPos1 - 1);

  // alarm.htm?IP1=255&IP2=&IP3=&IP4=&ID=Controller1
  if (nPos1 = strFormData.indexOf(F("?")) >= 0)
    strFormData = strFormData.substring(nPos1 + 1, strFormData.length() - 1);

  // IP1=255&IP2=&IP3=&IP4=&ID=Controller1
  while (!strFormData.isEmpty())
  {
    if ((nPos1 = strFormData.indexOf(F("=")) >= 0) && (nPos2 = strFormData.indexOf(F("&")) >= 0))
    {
      strKey = strFormData.substring(0, nPos1 - 1);
      strVal = strFormData.substring(nPos1 + 1, nPos2 - 1);
      strFormData = strFormData.substring(nPos2 + 1, strFormData.length() - 1);

      if (strKey == F("IP1"))
        m_ipaddrController[0] = strVal.toUint();
      else if (strKey == F("IP2"))
        m_ipaddrController[1] = strVal.toUint();
      else if (strKey == F("IP3"))
        m_ipaddrController[2] = strVal.toUint();
      else if (strKey == F("IP4"))
        m_ipaddrController[3] = strVal.toUint();
      else if (strKey == F("OnTime"))
        m_strControllerID = strVal;
    }
  }
  CBuff<129> buff;
  CString strMsg(buff);
  saveControllerDetails(strMsg);
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

bool CWifiManager::processRequest()
{
  CBuff<5> Buff5;
  CBuff<512> LargeBuff;
  CString strReq(LargeBuff), strHexCode(Buff5);
  boolean bNewLine = false;
  char cCh = 0;
  const uint16_t nWait = 500;
  uint32_t nTimeout = 0;
  bool bResult = false;

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
            bResult = processHTTPRequest(WifiClient, strReq);
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
    bResult = processUDPRequest(&m_UDPServer);
  }
  return bResult;
}

bool CWifiManager::processUDPRequest(WiFiEspUDP *pUDPServer)
{
  CBuff<24> Buff24;
  CString strWord(Buff24);
  IPAddress IP;
  bool bResult = false;
  
  debug.log(F("***********************************************************************"));
  debug.log(F("New UDP request\n"));
  IP = pUDPServer->remoteIP();
  strWord = IP;
  debug.dump(F("\tRemote IP address"), strWord);
  debug.dump(F("\tRemote port"), pUDPServer->remotePort());
  debug.log(F("***********************************************************************"));

  return bResult;
}

bool CWifiManager::processHTTPRequest(WiFiEspClient &WifiClient, CString &strReq)
{
  CBuff<24> Buff24;
  CString str(Buff24);
  IPAddress IP;
  bool bResult = false;

  if (!strReq.isEmpty())
  {
    bResult = true;
    debug.log(F("***********************************************************************"));
    debug.log(F("New HTTP request\n"));
    IP = WifiClient.remoteIP();
    str = IP;
    debug.dump(F("\tRemote IP address"), str);
    debug.dump(F("\tRemote port"), WifiClient.remotePort());
    debug.dump(F("\tContents"), strReq);
    debug.log(F("***********************************************************************"));

    if (((strReq.indexOf(F("GET / HTTP/1.1")) >= 0) || (strReq.indexOf(F("GET /alarm.htm HTTP/1.1")) >= 0)) && !sendAlarmContent(WifiClient, false))
      debug.logFileOpenError(m_strHTTPAlarmFilename);
    else if ((strReq.indexOf(F("GET /controller.htm HTTP/1.1")) >= 0) && !sendControllerContent(WifiClient, false))
      debug.logFileOpenError(m_strHTTPControllerFilename);
    else if ((strReq.indexOf(F("GET /station.htm HTTP/1.1")) >= 0) && !sendStationContent(WifiClient, false))
      debug.logFileOpenError(m_strHTTPStationFilename);
    else if ((strReq.indexOf(F("GET /favicon.ico HTTP/1.1")) >= 0) && !sendIcon(WifiClient))
      debug.logFileOpenError(m_strFaviconFileName);
    else if (strReq.indexOf(F("alarm.htm?")) >= 0)
    {
      strReq = strReq.substring(strReq.indexOf(F("?")) + 1, strReq.indexOf(F(" HTTP/1.1")) - 1);
      processAlarmFormData(strReq);

      if (!sendAlarmContent(WifiClient, true))
        debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
    }
    else if (strReq.indexOf(F("control.htm?")) >= 0)
    {
      strReq = strReq.substring(strReq.indexOf(F("?")) + 1, strReq.indexOf(F(" HTTP/1.1")) - 1);
      processControllerFormData(strReq);

      if (!sendControllerContent(WifiClient, true))
        debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
    }
    else if (strReq.indexOf(F("station.htm?")) >= 0)
    {
      strReq = strReq.substring(strReq.indexOf(F("?")) + 1, strReq.indexOf(F(" HTTP/1.1")) - 1);
      processStationFormData(strReq);

      if (!sendStationContent(WifiClient, true))
        debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
    }
  }
  return bResult;
}

const char *CWifiManager::getDryTimeStr()
{
  CString str(m_buff);
  str.fromUint(m_nDryTime);
  return str;
};
const char *CWifiManager::getOnTimeStr()
{
  CString str(m_buff);
  str.fromUint(m_nOnTime);
  return str;
};
const char *CWifiManager::getThresholdStr()
{
  CString str(m_buff);
  str.fromUint(m_nThresh);
  return str;
};

const char *CWifiManager::getIP(const uint8_t nI)
{
  CString strIP(m_buff);

  if ((nI >= 0) && (nI <= 3))
  {
    strIP.fromUint(m_ipaddrController[nI]);
  }
  else
  {
    strIP.empty();
  }
  return strIP;
}

WiFiEspClient CWifiManager::getClient()
{
  return m_HTTPServer.available();
}

bool CWifiManager::readControllerDetails(CString &strMsg)
{
  CBuff<16> SmallBuff;
  CBuff<25> BuffIP;
  CString strFilename(SmallBuff), strIP(BuffIP);
  bool bResult = false;
  CTextFile file;

  strFilename = m_strTXTControllerFilename;
  if (SD.exists(strFilename))
  {
    if (file.open(strFilename, FILE_READ))
    {
      file.readLine(strIP);
      m_ipaddrController.fromString(strIP);
      file.readLine(m_strControllerID);
      bResult = (strIP.length() > 0) && (m_strControllerID.length() > 0);
    }
    else
    {
      debug.logFileOpenError(strFilename);
    }
  }
  else
  {
    debug.logFileFoundError(strFilename);
  }
  return bResult;
}

bool CWifiManager::readWifiDetails(CString &strMsg)
{
  CBuff<16> SmallBuff;
  CString strFilename(SmallBuff);
  bool bResult = false;
  CTextFile file;

  strFilename = m_strTXTWifiFilename;
  if (SD.exists(strFilename))
  {
    if (file.open(strFilename, FILE_READ))
    {
      file.readLine(m_strNetworkName);
      file.readLine(m_strNetworkKey);
      debug.log(F("From SD card SSID = "), false);
      debug.log(m_strNetworkName, false);
      debug.log(F(", KEY = "), false);
      debug.log(m_strNetworkKey, true);
      bResult = (m_strNetworkName.length() > 0) && (m_strNetworkKey.length() > 0);
    }
    else
    {
      debug.logFileOpenError(strFilename);
    }
  }
  else
  {
    debug.logFileFoundError(strFilename);
  }
  return bResult;
}

bool CWifiManager::saveStationDetails(const uint8_t nStationNum, const char* strStationDesc)
{
	m_strStationDesc = strStationDesc;
	m_nStationNum = nStationNum;

	CBuff<129> buff;
	CString strMsg(buff);
	saveStationDetails(strMsg);
}

bool CWifiManager::saveAlarmDetails(const bool bAuto, const uint8_t nDryTime, const uint8_t nOnTime, const uint16_t nThresh)
{
  m_bAuto = bAuto;
  m_nDryTime = nDryTime;
  m_nOnTime = nOnTime;
  m_nThresh = nThresh;

  CBuff<129> buff;
  CString strMsg(buff);
  saveAlarmDetails(strMsg);
}

bool CWifiManager::saveWifiDetails(const char *strName, const char *strKey)
{
  m_strNetworkName = strName;
  m_strNetworkKey = strKey;

  CBuff<129> buff;
  CString strMsg(buff);
  saveWifiDetails(strMsg);
}

bool CWifiManager::saveControllerDetails(const char *strControllerID, IPAddress &ipaddr)
{
  m_strControllerID = strControllerID;
  m_ipaddrController[0] = strControllerID[0];
  m_ipaddrController[1] = strControllerID[1];
  m_ipaddrController[2] = strControllerID[2];
  m_ipaddrController[3] = strControllerID[3];

  CBuff<129> buff;
  CString strMsg(buff);
  saveControllerDetails(strMsg);
}

bool CWifiManager::saveControllerDetails(CString &strMsg)
{
  bool bResult = false;
  CBuff<16> SmallBuff;
  CString strFilename(SmallBuff);

  debug.log(F("Updating Wifi details\n"));

  if ((strlen(m_strNetworkName) > 0) && (strlen(m_strNetworkKey) > 0))
  {
    strFilename = m_strTXTControllerFilename;
    if (SD.exists(strFilename))
    {
      CTextFile file;
      if (file.open(strFilename, REWRITE))
      {
        file.write(m_ipaddrController[0]);
        file.write(F("."));
        file.write(m_ipaddrController[1]);
        file.write(F("."));
        file.write(m_ipaddrController[2]);
        file.write(F("."));
        file.writeLine(m_strControllerID);
        file.close();
        strMsg = F("Upload successful!");
        delay(500);
        resetArd();
      }
      else
      {
        debug.logFileOpenError(strFilename);
      }
    }
    else
    {
      debug.logFileFoundError(strFilename);
    }
  }
  else
    strMsg = F("Error reading details!");

  return bResult;
}

bool CWifiManager::saveWifiDetails(CString &strMsg)
{
  bool bResult = false;
  CBuff<16> SmallBuff;
  CString strFilename(SmallBuff);

  debug.log(F("Updating Wifi details\n"));

  if ((strlen(m_strNetworkName) > 0) && (strlen(m_strNetworkKey) > 0))
  {
    strFilename = m_strTXTWifiFilename;
    if (SD.exists(strFilename))
    {
      CTextFile file;
      if (file.open(strFilename, REWRITE))
      {
        file.writeLine(m_strNetworkName);
        file.writeLine(m_strNetworkKey);
        file.close();
        strMsg = F("Upload successful!");
        delay(500);
        resetArd();
      }
      else
      {
        debug.logFileOpenError(strFilename);
      }
    }
    else
    {
      debug.logFileFoundError(strFilename);
    }
  }
  else
    strMsg = F("Error reading details!");

  return bResult;
}

bool CWifiManager::readAlarmsDetails(CString &strMsg)
{
  CBuff<16> SmallBuff;
  CBuff<9> NumBuff;
  CString strFilename(SmallBuff), strNum(NumBuff);
  bool bResult = false;
  CTextFile file;

  strFilename = m_strTXTAlarmFilename;
  if (SD.exists(strFilename))
  {
    if (file.open(strFilename, FILE_READ))
    {
	    file.readLine(strNum);
	    bResult &= strNum.length() > 0;
	    m_nStationNum = strNum.toUint();

	    file.readLine(m_strStationDesc);
      bResult = (m_strStationDesc.length() > 0);     

      file.readLine(strNum);
      bResult &= strNum.length() > 0;
      m_nDryTime = strNum.toUint();    
     
      file.readLine(strNum);
      bResult &= strNum.length() > 0;
      m_nThresh = strNum.toUint();      

      file.readLine(strNum);
      bResult &= strNum.length() > 0;
      m_nOnTime = strNum.toUint();      
      
      file.readLine(strNum);
      bResult &= strNum.length() > 0;
      m_bAuto = strNum == F("1");  
    }
    else
    {
      debug.logFileOpenError(strFilename);
    }
  }
  else
  {
    debug.logFileFoundError(strFilename);
  }
  return bResult;
}

bool CWifiManager::saveStationDetails(CString &strMsg)
{
	return saveAlarmDetails(strMsg);
}

bool CWifiManager::saveAlarmDetails(CString &strMsg)
{
  bool bResult = false;
  CBuff<16> SmallBuff;
  CString strFilename(SmallBuff);

  debug.log(F("Updating Wifi details\n"));

  if ((strlen(m_strNetworkName) > 0) && (strlen(m_strNetworkKey) > 0))
  {
    strFilename = m_strTXTWifiFilename;
    if (SD.exists(strFilename))
    {
      CTextFile file;
      if (file.open(strFilename, REWRITE))
      {
		file.writeLine(m_nStationNum);
		file.writeLine(m_strStationDesc);
		file.writeLine(m_nDryTime);
        file.writeLine(m_nThresh);
        file.writeLine(m_nOnTime);
        file.writeLine(m_bAuto);
        file.close();
        strMsg = F("Upload successful!");
        delay(500);
        resetArd();
      }
      else
      {
        debug.logFileOpenError((const char*)strFilename);
      }
    }
    else
    {
      debug.logFileFoundError(strFilename);
    }
  }
  else
    strMsg = F("Error reading details!");

  return bResult;
}
 
bool CWifiManager::begin()
{
  bool bResult = false;
  uint8_t nRetries = 0;
  CBuff<16> Buff1, Buff2, Buff3;
  CBuff<48> Buff48;
  CString strTemp(Buff1), strFilename(Buff2), strIP(Buff3), strSubject(Buff48);
  uint8_t nStatus = WL_IDLE_STATUS;

  // Initialize serial for ESP module
  m_pSerial->begin(38400);

  // Initialize ESP module
  WiFi.init(m_pSerial);

  // Check for the presence of the shield
  if ((nStatus = WiFi.status()) == WL_NO_SHIELD)
  {
    debug.log(F("WiFi shield not present!"));
    resetESP();
    resetArd();
  }
  else
  {
    CBuff<129> buff;
    CString strMsg(buff);
    if (!readAlarmsDetails(strMsg))
      debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);
    if (!readControllerDetails(strMsg))
      debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);

    if (readWifiDetails(strMsg))
    {
      // Attempt to connect to WiFi network
      while ((nStatus != WL_CONNECTED) && (nRetries < 2))
      {
        debug.log(F("Attempting to connect to WPA SSID: "), false);
        debug.log(m_strNetworkName, false);
        debug.log(F(", KEY: "), false);
        debug.log(m_strNetworkKey, false);
        debug.log(F("..."), true);

        // Connect to WPA/WPA2 network
        nStatus = WiFi.begin(m_strNetworkName, m_strNetworkKey);
        nRetries++;
      }
      if (nStatus == WL_CONNECTED)
      {
        IPAddress IPAddr = WiFi.localIP();
        debug.log(F("You're connected to the network with IP address: "), false);
        debug.log(IPAddr, true);

        // Start the web server on port 80
        m_HTTPServer.begin();
        if (!m_UDPServer.begin(10005))
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



