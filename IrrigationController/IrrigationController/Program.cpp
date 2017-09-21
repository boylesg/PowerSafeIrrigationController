#include <Arduino.h>
#include <RealTimeClockDS1307.h>
#include <MySD.h>
#include "Program.h"
#include "TextFile.h"
#include "Debug.h"
#include "Common.h"
#include "Timer.h"
#include "List.h"
#include "SerialManager.h"




#ifdef __SAM3X8E__
  const uint8_t CProgram::m_arrayRelayPinNums[] PROGMEM = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
  const uint8_t CProgram::m_arrayProbePinNums[] PROGMEM = {21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51};
#elif defined (__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  const uint8_t CProgram::m_arrayRelayPinNums[] PROGMEM = {22, 24, 26, 28, 30, 32, 34, 36};
  const uint8_t CProgram::m_arrayProbePinNums[] PROGMEM = {A8, A9, A10, A11, A12, A13, A14, A15};
#elif defined (ESP32)
  const uint8_t CProgram::m_arrayRelayPinNums[] PROGMEM = {36, 39, 34, 35, 32, 33, 25, 26};
  const uint8_t CProgram::m_arrayProbePinNums[] PROGMEM = {27, 14, 12, 13, 9, 10, 11, 15};
#endif

CProgram::CProgram(): m_strProgramFilename(F("program.txt")), m_strAlarmsFilename(F("walarms.txt")), m_strProgramPageDataFilename(F("progdata.txt"))
{
  m_pTimer = NULL;
}

CProgram::~CProgram()
{
}

void CProgram::begin(CSerialManager& SerialManager, const WiFiEspUDP* pSerialSrc)
{
  m_pTimer = timers.add(0, 0, NULL);
  #ifdef AUTO_SOLENOID
    while (true)
    SerialManager.getRemoteStations(pSerialSrc, *this);
  #else
    for (uint8_t nStationNum = 0, nPinNum = 0; nStationNum < MAX_STATIONS; nStationNum++)
    {
      nPinNum = pgm_read_byte(m_arrayProbePinNums + nStationNum);
      //pinMode(nPinNum, INPUT);
      nPinNum = pgm_read_byte(m_arrayRelayPinNums + nStationNum);
      pinMode(nPinNum, OUTPUT);
      digitalWrite(nPinNum, LOW);
    }
  #endif
}

#ifdef AUTO_SOLENOID
  bool CProgram::isActive(const uint8_t nStationNum)
  {
    bool bResult = false;
  
    if ((nStationNum >= 1) && (nStationNum <= MAX_STATIONS))
      bResult = m_arrayStations[nStationNum - 1].m_bActive;
  
    return bResult;
  }
  
  void CProgram::setActive(const uint8_t nStationNum, IPAddress ipAddr)
  {
    if ((nStationNum >= 1) && (nStationNum <= MAX_STATIONS))
    {
      m_arrayStations[nStationNum - 1].m_bActive = true;
      m_arrayStations[nStationNum - 1].m_ipAddr = ipAddr;
    }
  }
#endif

CTime& CProgram::checkAlarms(CWifiManager& WifiManager, CTime& timeLast)
{
  static CTime timeCurrent;
  float fElapsedMinutes = 0;
  CBuff<256> buffMsg;
  CBuff<64> buffSubject;
  CString strAlarmMsg(buffMsg), strSubject(buffSubject);
  bool bAlarmTriggered = false;
  CDateTime datetime(rtc);
  
  strSubject = F("Soil moisture alarm trigered for '");
  strSubject += WifiManager.getControllerID();
  strSubject += F("' - ");

  strSubject += datetime.toString();
  rtc.readClock();
  timeCurrent.set(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  fElapsedMinutes = (float)(timeCurrent - timeLast) / 60;

  for (uint8_t nI = 0; nI < MAX_STATIONS; nI++)
  {
    if (m_arrayStations[nI].checkAlarms(fElapsedMinutes, strAlarmMsg, &m_listRun))
    {
      bAlarmTriggered = true;
    }
  }
  if (bAlarmTriggered)
  {
    WifiManager.sendEmail(strSubject, strAlarmMsg);
    debug.log(strSubject);
    debug.dump(F("Message"), strAlarmMsg);
    debug.log(F("\n"));
    timeLast = timeCurrent;
  }
  return timeLast;
}

uint8_t CProgram::getAlarmThreshold(const int nStation)
{
  uint8_t nVal = 0;

  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    nVal = m_arrayStations[nStation - 1].m_nProbeThreshold;
  }
  return nVal;
}

