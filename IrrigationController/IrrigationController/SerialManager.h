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
		void processData(Stream *pSerialHC05);
		void processData(WiFiEspUDP *pUDPServer);
    bool readWord(CString& strBuff);
    bool writeWord(const char *strBuff)
    {
      writeWord(strBuff, m_pUDPServer->remoteIP(), m_pUDPServer->remotePort());
    };
    bool writeWord(const __FlashStringHelper *strBuff)
    {
      writeWord(strBuff, m_pUDPServer->remoteIP(), m_pUDPServer->remotePort());
    };
    bool writeWord(const char *strBuff, IPAddress ip, const uint16_t nPort = 10002);
    bool writeWord(const __FlashStringHelper *strBuff, IPAddress ip, const uint16_t nPort = 10002);
    const char* getDelim()
    {
      return m_strDelimiter;
    };
    
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
};

#endif
