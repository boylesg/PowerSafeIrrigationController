#ifndef __WIFIMANAGER_H_
#define __WIFIMANAGER_H_

#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>
#include <SMTPClient.h>
#include "TextFile.h"
#include "CString.h"




class CWifiManager
{
  private:
    // Construction, destruction and initialisation
    CWifiManager();

  public:
    // Construction, destruction and initialisation
    CWifiManager(HardwareSerial* pSerial);
    virtual ~CWifiManager();
    bool begin();

    // Interface
    WiFiEspClient getClient();
    bool processRequest();
    bool available()
    {
      return m_HTTPServer.available() > 0;
    };
    void doLowBatteryWarning(const float fVoltage);
    void checkAlarm(const uint16_t nReading);

    void updateWifiSSIDKey(const char *strNetworkName, const char *strNetworkKey, CString &strMsg);

    // Miscellaneous
    bool sendEmail(CString& strSubject, CString& strMsg);

    // Individual data items
    const char* getStationDesc()
    {
      return m_strStationDesc;
    };
  	uint8_t getStationNum()
  	{
  		return m_nStationNum;
  	};
  	const char* getNetworkName()
    {
      return m_strNetworkName;
    };
    const char* getNetworkKey()
    {
      return m_strNetworkKey;
    };
    uint8_t getDryTime()
    {
      return m_nDryTime;
    };
    uint8_t getOnTime()
    {
      return m_nOnTime;
    };
    uint16_t getThreshold()
    {
      return m_nThresh;
    };
    const char *getDryTimeStr();
    const char *getOnTimeStr();
    const char *getThresholdStr();
    bool getAuto()
    {
      return m_bAuto;
    };
    const char *getControllerID()
    {
      return m_strControllerID;
    };
    const char *getIP(const uint8_t nI);

	  bool saveStationDetails(const uint8_t nStationNum, const char* strStationDesc);
    bool saveAlarmDetails(const bool bAuto, const uint8_t nDryTime, const uint8_t nOnTime, const uint16_t nThresh);
    bool saveWifiDetails(const char *strName, const char *strKey);
    bool saveControllerDetails(const char *strControllerID, IPAddress &ipaddr);

  protected:
    // Helper functions
    bool readControllerDetails(CString &strMsg);
    bool saveControllerDetails(CString &strMsg);
    bool readWifiDetails(CString &strMsg);
    bool saveWifiDetails(CString &strMsg);
    bool readAlarmsDetails(CString &strMsg);
	  bool saveAlarmDetails(CString &strMsg);
	  bool saveStationDetails(CString &strMsg);
	  bool writeUDPWord(const __FlashStringHelper *strBuff);
    bool writeUDPWord(const char *strBuff);

    bool processHTTPRequest(WiFiEspClient &WifiClient, CString &strRequest);
    bool processUDPRequest(WiFiEspUDP *pUDPServer);

    bool sendAlarmContent(WiFiEspClient& WifiClient, const bool bFormResponse);
    bool processAlarmFormData(CString& strFormData);
    bool sendControllerContent(WiFiEspClient& WifiClient, const bool bFormResponse);
    bool processControllerFormData(CString& strFormData);
    bool sendStationContent(WiFiEspClient& WifiClient, const bool bFormResponse);
    bool processStationFormData(CString& strFormData);
    bool sendFile(WiFiEspClient& WifiClient, const char* strFileName);
    bool sendIcon(WiFiEspClient& WifiClient);
    #ifndef __SAM3X8E__
      void sendErrorResponse(WiFiEspClient& WifiClient, const __FlashStringHelper* strFileName);
    #endif
    void sendErrorResponse(WiFiEspClient& WifiClient, const char* strFileName);
    bool isHexDigit(const char cCh);
    void writeToClient(WiFiEspClient& WifiClient, const __FlashStringHelper* strData, const int nLength);
    void writeToClient(WiFiEspClient& WifiClient, const char* strData, const int nLength);
    void sendHTTPHeader(WiFiEspClient& WifiClient);
    uint8_t getElapsedMinutes();
    
    // Data - WiFiEsp stuff
    WiFiEspServer m_HTTPServer;
    WiFiEspUDP m_UDPServer;
    HardwareSerial* m_pSerial;

    // Data - http file names
    const __FlashStringHelper* m_strHTTPAlarmFilename;
    const __FlashStringHelper* m_strHTTPControllerFilename;
    const __FlashStringHelper* m_strHTTPStationFilename;


    // Data - text based data file names
    const __FlashStringHelper* m_strTXTWifiFilename;
    const __FlashStringHelper* m_strTXTAlarmFilename;
    const __FlashStringHelper* m_strFaviconFileName;
    const __FlashStringHelper* m_strTXTControllerFilename;

    // Data - wifi stuff
    CBuff<41> m_buffNetworkName, m_buffNetworkKey;
    CString m_strNetworkName, m_strNetworkKey;

    // Data - alarm details
    CBuff<13> m_buffStationDesc;
    CString m_strStationDesc;
    const uint16_t m_nIrrigControllerPort = 10002;
    uint8_t m_nDryTime, m_nOnTime;
    int8_t m_nStationNum;
    uint16_t m_nThresh;
    bool m_bAuto;
    uint32_t m_nLastMillis;

    // Data - controller details
    IPAddress m_ipaddrController;
    CBuff<24> m_buffControllerD;
    CString m_strControllerID;

    // Data - misc
    uint16_t m_nBytesRead;
    static const char *m_strDelimiter;
    CBuff<5> m_buff;
};

#endif


