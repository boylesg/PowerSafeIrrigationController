#include <Arduino.h>
#include <EEPROMex.h>
#include <WiFiEsp.h>
#include <WifiEspUdp.h>
#include "SerialManager.h"
#include "Debug.h"
#include "Common.h"




CSerialManager::CSerialManager(Stream* pSerialSrc, WiFiEspUDP* pUDPServer, HardwareSerial* pSerialHC05, const uint8_t nRelayPin, const uint8_t nStationNum): m_strNetworkName(m_buffNetworkName), m_strNetworkKey(m_buffNetworkKey), 
                                                                                                                                                          m_pUDPServer(pUDPServer), m_pSerialHC05(pSerialHC05), m_strDelim(F("`"))
{
  m_pSerialSrc = pSerialSrc;
  m_nRelayPin = nRelayPin;
  m_nStationNum = nStationNum;
  readWifiSSIDKey(m_strNetworkName, m_strNetworkKey);
}

CSerialManager::~CSerialManager()
{
  
}

bool CSerialManager::readWord(CString& strBuff)
{
  char cCh = 0;
  int32_t nTimeout = millis() + ((uint32_t)10 * 1000);

  strBuff.empty();
  while (true)
  {
    if (strBuff.full())
      break;
  
    if ((nTimeout - millis()) < 0)
      break;
        
    if ((m_pSerialSrc->available() > 0))
    {
      nTimeout = millis() + ((uint32_t)10 * 1000);
      cCh = m_pSerialSrc->read();
      if (cCh != '`')
      {
        strBuff += cCh;
      }
      else
      {
        break;
      }
    }
  }
  return strBuff.length() > 0;
}

void CSerialManager::turnOn()
{
  digitalWrite(m_nRelayPin, HIGH);
}

void CSerialManager::turnOff()
{
  digitalWrite(m_nRelayPin, LOW);
}

void CSerialManager::processData()
{
  CBuff<16> buff;
  CString strTag(buff);

  readWord(strTag);
  
  debug.log(F("Recieived '"), false);
  debug.log(strTag, false);
  debug.log(F("'"));
  
  if (strTag == F("wifi"))
  {
    CBuff<24> buffName, buffKey;
    CString strNetworkName(buffName), strNetworkKey(buffKey);
    if (readWord(strNetworkName) && readWord(strNetworkKey))
    {
      writeWifiSSIDKey(strNetworkName, strNetworkKey);
      resetArd();
    }
    else
      debug.log(F("Failed to read Wifi network name and/or password!"));
  }
  else if (strTag == F("control"))
  {
    if (readWord(strTag))
    {
      uint8_t nStationNum = strTag.toUint();
      if (m_nStationNum == nStationNum)
      {
        if (readWord(strTag))
        {
          if (strTag == F("on"))
            turnOn();
          else if (strTag == F("off"))
            turnOff();
        }
        else
          debug.logRuntimeError(F("SerialManager.cpp"), __LINE__);
      }
    }
    else
      debug.logRuntimeError(F("SerialManager.cpp"), __LINE__);
  }
  else if (strTag == F("request"))
  {
    if (readWord(strTag))
    {      
      uint8_t nStationNum = strTag.toUint();
      if (m_nStationNum == nStationNum)
      {
        if (readWord(strTag))
          notifyController(strTag);
        else
          debug.logRuntimeError(F("program.cpp"), __LINE__);
      }
    }
    else
      debug.logRuntimeError(F("SerialManager.cpp"), __LINE__);
  }
}

void CSerialManager::notifyController(const char* strIP)
{
  CBuff<16> buff;
  CString str(buff);
  IPAddress ipaddrController(255, 255, 255, 255);

  if (strIP)
    ipaddrController.fromString(strIP);
    
  str = F("notify");
  str += m_strDelim;
  str += fromUint(m_nStationNum, 10);
  str += m_strDelim;
  
  CBuff<24> buffIPAddr;
  CString strIPAddrSolenoid(buffIPAddr);
  strIPAddrSolenoid = WiFi.localIP();
  str += strIPAddrSolenoid;

  debug.log(F("Sending '"), false);
  debug.log(str, false);
  debug.log(F("'"));
  writeWord(str, ipaddrController);
}

bool CSerialManager::writeWord(const char *strBuff)
{
  return writeWord(strBuff, m_pUDPServer->remoteIP(), m_pUDPServer->remotePort());
}

bool CSerialManager::writeWord(const char *strBuff, IPAddress ip, const uint16_t nPort)
{
  int nSize = strlen(strBuff);
  char *strDelimAdded = new char[nSize + 2];

  if (strDelimAdded)
  {
    memset(strDelimAdded, 0, nSize + 2);
    strcpy(strDelimAdded, strBuff);
    strcat(strDelimAdded, "`");

    debug.log(F("Sending '"), false);
    debug.log(strBuff, false);
    debug.log(F("', "), false);
    debug.log(nSize, false);
    if (nSize > 1)
      debug.log(F(" bytes "), false);
    else
      debug.log(F(" byte "), false);

    if (m_pSerialSrc == m_pUDPServer)
    {
      debug.log(F("via UDP to "), false);
      WiFiEspUDP *pUDP = (WiFiEspUDP*)m_pSerialSrc;
      pUDP->beginPacket(ip, nPort);
      nSize = pUDP->write(strDelimAdded, strlen(strDelimAdded));
      pUDP->endPacket();
    }
    else
    {
      debug.log(F(" via serial to "));
      nSize = m_pSerialSrc->write(strDelimAdded);
    }
    debug.log(ip, false);
    debug.log(F(" on port "), false);
    debug.log(nPort, false);
    debug.log(F("..."), false);
    if (nSize == strlen(strDelimAdded))
      debug.log(F("done!"));
    delete strDelimAdded;
  }
}

bool CSerialManager::readWifiSSIDKey(CString& strNetworkName, CString& strNetworkKey)
{
  bool bResult = true;

  //EEPROM.writeBlock<char>(0, "Telstra6A61B7\0\0\0\0\0\0\0\0\0\0\0", m_nBuffSize);
  //EEPROM.writeBlock<char>(m_nBuffSize + 1, "0AC9703D29\0\0\0\0\0\0\0\0\0\0\0\0", m_nBuffSize);
  
  EEPROM.readBlock<char>(0, strNetworkName, m_nBuffSize);
  EEPROM.readBlock<char>(m_nBuffSize + 1, strNetworkKey, m_nBuffSize);
  bResult = !strNetworkName.isEmpty() && !strNetworkKey.isEmpty();

  return bResult;
}

void CSerialManager::writeWifiSSIDKey(CString& strNetworkName, CString& strNetworkKey)
{
  EEPROM.updateBlock<char>(0, strNetworkName, strNetworkName.capacity());
  EEPROM.updateBlock<char>(strNetworkName.capacity(), strNetworkKey, strNetworkKey.capacity());
}