uint8_t CProgram::getAlarmOnTime(const int nStation)
{
  uint8_t nVal = 0;

  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    nVal = m_arrayStations[nStation - 1].m_nAutoRunMinutes;
  }
  return nVal;
}

uint8_t CProgram::getAlarmDryTime(const int nStation)
{
  uint8_t nVal = 0;

  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    nVal = m_arrayStations[nStation - 1].m_nAllowedDryTimeMinutes;
  }
  return nVal;
}

bool CProgram::getAlarmAuto(const int nStation)
{
  bool bVal = false;
  
  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    bVal = m_arrayStations[nStation - 1].m_bProbeAuto;
  }
  return bVal;
}

bool CProgram::getAlarmInstalled(const int nStation)
{
  bool bVal = false;
  
  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    bVal = m_arrayStations[nStation - 1].m_bMoistureProbe;
  }
  return bVal;
}

void CProgram::dumpPageData()
{
  debug.log(F("===================================================================================="));
  debug.log(F("PAGE DATA\n"));
  for (int nI = 0; nI < MAX_STATIONS; nI++)
  {
    m_arrayStations[nI].dumpPageData();
  }
  debug.log(F("===================================================================================="));
}

void CProgram::dumpPageData(const uint8_t nStation)
{
    if ((nStation >= 1) && (nStation <= MAX_STATIONS))
    {
      m_arrayStations[nStation  -1].dumpPageData();
    }
}

uint8_t CProgram::getRadioFreq(const uint8_t nStation)
{
  uint8_t nVal = 0;

  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    nVal = m_arrayStations[nStation - 1].m_nRadioFreq;
  }
  return nVal;
}

uint8_t CProgram::getSelectFreq(const uint8_t nStation)
{
  uint8_t nVal = 0;

  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    nVal = m_arrayStations[nStation - 1].m_nSelectFreq;
  }
  return nVal;
}

uint8_t CProgram::getRunFreq(const uint8_t nStation)
{
  uint8_t nVal = 0;

  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    nVal = m_arrayStations[nStation - 1].m_nRunFreq;
  }
  return nVal;
}

uint8_t CProgram::getRunTime(const uint8_t nStation)
{
  uint8_t nVal = 0;

  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    nVal = m_arrayStations[nStation - 1].m_nRuntime;
  }
  return nVal;
}

CTime& CProgram::getStartTime(const uint8_t nStation)
{
  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    m_time = m_arrayStations[nStation - 1].m_timeEnd;
  }
  return m_time;
}

CTime& CProgram::getEndTime(const uint8_t nStation)
{
  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    m_time = m_arrayStations[nStation - 1].m_timeEnd;
  }
  return m_time;
}

CDate& CProgram::getStartSuspendDate(const uint8_t nStation)
{
  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    m_date = m_arrayStations[nStation - 1].m_dateSuspendStart;
  }
  return m_date;
}

CDate& CProgram::getEndSuspendDate(const uint8_t nStation)
{
  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    m_date = m_arrayStations[nStation - 1].m_dateSuspendEnd;
  }
  return m_date;
}

const char* CProgram::getStationDesc(const uint8_t nStation)
{
  static char strBlank[1] = "";
  char* strDesc = strBlank;
  
  if ((nStation >= 1) && (nStation <= 8))
    strDesc = m_arrayStations[nStation - 1].m_strDescription;

  return strDesc;
}

