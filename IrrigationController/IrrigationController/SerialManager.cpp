#include "CString.h"
#include "Common.h"
#include "debug.h"
#include "TextFile.h"
#include "SerialManager.h"




const char CSerialManager::m_cDelimiter = '`';
const char *CSerialManager::m_strDelimiter = "`";
char CSerialManager::m_strRemoteIP[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

CSerialManager::CSerialManager(CWifiManager &WifiManager, CProgram &program): m_WifiManager(WifiManager), m_program(program)
{
	m_pUDPServer = NULL;
	m_pSerialHC05 = NULL;
  memset(m_strRemoteIP, 0, sizeof m_strRemoteIP);
}

CSerialManager::~CSerialManager()
{
}

void CSerialManager::setRemote(const char* strIPAddr)
{
  strcpy(m_strRemoteIP, strIPAddr);
}

void CSerialManager::empty()
{
	if (m_pUDPServer)
	{
		while (m_pUDPServer->available() > 0)
			m_pUDPServer->read();
	}
	else if (m_pSerialHC05)
	{
		while (m_pSerialHC05->available() > 0)
			m_pSerialHC05->read();
	}
}

void CSerialManager::dump()
{
  if (m_pUDPServer)
  {
    while (m_pUDPServer->available() > 0)
      debug.log(m_pUDPServer->read());
  }
  else if (m_pSerialHC05)
  {
    while (m_pSerialHC05->available() > 0)
      debug.log(m_pSerialHC05->read());
  }
}

void CSerialManager::processAlarmData()
{
	uint8_t nStation = 0, nRuntime = 0, nDryTime = 0, nI = 0;
	uint16_t nThreshold = 0;
	bool bInstalled = false, bAuto = false;
	CBuff<4> Buff4;
	CBuff<128> Buff128;
	CString strWord(Buff4), strMsg(Buff128);

	for (nI = 1; nI <= MAX_STATIONS; nI++)
	{
		if (readWord(strWord))
		{
			nStation = strWord.toUint();

			if ((nStation >= 1) && (nStation <= MAX_STATIONS))
			{
				if (readWord(strWord) && ((strWord == F("1")) || (strWord == F("0"))))
				{
					bInstalled = strWord == "1";

					if (readWord(strWord) && ((strWord == F("1")) || (strWord == F("0"))))
					{
						bAuto = strWord == "1";

						if (readWord(strWord))
						{
							nDryTime = strWord.toUint();

							if ((nDryTime >= 0) && (nDryTime <= 120))
							{
								if (readWord(strWord))
								{
									nThreshold = strWord.toUint();

									if ((nThreshold >= 0) && (nThreshold <= 2000))
									{
										if (readWord(strWord))
										{
											nRuntime = strWord.toUint();

											if ((nRuntime >= 0) && (nRuntime <= 30))
											{
												m_program.updateAlarmData(nStation, bInstalled, bAuto, nDryTime, nThreshold, nRuntime);
                        strMsg = F("Upload for station ");
                        strMsg += fromUint(nStation, 10);
                        strMsg += F(" was successful!");
                        writeWord(strMsg);
											}
											else
											{
												debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
												break;
											}
										}
										else
										{
											debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
											break;
										}
									}
									else
									{
										debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
										break;
									}
								}
								else
								{
									debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
									break;
								}
							}
							else
							{
								debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
								break;
							}
						}
						else
						{
							debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
							break;
						}
					}
					else
					{
						debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
						break;
					}
				}
				else
				{
					debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
					break;
				}
			}
			else
			{
				debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
				break;
			}
		}
		else
		{
			debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
			break;
		}
	}
	if (nI > MAX_STATIONS)
	{
		if (m_program.saveAlarmsData(strMsg))
      debug.log(F("Upload successful!"));
		else
      debug.log(F("Upload was unsuccessful!"));
	}
  else
  {
    empty();
    debug.log(F("Upload was unsuccessful!"));
  }
}

//*************************************************************************************************
// Specifically process the incoming irrigation program data
//*************************************************************************************************
void CSerialManager::processIrrigProgData()
{
	uint8_t nStation = 0, nRadioFreq = 0, nSelFreq = 0, nRuntime = 0, nRunFreq = 0, nI = 0;
	CBuff<6> Buff6One, Buff6Two, Buff6Three, Buff6Four;
	CBuff<16> Buff16;
	CBuff<12> Buff12;
	CBuff<128> Buff128;
	CBuff<4> Buff4;
	CString strStartDate(Buff6One), strEndDate(Buff6Two), strDesc(Buff16), strStartTime(Buff6Three), strEndTime(Buff6Four), strStation(Buff12), strMsg(Buff128), strWord(Buff4);
	CTextFile fileIrrigProgPageData, fileIrrigProg;

	strMsg = F("Error reading bluetooth data - please try again!");

	if (m_program.openIrrigProgramPageDataFile(fileIrrigProgPageData, strMsg) && m_program.openIrrigProgramFile(fileIrrigProg, strMsg))
	{
		debug.log(F("------------------------------"));
		
		for (nI = 1; nI <= MAX_STATIONS; nI++)
		{
			if (readWord(strStation))
      {
        nStation = strStation.toUint();
        if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  			{
  			  readWord(strDesc);
  				readWord(strStartDate);
  				readWord(strEndDate);
  
  				readWord(strWord);
  				nRadioFreq = strWord.toUint();
  				if ((nRadioFreq >= 1) && (nRadioFreq <= 3))
  				{
  					readWord(strWord);
  					nSelFreq = strWord.toUint();
  					if ((nSelFreq >= 0) && (nSelFreq <= 5))
  					{
  						readWord(strWord);
  						nRunFreq = strWord.toUint();
  						if ((nRunFreq >= 0) && (nRunFreq <= 5))
  						{
  							readWord(strWord);
  							nRuntime = strWord.toUint();
  							readWord(strStartTime);
  							readWord(strEndTime);
  
								debug.dump(F("nStation"), nStation);
								debug.dump(F("strStartDate"), strStartDate);
								debug.dump(F("strEndDate"), strEndDate);
								debug.dump(F("nRadioFreq"), nRadioFreq);
								debug.dump(F("nSelFreq"), nSelFreq);
								debug.dump(F("nRunFreq"), nRunFreq);
								debug.dump(F("nRuntime"), nRuntime);
								debug.dump(F("strStartTime"), strStartTime);
								debug.dump(F("strEndTime"), strEndTime);
								debug.log(F("------------------------------"));
 
								m_program.saveIrrigProgPageData(nStation, fileIrrigProgPageData, strDesc, nRadioFreq, nSelFreq, nRuntime, nRunFreq, strStartTime, strEndTime, strStartDate, strEndDate);
 								m_program.saveIrrigProg(nStation, fileIrrigProg, m_program.getDayInterval(nRadioFreq, nSelFreq), strStartTime, strEndTime, 
								m_program.getHourInterval(nRunFreq), nRuntime, strStartDate, strEndDate, strDesc);
  
                strMsg = F("Upload for station ");
                strMsg += fromUint(nStation, 10);
                strMsg += F(" was successful!");
                writeWord(strMsg);
  						}
  						else
  						{
  							debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
  							break;
  						}
  					}
  					else
  					{
  						debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
  						break;
  					}
  				}
  				else
  				{
  					debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
  					break;
  				}
  			}
  			else
  			{
  				debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
  				break;
  			}
      }
      else
      {
        debug.logRuntimeError(F("Serialmanager.cpp"), __LINE__);
        break;
      }
		}
    fileIrrigProgPageData.close();
    fileIrrigProg.close();
	}
  if (nI > MAX_STATIONS)
    debug.log(F("Upload successful!"));
  else
  {
    debug.log(F("Upload was unsuccessful!"));
		empty();
  }
}

uint16_t CSerialManager::getFileSize()
{
  CBuff<128> Buff128;
  CBuff<16> Buff16;
  CBuff<8> Buff8;
  CString strFilename(Buff16), strMsg(Buff128), strSize(Buff8);
  uint32_t nSize = 0;
  CTextFile file;

  readWord(strFilename);
  if (file.open(strFilename, READ))
  {
    nSize = file.size();
    strSize.fromUint(nSize);
    file.close();
  }
  strMsg = F("Sending size of '");
  strMsg += strFilename;
  strMsg += F("'");
  writeWord(strSize);
}

void CSerialManager::downloadFile()
{
  CBuff<16> Buff16;
  CBuff<128> BuffMsg;
  CBuff<64> BuffData;
  CBuff<8> Buff8;
  CString strFilename(Buff16), strFileData(BuffData), strFilePos(Buff8), strMsg(BuffMsg);
  uint32_t nFileSize = 0;

  readWord(strFilename);
  readWord(strFilePos);
  nFileSize = strFilePos.toUint();
  
  debug.dump(F("strFilename"), strFilename);
  debug.dump(F("strFilePos"), strFilePos);
  
	strMsg = F("Sending file '");
	strMsg += strFilename;
	strMsg += F("', file position ");
  strMsg += strFilePos;

	strMsg = F("<ERROR>");

	if (SD.exists(strFilename))
	{
		CTextFile file;

		strFilename.toLower();
		if (file.open(strFilename, READ))
		{
      file.seek(nFileSize);
      if (file.read(strFileData))
      {
        writeWord(strFileData);  
      }
      else
      {
        strMsg = m_WifiManager.getControllerID();
        strMsg += F(" - read on '");
        strMsg += strFilename;
        strMsg += F("' failed!");
        writeWord(strMsg);
        debug.log(strMsg);
      }
		}
		else
		{
			strMsg = m_WifiManager.getControllerID();
			strMsg += F(" - could not open '");
			strMsg += strFilename;
			strMsg += F("'!");
      writeWord(strMsg);
      debug.log(strMsg);
		}
	}
	else
	{
		strMsg = m_WifiManager.getControllerID();
		strMsg += F(" - you cannot download this file!");
		empty();
    writeWord(strMsg);
    debug.log(strMsg);
	}
}

void CSerialManager::uploadFile()
{
	CBuff<16> Buff16;
	CBuff<64> BuffData;
	CBuff<128> Buff128;
  CBuff<8> Buff8;
	CString strFilename(Buff16), strFileData(BuffData), strMsg(Buff128), strByteNum(Buff8);
  uint32_t nByteNum = 0;
  CTextFile file;

  readWord(strFilename);
  readWord(strByteNum);
  nByteNum = strByteNum.toUint();
  readWord(strFileData);

  if (SD.exists(strFilename))
  {
    strFilename.toLower();
    if (nByteNum == 0)
    {
      if (file.open(strFilename, REWRITE))
      {
        file.write(strFileData);
        file.close();
        writeWord(F("OK"));
      }
      else
      {
        strMsg = m_WifiManager.getControllerID();
        strMsg += F(" - could not open '");
        strMsg += strFilename;
        strMsg += F("'!");
        empty();
        debug.log(strMsg);
        writeWord(strMsg);
        empty();
      }
    }
    else
    {
      if (file.open(strFilename, APPEND))
      {
        file.seek(file.size());
        file.write(strFileData);
        file.close();
        writeWord(F("OK"));
      }  
      else
      {
        strMsg = m_WifiManager.getControllerID();
        strMsg += F(" - could not open '");
        strMsg += strFilename;
        strMsg += F("'!");
        empty();
        debug.log(strMsg);
        writeWord(strMsg);
        empty();
      }
    }
  }
  else
  {
    strMsg = m_WifiManager.getControllerID();
    strMsg += F(" - you cannot upload this file!");
    debug.log(strMsg);
    writeWord(strMsg);
    empty();
  }
	if (strFilename == F("wifi.txt"))
		resetArd();
}

void CSerialManager::updateEmailSettings()
{
	CBuff<16> SmallBuff;
	CBuff<40> Buff1, Buff2, Buff3, Buff4;
	CBuff<128> Buff5;
	CString strEmail(Buff1), strMailServer(Buff2), strUsername(Buff3), strPassword(Buff4), strMsg(Buff5);

	debug.log(F("Updating email details\n"));
	strMsg = F("The bluetooth data was interrupted - please try again!");

	readWord(strEmail);
	readWord(strMailServer);
	readWord(strUsername);
	readWord(strPassword);

	debug.log(F("From HC-05...\n"));
	debug.dump(F("Email address"), strEmail);
	debug.dump(F("Outgoing mail server"), strMailServer);
	debug.dump(F("Mailer server username"), strUsername);
	debug.dump(F("Mail server password"), strPassword);

	m_WifiManager.updateEmailSettings(strEmail, strMailServer, strUsername, strPassword, strMsg);

	debug.log(strMsg);
	writeWord(strMsg);
}

void CSerialManager::updateSytemTime()
{
	CBuff<16> Buff1, Buff2, Buff3;
	CBuff<128> Buff128;
	CString strWord(Buff1), strID(Buff2), strFilename(Buff3), strMsg(Buff128);
	uint8_t nDOW = 0, nDOM = 0, nMonth = 0, nHour = 0, nMinute = 0, nSecond = 0, nTimezoneAdjust = 0;
	uint16_t nYear = 0;
	bool bAdjustDaylighSavings = false;

	debug.log(F("Updating system time\n"));
	strMsg = F("The bluetooth data was interrupted - please try again!");
	readWord(strWord);
	if (!strWord.isEmpty())
	{
		nDOM = strWord.toUint();
		readWord(strWord);
		if (!strWord.isEmpty())
		{
			nMonth = strWord.toUint();
			readWord(strWord);
			if (!strWord.isEmpty())
			{
				nYear = strWord.toUint() - CDate::m_nCentury;
				readWord(strWord);
				if (!strWord.isEmpty())
				{
					nHour = strWord.toUint();
					rtc.setHours(strWord.toUint());
					readWord(strWord);
					if (!strWord.isEmpty())
					{
						nMinute = strWord.toUint();
						readWord(strWord);
						if (!strWord.isEmpty())
						{
							nSecond = strWord.toUint();
							readWord(strWord);
							if (!strWord.isEmpty())
							{
								bAdjustDaylighSavings = strWord.toUint() > 0;
								readWord(strWord);
								if (!strWord.isEmpty())
								{
									nTimezoneAdjust = strWord.toUint();
									readWord(strID);
									if (!strID.isEmpty())
									{
										CDateTime datetime;
										datetime.set(nDOM, nMonth, nYear, nHour, nMinute, nSecond);
										nDOW = datetime.getDOW();

										debug.log(F("From HC-05...\n"));
										debug.dump(F("Day of month"), nDOM);
										debug.dump(F("Month"), nMonth);
										debug.dump(F("Year"), nYear);
										debug.dump(F("Hour"), nHour);
										debug.dump(F("Minute"), nMinute);
										debug.dump(F("Second"), nDOW);
										debug.dump(F("Timezone adjustment"), nTimezoneAdjust);
										debug.dump(F("Daylight savings"), bAdjustDaylighSavings ? F("yes") : F("no"));
										debug.dump(F("Controller ID"), strID);
										m_WifiManager.updateSytemTime(datetime, bAdjustDaylighSavings, nTimezoneAdjust, strID, strMsg);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	debug.log(strMsg);
	writeWord(strMsg);
}

void CSerialManager::updateWifiSSIDKey()
{
	CBuff<128> BuffMsg;
	CBuff<40> BuffName, BuffKey;
	CString strNetworkName(BuffName), strNetworkKey(BuffKey), strMsg(BuffMsg);
  
	readWord(strNetworkName);
	readWord(strNetworkKey);

	debug.log(F("SSID = "), false);
	debug.log(strNetworkName, false);
	debug.log(F(", KEY = "), false);
	debug.log(strNetworkKey, true);

	if ((strNetworkName.length() > 0) && (strNetworkKey.length() > 0))
	{
		m_WifiManager.updateWifiSSIDKey(strNetworkName, strNetworkKey, strMsg);
    debug.log(strMsg);
		writeWord(strMsg);
    delay(5000);
		resetArd();
  }
  else
  {
    strMsg = F("Bluetooth data error - please try again!");
    debug.log(strMsg);
    writeWord(strMsg);
  }
}

bool CSerialManager::writeWord(const char *strBuff, char *strIPAddr, const uint16_t nPort)
{
  int nSize = strlen(strBuff), nSendSize = 0;

  char *strDelimAdded = new char[nSize + 2];
  if (strDelimAdded)
  {
    memset(strDelimAdded, 0, sizeof strDelimAdded);
    strcpy(strDelimAdded, strBuff);
    strcat(strDelimAdded, m_strDelimiter);

  	if (m_pUDPServer)
  	{
      IPAddress ip;
      ip.fromString(strIPAddr);
		  m_pUDPServer->beginPacket(ip, nPort);
      nSendSize = m_pUDPServer->write(strDelimAdded, strlen(strDelimAdded));
		  m_pUDPServer->endPacket();
	  }
	  else if (m_pSerialHC05)
	  {
      nSendSize = m_pSerialHC05->write(strDelimAdded);
	  }
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
  else
    debug.logRuntimeError(F("SerialManager.cpp"), __LINE__);
   
  return nSendSize == nSize;
}

bool CSerialManager::writeWord(const __FlashStringHelper *strBuff, char *strIPAddr, const uint16_t nPort)
{
  uint16_t nSize = strlen_P((PGM_P)strBuff), nSentSize = 0;
 
  if (nSize > 0)
  {
    char *strDelimAdded = new char[nSize + 2];
    if (strDelimAdded)
    {
      if (strDelimAdded)
      {
        memset(strDelimAdded, 0, nSize + 2);
        strcpy_P(strDelimAdded, (PGM_P)strBuff);
        strcat(strDelimAdded, m_strDelimiter);
        
        if (m_pUDPServer)
        {
          IPAddress ip;
          ip.fromString(strIPAddr);
debug.log(strIPAddr);
debug.log(nPort);
          m_pUDPServer->beginPacket(ip, nPort);
          nSentSize = m_pUDPServer->write(strDelimAdded, strlen(strDelimAdded));
          m_pUDPServer->endPacket();
        }
        else if (m_pSerialHC05)
        {
          nSentSize = m_pSerialHC05->write(strDelimAdded);
        }
        debug.log(F("Sending '"), false);
        debug.log(strBuff, false);
        debug.log(F("', "), false);
        debug.log(nSize, false);
        debug.log(F(" bytes."));
        delete strDelimAdded;
      }
    }
    else
      debug.logRuntimeError(F("SerialManager.cpp"), __LINE__);
  }
  else
    debug.logRuntimeError(F("SerialManager.cpp"), __LINE__);

  return nSize == nSentSize;
}

bool CSerialManager::readWord(CString& strBuff)
{
	char cCh = 0;
	uint32_t nMillis = millis(), nTimeout = (uint32_t)10000;
  Stream *pSerialSrc = NULL;

	strBuff.empty();
	if (m_pUDPServer)
    pSerialSrc = m_pUDPServer;
  else if (m_pSerialHC05)
    pSerialSrc = m_pSerialHC05;
    
	if (pSerialSrc)
  { 
  	while (true)
  	{
  		if (strBuff.full())
  			break;

  		if ((millis() - nMillis) > nTimeout)
  			break;

  		if ((pSerialSrc->available() > 0))
  		{
  			nMillis = millis();
  			cCh = pSerialSrc->read();
  			if (cCh != m_cDelimiter)
        {
  				strBuff += cCh;
        }
        else
        {
          break;
        }
  		}
  	}
  }
	return strBuff.length() > 0;
}

void CSerialManager::processWirelessAlarmData()
{
  CBuff<16> BuffControllerID;
  CBuff<5> BuffNum;
  CBuff<17> BuffDesc;
  CBuff<129> buff1, buff2;
  CString strControllerID(BuffControllerID), strNum(BuffNum), strStationDesc(BuffDesc), strMsg(buff1), strSubject(buff2);
  uint8_t nStation = 0, nOnTime = 0;

  readWord(strControllerID);
  readWord(strNum);
  nStation = strNum.toUint();
  readWord(strNum);
  readWord(strStationDesc);
  nOnTime = strNum.toUint();
  if (strControllerID == m_WifiManager.getControllerID())
  {
    if (nStation > MAX_STATIONS)
    {
      strSubject = F("Wireless alarm / station number mismatch");
      strMsg += F("'");
      strMsg += m_WifiManager.getControllerID();
      strMsg += F("' does not have a station ");
      strMsg += fromUint(nStation, 10);
      strMsg += F("! Please check your settings on soil moisture probe '");
      strMsg += strStationDesc;
      strMsg += F("'.");
      m_WifiManager.sendEmail(strSubject, strMsg);
    }
    else
    {
      if (strStationDesc != m_program.getStationDesc(nStation))
      {
        strSubject = F("Wireless alarm / station description mismatch");
        strMsg += F("Irrigation controller '");
        strMsg += m_WifiManager.getControllerID();
        strMsg += F("' station ");
        strMsg += fromUint(nStation, 10);
        strMsg += F(" has the description '");
        strMsg += m_program.getStationDesc(nStation);
        strMsg += F("' rather than '");
        strMsg += strStationDesc;
        strMsg += F("'. Are you sure you have this soil moisture probe linked to the correct station?");

        m_WifiManager.sendEmail(strSubject, strMsg);
      }
      m_program.stationOn(nStation, nOnTime);
    }
  }
}

void CSerialManager::processData()
{
  uint8_t nStation = 0;
  CBuff<56> Buff56;
  CString strWord(Buff56);
  
	// Wait for all the data to be transmitted
	readWord(strWord);

	
  if (!strWord.isEmpty())
  {
    debug.log(F("================================================================"));

    if (m_pSerialHC05)
      debug.dump(F("From HC-05, request"), strWord);
    else if (m_pUDPServer)
      debug.dump(F("From UDP, request"), strWord);
  }
	if (strWord.indexOf(F("wifi")) > -1)
		updateWifiSSIDKey();
	else if (strWord.indexOf(F("upload")) > -1)
		uploadFile();
	else if (strWord.indexOf(F("download")) > -1)
		downloadFile();
	else if (strWord.indexOf(F("filesize")) > -1)
		getFileSize();
  else if (strWord.indexOf(F("irrigation")) > -1)
    writeWord(F("irrigation"));
	else if (strWord.indexOf(F("id")) > -1)
		writeWord(m_WifiManager.getControllerID());
	else if (strWord.indexOf(F("reset")) > -1)
		resetArd();
	else if (strWord.indexOf(F("networkname")) > -1)
		writeWord(m_WifiManager.getNetworkName());
	else if (strWord.indexOf(F("networkkey")) > -1)
		writeWord(m_WifiManager.getNetworkKey());
	else if (strWord.indexOf(F("emailaddress")) > -1)
		writeWord(m_WifiManager.getEmailAddress());
	else if (strWord.indexOf(F("mailserver")) > -1)
		writeWord(m_WifiManager.getMailServer());
	else if (strWord.indexOf(F("username")) > -1)
		writeWord(m_WifiManager.getUsername());
	else if (strWord.indexOf(F("password")) > -1)
		writeWord(m_WifiManager.getPassword());
  else if (strWord.indexOf(F("dow")) > -1)
  {
    strWord.fromUint(rtc.getDayOfWeek());
    writeWord(strWord);
  }
  else if (strWord.indexOf(F("dom")) > -1)
  {
    strWord.fromUint(rtc.getDay());
    writeWord(strWord);
  }
	else if (strWord.indexOf(F("month")) > -1)
	{
		strWord.fromUint(rtc.getMonth());
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("year")) > -1)
	{
		strWord.fromUint(rtc.getYear() + CDate::m_nCentury);
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("hour")) > -1)
	{
		strWord.fromUint(rtc.getHours());
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("minute")) > -1)
	{
		strWord.fromUint(rtc.getMinutes());
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("second")) > -1)
	{
		strWord.fromUint(rtc.getSeconds());
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("daylightsavings")) > -1)
	{
		strWord.fromUint(m_WifiManager.getDaylightSavings());
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("timezone")) > -1)
	{
		strWord.fromUint(m_WifiManager.getTimezone());
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("emailsettings")) > -1)
		updateEmailSettings();
	else if (strWord.indexOf(F("datetime")) > -1)
		updateSytemTime();
	else if (strWord.indexOf(F("radfrq")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord.fromUint(m_program.getRadioFreq(nStation));
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("selfrq")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord.fromUint(m_program.getSelectFreq(nStation));
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("runfrq")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord.fromUint(m_program.getRunFreq(nStation));
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("runtme")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord.fromUint(m_program.getRunTime(nStation));
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("stme")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord = m_program.getStartTime(nStation).toString(false);
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("etme")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord = m_program.getEndTime(nStation).toString(false);
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("sdte")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord = m_program.getStartSuspendDate(nStation).toString(false, false);
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("edte")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord = m_program.getEndSuspendDate(nStation).toString(false, false);
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("desc")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord = m_program.getStationDesc(nStation);
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("inst")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord.fromUint(m_program.getAlarmInstalled(nStation));
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("auto")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord.fromUint(m_program.getAlarmAuto(nStation));
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("dry")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord.fromUint(m_program.getAlarmDryTime(nStation));
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("on")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord.fromUint(m_program.getAlarmOnTime(nStation));
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("thresh")) > -1)
	{
		readWord(strWord);
		nStation = strWord.toUint();
    debug.log(F("Station "), false);
    debug.log(nStation);
		strWord.fromUint(m_program.getAlarmThreshold(nStation));
		writeWord(strWord);
	}
	else if (strWord.indexOf(F("program")) > -1)
	{
		processIrrigProgData();
	}
	else if (strWord.indexOf(F("alarms")) > -1)
	{
		processAlarmData();
	}
  else if (strWord.indexOf(F("wrlsalrm")) > -1)
  {
    processWirelessAlarmData();
  }
  else if (!strWord.isEmpty())
  {
    debug.log(F("Unknown request!"));
    dump();
    writeWord(F("Data error!"));
  }
  if (!strWord.isEmpty())
  	debug.log(F("================================================================"));
}

#ifdef AUTO_SOLENOID
  void CSerialManager::getRemoteStations(WiFiEspUDP *pUDPServer, CProgram& program)
  {
    uint8_t nStationNum = 0;
    CBuff<32> buff;
    CString strData(buff);
    uint32_t nStartMillis = 0;
    bool bRespRec = false;

    m_pUDPServer = pUDPServer;
    m_pSerialHC05 = NULL;
    
    for (nStationNum = 1; nStationNum <= MAX_STATIONS; nStationNum++)
    { 
      IPAddress ipL = WiFi.localIP(), ipB(ipL[0] & 255, ipL[1] & 255, ipL[2] & 255, 255);
      strData = F("request");
      strData += m_strDelimiter;
      strData += fromUint(nStationNum, 10);
      strData += m_strDelimiter;
      strData += ipL;

      if (!writeWord(strData, ipB, 10004))
        debug.logRuntimeError(F("SerialManager.cpp"), __LINE__);

      nStartMillis = millis();
      do
      {

        if (readWord(strData))
        {
          if (strData.indexOf(F("notify")))
          {
            if (readWord(strData))
            {
              uint8_t nStation = strData.toUint();
              if ((nStation >= 1) && (nStation <= MAX_STATIONS))
              {
                IPAddress ipAddr;
                if (readWord(strData))
                  ipAddr.fromString(strData);
                else
                  debug.logRuntimeError(F("SerialManager.cpp"), __LINE__);
                program.setActive(nStation, ipAddr);

                debug.log(F("received "), false);
                debug.log(nStation, false);
                debug.log(F(", "), false);
                debug.log(ipAddr);
                debug.log(F(""));
                bRespRec = true;
              }
              else
                debug.logRuntimeError(F("SerialManager.cpp"), __LINE__);
            }
            else
              debug.logRuntimeError(F("SerialManager.cpp"), __LINE__);
          }
        }
      }
      while ((millis() - nStartMillis) <= 1000);

      if (!bRespRec)
        debug.log(F("no response"));
    }
  }
#endif

void CSerialManager::processData(Stream *pSerialHC05)
{
	m_pSerialHC05 = pSerialHC05;
	m_pUDPServer = NULL;
	processData();
}

void CSerialManager::processData(WiFiEspUDP *pUDPServer)
{
	m_pUDPServer = pUDPServer;
	m_pSerialHC05 = NULL;
	processData();
}

