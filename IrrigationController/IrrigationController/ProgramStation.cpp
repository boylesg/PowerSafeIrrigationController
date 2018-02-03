#include <Arduino.h>
#include <MySD.h>
#include "ProgramStation.h"
#include "Common.h"
#include "Debug.h"
#include "program.h"
#include "SerialManager.h"




uint8_t g_nStationNum = 0;
CTimer* g_pTimer = NULL;
CSerialManager* g_pSerialManager = NULL;

void turnStationOff()
{
  program.stationOff(g_nStationNum, g_pTimer, g_pSerialManager);
}




CProgramStation::CProgramStation()
{
  m_nStationNum = m_nRelayPinNum = 0;
  m_bMoistureProbe = m_bProbeAuto = false;
  m_nAllowedDryTimeMinutes = m_nProbeThreshold = m_nAutoRunMinutes = 0;
  m_nRadioFreq = m_nRunFreq = 1;
  m_nSelectFreq = m_nRuntime = 0;
  #ifdef AUTO_SOLENOID
    m_bActive = true;
  #endif
}

CProgramStation::~CProgramStation()
{
}

void CProgramStation::setDescription(const char* strDesc)
{
  CString strDescription(m_buffDescription);
  strDescription = strDesc;
}

bool CProgramStation::findDate(CTextFile& file, CDate& dateNow, CString& strLine)
{
  int8_t nPos = -1;
  bool bResult = false;
  CBuff<6> buff;
  CString strDateNow(buff);
  uint64_t nFilePos = file.position();

  strDateNow = dateNow.toString();

  while (true)
  {
    if (!file.eof())
    {
      nFilePos = file.position();
      file.readLine(strLine);

      if (strLine.indexOf(strDateNow) > -1)
      {
        nPos = strLine.indexOf('=');
        if (nPos >= 0)
        {
          strLine = strLine.substring(nPos + 1, strLine.length());
          strLine.trim();
          bResult = true;
        }
        break;
      }
      else if (strLine.indexOf(F("station")) > -1)
      {
        strLine = "";
        file.seek(nFilePos);
        break;
      }
    }
    else
      break;
  }
  return bResult;
}

void CProgramStation::readProgramStarts(CTextFile& file, CDate& dateNow)
{
  CBuff<256> Buff256;
  CString strLine(Buff256);
  uint8_t nI = 0;
  rtc.readClock();
  CTime timeNow(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());

  if (findDate(file, dateNow, strLine))
  {
    while (strLine.length() > 0)
    {
      if (nI < MAX_STARTS)
      {
        if (!m_arrayStarts[nI].parseStartRun(strLine, timeNow))
          debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
        nI++;
      }
    }
    m_nAutoRunMinutes = m_arrayStarts[0].getRuntime();
  }
  else
    file.seekRel(-10);
}

bool CProgramStation::readDescription(CTextFile& file)
{
  bool bResult = false;
  CBuff<32> BuffLine;
  CString strLine(BuffLine), strDescription(m_buffDescription);  
  int8_t nPos = 0;

  if (file.readLine(strLine) && (strLine.length() > 0) && (strLine.indexOf(F("description") >= 0)))
  {
    nPos = strLine.indexOf(':');
    strDescription = strLine.substring(nPos + 1, strLine.length());
    bResult = true;
  }
  else
    debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);

  return bResult;
}

bool CProgramStation::readProgramSuspend(CTextFile& file)
{
  bool bResult = false;
  CBuff<12> Buff12;
  CBuff<32> Buff32;
  CString strLine(Buff32), strSuspendDate(Buff12);
  int8_t nPos = 0;
  rtc.readClock();
  uint16_t nYear = rtc.getYear() + CDate::m_nCentury;

  if (file.readLine(strLine) && (strLine.length() > 0) && (strLine.indexOf(F("suspend") >= 0)))
  {
    nPos = strLine.indexOf(':');
    strSuspendDate = strLine.substring(nPos + 1, strLine.length());
    if (strSuspendDate.length() == 0)
      bResult = true;
    else if (strSuspendDate.length() > 0)
    {
      strLine = strSuspendDate;
      nPos = strLine.indexOf('-');
      strSuspendDate = strLine.substring(0, nPos - 1);
      bResult = m_dateSuspendStart.parse(strSuspendDate.c_str());
      m_dateSuspendStart.setYear(nYear);
      strSuspendDate = strLine.substring(nPos + 1, strLine.length());
      bResult &= m_dateSuspendEnd.parse(strSuspendDate.c_str());

      if (m_dateSuspendStart.getMonth() > m_dateSuspendEnd.getMonth())
        m_dateSuspendEnd.setYear(nYear + 1);

      if (!bResult)
        debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
    }
  }
  else
    debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);

  return bResult;
}