uint8_t CProgram::stationOn(const uint8_t nStation, const uint16_t nMinutes, CSerialManager* pSerialManager)
{
  uint8_t nResult = 0;
  
  if ((nStation >= 1) && (nStation < MAX_STATIONS))
  {
    if (m_listRun.isRunning())
    {
      if (m_pTimer->getOwnerID() == nStation)
      {
        nResult = STATION_ALREADY_ON;
      }
      else if (!m_listRun.isRunning(nStation))
      {
        m_listRun.push(nStation, nMinutes);
        nResult = STATION_LISTED;
      }
      else
        nResult = STATION_ALREADY_LISTED;
    }
    else if (m_pTimer->getOwnerID() > 0)
    {
      if (m_pTimer->getOwnerID() == nStation)
        nResult = STATION_ALREADY_ON;
      else
      {
        m_listRun.push(nStation, nMinutes);
        nResult = STATION_LISTED;
      }
    }
    else
    {
      m_arrayStations[nStation - 1].stationOn(nMinutes, m_pTimer, pSerialManager);
      nResult = STATION_ON;
    }
  }
  else
    nResult = STATION_INVALID;  

  return nResult;
}

void CProgram::stationOff(const uint8_t nStation, CTimer* pTimer, CSerialManager *pSerialManager)
{
  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    m_arrayStations[nStation - 1].stationOff(pTimer, pSerialManager);
  }
}

uint8_t CProgram::stationOff(const uint8_t nStation, CSerialManager* pSerialManager)
{
  uint8_t nResult = 0;
  
  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    if (m_listRun.isRunning())
    {
      if (m_listRun.isRunning(nStation))
      {
        if (m_pTimer->getOwnerID() == nStation)
        {
          m_listRun.remove(nStation);
          m_arrayStations[nStation - 1].stationOff(m_pTimer, pSerialManager);
          nResult = STATION_OFF;
        }
        else
        {
          m_listRun.remove(nStation);
          nResult = STATION_DELISTED;
        }
      }
      else if (m_pTimer->getOwnerID() == nStation)
      {
        m_arrayStations[nStation - 1].stationOff(m_pTimer, pSerialManager);
        nResult = STATION_OFF;
      }
      else
        nResult = STATION_ALREADY_DELISTED;
    }
    else if (m_pTimer->getOwnerID() == nStation)
    {
      m_arrayStations[nStation - 1].stationOff(m_pTimer, pSerialManager);
      nResult = STATION_OFF;
    }
    else
      nResult = STATION_ALREADY_OFF;    
  }
  else
    nResult = STATION_INVALID;  
      
  return nResult;
}

bool CProgram::isRunning(const uint8_t nStation)
{
  bool bIsRunning = false;
  
  if ((nStation >= 1) && (nStation <= MAX_STATIONS))
  {
    bIsRunning = m_listRun.isRunning(nStation) || (m_pTimer->getOwnerID() == nStation);
  }
  return bIsRunning;
}

void CProgram::updateAlarmData(const uint8_t nStation, const bool bInstalled, const bool bAuto, const uint8_t nDryTime, const uint8_t nThreshold, const uint8_t nRuntime)
{
  CProgramStation* pStation = getStationDetails(nStation); 
   if (pStation)
   {
     pStation->m_bMoistureProbe = bInstalled;
     pStation->m_bProbeAuto = bAuto;
     pStation->m_nProbeThreshold = nThreshold;
     pStation->m_nAutoRunMinutes = nRuntime;
     pStation->m_nAllowedDryTimeMinutes = nDryTime;
   }
}

uint8_t CProgram::getDayInterval(const uint8_t nRadioFreq, const uint8_t nSelFreq)
{
  uint8_t nDayInterval = 1;
  switch (nRadioFreq)
  {
    case 0: nDayInterval = (nSelFreq + 1) * 30; break;
    case 1: nDayInterval = (nSelFreq + 1) * 7; break;
    case 2: nDayInterval = nSelFreq + 1; break;
  }
  return nDayInterval;
}

uint8_t CProgram::getHourInterval(const uint8_t nRunFreq)
{
  uint8_t nHourInterval = 2;

  switch (nRunFreq)
  {
    case 0: nHourInterval = 2; break;
    case 1: nHourInterval = 3; break;
    case 2: nHourInterval = 4; break;
    case 3: nHourInterval = 6; break;
    case 4: nHourInterval = 8; break;
    case 5: nHourInterval = 12; break;
    case 6: nHourInterval = 24; break;
  }
  return nHourInterval;
}

