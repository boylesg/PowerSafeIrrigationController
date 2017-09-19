#ifndef __DEBUG_H
#define __DEBUG_H

#include <assert.h>
#include <IPAddress.h>




#define STRINGIFY(VAR) #VAR




class CDebug
{
  public:

    // Construction, destruction and initialisation
    CDebug(Stream *pSerial = &Serial);
    ~CDebug();
    void init();
    void setSerial(Stream *pSerial)
    {
      m_pSerial = pSerial;
    };

    // Interface
#ifndef __SAM3X8E__
    void dump(const __FlashStringHelper* strVarName, const uint32_t nVal, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, const int32_t nVal, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, const uint16_t nVal, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, const int16_t nVal, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, const uint8_t nVal, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, const int8_t nVal, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, const float fVal, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, const double dVal, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, const char* str, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, const __FlashStringHelper* str, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, const char cChar, const bool bNewLine = true);
    void dump(const __FlashStringHelper* strVarName, IPAddress IPAddr, const bool bNewLine = true);
#endif
    void dump(const char* strVarName, const uint32_t nVal, const bool bNewLine = true);
    void dump(const char* strVarName, const int32_t nVal, const bool bNewLine = true);
    void dump(const char* strVarName, const uint16_t nVal, const bool bNewLine = true);
    void dump(const char* strVarName, const int16_t nVal, const bool bNewLine = true);
    void dump(const char* strVarName, const uint8_t nVal, const bool bNewLine = true);
    void dump(const char* strVarName, const int8_t nVal, const bool bNewLine = true);
    void dump(const char* strVarName, const float fVal, const bool bNewLine = true);
    void dump(const char* strVarName, const double dVal, const bool bNewLine = true);
    void dump(const char* strVarName, const char* str, const bool bNewLine = true);
    void dump(const char* strVarName, const __FlashStringHelper* str, const bool bNewLine = true);
    void dump(const char* strVarName, const char cChar, const bool bNewLine = true);
    void dump(const char* strVarName, IPAddress IPAddr, const bool bNewLine = true);

    void log(const uint8_t nVal, const bool bNewLine = true);
    void log(const int8_t nVal, const bool bNewLine = true);
    void log(const uint32_t nVal, const bool bNewLine = true);
    void log(const int32_t nVal, const bool bNewLine = true);
    void log(const uint16_t nVal, const bool bNewLine = true);
    void log(const int16_t nVal, const bool bNewLine = true);
    void log(const float fVal, const bool bNewLine = true);
    void log(const double dVal, const bool bNewLine = true);
    void log(const char* str, const bool bNewLine = true);
#ifndef __SAM3X8E__
    void log(const __FlashStringHelper* str, const bool bNewLine = true);
#endif
    void log(const char cChar, const bool bNewLine = true);
    void log(IPAddress IPAddr, const bool bNewLine = true);

#ifdef __SAM3X8E__
    void logRuntimeError(const char* strFileName, const int16_t nLineNum);
    
    void logFileFoundError(const char* strFileName);
    void logFileFoundError(__FlashStringHelper* strFileName);
    void logFileOpenError(const char* strFileName);
    void logFileOpenError(__FlashStringHelper* strFileName);
#else
    void logRuntimeError(const __FlashStringHelper* strFileName, const int16_t nLineNum);
    
    void logFileFoundError(const char* strFileName);
    void logFileFoundError(const __FlashStringHelper* strFileName);
    void logFileOpenError(const char* strFileName);
    void logFileOpenError(const __FlashStringHelper* strFileName);
#endif

  protected:
    Stream *m_pSerial;
    
};

extern CDebug debug;

#endif


