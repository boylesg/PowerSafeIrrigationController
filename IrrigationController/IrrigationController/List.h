#ifndef __LIST_H_
#define __LIST_H_

#include "Constants.h"




class CListItem
{
  public:
    // Construction & destruction
    CListItem();
    CListItem(const uint8_t nStationNum, const uint8_t nRunMinutes);
    ~CListItem();
    void set(const uint8_t nStationNum, const uint8_t nRunMinutes);
    void reset();
    bool isEmpty();
    CListItem& operator =(CListItem& other);
    void dump();

    // Implementation
    uint8_t m_nStationNum, m_nRunMinutes;
  
};




class CList
{
  public:
    // Construction, destruction and initialisation
    CList();
    ~CList();
    bool push(uint8_t nStationNum, uint8_t nRunMinutes);
    bool add(uint8_t nStationNum, uint8_t nRunMinutes);
    bool getNext(uint8_t& nStationNum, uint8_t& nRunMinutes);
    bool remove(const uint8_t nStationNum);
    bool isEmpty();
    uint8_t size()
    {
      return m_nSize;
    };
    bool isRunning(const uint8_t nStation);
    bool isRunning()
    {
      return m_nSize > 0;
    };
    void dump();

  protected:
    // Helper functions

    // Implementation
    CListItem m_arrayStarts[MAX_STARTS];
    uint8_t m_nSize;
};

#endif






