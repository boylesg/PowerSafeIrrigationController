#include <Arduino.h>
#include "Timer.h"
#include "debug.h"
#include "common.h"




void CTimer::poll()
{
/*
  debug.log(F("********** Polling timer "), false);
  debug.log(m_nOwnerID, false);
  debug.log(F(" **********"));
  debug.dump("m_nIntervalSec", m_nIntervalSec);
  debug.dump("m_pCallbackFunc", (uint32_t)m_pCallbackFunc);
  debug.dump("m_nTrigger", m_nTrigger);
  debug.dump("millis()", millis());
  debug.log(F("****************************************"));
*/
  if ((m_nIntervalSec > 0) && m_pCallbackFunc && (millis() >= m_nTrigger))
  {
    m_pCallbackFunc();
    set(m_nOwnerID, m_nIntervalSec, m_pCallbackFunc);
  }
}

void CTimer::clear()
{
  m_nIntervalSec = 0;
  m_nTrigger = 0;
  m_pCallbackFunc = NULL;
  m_nOwnerID = 0;
}

void CTimer::set(const uint8_t nOwnerID, const uint32_t nIntervalSec, const GlobalFunctionPtrType pCallbackFunc)
{
  m_nIntervalSec = nIntervalSec;
  m_nTrigger = millis() + (m_nIntervalSec * 1000);
  m_pCallbackFunc = pCallbackFunc;
  m_nOwnerID = nOwnerID;
}

void CTimer::dump()
{
  debug.log(F("===================================================================================="));
  debug.log(F("Timer"));
  debug.dump(F("m_nIntervalSec"), m_nIntervalSec);
  debug.dump(F("m_nTrigger"), m_nTrigger);
  debug.dump(F("m_nOwnerID"), m_nOwnerID);
  debug.log(F("===================================================================================="));
}





void CTimerList::poll()
{
  for (uint8_t nI = 0; nI < MAX_TIMERS; nI++)
    m_arrayTimers[nI].poll();
}

void CTimerList::clear(const uint8_t nI)
{
  if ((nI >= 0) && (nI < MAX_TIMERS))
  {
    m_arrayTimers[nI].clear();
  }
}

CTimer* CTimerList::add(const uint8_t nOwnerID, const uint32_t nIntervalSec, const GlobalFunctionPtrType pCallbackFunc)
{
  CTimer* pTimer = NULL;
  
  if (m_nSize < MAX_TIMERS)
  {
    m_arrayTimers[m_nSize++].set(nOwnerID, nIntervalSec, pCallbackFunc);
    pTimer = &m_arrayTimers[m_nSize - 1];
  }
  return pTimer;
}

CTimerList timers;




