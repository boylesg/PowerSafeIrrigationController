#include <Arduino.h>
#include "Common.h"
#include "Debug.h"




#define DEBUG 1
#define serialHC05 Serial2




CDebug::CDebug(Stream *pSerial)
{
  m_pSerial = pSerial;
}

CDebug::~CDebug()
{
}

void CDebug::init()
{
}

#ifndef __SAM3X8E__
  void CDebug::dump(const __FlashStringHelper* strVarName, const uint32_t nVal, const bool bNewLine)
  {  
    #ifdef DEBUG
      m_pSerial->print(strVarName);
      m_pSerial->print(F(" = "));
      m_pSerial->print(nVal);
      if (bNewLine)
        m_pSerial->println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const int32_t nVal, const bool bNewLine)
  {
    #ifdef DEBUG
      m_pSerial->print(strVarName);
      m_pSerial->print(F(" = "));
      m_pSerial->print(nVal);
      if (bNewLine)
        m_pSerial->println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
   #endif
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const uint16_t nVal, const bool bNewLine)
  {  
    #ifdef DEBUG
      m_pSerial->print(strVarName);
      m_pSerial->print(F(" = "));
      m_pSerial->print(nVal);
      if (bNewLine)
        m_pSerial->println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
   #endif
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const int16_t nVal, const bool bNewLine)
  {
    #ifdef DEBUG
      m_pSerial->print(strVarName);
      m_pSerial->print(F(" = "));
      m_pSerial->print(nVal);
      if (bNewLine)
        m_pSerial->println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const uint8_t nVal, const bool bNewLine)
  {  
    #ifdef DEBUG
      m_pSerial->print(strVarName);
      m_pSerial->print(F(" = "));
      m_pSerial->print(nVal);
      if (bNewLine)
        m_pSerial->println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const int8_t nVal, const bool bNewLine)
  {
    #ifdef DEBUG
      m_pSerial->print(strVarName);
      m_pSerial->print(F(" = "));
      m_pSerial->print(nVal);
      if (bNewLine)
        m_pSerial->println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(nVal);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }

#endif

void CDebug::dump(const char* strVarName, const uint32_t nVal, const bool bNewLine)
{  
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const int32_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const uint16_t nVal, const bool bNewLine)
{  
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const int16_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const uint8_t nVal, const bool bNewLine)
{  
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const int8_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const __FlashStringHelper* strVarName, const float fVal, const bool bNewLine)
{
  dump(strVarName, (double)fVal, bNewLine);
}

