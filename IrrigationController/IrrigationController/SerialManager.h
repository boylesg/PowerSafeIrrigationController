#ifndef __SERIALMANAGER_H_
#define __SERIALMANAGER_H_

#include <Arduino.h>
#include <WiFiEspUdp.h>
#include "WifiManager.h"
#include "Program.h"




class CSerialManager
{
	public:
		// Construction, destruction & initialisation
		CSerialManager(CWifiManager &WifiManager, CProgram &program);
		virtual ~CSerialManager();

		// Interface
    #ifdef AUTO_SOLENOID
      void getRemoteStations(WiFiEspUDP *pUDPServer, CProgram& program);
    #endif
    void processData(Stream *pSerialHC05);
    void processData(WiFiEspUDP *pUDPServer);
    bool readWord(CString& strBuff);
    bool writeWord(const char *strBuff, char *pIPAddr = m_strRemoteIP, const uint16_t nPort = 10004);
    bool writeWord(const __FlashStringHelper *strBuff, char *strIPAddr = m_strRemoteIP, const uint16_t nPort = 10004);
    const char* getDelim()
    {
      return m_strDelimiter;
    };
    void setRemote(const char* strIPAddr);
    
	protected:
		// Helpers
		void processAlarmData();
		void processIrrigProgData();
    void processWirelessAlarmData();

		void updateWifiSSIDKey();
		void updateEmailSettings();
		void updateSytemTime();

		void uploadFile();
    uint16_t getFileSize();
		void downloadFile();
		void processData();

		void empty();
    void dump();

		// Data
		CWifiManager &m_WifiManager;
		CProgram &m_program;
		WiFiEspUDP *m_pUDPServer;
		Stream* m_pSerialHC05;
    static const char m_cDelimiter;
    static const char *m_strDelimiter;
    static char m_strRemoteIP[16];
};

#endif