bool CProgramStation::readAlarms(CTextFile& file, const uint8_t nStationNum, const uint8_t nProbePinNum)
{
  bool bResult = true;
  CBuff<16> Buff16;
  CBuff<32> Buff32;
  CString strLine(Buff32), strTemp(Buff16);
  int8_t nPos = 0;

  m_nProbePinNum = nProbePinNum;

  if (findStation(file, nStationNum, strLine))
  {
    if (file.readLine(strLine) && (strLine.length() > 0) && (strLine.indexOf(F("probe") >= 0)))
    {
      nPos = strLine.indexOf(':');
      strTemp = strLine.substring(nPos + 1, strLine.length());
      bResult &= true;
      m_bMoistureProbe = strTemp == F("installed");
    }
    else
    {
      debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
      bResult &= false;
    }
    if (file.readLine(strLine) && (strLine.length() > 0) && (strLine.indexOf(F("allowed_dry_time") >= 0)))
    {
      nPos = strLine.indexOf(':');
      strTemp = strLine.substring(nPos + 1, strLine.length());
      m_nAllowedDryTimeMinutes = strTemp.toInt();
      bResult &= true;
    }
    else
    {
      debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
      bResult &= false;
    }
    if (file.readLine(strLine) && (strLine.length() > 0) && (strLine.indexOf(F("probe_threshold_value") >= 0)))
    {
      nPos = strLine.indexOf(':');
      strTemp = strLine.substring(nPos + 1, strLine.length());
      m_nProbeThreshold = strTemp.toInt();
      bResult &= true;
    }
    else
    {
      debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
      bResult &= false;
    }
    if (file.readLine(strLine) && (strLine.length() > 0) && (strLine.indexOf(F("probe_auto") >= 0)))
    {
      nPos = strLine.indexOf(':');
      strTemp = strLine.substring(nPos + 1, strLine.length());
      m_bProbeAuto = strTemp == F("yes");
      bResult &= true;
    }
    else
    {
      debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
      bResult &= false;
    }
    if (file.readLine(strLine) && (strLine.length() > 0) && (strLine.indexOf(F("auto_runtime") >= 0)))
    {
      nPos = strLine.indexOf(':');
      strTemp = strLine.substring(nPos + 1, strLine.length());
      m_nAutoRunMinutes = strTemp.toInt();
      bResult &= true;
    }
    else
    {
      debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
      bResult &= false;
    }
  }
  else
  {
    debug.log(F("Can't find alarm details for station "), false);
    debug.log(nStationNum, false);
    debug.log(F("!"));
    debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
  }
  return bResult;
}

bool CProgramStation::findStation(CTextFile& file, const uint8_t nStation, CString& strLine)
{
  int8_t nPos = -1;
  CBuff<12> buffStation;
  CString strStation(buffStation);

  strStation = F("station");
  strStation += fromUint(nStation, 10);
  while (!file.eof() && (nPos < 0))
  {
    file.readLine(strLine);
    nPos = strLine.indexOf(strStation);
  }
  strLine.trim();

  return nPos >= 0;
}

void CProgramStation::saveProgramPageData(CTextFile& file)
{
	CBuff<12> Buff12;
	CString strStation(Buff12);

	strStation = F("station");
	strStation += fromUint(m_nStationNum, 10);
	file.writeLine(strStation);
	file.writeLine(m_buffDescription);
	file.writeLine(m_nRadioFreq);
	file.writeLine(m_nSelectFreq);
	file.writeLine(m_nRunFreq);
	file.writeLine(m_nRuntime);
	file.writeLine(m_timeStart.toString(false));
	file.writeLine(m_timeEnd.toString(false));
	if (!m_dateSuspendStart.isEmpty())
		file.writeLine(m_dateSuspendStart.toString(false));
	else
		file.writeLine(F(""));
	if (!m_dateSuspendEnd.isEmpty())
		file.writeLine(m_dateSuspendEnd.toString(false));
	else
		file.writeLine(F(""));
}

