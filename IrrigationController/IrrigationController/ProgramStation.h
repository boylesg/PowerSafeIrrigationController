#ifndef __PROGRAM_STATION_H_
#define __PROGRAM_STATION_H_

#include "DateTime.h"
#include "Common.h"
#include "ProgramStart.h"
#include "TextFile.h"
#include "WifiManager.h"
#include "List.h"
#include "CString.h"
#include "Timer.h"
#include "Constants.h"
#include "AutoSolenoid.h"



class CSerialManager;

class CProgramStation
{
  public:
     // Construction, destruction and initialisation
     CProgramStation();
     virtual ~CProgramStation();
     
    // Interface
    void readProgramPageData(CTextFile& file);
    void saveProgramPageData(CTextFile& file);
    bool read(CTextFile& file, CDate& dateNow, uint8_t nStationNum, uint8_t nDigitalPinNum);
    bool readAlarms(CTextFile& file, const uint8_t nStationNum, const uint8_t nProbePinNum);
    void dump();
    void dumpPageData();
    bool getRun(CDate dateNow, CTime timeNow, CList* pRunList);
    bool checkAlarms(const float fElapsedMinutes, CString& strMessage, CList *pList);
    void stationOn(const uint16_t nMinutes, CTimer* pTimer, CSerialManager* pSerialManager);
    void stationOff(CTimer* pTimer, CSerialManager* pSerialManager);
    bool isEmpty();
    void empty();
    bool isDone();
    void setDescription(const char* strDesc)
    {
      strncpy(m_strDescription, strDesc, m_nDescBuffLen);
    };

  protected:
    // Helper functions
    char* getDOWName(uint8_t nDOW);
    bool readDescription(CTextFile& file);
    bool readProgramSuspend(CTextFile& file);
    void readProgramStarts(CTextFile& file, CDate& dateNow);
    bool findDate(CTextFile& file, CDate& dateNow, CString& strLine);
    bool findStation(CTextFile& file, const uint8_t nStation, CString& strLine);
    bool parseStarts(char* strLine, char* strTime, uint8_t& nRuntimeMinutes);    
    
    static const uint16_t m_nDescBuffLen = 31;

  public:
    // For web page data persistence
    uint8_t m_nRadioFreq, m_nSelectFreq, m_nRunFreq;

    // Data
    uint8_t m_nStationNum, m_nRelayPinNum, m_nProbePinNum, m_nAutoRunMinutes, m_nAllowedDryTimeMinutes, m_nRuntime, m_nProbeThreshold;

    CTime m_timeStart, m_timeEnd;
    CDate m_dateSuspendStart, m_dateSuspendEnd;
    bool m_bMoistureProbe, m_bProbeAuto;
    CProgramStart m_arrayStarts[MAX_STARTS];
    char m_strDescription[m_nDescBuffLen];
    #ifdef AUTO_SOLENOID
      IPAddress m_ipAddr;
      bool m_bActive;
    #endif
};


#endif


