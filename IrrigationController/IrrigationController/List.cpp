#include <Arduino.h>
#include "Debug.h"
#include "Common.h"
#include "ProgramStation.h"
#include "List.h"




CListItem::CListItem()
{
  m_nStationNum = m_nRunMinutes = 0;
}

CListItem::CListItem(const uint8_t nStationNum, const uint8_t nRunMinutes)
{
  m_nStationNum = nStationNum;
  m_nRunMinutes = nRunMinutes;
}

CListItem::~CListItem()
{
  m_nStationNum = m_nRunMinutes = 0;
}

void CListItem::set(const uint8_t nStationNum, const uint8_t nRunMinutes)
{
  m_nStationNum = nStationNum;
  m_nRunMinutes = nRunMinutes;
}

void CListItem::reset()
{
  m_nStationNum = m_nRunMinutes = 0;
}

bool CListItem::isEmpty()
{
  return (m_nStationNum == 0) && (m_nRunMinutes == 0);
}

CListItem& CListItem::operator =(CListItem& other)
{
  m_nStationNum = other.m_nStationNum;
  m_nRunMinutes = other.m_nRunMinutes;
}

void CListItem::dump()
{
  debug.dump(F("m_nStationNum"), m_nStationNum, false);
  debug.log(F(", "), false);
  debug.dump(F("m_nRunMinutes"), m_nRunMinutes);
  
}




CList::CList()
{
  m_nSize = 0;
}

CList::~CList()
{
  m_nSize = 0;
}

bool CList::isEmpty()
{
  return m_nSize == 0;
}

bool CList::add(uint8_t nStationNum, uint8_t nRunMinutes)
{
  bool bResult = false;

  if (m_nSize < MAX_STARTS)
  {
    m_arrayStarts[m_nSize].set(nStationNum, nRunMinutes);
    m_nSize++;
    bResult = true;
  }
  return bResult;
}

bool CList::push(uint8_t nStationNum, uint8_t nRunMinutes)
{
  bool bResult = false;

  if (m_nSize < MAX_STARTS)
  {
    for (uint8_t nI = m_nSize; nI > 0; nI--)
    {
      m_arrayStarts[nI] = m_arrayStarts[nI - 1];
    }
    m_arrayStarts[0].set(nStationNum, nRunMinutes);
    m_nSize++;
    bResult = true;
  }
  return bResult;
}

bool CList::remove(const uint8_t nStationNum)
{
  bool bResult = false;

  if (m_nSize < MAX_STARTS)
  {
    for (uint8_t nI = 0; nI < MAX_STARTS; nI++)
    {
      if (m_arrayStarts[nI].m_nStationNum == nStationNum)
      {
        if (nI == (MAX_STARTS - 1))
          m_arrayStarts[nI].reset();
        else
        {
          for (uint8_t nJ = nI; nJ < (m_nSize - 1); nJ++)
          {
            m_arrayStarts[nJ] = m_arrayStarts[nJ + 1];
          }
          m_arrayStarts[m_nSize - 1].reset();
          m_nSize--;
        }
        bResult = true;
        break;
      }
    }
  }
  return bResult;
}

bool CList::getNext(uint8_t& nStationNum, uint8_t& nRunMinutes)
{
  bool bResult = false;

  if (m_nSize > 0)
  {
    nStationNum = m_arrayStarts[0].m_nStationNum;
    nRunMinutes = m_arrayStarts[0].m_nRunMinutes;
    bResult = true;
    for (uint8_t nI = 0; nI < (m_nSize - 1); nI++)
      m_arrayStarts[nI] = m_arrayStarts[nI + 1];
    m_arrayStarts[m_nSize - 1].reset();
    m_nSize--;
  }
  return bResult;
}

void CList::dump()
{
  debug.log(F("===================================================================================="));
  debug.log(F("Run list - "), false);
  debug.dump(F("m_nSize"), m_nSize);
  debug.log(F(""));
  for (uint8_t nStation = 0; nStation < MAX_STARTS; nStation++)
    m_arrayStarts[nStation].dump();
  debug.log(F("===================================================================================="));
}

bool CList::isRunning(const uint8_t nStation)
{
  bool bResult = false;

  if ((m_nSize >= 1) && (m_nSize <= MAX_STARTS))
  {
    for (uint8_t nI = 0; nI < m_nSize; nI++)
    {
      bResult = m_arrayStarts[nI].m_nStationNum == nStation;
      if (bResult)
        break;
    }
  }
  return bResult;
}








