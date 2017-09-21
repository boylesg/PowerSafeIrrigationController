#ifndef __PROGRAM__START_H_
#define __PROGRAM__START_H_

#include "DateTime.h"
#include "CString.h"




class CProgramStart
{
  public:
    // Construction, destruction and initialisation
    CProgramStart();
    virtual ~CProgramStart();

    // Interface
    bool parseStartRun(CString& strLine, CTime& timeNow);
    void dump();
    bool isEmpty()
    {
      return  m_nRunMinutes == 0;
    }
    uint8_t getRuntime(CTime timeNow);
    uint8_t getRuntime()
    {
      return m_nRunMinutes;
    }
    CTime& getStartTime()
    {
      return m_timeStart;
    }
    void setRunning(const bool bIsRunning)
    {
      m_bIsRunning = bIsRunning;
    }
    bool isRunning()
    {
      return m_bIsRunning;
    }
    bool getHasBeenRun()
    {
      return m_bHasBeenRun;
    }
    bool setHasBeenRun(const bool bHasBeenRun)
    {
      m_bHasBeenRun = bHasBeenRun;
    }
    void empty();

  protected:
    // Helper functions
    
    // Data
    CTime m_timeStart;
    uint8_t m_nRunMinutes;
    bool m_bHasBeenRun, m_bIsRunning;
};

#endif