const char* CProgram::getStartTimes(const char* strStartTime, const char* strEndTime, const uint8_t nNumHours, const uint8_t nRunMins)
{
  static char strBuff[129];
  CString str(strBuff, 129);
  CTime timeEnd, timeCurr;
  uint8_t nCurrHour = 0, nEndHour = 0, nStartHour = 0;

  if (!timeCurr.parse(strStartTime))
    debug.logRuntimeError(F("Program.cpp"), __LINE__);
  else if (!timeEnd.parse(strEndTime))
    debug.logRuntimeError(F("Program.cpp"), __LINE__);
  else
  {
    nCurrHour = nStartHour = timeCurr.getHours();
    nEndHour = timeEnd.getHours();
    if (nEndHour <= nCurrHour)
      nEndHour += 24;

//debug.log("**************");
//debug('*', nCurrHour);
//debug('*', nEndHour);
//debug('*', nNumHours);
//debug.log("**************");

    memset(str, 0, 129);
    do
    {
      str += timeCurr.toString(false);
      str += F("-");
      str += fromUint(nRunMins, 10);
      str += F(",");
      timeCurr.addHours(nNumHours);
      nCurrHour += nNumHours;

      if ((nCurrHour == nEndHour) && (nStartHour == (nEndHour % 24)))
        break;
    }
    while (nCurrHour <= nEndHour);

    str[str.length() - 1] = 0;
  }
  return strBuff;
}

bool CProgram::openIrrigProgramFile(CTextFile& file, CString &strMsg)
{
	bool bResult = false;
	CBuff<16> Buff16;
	CString strFilename(Buff16);

	strFilename = m_strProgramFilename;
	if (SD.exists(strFilename))
	{
		if (file.open(strFilename, REWRITE))
		{
			bResult = true;
		}
		else
		{
			strMsg = F("Could not open file '");
			strMsg += strFilename;
			strMsg += F("' for writing!");
		}
	}
	return bResult;
}

bool CProgram::openIrrigProgramPageDataFile(CTextFile& file, CString &strMsg)
{
	bool bResult = false;
	CBuff<16> Buff16;
	CString strFilename(Buff16);

	strFilename = m_strProgramPageDataFilename;
	if (SD.exists(strFilename))
	{
		if (file.open(strFilename, REWRITE))
		{
			bResult = true;
		}
		else
		{
			strMsg = F("Could not open file '");
			strMsg += strFilename;
			strMsg += F("' for writing!");
		}
	}
	return bResult;
}

void CProgram::saveIrrigProg(const uint8_t nStationNum, CTextFile& fileIrrigProg, const uint8_t nNumDays, const char* strStartTime, const char* strEndTime, const uint8_t nNumHours, const uint8_t nRunMins, const char* strSuspendStartDate, const char* strSuspendEndDate, const char* strDesc)
{
  rtc.readClock();
  CDate date(1, 1, rtc.getYear());
  char strBuff[129];
  CString strLine(strBuff, 129);
  uint16_t nYear = date.getYear();

  strLine = F("station");
  strLine += fromUint(nStationNum, 10);
  fileIrrigProg.writeLine(strLine);
  fileIrrigProg.write(F("\tdescription:"));
  fileIrrigProg.writeLine(strDesc);
  strLine = F("\tsuspend:");
  if ((strlen(strSuspendStartDate) > 0) && (strlen(strSuspendEndDate) > 0))
  {
    strLine += strSuspendStartDate;
    strLine += F("-");
    strLine += strSuspendEndDate;
  }
  fileIrrigProg.writeLine(strLine);

  if (nRunMins > 0)
  {
    while (nYear == date.getYear())
    {
      strLine = F("\t");
      strLine += date.toString(false);
      strLine += F("=");
      strLine += getStartTimes(strStartTime, strEndTime, nNumHours, nRunMins);
      fileIrrigProg.writeLine(strLine);
      date.addDays(nNumDays);
    }
  }
}

