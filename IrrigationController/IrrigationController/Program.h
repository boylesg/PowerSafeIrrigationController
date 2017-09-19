#ifndef __PROGRAM_H_
#define __PROGRAM_H_

#include "ProgramStation.h"
#include "WifiManager.h"
#include "List.h"
#include "CString.h"
#include "Common.h"
#include "Constants.h"
#include "AutoSolenoid.h"




class CSerialManager;

#define STATION_OFF               1
#define STATION_LISTED            2
#define STATION_DELISTED          3
#define STATION_ON                4
#define STATION_ALREADY_ON        5
#define STATION_ALREADY_OFF       6
#define STATION_ALREADY_LISTED    7
#define STATION_ALREADY_DELISTED  8
#define STATION_INVALID           9




class CProgram
{
  public:
     // Construction, destruction and initialisation
     CProgram();
     virtual ~CProgram();

    // Interface
    bool read();
    void dump();
    void dumpPageData(const uint8_t nStation);
    void dumpPageData();
    void run(CSerialManager& SerialManager);
    void begin(CSerialManager& SerialManager);

  	bool openIrrigProgramPageDataFile(CTextFile& file, CString &strMsg);
  	bool openIrrigProgramFile(CTextFile& file, CString &strMsg);
  	void saveIrrigProgPageData(const uint8_t nStationNum, CTextFile& fileIrrigProgPageData, const char* strDesc, const uint8_t nRadioFreq, const uint8_t nSelectFreq,
  								const uint8_t nRuntime, const uint8_t nRunFreq, const char* strStartTime, const char* strEndTime, const char* strSuspendStartDate, 
  								const char* strSuspendEndDate);
  	void saveIrrigProg(const uint8_t nStationNum, CTextFile& fileIrrigProg, const uint8_t nNumDays, const char* strStartTime, const char* strEndTime, 
  					   const uint8_t nNumHours, const uint8_t nRunMins, const char* strSuspendStartDate, const char* strSuspendEndDate, const char* strDesc);
  
  	void updateAlarmData(const uint8_t nStation, const bool bInstalled, const bool bAuto, const uint8_t nDryTime, const uint8_t nThreshold, const uint8_t nRuntime);
  	bool saveAlarmsData(CString &strMsg);

    CProgramStation* getStationDetails(uint8_t nI);
    CTime& checkAlarms(CWifiManager& WifiManager, CTime& timeLast);
    uint8_t stationOn(const uint8_t nStation, const uint16_t nMinutes, CSerialManager* pSerialManager = NULL);
    void stationOff(const uint8_t nStation, CTimer* pTimer, CSerialManager *pSerialManager = NULL);
    uint8_t stationOff(const uint8_t nStation, CSerialManager* pSerialManager = NULL);
    void empty();
    void poll();
    bool isRunning(const uint8_t nStation);
    const char* getStationDesc(const uint8_t nStation);
    uint8_t getRadioFreq(const uint8_t nStation);
    uint8_t getSelectFreq(const uint8_t nStation);
    uint8_t getRunFreq(const uint8_t nStation);
    uint8_t getRunTime(const uint8_t nStation);
    CTime& getStartTime(const uint8_t nStation);
    CTime& getEndTime(const uint8_t nStation);
    CDate& getStartSuspendDate(const uint8_t nStation);
    CDate& getEndSuspendDate(const uint8_t nStation);
    uint8_t getAlarmThreshold(const int nStation);
    uint8_t getAlarmOnTime(const int nStation);
    uint8_t getAlarmDryTime(const int nStation);
    bool getAlarmAuto(const int nStation);
    bool getAlarmInstalled(const int nStation);
    uint8_t getDayInterval(const uint8_t nRadioFreq, const uint8_t nSelFreq);
    uint8_t getHourInterval(const uint8_t nRunFreq);

    #ifdef AUTO_SOLENOID
      bool isActive(const uint8_t nStationNum);
      void setActive(const uint8_t nStationNum, IPAddress ipAddr);
      void getRemoteStations(CSerialManager& SerialManager);
    #endif    
  protected:

    // Helpers
    const char* getStartTimes(const char* strStartTime, const char* strEndTime, const uint8_t nNumHours, const uint8_t nRunMins);
	  bool readProgramPageData();

    // Data
    CProgramStation m_arrayStations[MAX_STATIONS];
    static const uint8_t m_arrayRelayPinNums[MAX_STATIONS];
    static const uint8_t m_arrayProbePinNums[MAX_STATIONS];
    
    const __FlashStringHelper* m_strProgramPageDataFilename;
    const __FlashStringHelper* m_strProgramFilename;
    const __FlashStringHelper* m_strAlarmsFilename;
    
    CList m_listRun;
    CTimer *m_pTimer;
    CTime m_time;
    CDate m_date;

    static CTextFile m_file;
};

extern CProgram program;

#endif

