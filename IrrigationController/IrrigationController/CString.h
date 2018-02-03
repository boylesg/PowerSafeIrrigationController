#ifndef __CSTRING_H__
#define __CSTRING_H__

#include <IPAddress.h>




class CBuffBase
{
  public:
    char *m_strBuffer;
    CBuffBase()
    {
      m_strBuffer = NULL;
    };
    virtual void empty() = 0;
    virtual uint16_t capacity() = 0;
    inline uint16_t length()
    {
      return strlen(m_strBuffer);
    }
    char& operator[](const uint16_t nI)
    {
      return m_strBuffer[nI];
    };
};

template<uint16_t nBuffSize>
class CBuff: public CBuffBase
{
	public:
		char m_strArray[nBuffSize + 1];
		CBuff(): CBuffBase()
		{
			memset(m_strArray, 0, capacity());
			m_strBuffer = m_strArray;
		};
		inline virtual void empty()
		{
			memset(m_strArray, 0, sizeof(m_strArray));
		};
    inline virtual uint16_t capacity()
    {
      return sizeof m_strArray;
    };
    operator char*()
    {
      return m_strBuffer;
    };
    operator const char*()
    {
      return m_strBuffer;
    };
};




class CString
{
  protected:

    //****************************************************************
    //* Data
    //****************************************************************
    CBuffBase *m_pBuff;
    static char *m_strStartPtr, *m_strEndPtr;

  
    //****************************************************************
    //* Assorted helper functions
    //****************************************************************
	  void convertHex2Bin(char *strHex, const uint8_t nCapacity);
	  void copy(const char *strStart, const char* strEnd, char* strDest, const uint16_t nSize);
	  void concat(const char *strStart, const char* strEnd, char* strDest, const uint16_t nSize);

  public:

    //****************************************************************
    //* Construction and destuction
    //****************************************************************
    // So you can declare a global CString variable, but make sure you call init(...) before using it.
	  CString();
  
    inline CString(CBuffBase &buff, const bool bClear = false)
    {
      init(buff, bClear);
    };
    void init(CBuffBase &buff, const bool bClear = false);

    CString(CString& strOther);
    ~CString()
    {
    }
  
    //****************************************************************
    //* Miscellaneous
    //****************************************************************

    // Returns the length of the current string, not counting the 0 terminator
    inline const uint16_t length() 
    { 
		  return strlen(m_pBuff->m_strBuffer);
    }

    // Returns the capacity of the buffer
    inline const uint16_t capacity() 
    { 
		  return m_pBuff->capacity(); 
    }

    inline const uint16_t maxLength() 
    { 
      return m_pBuff->capacity() - 1; 
    }

    // Returns the capacity of the buffer
    inline const bool full() 
    { 
      return strlen(m_pBuff->m_strBuffer) == (m_pBuff->capacity() - 1); 
    }

    const char* substring(uint16_t nStartPos, uint16_t nEndPos = -1);

    //****************************************************************
    //* Other operator functions
    //****************************************************************
    inline const char* c_str() 
    { 
      return m_pBuff->m_strBuffer; 
    }
  
    inline operator const char*() 
    { 
      return m_pBuff->m_strBuffer; 
    }
  
    inline operator char*() 
    { 
      return m_pBuff->m_strBuffer; 
    }
  
    inline char& operator [](const int nI)
    {
      return m_pBuff->m_strBuffer[nI];
    }
  
    // Empty the string.
    void empty()
    {
      memset(m_pBuff->m_strBuffer, 0, m_pBuff->capacity());
    }
  
    // Is it an empty string ""
    inline bool isEmpty()
    {
      return m_pBuff->length() == 0;
    }
   
    //****************************************************************
    //* Concatenation operator functions
    //****************************************************************
    void operator +=(CString& str);    
    void operator +=(const char* str);    
    void operator +=(const char cCh);
#ifndef __SAM3X8E__
    void operator +=(const __FlashStringHelper* str);
#endif
    
    //****************************************************************
    //* Assignment operator functions
    //****************************************************************
    CString& operator =(const char cCh);
    CString& operator =(const char* str);
#ifndef __SAM3X8E__
    CString& operator =(const __FlashStringHelper* str);
#endif
    CString& operator =(CString& str);
  
  
    //****************************************************************
    //* Comparaison operator functions
    //****************************************************************
    bool operator ==(const char *str);
#ifndef __SAM3X8E__
    bool operator ==(const __FlashStringHelper *str);
#endif
    bool operator >=(const char *str);
#ifndef __SAM3X8E__
    bool operator >=(const __FlashStringHelper *str);
    bool operator >(const __FlashStringHelper *str);
    bool operator <=(const __FlashStringHelper *str);
    bool operator <(const __FlashStringHelper *str);
    bool operator !=(const __FlashStringHelper *str);
#endif
    bool operator >(const char *str);
    bool operator <=(const char *str);
    bool operator <(const char *str);
    bool operator !=(const char *str);