void CProgram::saveIrrigProgPageData(const uint8_t nStationNum, CTextFile& fileIrrigProgPageData, const char* strDesc, const uint8_t nRadioFreq, const uint8_t nSelectFreq, 
										const uint8_t nRuntime, const uint8_t nRunFreq, const char* strStartTime, const char* strEndTime, const char* strSuspendStartDate, 
										const char* strSuspendEndDate)
{
	CProgramStation* pStationDetails = getStationDetails(nStationNum);
	CBuff<16> Buff16;
	CString strStation(Buff16);

	if (pStationDetails)
	{
		pStationDetails->setDescription(strDesc);
		pStationDetails->m_nRadioFreq = nRadioFreq;
		pStationDetails->m_nSelectFreq = nSelectFreq;
		pStationDetails->m_nRunFreq = nRunFreq;
		pStationDetails->m_nRuntime = nRuntime;
		pStationDetails->m_timeStart.parse(strStartTime);
		pStationDetails->m_timeEnd.parse(strEndTime);
		pStationDetails->m_dateSuspendStart.parse(strSuspendStartDate);
		pStationDetails->m_dateSuspendEnd.parse(strSuspendEndDate);
		pStationDetails->saveProgramPageData(fileIrrigProgPageData);
	}
}

bool CProgram::saveAlarmsData(CString &strMsg)
{
	CBuff<16> Buff16;
	CString strFilename(Buff16);
	CTextFile file;
	CProgramStation *pStation = NULL;
	bool bResult = false;

	strFilename = m_strAlarmsFilename;
	if (SD.exists(strFilename))
	{
	  if (file.open(strFilename, REWRITE))
		{
			for (uint8_t nI = 1; nI <= MAX_STATIONS; nI++)
			{
				pStation = getStationDetails(nI);
				if (pStation)
				{
					file.write(F("station"));
					file.writeLine(fromUint(nI, 10));
					file.write(F("\tprobe:"));
          
					if (pStation->m_bMoistureProbe)
						file.writeLine(F("installed"));
					else
						file.writeLine(F(""));
  
					file.write(F("\tallowed_dry_time:"));
					file.writeLine(pStation->m_nAllowedDryTimeMinutes);
          
					file.write(F("\tprobe_threshold_value:"));
					file.writeLine(pStation->m_nProbeThreshold);
  
					file.write(F("\tprobe_auto:"));
					if (pStation->m_bProbeAuto)
						file.writeLine(F("yes"));
					else
						file.writeLine(F(""));
  
					file.write(F("\tauto_runtime:"));
					file.writeLine(pStation->m_nAutoRunMinutes);
				}
			}
			bResult = true;
			strMsg = F("Upload successful!");
      file.close();
		}
		else
		{
			debug.logRuntimeError(F("Program.cpp"), __LINE__);

			strMsg = F("Could not open '");
			strMsg += strFilename;
			strMsg += F("'!");

		}
	}
	else
	{
		debug.logRuntimeError(F("Program.cpp"), __LINE__);
		strMsg = F("Could not find '");
		strMsg += strFilename;
		strMsg += F("'!");
	}
	//file.dump(m_strTXTAlarmsFilename);
	return bResult;
}

bool CProgram::readProgramPageData()
{
  bool bResult = false;
  CBuff<16> Buff16;
  CString strFilename(Buff16);
  
  strFilename = m_strProgramPageDataFilename;
  if (SD.exists(strFilename)) 
  {  
    CTextFile file;
    
    if (file.open(strFilename, READ))
    {
      for (uint8_t nStation = 0; nStation < MAX_STATIONS; nStation++)
      {
        m_arrayStations[nStation].readProgramPageData(file);
      }
      bResult = true;
      dumpPageData();
    }
  }
  return bResult;
}

