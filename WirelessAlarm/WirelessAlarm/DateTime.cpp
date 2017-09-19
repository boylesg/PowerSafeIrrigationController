#include <Arduino.h>
#include <RealTimeClockDS1307.h>
#include "DateTime.h"
#include "Common.h"
#include "Debug.h"
#include "CString.h"




bool isLeapYear(const uint16_t nYear)
{
  return (((nYear % 4) == 0) && ((nYear % 100) != 0)) || ((nYear % 400) == 0);
}

uint8_t getMaxDOM(const uint8_t nMonth, const uint16_t nYear)
{
  int16_t nMaxDOM = 0;

  if ((nMonth == 1) || (nMonth == 3) || (nMonth == 5) || (nMonth == 7) || (nMonth == 8) || (nMonth == 10) || (nMonth == 12))
    nMaxDOM = 31;
  else if ((nMonth == 4) || (nMonth == 6) || (nMonth == 9) || (nMonth == 11))
    nMaxDOM = 30;
  else if (nMonth == 2)
  {
    if (isLeapYear(nYear))
      nMaxDOM = 29;
    else
      nMaxDOM = 28;
  }
  return nMaxDOM;
}

const char* getDayName(const uint8_t nDayNum, const bool bShort)
{
  static char strDayName[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  memset(strDayName, 0, 10);

  if (nDayNum == 1)
    strcpy(strDayName, "Sunday");
  else if (nDayNum == 2)
    strcpy(strDayName, "Monday");
  else if (nDayNum == 3)
    strcpy(strDayName, "Tuesday");
  else if (nDayNum == 4)
    strcpy(strDayName, "Wednesday");
  else if (nDayNum == 5)
    strcpy(strDayName, "Thursday");
  else if (nDayNum == 6)
    strcpy(strDayName, "Friday");
  else if (nDayNum == 7)
    strcpy(strDayName, "Saturday");
  else
    strcpy(strDayName, "Invalid day number");

  if ((strlen(strDayName) > 3) && bShort)
    strDayName[3] = 0;

  return strDayName;
}

uint8_t calcDOW(const uint8_t nDOM, const uint8_t nMonth, uint16_t nYear)
{
  uint8_t nDOW = 0;
  int arrayCenturyTable[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
 
  nYear -= nMonth < 3;
  nDOW = ((nYear + (nYear / 4) - (nYear / 100) + (nYear / 400) + arrayCenturyTable[nMonth - 1] + nDOM) % 7) + 1;
  
  return nDOW;
}

bool isNumeric(const char* str)
{
  bool bResult = true;

  for (uint8_t nI = 0; nI < strlen(str); nI++)
    bResult &= isDigit(str[nI]);

  return bResult;
}




const uint16_t CDate::m_nCentury = 2000;

CDate::CDate()
{
    m_nDOW = m_nDOM = m_nMonth = 0;
    m_nYear = 0;
}

CDate::CDate(CDate& date)
{
  set(date.m_nDOW ,date.m_nDOM, date.m_nMonth, date.m_nYear);
}

CDate::CDate(RealTimeClockDS1307& rtc)
{
  rtc.readClock();
  m_nDOW = rtc.getDayOfWeek();
  m_nDOM = rtc.getDate();
  m_nMonth = rtc.getMonth();
  m_nYear = rtc.getYear() + m_nCentury;
}

 CDate::CDate(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear)
 {
   set(nDOW ,nDOM, nMonth, nYear);
 }
 
 CDate::CDate(const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear)
 {
   m_nDOW = calcDOW(nDOM, nMonth, nYear);
   set(m_nDOW, nDOM, nMonth, nYear);
 }
 
CDate::~CDate()
{
}

int16_t CDate::getYear(const bool bTwoDigit)
{
  if (bTwoDigit)
    return m_nYear - m_nCentury;
  else
    return m_nYear;
}

void CDate::addDays(const int8_t nDays)
{
  if (nDays > 0)
  {
    for (uint8_t nI = 0; nI < nDays; nI++)
    {
      m_nDOM++;
      if (m_nDOM > ::getMaxDOM(m_nMonth, m_nYear))
      {
        m_nDOM = 1;
        m_nMonth++;

        if (m_nMonth > 12)
        {
          m_nMonth = 1;
          m_nYear++;
        }
      }
    }
  }
  else if (nDays < 0)
  {
    for (uint8_t nI = 0; nI > nDays; nI--)
    {
      m_nDOM--;
      if (m_nDOM <= 0)
      {
        m_nMonth--;
        if (m_nMonth <= 0)
        {
          m_nMonth = 12;
          m_nYear--;
          m_nDOM = ::getMaxDOM(m_nMonth, m_nYear);
        }
        else
        {
          m_nDOM = ::getMaxDOM(m_nMonth, m_nYear);
        }
      }
    }
  }
  m_nDOW = calcDOW(m_nDOM, m_nMonth, m_nYear);
}

bool CDate::isValid()
{
  return (m_nDOW >= 1) && (m_nDOW <= 7) && (m_nMonth >= 1) && (m_nMonth <= 12) && (m_nDOM >= 1) && (m_nDOM <= ::getMaxDOM(m_nMonth, m_nYear));
}

void CDate::setYear(const uint16_t nYear)
{
  m_nYear = nYear;
  if (m_nYear < m_nCentury)
    m_nYear += m_nCentury;
}

bool CDate::parse(const char* str)
{
  bool bResult = true;
  char strBuffDate[33];
  CString strDate(strBuffDate, 33);

  strDate = str;
  if (strDate.length() > 0)
  {
    int8_t nPos1 = strDate.indexOf("/"), nPos2 = strDate.indexOf("/", nPos1 + 1);
    char strBuffTemp[17];
    CString strTemp(strBuffTemp, 17);

    // Day of month and month only
    if ((nPos1 > -1) && (nPos2 == -1))
    {
      strTemp = strDate.substring(0, nPos1 - 1);
      bResult &= isNumeric(strTemp.c_str());
      m_nDOM = strTemp.toInt();
      strTemp = strDate.substring(nPos1 + 1, strDate.length());
      bResult &= isNumeric(strTemp.c_str());
      m_nMonth = strTemp.toInt();
      rtc.readClock();
      m_nYear = m_nCentury + rtc.getYear();
      m_nDOW = calcDOW(m_nDOM, m_nMonth, m_nYear);
    }
    // Day of month, month and year
    else if ((nPos1 > -1) && (nPos2 > -1))
    {
      strTemp = strDate.substring(0, nPos1 - 1);
      bResult &= isNumeric(strTemp.c_str());
      m_nDOM = strTemp.toInt();

      strTemp = strDate.substring(nPos1 + 1, nPos2 - 1);
      bResult &= isNumeric(strTemp.c_str());
      m_nMonth = strTemp.toInt();

      strTemp = strDate.substring(nPos2 + 1, strDate.length());
      bResult &= isNumeric(strTemp.c_str());
      m_nYear = strTemp.toInt();

      if (m_nYear < 100)
        m_nYear += m_nCentury;
      m_nDOW = calcDOW(m_nDOM, m_nMonth, m_nYear);
    }
    bResult = isValid();
  }    
  return bResult;
}

const char* CDate::toString(const bool bIncludeDOW, const bool bIncludeYear)
{
  static char strFormatted[33] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  char strDOM[3] = {0,0,0}, strMonth[3] = {0,0,0}, strYear[5] = {0,0,0,0,0};

  memset(strFormatted, 0, 33);
  if (isValid())
  {
    if (bIncludeDOW)
    {
      strcpy(strFormatted, getDayName(m_nDOW, false));
      strcat_P(strFormatted, (char*)F(", "));
    }
    strcpy(strDOM, fromUint(m_nDOM, 10));
    strcpy(strMonth, fromUint(m_nMonth, 10));
    strcpy(strYear, fromUint(m_nYear, 10));
  
    strcat(strFormatted, strDOM);
    strcat_P(strFormatted, (PGM_P)F("/"));
    strcat(strFormatted, strMonth);
  
    if (bIncludeYear)
    {
      strcat_P(strFormatted, (char*)F("/"));
      strcat(strFormatted, strYear);
    }
  }
  return strFormatted;
}

bool CDate::isEmpty()
{
  return (m_nDOW == 0) && (m_nDOM == 0) && (m_nMonth == 0);
}

uint16_t CDate::getDOY()
{
  uint16_t nDOY = 0;
  
  for (uint8_t nI = 1; nI < m_nMonth; nI++)
     nDOY += ::getMaxDOM(m_nMonth, m_nYear);
  nDOY += m_nDOM;
  
  return nDOY;
}

void CDate::set(const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear)
{
  uint8_t nDOW = calcDOW(nDOM, nMonth, nYear);
  set(nDOW, nDOM, nMonth, nYear);
}

void CDate::set(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear)
{
  m_nDOW = nDOW;
  m_nDOM = nDOM;
  m_nMonth = nMonth;
  m_nYear = nYear;

  if (m_nYear < m_nCentury)
    m_nYear += m_nCentury;
       
  if (!isValid())
  {
    m_nDOW = m_nDOM = m_nMonth = 0;
    m_nYear = 0;
  }
}

CDate& CDate::operator =(CDate& dateOther)
{
  m_nDOM = dateOther.m_nDOM;
  m_nMonth = dateOther.m_nMonth;
  m_nYear = dateOther.m_nYear;
  m_nDOW = dateOther.m_nDOW;
  return *this;
} 

bool CDate::operator ==(CDate& dateOther)
{
  return (m_nDOM == dateOther.m_nDOM) && (m_nMonth == dateOther.m_nMonth) && (m_nYear == dateOther.m_nYear);
}

bool CDate::operator >=(CDate& dateOther)
{
  return (*this == dateOther) || (m_nYear > dateOther.m_nYear) || ((m_nYear == dateOther.m_nYear) && (m_nMonth > dateOther.m_nMonth)) || ((m_nYear == dateOther.m_nYear) && (m_nMonth == dateOther.m_nMonth) && (m_nDOM > dateOther.m_nDOM));
}

bool CDate::operator >(CDate& dateOther)
{
  return (m_nYear > dateOther.m_nYear) || ((m_nYear == dateOther.m_nYear) && (m_nMonth > dateOther.m_nMonth)) || ((m_nYear == dateOther.m_nYear) && (m_nMonth == dateOther.m_nMonth) && (m_nDOM > dateOther.m_nDOM));
}

bool CDate::operator <=(CDate& dateOther)
{
  return (*this == dateOther) || (m_nYear > dateOther.m_nYear) || ((m_nYear == dateOther.m_nYear) && (m_nMonth < dateOther.m_nMonth)) || ((m_nYear == dateOther.m_nYear) && (m_nMonth == dateOther.m_nMonth) && (m_nDOM < dateOther.m_nDOM));
}

bool CDate::operator <(CDate& dateOther)
{
  return (m_nYear > dateOther.m_nYear) || ((m_nYear == dateOther.m_nYear) && (m_nMonth < dateOther.m_nMonth)) || ((m_nYear == dateOther.m_nYear) && (m_nMonth == dateOther.m_nMonth) && (m_nDOM < dateOther.m_nDOM));
}

bool CDate::operator !=(CDate& dateOther)
{
  return (m_nYear != dateOther.m_nYear) || (m_nMonth != dateOther.m_nMonth) || (m_nDOM != dateOther.m_nDOM);
}




CTime::CTime()
{
  m_nHours = m_nMinutes = m_nSeconds = 0;
}

CTime::CTime(RealTimeClockDS1307& rtc)
{
  rtc.readClock();
  set(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
}

CTime::~CTime()
{
}

bool CTime::parse(const char* str)
{
  bool bResult = false;
  char strBuffTime[17];
  CString strTime(strBuffTime, 17);
  strTime = str;
  
  if (strTime.length() > 0)
  {
    int8_t nPos1 = strTime.indexOf(':'), nPos2 = strTime.indexOf(':', nPos1 + 1);
    char strBuffTemp[17];
    CString strTemp(strBuffTemp, 17);

    // Hours and minutes
    if ((nPos1 > -1) && (nPos2 == -1))
    {
      strTemp = strTime.substring(0, nPos1 - 1);
      bResult = isNumeric(strTemp.c_str());
      m_nHours = atoi(strTemp.c_str());
      strTemp = strTime.substring(nPos1 + 1, strTime.length());
      bResult &= isNumeric(strTemp.c_str());
      m_nMinutes = atoi(strTemp.c_str());
    }
    // Hours,  minutes and sconds
    else if ((nPos1 > -1) && (nPos2 > -1))
    {
      strTemp = strTime.substring(0, nPos1);
      bResult = isNumeric(strTemp.c_str());
      m_nHours = atoi(strTemp.c_str());
      strTemp = strTime.substring(nPos1 + 1, nPos2);
      bResult &= isNumeric(strTemp.c_str());
      m_nMinutes = atoi(strTemp.c_str());
      strTemp = strTime.substring(nPos2 + 1, strTime.length());
      bResult &= isNumeric(strTemp.c_str());
      m_nSeconds = atoi(strTemp.c_str());
    }
  }
  return bResult;
}
void CTime::padLeft(char* str, const uint8_t nCapacity)
{
  if ((strlen(str) == 1) && (nCapacity > 1))
  {
    str[1] = str[0];
    str[0] = '0';
  }
}

const char* CTime::toString(const bool bIncludeSeconds)
{
  static const uint8_t nSize = 9;
  static char strFormatted[nSize] = {0,0,0,0,0,0,0,0,0};
  char strSec[3] = {0,0,0}, strMin[3] = {0,0,0}, strHr[3] = {0,0,0};
  
  memset(strFormatted, 0, nSize);
  strcpy(strHr, fromUint(m_nHours, 10));
  strcpy(strMin, fromUint(m_nMinutes, 10));
  strcpy(strSec, fromUint(m_nSeconds, 10));
  padLeft(strSec, 2);
  padLeft(strMin, 2);
  padLeft(strHr, 2);
  
  strcpy(strFormatted, strHr);
  strcat_P(strFormatted, (char*)F(":"));
  strcat(strFormatted, strMin);

  if (bIncludeSeconds)
  {
    strcat_P(strFormatted, (char*)F(":"));
    strcat(strFormatted, strSec);
  }
  return strFormatted;
}

bool CTime::isEmpty()
{
  return (m_nHours == 0) && (m_nMinutes == 0) && (m_nSeconds == 0);
}

bool CTime::isValid()
{
  return (m_nHours >= 0) && (m_nHours <= 23) && (m_nMinutes >= 0) && (m_nMinutes <= 59) && (m_nSeconds >= 0) && (m_nSeconds <= 59);
}

void CTime::set(const uint8_t nHour, const uint8_t nMinute, const uint8_t nSecond)
{
  m_nHours = nHour;
  m_nMinutes = nMinute;
  m_nSeconds = nSecond;

  if (!isValid())
  {
    m_nHours = m_nMinutes = m_nSeconds = 0;
  }
}

CTime& CTime::operator =(CTime& timeOther)
{
  m_nHours = timeOther.m_nHours;
  m_nMinutes = timeOther.m_nMinutes;
  m_nSeconds = timeOther.m_nSeconds;
  return *this;
} 

bool CTime::operator ==(CTime& timeOther)
{
  return (m_nSeconds == timeOther.m_nSeconds) && (m_nMinutes == timeOther.m_nMinutes) && (m_nHours == timeOther.m_nHours);
}

bool CTime::operator >=(CTime& timeOther)
{
  return (*this == timeOther) || (m_nHours > timeOther.m_nHours) || ((m_nHours == timeOther.m_nHours) && (m_nMinutes > timeOther.m_nMinutes)) || ((m_nHours == timeOther.m_nHours) && (m_nMinutes == timeOther.m_nMinutes) && (m_nSeconds > timeOther.m_nSeconds));
}

bool CTime::operator >(CTime& timeOther)
{
  return (m_nHours > timeOther.m_nHours) || ((m_nHours == timeOther.m_nHours) && (m_nMinutes > timeOther.m_nMinutes)) || ((m_nHours == timeOther.m_nHours) && (m_nMinutes == timeOther.m_nMinutes) && (m_nSeconds > timeOther.m_nSeconds));
}

bool CTime::operator <=(CTime& timeOther)
{
  return (*this == timeOther) || (m_nHours < timeOther.m_nHours) || ((m_nHours == timeOther.m_nHours) && (m_nMinutes < timeOther.m_nMinutes)) || ((m_nHours == timeOther.m_nHours) && (m_nMinutes == timeOther.m_nMinutes) && (m_nSeconds < timeOther.m_nSeconds));;
}

bool CTime::operator <(CTime& timeOther)
{
  return (m_nHours < timeOther.m_nHours) || ((m_nHours == timeOther.m_nHours) && (m_nMinutes < timeOther.m_nMinutes)) || ((m_nHours == timeOther.m_nHours) && (m_nMinutes == timeOther.m_nMinutes) && (m_nSeconds < timeOther.m_nSeconds));;
}

bool CTime::operator !=(CTime& timeOther)
{
  return (m_nHours != timeOther.m_nHours) || (m_nMinutes != timeOther.m_nMinutes) || (m_nSeconds != timeOther.m_nSeconds);
}

uint32_t CTime::operator -(CTime& timeOther)
{
  uint8_t nDiffHrs = 0, nDiffMins = 0, nDiffSecs = 0;
  
  if (m_nHours >= timeOther.m_nHours)
    nDiffHrs = m_nHours - timeOther.m_nHours;
  else
    nDiffHrs = (24 - timeOther.m_nHours) + m_nHours;

  if (m_nMinutes >= timeOther.m_nMinutes)
    nDiffMins = m_nMinutes - timeOther.m_nMinutes;
  else
    nDiffMins = (60 - timeOther.m_nMinutes) + m_nMinutes;
    
  if (m_nSeconds >= timeOther.m_nSeconds)
    nDiffSecs = m_nSeconds - timeOther.m_nSeconds;
  else
    nDiffSecs = (60 - timeOther.m_nSeconds) + m_nSeconds;

  return (nDiffHrs * 3600) + (nDiffMins * 60) + nDiffSecs;
}

bool CTime::equalApprox(CTime& timeNow, const uint8_t nMinutes, const uint8_t nSeconds)
{
  return abs(m_nMinutes - timeNow.m_nMinutes) <= nMinutes;
}

void CTime::addSeconds(const int8_t nSeconds)
{
  m_nSeconds += nSeconds;
  if ((nSeconds > 0) && (m_nSeconds >= 60))
  {
    addMins(nSeconds / 60);
    m_nSeconds = nSeconds % 60;
  }
  else if ((nSeconds < 0) && (m_nSeconds < 0))
  {
    addMins(nSeconds / 60);
    m_nSeconds += 60;
  }
}

void CTime::addMins(const int8_t nMinutes)
{
  m_nMinutes += nMinutes;
  if ((nMinutes > 0) && (m_nMinutes >= 60))
  {
    addHours(nMinutes / 60);
    m_nMinutes = nMinutes % 60;
  }
  else if ((nMinutes < 0) && (m_nMinutes < 0))
  {
    addHours(nMinutes / 60);
    m_nMinutes += 60;
  }
}

void CTime::addHours(const int8_t nHours)
{
  m_nHours += nHours;

  if ((nHours > 0) && (m_nHours >= 24))
  {
    m_nHours %= 24;
  }
  else if ((nHours < 0) && (m_nHours < 0))
  {
    m_nHours += 24;
  }
}




CDateTime::CDateTime()
{
}

CDateTime::CDateTime(RealTimeClockDS1307& rtc)
{
  rtc.readClock();
  m_date.set(rtc.getDayOfWeek(), rtc.getDate(), rtc.getMonth(), rtc.getYear());
  m_time.set(rtc.getHours(), rtc.getMinutes(), rtc.getHours());
}

CDateTime::CDateTime(CDateTime& datetime)
{
  set(datetime.m_date.m_nDOW ,datetime.m_date.m_nDOM, datetime.m_date.m_nMonth, datetime.m_date.m_nYear, datetime.m_time.m_nHours, datetime.m_time.m_nMinutes, datetime.m_time.m_nSeconds);
}

CDateTime::CDateTime(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear, const uint8_t nHours, const uint8_t nMinutes, const uint8_t nSeconds)
{
  set(nDOW ,nDOM, nMonth, nYear, nHours, nMinutes, nSeconds);
}
 
CDateTime::CDateTime(const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear, const uint8_t nHours, const uint8_t nMinutes, const uint8_t nSeconds)
{
  set(calcDOW(nDOM, nMonth, nYear) ,nDOM, nMonth, nYear, nHours, nMinutes, nSeconds);
}
 
CDateTime::~CDateTime()
{
}

void CDateTime::set(const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear, const uint8_t nHours, const uint8_t nMinutes, const uint8_t nSeconds)
{
	m_date.set(nDOM, nMonth, nYear);
	m_time.set(nHours, nMinutes, nSeconds);
}

void CDateTime::set(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear, const uint8_t nHours, const uint8_t nMinutes, const uint8_t nSeconds)
{
	m_date.set(nDOW, nDOM, nMonth, nYear);
	m_time.set(nHours, nMinutes, nSeconds);
}

const char* CDateTime::toString(const bool bIncludeSeconds, const bool bIncludeDOW, const bool bIncludeYear)
{
  static const uint8_t nSize = 33;
  static char str[nSize] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  memset(str, 0, nSize);
  strcpy(str, m_date.toString(bIncludeDOW, bIncludeYear));
  str[strlen(str)] = ' ';
  strcat(str, m_time.toString(bIncludeSeconds));
  
  return str;
}

bool CDateTime::isValid()
{
  return m_date.isValid() && m_time.isValid();
}
 
bool CDateTime::isEmpty()
{
  return m_date.isEmpty() && m_time.isEmpty();
}

void CDateTime::addSeconds(const int8_t nSeconds)
{
  m_time.addSeconds(nSeconds);
}

void CDateTime::addMins(const int8_t nMinutes)
{
  m_time.addMins(nMinutes);
}

void CDateTime::addHours(const int8_t nHours)
{
  uint8_t nTempHours = m_time.m_nHours;
  m_time.addHours(nHours);
  m_date.addDays((nTempHours + nHours) / 24);
}

void CDateTime::adjustDaylightSavings()
{
  if (((m_date.getMonth() >= 10) && (m_date.getMonth() <= 12)) || ((m_date.getMonth() >= 1) && (m_date.getMonth() < 4)))
  {
    int8_t nHours = m_time.m_nHours + 1, nDays = 0;
    if (nHours == 24)
    {
      nHours = 0;
      nDays = 1;
    }
    m_time.m_nHours = nHours;
    m_date.addDays(nDays);
  }
}




