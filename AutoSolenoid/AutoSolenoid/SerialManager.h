#pragma once
#include <Arduino.h>
#include <WiFiEspUdp.h>
#include "CString.h"




class CSerialManager
{
  public:
    //Construction & destruction
    CSerialManager(Stream* pSerialSrc, WiFiEspUDP* pUDPServer, HardwareSerial* pSerialHC05, const uint8_t nRelayPin, const uint8_t nStationNum);
    ~CSerialManager();

    // Interface
    void processData();
    void turnOn();
    void turnOff();
    void notifyController(const char* strIP = NULL);
    const char* getNetworkName()
    {
      return m_strNetworkName;
    };
    const char* getNetworkKey()
    {
      return m_strNetworkKey;
    };

  protected:
    // Data
    Stream *m_pSerialSrc;
    WiFiEspUDP* m_pUDPServer;
    HardwareSerial* m_pSerialHC05;
    uint8_t m_nRelayPin, m_nStationNum;
    static const uint8_t m_nBuffSize = 24;
    CBuff<m_nBuffSize> m_buffNetworkName, m_buffNetworkKey;
    CString m_strNetworkName, m_strNetworkKey;
    const __FlashStringHelper* m_strDelim;

    // Helpers    
    bool readWord(CString& strBuff);
    bool writeWord(const char *strBuff);
    bool writeWord(const char *strBuff, IPAddress ip, const uint16_t nPort = 10002);
    bool readIPAddr(IPAddress& m_ipIrrigationController);
    void writeIPAddr(IPAddress& m_ipIrrigationController);
    bool readWifiSSIDKey(CString& strNetworkName, CString& strNetworkKey);
    void writeWifiSSIDKey(CString& strNetworkName, CString& strNetworkKey);
};

