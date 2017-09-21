#ifndef __WIFIMANAGER_H_
#define __WIFIMANAGER_H_


#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>
#include "DateTime.h"
#include "TextFile.h"
#include "CString.h"




class CWifiManager
{
	private:
		// Construction, destruction and initialisation
		CWifiManager();

	public:
		// Construction, destruction and initialisation
		CWifiManager(Stream* pSerial);
		virtual ~CWifiManager();
		bool begin();

		// Interface
		WiFiEspClient getClient();
		void processRequest();
		bool available()
		{
			return m_HTTPServer.available() > 0;
		};

		void updateWifiSSIDKey(const char *strNetworkName, const char *strNetworkKey, CString &strMsg);
		void updateEmailSettings(const char *strEmail, const char *strMailServer, const char *strUsername, const char *strPassword, CString &strMsg);
		void updateSytemTime(CDateTime &datetime, const bool bAdjustDaylighSavings, const uint8_t nTimezoneAdjust, const char *strID, CString &strMsg);

		// Miscellaneous
		bool sendEmail(CString& strSubject, CString& strMsg);
		bool checkLowRTCBatteryVoltage();
		void synchClock();

		// Individual data items
		const char* getControllerID()
		{
			return m_strControllerID;
		}
		const char* getNetworkName()
		{
			return m_strNetworkName;
		}
		const char* getNetworkKey()
		{
			return m_strNetworkKey;
		}
		const char* getEmailAddress()
		{
			return m_strToEmail;
		}
		const char* getMailServer()
		{
			return m_strMailServer;
		}
		const char* getUsername()
		{
			return m_strUsername;
		}
		const char* getPassword()
		{
			return m_strPassword;
		}
		bool getDaylightSavings()
		{
			return m_bAdjustDaylightSaving;
		}
		int8_t getTimezone()
		{
			return m_nTimezone;
		} 
    WiFiEspUDP* getUDPServer()
    {
      return &m_UDPServer;
    };

	protected:
		// Helper functions
		bool readEmailSettings();
		bool readWifiSSIDKey(CString &strSSID, CString &strKey);
		bool readTimeZone();

		void processHTTPResponse(WiFiEspClient& WifiClient);
		void processHTTPRequest(WiFiEspClient &WifiClient, CString &strRequest);
		void processUDPRequest(WiFiEspUDP *pUDPServer);
    void processWirlessAlarmData(CString &strRequest);

		float getRTCBatteryVoltage();
		bool sendIrrigProgContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg);
		bool sendManualContent(WiFiEspClient& WifiClient, uint8_t nStation, const uint16_t nRuntime, const uint8_t nStatus);
		bool sendAlarmContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg);
		bool sendEmailContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg);
		bool sendSystemClockContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg);
		bool sendFile(WiFiEspClient& WifiClient, const char* strFileName);
		bool sendIcon(WiFiEspClient& WifiClient);
		#ifndef __SAM3X8E__
			void sendFileErrorResponse(WiFiEspClient& WifiClient, const __FlashStringHelper* strFileName);
		#endif
		void sendFileErrorResponse(WiFiEspClient& WifiClient, const char* strFileName);
		void saveIrrigProgPageData(const uint8_t nStationNum, const uint8_t nRadioFreq, const uint8_t nSelectFreq, const uint8_t nRuntime, const uint8_t nRunFreq, const char* strStartTime, const char* strEndTime);
		bool processClockFormData(CString& strFormData, CString& strMsg);
		bool processProgramFormData(CString& strFormData, CString& strMsg);
		bool processManualFormData(CString& strFormData, uint8_t& nStation, uint16_t& nRuntime);
		bool processAlarmFormData(CString& strFormData, CString& strMsg);
		bool processEmailFormData(CString& strFormData, CString& strMsg);
		bool isHexDigit(const char cCh);
		void displayError(const __FlashStringHelper* strFilename);
		void writeToClient(WiFiEspClient& WifiClient, const __FlashStringHelper* strData, const int nLength);
		void writeToClient(WiFiEspClient& WifiClient, const char* strData, const int nLength);
		void sendHTTPHeader(WiFiEspClient& WifiClient);
    
		// Data - WiFiEsp stuff
		WiFiEspServer m_HTTPServer;
		WiFiEspUDP m_UDPServer;
		Stream* m_pSerial;

		// Data - http file names
		const __FlashStringHelper* m_strHTTPProgramFilename;
		const __FlashStringHelper* m_strHTTPManualFilename;
		const __FlashStringHelper* m_strHTTPAlarmsFilename;
		const __FlashStringHelper* m_strHTTPClockFilename;
		const __FlashStringHelper* m_strHTTPEmailFilename;
    const __FlashStringHelper* m_strHTTPUploadFilename;

		// Data - text based data file names
		const __FlashStringHelper* m_strTXTWifiFilename;
		const __FlashStringHelper* m_strFaviconFileName;
		const __FlashStringHelper* m_strTXTEmailFilename;
		const __FlashStringHelper* m_strTXTTimezoneFilename;

		// Data - miscellaneous
		const float m_fMinRTCBatteryVoltage;
		uint16_t m_nBytesRead, m_nSendDelay;

		// Data - wifi stuff
		CBuff<41> m_buffNetworkName, m_buffNetworkKey;
		CString m_strNetworkName, m_strNetworkKey;
    
		// Data - email stuff
		CBuff<41> m_buffFromEmail, m_buffToEmail, m_buffMailServer, m_buffUsername, m_buffPassword;
    CBuff<17> m_buffIPAddr;
		CString m_strFromEmail, m_strToEmail, m_strMailServer, m_strUsername, m_strPassword, m_strIPAddr;

		// Data - controller ID
		CBuff<13> m_buffControllerID;
		CString m_strControllerID;

		// Data - clock stuff
		int8_t m_nTimezone;
		bool m_bAdjustDaylightSaving;
};

#endif