void CProgramStation::readProgramPageData(CTextFile& file)
{
  CBuff<256> BuffLine;
  CString strLine(BuffLine);
  CString strDesc(m_buffDescription);
  
  file.readLine(strLine); // station1
  file.readLine(strDesc);

  file.readLine(strLine);
  if (!strLine.isEmpty())
    m_nRadioFreq = strLine.toUint();

  file.readLine(strLine);
  if (!strLine.isEmpty())
    m_nSelectFreq = strLine.toUint();

  file.readLine(strLine);
  if (!strLine.isEmpty())
    m_nRunFreq = strLine.toUint();

  file.readLine(strLine);
  if (!strLine.isEmpty())
    m_nRuntime = strLine.toUint();

  file.readLine(strLine);
  if (!strLine.isEmpty())
    m_timeStart.parse(strLine);

  file.readLine(strLine);
  if (!strLine.isEmpty())
    m_timeEnd.parse(strLine);

  file.readLine(strLine);
  if (!strLine.isEmpty())
    m_dateSuspendStart.parse(strLine);

  file.readLine(strLine);
  if (!strLine.isEmpty())
    m_dateSuspendEnd.parse(strLine);
}

bool CProgramStation::read(CTextFile& file, CDate& dateNow, uint8_t nStationNum, uint8_t nDigitalPinNum)
{
  bool bResult = true;
  CBuff<256> Buff256;
  CString strLine(Buff256);
  uint8_t nNum = 0;

  m_nStationNum = nStationNum;

  m_nRelayPinNum = nDigitalPinNum;
  if (findStation(file, nStationNum, strLine))
  {
    bResult = true;
    readDescription(file);
    readProgramSuspend(file);
    readProgramStarts(file, dateNow);
  }
  else if (file.eof())
  {
    debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
    debug.dump(F("nStationNum"), nStationNum);
  }
  return bResult;
}

void CProgramStation::dumpPageData()
{
  debug.dump(F("m_nStationNum"), m_nStationNum);
  debug.dump(F("m_nRadioFreq"), m_nRadioFreq);
  debug.dump(F("m_nSelectFreq"), m_nSelectFreq);
  debug.dump(F("m_nRunFreq"), m_nRunFreq);
  debug.dump(F("m_nRuntime"), m_nRuntime);
  debug.dump(F("m_timeStart"), m_timeStart.toString(false));
  debug.dump(F("m_timeEnd"), m_timeEnd.toString(false));
  debug.dump(F("m_dateSuspendStart"), m_dateSuspendStart.toString(false, false));
  debug.dump(F("m_dateSuspendEnd"), m_dateSuspendEnd.toString(false, false));
  debug.log(F(""));
}

void CProgramStation::dump()
{
  debug.log(F("station"), false);
  debug.log(fromUint(m_nStationNum, 10), true);

  debug.log(F("\tdescription: "), false);
  debug.log(m_buffDescription);

  debug.log(F("\tprobe: "), false);
  if (m_bMoistureProbe)
    debug.log(F("installed"));
  else
    debug.log(F(""));

  debug.log(F("\tallowed_dry_time: "), false);
  debug.log(fromUint(m_nAllowedDryTimeMinutes, 10));
  debug.log(F("\tprobe_threshold_value: "), false);
  debug.log(fromUint(m_nProbeThreshold, 10), true);
  debug.log(F("\tprobe_auto: "), false);
  if (m_bProbeAuto)
    debug.log(F("yes"));
  else
    debug.log(F(""));
  debug.log(F("\tauto_run_time: "), false);
  debug.log(fromUint(m_nAutoRunMinutes, 10), true);

  debug.log(F("\tsuspend: "), false);
  if (!m_dateSuspendStart.isEmpty() && !m_dateSuspendEnd.isEmpty())
  {
    debug.log(m_dateSuspendStart.toString(), false);
    debug.log(F(" -> "), false);
    debug.log(m_dateSuspendEnd.toString());
  }
  else
    debug.log(F(""));

  CDate date(rtc);
  debug.log(F("\t"), false);
  debug.log(date.toString(false), false);
  debug.log(F(": "), false);

  bool bFlag = false;
  for (uint8_t nI = 0; nI < MAX_STARTS; nI++)
  {
    if (!m_arrayStarts[nI].isEmpty())
    {
      if (nI > 0)
        debug.log(F(","), false);
      m_arrayStarts[nI].dump();
      bFlag = true;
    }
  }
  if (!bFlag)
    debug.log(F("No starts for this date..."));
  else
    debug.log(F(""));
}

extern CWifiManager WifiManager;

void CProgramStation::stationOff(CTimer* pTimer, CSerialManager* pSerialManager)
{
  #ifdef AUTO_SOLENOID
    if (pSerialManager)
    {
      CBuff<16> buff;
      CString str(buff);
      str = F("control");
      str += pSerialManager->getDelim();
      str += F("off");
      pSerialManager->writeWord(str, m_ipAddr);
    }
    else
      debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
  #else
    digitalWrite(m_nRelayPinNum, LOW);
  #endif
  CTime time(rtc);
  debug.logEventOpen('#', F("STATION OFF"), true);
  debug.log(F("Station "), false);
  debug.log(m_nStationNum, false);
  debug.log(F(" turned off at "), false);
  debug.log(time.toString(true), false);
  debug.log(F("!"));
  pTimer->clear();
  debug.logEventClose('#', F(""));
}

