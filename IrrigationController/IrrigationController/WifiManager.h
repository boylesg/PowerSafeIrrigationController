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
		bool sendEmail(const char* strSubject, const char* strMsg);
		bool checkLowRTCBatteryVoltage();
		void synchClock();

		// Individual data items
		const char* getControllerID()
		{
			return m_buffControllerID;
		}
		const char* getNetworkName()
		{
			return m_buffNetworkName;
		}
		const char* getNetworkKey()
		{
			return m_buffNetworkKey;
		}
		const char* getEmailAddress()
		{
			return m_buffToEmail;
		}
		const char* getMailServer()
		{
			return m_buffMailServer;
		}
		const char* getUsername()
		{
			return m_buffUsername;
		}
		const char* getPassword()
		{
			return m_buffPassword;
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

    float getRTCBatteryVoltage();

	protected:
		// Helper functions
		bool readEmailSettings();
		bool readWifiSSIDKey(CString &strSSID, CString &strKey);
		bool readTimeZone();

		void processHTTPResponse(WiFiEspClient& WifiClient);
		void processHTTPRequest(WiFiEspClient &WifiClient, CString &strRequest);
		void processUDPRequest(WiFiEspUDP *pUDPServer);
    bool processLoginFormData(CString& strFormData);

    bool sendLogFileContent(WiFiEspClient& WifiClient);
		bool sendIrrigProgContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg);
		bool sendManualContent(WiFiEspClient& WifiClient, uint8_t nStation, const uint16_t nRuntime, const uint8_t nStatus);
		bool sendAlarmContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg);
		bool sendEmailContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg);
		bool sendSystemClockContent(WiFiEspClient& WifiClient, const bool bFormResponse, const char* strMsg);
		bool sendFile(WiFiEspClient& WifiClient, const char* strFileName);
		bool sendIcon(WiFiEspClient& WifiClient);
    bool sendLoginContent(WiFiEspClient& WifiClient, const __FlashStringHelper* strMsg);
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
		void debugFileError(const __FlashStringHelper* strFilename, const bool bFind);
		void writeToClient(WiFiEspClient& WifiClient, const __FlashStringHelper* strData, const int nLength);
		void writeToClient(WiFiEspClient& WifiClient, const char* strData, const int nLength);
		void sendHTTPHeader(WiFiEspClient& WifiClient);
    
		// Data - WiFiEsp stuff
		WiFiEspServer m_HTTPServer;
		WiFiEspUDP m_UDPServer;
		Stream* m_pSerial;

		// Data - miscellaneous
    int32_t m_nSessionTimer;

		// Data - wifi stuff
		CBuff<24> m_buffNetworkName, m_buffNetworkKey;
    
		// Data - email stuff
		CBuff<40> m_buffFromEmail, m_buffToEmail, m_buffMailServer, m_buffUsername, m_buffPassword;
    CBuff<16> m_buffIPAddr;

		// Data - controller ID
		CBuff<12> m_buffControllerID;

		// Data - clock stuff
		int8_t m_nTimezone;
		bool m_bAdjustDaylightSaving;
};

#endif