    //****************************************************************
    //* Search functions
    //****************************************************************
    inline bool find(const char cCh, const int16_t nPos = 0)
    {
      return indexOf(cCh, nPos) >= 0;
    };
    bool find(const char* str, const int16_t nPos = 0)
    {
      return indexOf(str, nPos) >= 0;
    };
    int16_t findOneOf(const char* strCharList, const uint16_t nPos = 0);
    int16_t reverseFindOneOf(const char* strCharList, const uint16_t nPos = 0);
#ifndef __SAM3X8E__
    int16_t reverseFindOneOf(const __FlashStringHelper* strCharList, const uint16_t nPos = 0);
    int16_t findOneOf(const __FlashStringHelper* strCharList, const uint16_t nPos = 0);
    inline bool find(const __FlashStringHelper* str, const int16_t nPos = 0)
    {
      return indexOf(str, nPos) >= 0;
    };
#endif
    int16_t indexOf(const char cCh, const int16_t nPos = 0);
    int16_t indexOf(const char* str, const int16_t nPos = 0);
#ifndef __SAM3X8E__
    int16_t indexOf(const __FlashStringHelper* str, const int16_t nPos = 0);
#endif

    inline bool reverseFind(const char cCh, const int16_t nPos = -1)
    {
      return reverseIndexOf(cCh, nPos) >= 0;
    };
    inline bool reverseFind(const char* str, const int16_t nPos = -1)
    {
      return reverseIndexOf(str, nPos) >= 0;
    };
#ifndef __SAM3X8E__
    inline bool reverseFind(const __FlashStringHelper* str, const int16_t nPos = -1)
    {
      return reverseIndexOf(str, nPos) >= 0;
    };
#endif

    int16_t reverseIndexOf(const char cCh, const int16_t nPos = -1);
    int16_t reverseIndexOf(const char* str, const int16_t nPos = -1);
#ifndef __SAM3X8E__
    int16_t reverseIndexOf(const __FlashStringHelper* str, const int16_t nPos = -1);
#endif

	  //****************************************************************
    //* Formatting and conversion functions
    //****************************************************************
    void format(const char *str, ...);
    #ifndef __SAM3X8E__
      void format(const __FlashStringHelper *str, ...);
    #endif
  
    const char* fromIPAddr(IPAddress ip);
    const char* fromBool(const bool bVal);
    const char* fromUint(const uint32_t nNum, const uint8_t nBase = 10);
    const char* fromInt(const int32_t nNum, const uint8_t nBase = 10);
    const char* fromReal(const double dNum, const uint8_t nDecimalPlaces);
  	uint32_t toUint(const uint8_t nBase = 10);
  	int32_t toInt(const uint8_t nBase = 10);
  	double toReal(const uint8_t nBase);

	//****************************************************************
	//* Modification functions
	//****************************************************************
    void encodeBase64();
    void decodeBase64();
    void insert(const char cCh, const uint16_t nBeforePos);
    void insert(const char* str, const uint16_t nBeforePos);
#ifndef __SAM3X8E__
    void insert(const __FlashStringHelper* str, const uint16_t nBeforePos);
#endif
    void replace(const char cFind, const char cReplace, const uint16_t nStartFrom = 0);
    void replace(const char* strFind, const char* strReplace, const uint16_t nStartFrom = 0);
#ifndef __SAM3X8E__
    void replace(const char* strFind, const __FlashStringHelper* strReplace, const uint16_t nStartFrom = 0);
    void replace(const __FlashStringHelper* strFind, const char* strReplace, const uint16_t nStartFrom = 0);
    void replace(const __FlashStringHelper* strFind, const __FlashStringHelper* strReplace, const uint16_t nStartFrom = 0);
#endif
    void removeCh(const char cFind);
    inline void remove(const uint16_t nI)
    {
      remove(nI, 1);
    };
    void remove(const uint16_t nPos, uint16_t nCount);
    void remove(const char* strFind);
#ifndef __SAM3X8E__
    void remove(const __FlashStringHelper* strFind);
#endif
    void toLower();
    void toUpper();
    void trim();
    void trimLeft();
    void trimRight();
    void padLeft(const char cCh, const uint8_t nLength);
    void padRight(const char cCh, const uint8_t nLength);
};

#endif