void CProgramStation::stationOn(const uint16_t nMinutes, CTimer* pTimer, CSerialManager* pSerialManager)
{
  #ifdef AUTO_SOLENOID
    if (pSerialManager)
    {
      CBuff<16> buff;
      CString str(buff);
      str = F("control");
      str += pSerialManager->getDelim();
      str += F("on");
      pSerialManager->writeWord(str, m_ipAddr);
    }
    else
      debug.logRuntimeError(F("ProgramStation.cpp"), __LINE__);
  #else
    digitalWrite(m_nRelayPinNum, HIGH);
  #endif
  CTime time(rtc);
  debug.logEventOpen('#', F("STATION ON"), true);
  debug.log(F("Station "), false);
  debug.log(m_nStationNum, false);
  debug.log(F(" turned on at "), false);
  debug.log(time.toString(true), false);
  debug.log(F(" for "), false);
  debug.log(nMinutes, false);
  debug.log(F(" minute(s)!"));
  debug.logEventClose('#', F(""));

  g_nStationNum = m_nStationNum;
  g_pTimer = pTimer;
  g_pSerialManager = pSerialManager;
  pTimer->set(m_nStationNum, nMinutes * 60, turnStationOff);
}

bool CProgramStation::checkAlarms(const uint32_t nElapsedSecs, CString& strMessage, CList *pList)
{
  bool bResult = false;

  if (m_bMoistureProbe)
  {
    if (nElapsedSecs > (uint32_t)m_nAllowedDryTimeMinutes * 60)
    {
      uint16_t nExcessMin = (nElapsedSecs / 60) - m_nAllowedDryTimeMinutes,
               nExcessSec = nElapsedSecs % 60,
               nSoilReading = analogRead(m_nProbePinNum) * (100.0 / 1023);
      
      if (nSoilReading <= m_nProbeThreshold)
      {
        bResult = true;
        strMessage += F("Station ");
        strMessage += fromUint(m_nStationNum, DEC);
        strMessage += F(": moisture level below threshold for ");
        strMessage += fromUint(nExcessMin, DEC);
        strMessage += F(" minute(s) and ");
        strMessage += fromUint(nExcessSec, DEC);
        strMessage += F(" second(s) more that allowed duration of ");
        strMessage += fromUint(m_nAllowedDryTimeMinutes, DEC);
        strMessage += F(" minute(s)");

        if (m_bProbeAuto && !pList->isRunning(m_nStationNum))
        {
          pList->push(m_nStationNum, m_nAutoRunMinutes);
        }
      }
    }
  }
  return bResult;
}

bool CProgramStation::isEmpty()
{
  bool bResult = true;

  for (uint8_t nI = 0; nI < MAX_STARTS; nI++)
  {
    bResult &= m_arrayStarts[nI].isEmpty();
    if (!bResult)
      break;
  }
  return bResult;
}

bool CProgramStation::isDone()
{
  bool bResult = true;

  for (uint8_t nI = 0; nI < MAX_STARTS; nI++)
  {
    bResult &= m_arrayStarts[nI].getHasBeenRun();
    if (!bResult)
      break;
  }
  return bResult;
}

void CProgramStation::empty()
{
  for (uint8_t nI = 0; nI < MAX_STARTS; nI++)
    m_arrayStarts[nI].empty();
  m_dateSuspendStart.set(0, 0, 0, 0);
  m_dateSuspendEnd.set(0, 0, 0, 0);
  m_bMoistureProbe = false;
  m_nAllowedDryTimeMinutes = m_nProbeThreshold = 0;

}

bool CProgramStation::getRun(CDate dateNow, CTime timeNow, CList* pRunList)
{
  uint8_t nRuntime = 0;

  if (!isEmpty() && !isDone() && !m_bProbeAuto)
  {
    // Only run the station if 'dateNow' is after 'm_dateSuspendUntil' or if 'm_dateSuspendUntil' is empty.
    if (m_dateSuspendStart.isEmpty() || m_dateSuspendEnd.isEmpty() || ((dateNow >= m_dateSuspendStart) && (dateNow <= m_dateSuspendEnd)))
    {
      for (uint8_t nI = 0; nI < MAX_STARTS; nI++)
      {
        nRuntime = m_arrayStarts[nI].getRuntime(timeNow);
        if (nRuntime > 0)
        {
          // Add the station number and run time to the list.
          pRunList->add(m_nStationNum, nRuntime);
          break;
        }
      }
    }
  }
  return nRuntime > 0;
}





