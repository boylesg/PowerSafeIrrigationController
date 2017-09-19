#ifndef __DATE_TIME_H_
#define __DATE_TIME_H_




#include <RealTimeClockDS1307.h>




bool isLeapYear(const uint16_t nYear);
uint8_t getMaxDOM(const uint8_t nMonth, const uint16_t nYear);
const char* getDayName(const uint8_t nDayNum, const bool bShort);
uint8_t calcDOW(const uint8_t nDOM, const uint8_t nMonth, uint16_t nYear);




class CDate
{
  public:
     // Construction, destruction and initialisation
     CDate();
     CDate(CDate& date);
     CDate(RealTimeClockDS1307& rtc);
     CDate(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear);
     CDate(const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear);
     virtual ~CDate();
    
     bool parse(const char* str);
     void set(const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear);
     void set(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear);
     void empty()
     {
       set(0, 0, 0, 0);
     };
     void setDOW(const uint8_t nDOW)
     {
       m_nDOW = nDOW;
     }
     void setDOM(const uint8_t nDOM)
     {
       m_nDOM = nDOM;
     }
     void setMonth(const uint8_t nMonth)
     {
       m_nMonth = nMonth;
     }
     void setYear(const uint16_t nYear);
     void setCalcDOW()
     {
       m_nDOW = calcDOW(m_nDOM, m_nMonth, m_nYear);
     }
     CDate& operator =(CDate& dateOther);
     bool operator ==(CDate& dateOther);
     bool operator >=(CDate& dateOther);
     bool operator <=(CDate& dateOther);
     bool operator >(CDate& dateOther);
     bool operator <(CDate& dateOther);
     bool operator !=(CDate& dateOther);
    
     void addDays(const int8_t nDays);
     void addWeeks(const int16_t nWeeks)
     {
       addDays(nWeeks * 7);
     };
     void addYears(const int16_t nYears)
     {
       addDays((float)nYears * 365.25);
     };
     bool isEmpty();
    
     int16_t getDOM()
     {
       return m_nDOM;
     };

     int16_t getDOW()
     {
       return m_nDOW;
     };
    
     int16_t getMonth()
     {
       return m_nMonth;
     };
    
     uint16_t getDOY();
     
     int16_t getYear(const bool bTwoDigit = true);

     int16_t getMaxDOM()
     {
       return ::getMaxDOM(m_nMonth, m_nYear);
     };
     bool isValid();
     bool isLeapYear()
     {
       return ::isLeapYear(m_nYear);
     };

     const char* toString(const bool bIncludeDOW = false, const bool bIncludeYear = false);

     static const uint16_t m_nCentury;

     friend class CDateTime;
    
  protected:

    // Data
    int8_t m_nDOW, m_nDOM, m_nMonth;
    int16_t m_nYear;
};




class CTime
{
  public:
     // Construction, destruction and initialisation
     CTime();
     CTime(CTime& time)
     {
       set(time.m_nHours, time.m_nMinutes, time.m_nSeconds);
     }
     CTime(RealTimeClockDS1307& rtc);
     CTime(const uint8_t nHour, const uint8_t nMinute, const uint8_t nSecond)
     {
       set(nHour, nMinute, nSecond);
     };
     virtual ~CTime();

     bool parse(const char* str);
     void set(const uint8_t nHour, const uint8_t nMinute, const uint8_t nSecond);
     void empty()
     {
       set(0, 0, 0);
     };
     CTime& operator =(CTime& timeOther);
     bool operator ==(CTime& timeOther);
     bool operator >=(CTime& timeOther);
     bool operator <=(CTime& timeOther);
     bool operator >(CTime& timeOther);
     bool operator <(CTime& timeOther);
     bool operator !=(CTime& timeOther);
     uint32_t operator -(CTime& timeOther);
     bool equalApprox(CTime& timeNow, const uint8_t nMinutes, const uint8_t nSecond = 0);
     bool isValid();
     bool isEmpty();

     void addSeconds(const int8_t nSeconds);
     void addMins(const int8_t nMinutes);
     void addHours(const int8_t nHours);

     int8_t getSeconds()
     {
       return m_nSeconds;
     };

     int8_t getMinutes()
     {
       return m_nMinutes;
     };

     int8_t getHours()
     {
       return m_nHours;
     };

     const char* toString(const bool bIncludeSeconds = false);

  protected:
    // Data
    int8_t m_nHours, m_nMinutes, m_nSeconds;
    
    void padLeft(char* str, const uint8_t nCapacity);

	  friend class CDateTime;
};




class CDateTime
{
  public:
    // Construction, destruction and initialisation
    CDateTime();
    CDateTime(CDateTime& datetime);
    CDateTime(RealTimeClockDS1307& rtc);
    CDateTime(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear, const uint8_t nHours, const uint8_t nMinutes, const uint8_t nSeconds);
    CDateTime(const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear, const uint8_t nHours, const uint8_t nMinutes, const uint8_t nSeconds);
    virtual ~CDateTime();
  
	void set(const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear, const uint8_t nHours, const uint8_t nMinutes, const uint8_t nSeconds);
	void set(const uint8_t nDOW, const uint8_t nDOM, const uint8_t nMonth, const uint16_t nYear, const uint8_t nHours, const uint8_t nMinutes, const uint8_t nSeconds);
	void empty()
    {
      m_time.empty();
      m_date.empty();
    };
    int16_t getDOW()
    {
      return m_date.getDOW();
    };
    
    int16_t getDOM()
    {
      return m_date.getDOM();
    };
    
    int16_t getMonth()
    {
      return m_date.getMonth();
    };
    
    int16_t getYear(const bool bTwoDigit = true)
    {
      return m_date.getYear(bTwoDigit);
    };
  
    int16_t getMaxDOM()
    {
      return m_date.getMaxDOM();
    };
   
    uint16_t getDOY()
    {
      return m_date.getDOY();
    };
    
    int16_t getSeconds()
    {
      return m_time.getSeconds();
    };

    int16_t getMinutes()
    {
      return m_time.getMinutes();
    };

    int16_t getHours()
    {
      return m_time.getHours();
    };

    const char* toString(const bool bIncludeSeconds = false, const bool bIncludeDOW = false, const bool bIncludeYear = false);
     
    bool isValid();
    bool isEmpty();
  
    void addSeconds(const int8_t nSeconds);
    void addMins(const int8_t nMinutes);
    void addHours(const int8_t nHours);
    void addDays(const int16_t nDays)
    {
     m_date.addDays(nDays);
    };
    void addWeeks(const int16_t nWeeks)
    {
      m_date.addDays(nWeeks * 7);
    };
    void addYears(const int16_t nYears)
    {
    	m_date.addDays((float)nYears * 365.25);
    };
  
    void adjustDaylightSavings();

	operator CDate&()
	{
		return m_date;
	};

	operator CTime&()
	{
		return m_time;
	};
     
  protected:

    // Data
    CDate m_date;
    CTime m_time;
  
};




#endif