void CDebug::dump(const __FlashStringHelper* strVarName, const double dVal, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(dVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(dVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const char* str, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(str);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(str);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const char* strVarName, const __FlashStringHelper* str, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(str);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(str);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const __FlashStringHelper* strVarName, const char* str, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(str);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(str);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

#ifndef __SAM3X8E__
  void CDebug::dump(const __FlashStringHelper* strVarName, const __FlashStringHelper* str, const bool bNewLine)
  {
    #ifdef DEBUG
      m_pSerial->print(strVarName);
      m_pSerial->print(F(" = "));
      m_pSerial->print(str);
      if (bNewLine)
        m_pSerial->println();
    #elif defined DIAGNOSTIC
      serialHC05.print(strVarName);
      serialHC05.print(F(" = "));
      serialHC05.print(str);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }
#endif

void CDebug::dump(const __FlashStringHelper* strVarName, const char cChar, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(cChar);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(cChar);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::dump(const __FlashStringHelper* strVarName, IPAddress IPAddr, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(IPAddr);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(strVarName);
    serialHC05.print(F(" = "));
    serialHC05.print(IPAddr);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const uint16_t nVal, const bool bNewLine)
{  
  #ifdef DEBUG
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const int16_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const float fVal, const bool bNewLine)
{
  log((double)fVal, bNewLine);
}

void CDebug::log(const double dVal, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(dVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(dVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const char* str, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(str);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(str);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

#ifndef __SAM3X8E__
  void CDebug::log(const __FlashStringHelper* str, const bool bNewLine)
  {
    #ifdef DEBUG
      m_pSerial->print(str);
      if (bNewLine)
        m_pSerial->println();
    #elif defined DIAGNOSTIC
      serialHC05.print(str);
      if (bNewLine)
        serialHC05.print("\r\n");
    #endif
  }
#endif

void CDebug::log(const char cChar, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(cChar);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(cChar);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const uint8_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const int8_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const uint32_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(const int32_t nVal, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(nVal);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

void CDebug::log(IPAddress IPAddr, const bool bNewLine)
{
  #ifdef DEBUG
    m_pSerial->print(IPAddr);
    if (bNewLine)
      m_pSerial->println();
  #elif defined DIAGNOSTIC
    serialHC05.print(IPAddr);
    if (bNewLine)
      serialHC05.print("\r\n");
  #endif
}

#ifdef __SAM3X8E__
  void CDebug::logRuntimeError(const char* strFileName, const int16_t nLineNum)
  {
    #ifdef DEBUG
      m_pSerial->print(F("RUNTIME ERROR: "));
      m_pSerial->print(strFileName);
      m_pSerial->print(F(" line number "));
      m_pSerial->println(nLineNum);  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("RUNTIME ERROR: "));
      serialHC05.print(strFileName);
      serialHC05.print(F(" line number "));
      serialHC05.print(nLineNum);
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileFoundError(const char* strFileName)
  {
    #ifdef DEBUG
      m_pSerial->print(F("File '"));
      m_pSerial->print(strFileName);
      m_pSerial->println(F("' was not found!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' was not found!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileFoundError(const __FlashStringHelper* strFileName)
  {
    #ifdef DEBUG
      m_pSerial->print(F("File '"));
      m_pSerial->print(strFileName);
      m_pSerial->println(F("' was not found!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' was not found!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileOpenError(const char* strFileName)
  {
    #ifdef DEBUG
      m_pSerial->print(F("File '"));
      m_pSerial->print(strFileName);
      m_pSerial->println(F("' could not be opened!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' could not be opened!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileOpenError(const __FlashStringHelper* strFileName)
  {
    #ifdef DEBUG
      m_pSerial->print(F("File '"));
      m_pSerial->print(strFileName);
      m_pSerial->println(F("' could not be opened!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' could not be opened!"));  
      serialHC05.print("\r\n");
    #endif
  }
#else
  void CDebug::logRuntimeError(const __FlashStringHelper* strFileName, const int16_t nLineNum)
  {
    #ifdef DEBUG
      m_pSerial->print(F("RUNTIME ERROR: "));
      m_pSerial->print(strFileName);
      m_pSerial->print(F(" line number "));
      m_pSerial->println(nLineNum);  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("RUNTIME ERROR: "));
      serialHC05.print(strFileName);
      serialHC05.print(F(" line number "));
      serialHC05.print(nLineNum);
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileFoundError(const char* strFileName)
  {
    #ifdef DEBUG
      m_pSerial->print(F("File '"));
      m_pSerial->print(strFileName);
      m_pSerial->println(F("' was not found!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' was not found!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileFoundError(const __FlashStringHelper* strFileName)
  {
    #ifdef DEBUG
      m_pSerial->print(F("File '"));
      m_pSerial->print(strFileName);
      m_pSerial->println(F("' was not found!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' was not found!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileOpenError(const char* strFileName)
  {
    #ifdef DEBUG
      m_pSerial->print(F("File '"));
      m_pSerial->print(strFileName);
      m_pSerial->println(F("' could not be opened!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' could not be opened!"));  
      serialHC05.print("\r\n");
    #endif
  }
  void CDebug::logFileOpenError(const __FlashStringHelper* strFileName)
  {
    #ifdef DEBUG
      m_pSerial->print(F("File '"));
      m_pSerial->print(strFileName);
      m_pSerial->println(F("' could not be opened!"));  
    #elif defined DIAGNOSTIC
      serialHC05.print(F("File '"));
      serialHC05.print(strFileName);
      serialHC05.print(F("' could not be opened!"));  
      serialHC05.print("\r\n");
    #endif
  }
#endif

CDebug debug;


