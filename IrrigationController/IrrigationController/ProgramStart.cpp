#include <Arduino.h>
#include <MySD.h>
#include "ProgramStart.h"
#include "Common.h"
#include "Debug.h"

CProgramStart::CProgramStart()
{
  m_nRunMinutes = 0;
  m_bHasBeenRun = m_bIsRunning = false;
}

CProgramStart::~CProgramStart()
{
}

bool CProgramStart::parseStartRun(CString& strLine, CTime& timeNow)
{
  bool bResult = true;
  int8_t nPos = 0;
  char strBuffTime[9], strBuffMinutes[9], strBuffStart[17];
  CString strTime(strBuffTime, 9), strMinutes(strBuffMinutes, 9), strStart(strBuffStart, 17);
  CTime timeTemp;

  if (strLine.length() > 0)
  {
    nPos = strLine.indexOf(',');

    if (nPos > 0)
    {
      strStart = strLine.substring(0, nPos - 1);
      strLine = strLine.substring(nPos + 1, strLine.length());
    }
    else
    {
      strStart = strLine;
      strLine.empty();
    }
    nPos = strStart.indexOf('-');
    if (nPos >= 0)
    {
      strTime = strStart.substring(0, nPos - 1);
      strMinutes = strStart.substring(nPos + 1, strStart.length() - 1);
      m_nRunMinutes = strMinutes.toInt();
      bResult &= m_timeStart.parse(strTime.c_str()) && (m_nRunMinutes > 0);
      timeTemp = m_timeStart;
      timeTemp.addMins(m_nRunMinutes * 2);
      m_bHasBeenRun = timeNow >= timeTemp;
    }
    else
      debug.logRuntimeError(F("ProgramStart.cpp"), __LINE__);
  }
  return bResult;
}

void CProgramStart::dump()
{
  debug.log(m_timeStart.toString(), false);
  debug.log(F("-"), false);
  debug.log(fromUint(m_nRunMinutes, 10), false);
  if (m_bHasBeenRun)
    debug.log(F("(done)"), false);
  else
    debug.log(F("(not done)"), false);
}

void CProgramStart::empty()
{
  m_nRunMinutes = 0;
  m_timeStart.set(0, 0, 0);
}

uint8_t CProgramStart::getRuntime(CTime timeNow)
{
  uint8_t nRuntime = 0;

  if (!m_bHasBeenRun)
  {
    if (timeNow >= m_timeStart)
    {
      m_bHasBeenRun = true;
      nRuntime = m_nRunMinutes;
    }
  }
  return nRuntime;
}



