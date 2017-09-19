
#ifndef __TIMER_H_
#define __TIMER_H_

#include "Common.h"




const uint32_t WEEKLY = 604800;
const uint32_t DAILY = 1440;
const uint32_t TEN_MINUTES = 600;
const uint32_t FIVE_MINUTES = 300;
const uint32_t TWO_MINUTES = 120;
const uint32_t MINUTE = 60;




class CTimer
{
  public:
    // Construction, destruction & initialisation
    CTimer()
    {
      set(0, 0, NULL);
    }
    CTimer(const uint8_t nOwnerID, const uint32_t nIntervalSec, const GlobalFunctionPtrType pCallbackFunc)
    {
      set(nOwnerID, nIntervalSec, pCallbackFunc);
    }
    ~CTimer()
    {
    };

    // Interface
    void poll();
    void set(const uint8_t nOwnerID, const uint32_t nIntervalSec, const GlobalFunctionPtrType pCallbackFunc);
    void clear();
    uint8_t getOwnerID()
    {
      return m_nOwnerID;
    };
    bool isRunning()
    {
      return m_nIntervalSec > 0;
    };
    void dump();

  protected:

    // Impelementation
    uint32_t m_nTrigger, m_nIntervalSec;
    uint8_t m_nOwnerID;
    GlobalFunctionPtrType m_pCallbackFunc;
};


#if defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__) || defined (__SAM3X8E__)
  const uint8_t MAX_TIMERS = 5;
#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)
  const uint8_t MAX_TIMERS = 3;
#endif

class CTimerList
{
  public:
    // Construction, destruction & initialisation
    CTimerList()
    {
      m_nSize = 0;
    }
    ~CTimerList()
    {
      m_nSize = 0;
    }

    // Interface
    void poll();
    void clear(const uint8_t nI);
    CTimer* add(const uint8_t nOwnerID, const uint32_t nIntervalSec, const GlobalFunctionPtrType pCallbackFunc);
    uint8_t getSize()
    {
      return m_nSize;
    };
    int8_t getLast()
    {
      return (int8_t)m_nSize - 1;
    };

  protected:

    // Impelementation
    CTimer m_arrayTimers[MAX_TIMERS];
    uint8_t m_nSize;
};

extern CTimerList timers;

#endif