bool CProgram::read()
{
  bool bResult = true;  
  rtc.readClock();
  CDate dateNow(rtc.getDayOfWeek(), rtc.getDate(), rtc.getMonth(), rtc.getYear());
  CBuff<16> Buff16;
  CString strFilename(Buff16);

  strFilename = m_strProgramFilename;
  if (SD.exists(strFilename))
  {
	  CTextFile fileProgram;

	  if (fileProgram.open(strFilename, READ))
	  {
		  empty();
		  for (uint8_t nStation = 0; nStation < MAX_STATIONS; nStation++)
		  {
			  bResult &= m_arrayStations[nStation].read(fileProgram, dateNow, nStation + 1, pgm_read_byte(m_arrayRelayPinNums + nStation));
		  }
		  if (!bResult)
		  {
			  debug.log(F("An error occured while reading '"));
			  debug.log(m_strProgramFilename);
			  debug.log(F("'!"));
		  }
	  }
	  else
	  {
		  debug.log(F("Could not open '"), false);
		  debug.log(m_strProgramFilename);
		  debug.log(F("'!"));
	  }
  }
  else
  {
	  debug.log(F("Could not find '"), false);
	  debug.log(m_strProgramFilename);
	  debug.log(F("'!"));
  }
  #ifndef AUTO_SOLENOID
    strFilename = m_strAlarmsFilename;
    if (SD.exists(strFilename)) 
    {  
      CTextFile fileAlarms;
        
      if (fileAlarms.open(strFilename, READ))
      {
        for (uint8_t nStation = 0; nStation < MAX_STATIONS; nStation++)
          bResult &= m_arrayStations[nStation].readAlarms(fileAlarms, nStation + 1, pgm_read_byte(m_arrayProbePinNums + nStation));
  
  	  if (!bResult)
  	  {
    		debug.log(F("An error occured while reading '"));
    		debug.log(m_strAlarmsFilename);
    		debug.log(F("'!"));
  	  }
        else
          dump();
      }
      else
  	  {
        debug.log(F("Could not open '"), false);
  	    debug.log(m_strAlarmsFilename);
  	    debug.log(F("'!"));
  	  }
    }
    else
    {
    	debug.log(F("Could not find '"), false);
    	debug.log(m_strAlarmsFilename);
    	debug.log(F("'!"));
    }
  #endif
  bResult &= readProgramPageData();

  return bResult;
}

void CProgram::dump()
{
  debug.log(F("===================================================================================="));
  for (uint8_t nStation = 0; nStation < MAX_STATIONS; nStation++)
    m_arrayStations[nStation].dump();
  debug.log(F("===================================================================================="));
}

void CProgram::run(CSerialManager& SerialManager)
{
  if (!m_listRun.isRunning())
  {
    debug.log(F("********** BUILDING PROGRAM RUN LIST **********\n"));
    CDate dateNow;
    CTime timeNow;
  
    rtc.readClock();
    dateNow.set(rtc.getDayOfWeek(), rtc.getDate(), rtc.getMonth(), rtc.getYear());
    timeNow.set(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  
    for (uint8_t nI = 0; nI < MAX_STATIONS; nI++)
    {
      m_arrayStations[nI].getRun(dateNow, timeNow, &m_listRun);
    }
    if (m_listRun.size() == 0)
    {
      debug.log(F("********** NO ACTION **********\n"));
    }
    else
    {
      m_listRun.dump();
    }
  }
  else
  {
    uint8_t nStationNum = 0, nRunMinutes = 0;
  
    m_listRun.dump();
    //m_pTimer->dump();
    if (!m_pTimer->isRunning() && m_listRun.getNext(nStationNum, nRunMinutes))
    {
      debug.log(F("********** EXECUTING PROGRAM RUN LIST "), false);
      debug.log(nStationNum, false);
      debug.log(F(" -> "), false);
      debug.log(nRunMinutes, false);
      debug.log(F(" minute(s) **********\n"));
      
      if ((nStationNum >= 1) && (nStationNum <= MAX_STATIONS) && (nRunMinutes > 0))
      {
        m_arrayStations[nStationNum - 1].stationOn(nRunMinutes, m_pTimer, &SerialManager);
      }
    }
  }
}

void CProgram::empty()
{
  for (uint8_t nI = 0; nI < MAX_STATIONS; nI++)
    m_arrayStations[nI].empty();
}

CProgramStation* CProgram::getStationDetails(uint8_t nI)
{
  CProgramStation* pStation = NULL;
  
  if ((nI >= 1) && (nI <= MAX_STATIONS))
  {
    pStation = &m_arrayStations[nI - 1];
  }
  return pStation;
}

CProgram program;


