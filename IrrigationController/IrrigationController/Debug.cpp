#include <Arduino.h>
#include "Common.h"
#include "Debug.h"
#include "Constants.h"





CDebug::CDebug()
{
  m_pSerial = NULL;
}

CDebug::CDebug(Stream *pSerial)
{
  m_pSerial = pSerial;
}

CDebug::~CDebug()
{
}

bool CDebug::readLogFile(CTextFile*& pLogFile)
{
  bool bResult = false;

  pLogFile = NULL;
  logEventOpen('=', F("Opening log file for reading"));
   
  if (m_fileLog)
  { 
    m_fileLog.close();
    CBuff<16> buffFilename;
    CString strFilename(buffFilename);
    strFilename = fstr_TXT_LOG_FILE;
 
    if (m_fileLog.open(strFilename, READ))
    {
      pLogFile = &m_fileLog;
      logEventClose('=', F("successful"));
      bResult = true;
    }
    else
      logEventClose('=', F("failed (can't open)"));
  }
  else
    logEventClose('=', F("failed (can't close)"));
    
  return bResult;
}

bool CDebug::doneLogFile()
{
  bool bResult = false;
    
  if (m_fileLog)
  {
    m_fileLog.close();
    CBuff<16> buffFilename;
    CString strFilename(buffFilename);
    strFilename = fstr_TXT_LOG_FILE;
    
    logEventOpen('=', F("Re-opening log file for writing"));
    if (m_fileLog.open(strFilename, APPEND))
    {
      logEventClose('=', F("successful"));
      bResult = true;
    }
    else
      logEventClose('=', F("failed"));
  }
  return bResult;
}

bool CDebug::startLogFile()
{
  bool bResult = false;
  CBuff<16> buffFilename;
  CString strFilename(buffFilename);
  strFilename = fstr_TXT_LOG_FILE;

  logEventOpen('=', F("Opening log file for writing"));
  if (m_fileLog.open(strFilename, REWRITE))
  {
    logEventClose('=', F("successful"));
    bResult = true;
  }
  else
    logEventClose('=', F("failed"));

  return bResult;
}

void CDebug::setSerial(Stream *pSerial)
{
  m_pSerial = pSerial;
}

#ifndef __SAM3X8E__
  void CDebug::dump(const __FlashStringHelper* strVarName, const uint32_t nVal, const bool bNewLine)
  {  
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const int32_t nVal, const bool bNewLine)
  {
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const uint16_t nVal, const bool bNewLine)
  {  
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const int16_t nVal, const bool bNewLine)
  {
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const uint8_t nVal, const bool bNewLine)
  {  
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, const int8_t nVal, const bool bNewLine)
  {
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(nVal);
    if (bNewLine)
      m_pSerial->println();
  }

#endif

void CDebug::dump(const char* strVarName, const uint32_t nVal, const bool bNewLine)
{  
  if (m_fileLog)
  {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(strVarName);
  m_pSerial->print(F(" = "));
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::dump(const char* strVarName, const int32_t nVal, const bool bNewLine)
{
  if (m_fileLog)
  {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(strVarName);
  m_pSerial->print(F(" = "));
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::dump(const char* strVarName, const uint16_t nVal, const bool bNewLine)
{  
    if (m_fileLog)
  {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(strVarName);
  m_pSerial->print(F(" = "));
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::dump(const char* strVarName, const int16_t nVal, const bool bNewLine)
{
  if (m_fileLog)
  {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(strVarName);
  m_pSerial->print(F(" = "));
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::dump(const char* strVarName, const uint8_t nVal, const bool bNewLine)
{  
  if (m_fileLog)
  {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(strVarName);
  m_pSerial->print(F(" = "));
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::dump(const char* strVarName, const int8_t nVal, const bool bNewLine)
{
  if (m_fileLog)
  {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(nVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(strVarName);
  m_pSerial->print(F(" = "));
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

#ifndef __SAM3X8E__
  void CDebug::dump(const __FlashStringHelper* strVarName, const float fVal, const bool bNewLine)
  {
    dump(strVarName, (double)fVal, bNewLine);
  }

  void CDebug::dump(const __FlashStringHelper* strVarName, const double dVal, const bool bNewLine)
  {
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(dVal);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(dVal);
    if (bNewLine)
      m_pSerial->println();
  }
#endif

void CDebug::dump(const char* strVarName, const char* str, const bool bNewLine)
{
  if (m_fileLog)
  {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(str);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(strVarName);
  m_pSerial->print(F(" = "));
  m_pSerial->print(str);
  if (bNewLine)
    m_pSerial->println();
}

#ifndef __SAM3X8E__
  void CDebug::dump(const char* strVarName, const __FlashStringHelper* str, const bool bNewLine)
  {
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(str);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(str);
    if (bNewLine)
      m_pSerial->println();
  }

  void CDebug::dump(const __FlashStringHelper* strVarName, const char* str, const bool bNewLine)
  {
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(str);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(str);
    if (bNewLine)
      m_pSerial->println();
  }

  void CDebug::dump(const __FlashStringHelper* strVarName, const __FlashStringHelper* str, const bool bNewLine)
  {
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(str);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(str);
    if (bNewLine)
      m_pSerial->println();
  }

  void CDebug::dump(const __FlashStringHelper* strVarName, const char cChar, const bool bNewLine)
  {
  if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(cChar);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(cChar);
    if (bNewLine)
      m_pSerial->println();
  }
  
  void CDebug::dump(const __FlashStringHelper* strVarName, IPAddress IPAddr, const bool bNewLine)
  {
    if (m_fileLog)
    {
      m_fileLog.write(strVarName);
      m_fileLog.write(F(" = "));
      m_fileLog.write(IPAddr);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(strVarName);
    m_pSerial->print(F(" = "));
    m_pSerial->print(IPAddr);
    if (bNewLine)
      m_pSerial->println();
  }
#endif

void CDebug::logEventClose(const char cCh, const char* strEventResult)
{
  if (strlen(strEventResult) > 0)
  {
    if (m_fileLog)
    {
      m_fileLog.write(strEventResult);
      m_fileLog.writeLine(F("!"));
    }
    m_pSerial->print(strEventResult);
    m_pSerial->println(F("!"));
  }
  if (cCh > 0)
  {
    if (m_fileLog)
    {
      for (uint8_t nI = 0; nI < 60; nI++)
        m_fileLog.write(cCh);
      m_fileLog.writeLine(F(""));
    }
    for (uint8_t nI = 0; nI < 60; nI++)
      m_pSerial->print(cCh);
    m_pSerial->println(F(""));
  }
}

void CDebug::logEventOpen(const char cCh, const char* strEventDesc, const bool bNewLine)
{
  if (cCh > 0)
  {
    if (m_fileLog)
    {
      for (uint8_t nI = 0; nI < 60; nI++)
        m_fileLog.write(cCh);
      m_fileLog.writeLine(F(""));
    }
    for (uint8_t nI = 0; nI < 60; nI++)
      m_pSerial->print(cCh);
    m_pSerial->println(F(""));
  }
  if (strlen(strEventDesc) > 0)
  {
    if (!bNewLine)
    {
      if (m_fileLog)
      {
        m_fileLog.write(strEventDesc);
        m_fileLog.write(F("..."));
      }
      m_pSerial->print(strEventDesc);
      m_pSerial->print(F("..."));
    }
    else
    {
      if (m_fileLog)
      {
        m_fileLog.writeLine(strEventDesc);
        for (uint8_t nI = 0; nI < strlen(strEventDesc) + 1; nI++)
          m_fileLog.write('-');
        m_fileLog.writeLine(F("\n"));
      }
      m_pSerial->println(strEventDesc);
      for (uint8_t nI = 0; nI < strlen(strEventDesc) + 1; nI++)
        m_pSerial->print('-');
      m_pSerial->println(F("\n"));
    }
  }
}

#ifndef __SAM3X8E__
  void CDebug::logEventClose(const char cCh, const __FlashStringHelper* strEventResult)
  {
    const uint16_t nSize = strlen_P((PGM_P)strEventResult) + 1;
    char *strTemp = new char[nSize];
    memset(strTemp, 0, nSize);
    strcpy_P(strTemp, (PGM_P)strEventResult);
    logEventClose(cCh, strTemp);
    delete strTemp;
  }
  
  void CDebug::logEventOpen(const char cCh, const __FlashStringHelper* strEventDesc, const bool bNewLine)
  {
    const uint16_t nSize = strlen_P((PGM_P)strEventDesc) + 1;
    char *strTemp = new char[nSize];
    memset(strTemp, 0, nSize);
    strcpy_P(strTemp, (PGM_P)strEventDesc);
    logEventOpen(cCh, strTemp, bNewLine);
    delete strTemp;
  }
  
#endif

void CDebug::log(const uint16_t nVal, const bool bNewLine)
{  
  if (m_fileLog)
  {
    m_fileLog.write(nVal);
    if (bNewLine)
      m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::log(const int16_t nVal, const bool bNewLine)
{
  if (m_fileLog)
  {
    m_fileLog.write(nVal);
    if (bNewLine)
      m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::log(const float fVal, const bool bNewLine)
{
  log((double)fVal, bNewLine);
}

void CDebug::log(const double dVal, const bool bNewLine)
{
  if (m_fileLog)
  {
    m_fileLog.write(dVal);
    if (bNewLine)
      m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(dVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::log(const char* str, const bool bNewLine)
{
  if (m_fileLog)
  {
    m_fileLog.write(str);
    if (bNewLine)
      m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(str);
  if (bNewLine)
    m_pSerial->println();
}

#ifndef __SAM3X8E__
  void CDebug::log(const __FlashStringHelper* str, const bool bNewLine)
  {
    if (m_fileLog)
    {
      m_fileLog.write(str);
      if (bNewLine)
        m_fileLog.writeLine(F(""));
    }
    m_pSerial->print(str);
    if (bNewLine)
      m_pSerial->println();
  }
#endif

void CDebug::log(const char cChar, const bool bNewLine)
{
  if (m_fileLog)
  {
    m_fileLog.write(cChar);
    if (bNewLine)
      m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(cChar);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::log(const uint8_t nVal, const bool bNewLine)
{
  if (m_fileLog)
  {
    m_fileLog.write(nVal);
    if (bNewLine)
      m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::log(const int8_t nVal, const bool bNewLine)
{
  if (m_fileLog)
  {
    m_fileLog.write(nVal);
    if (bNewLine)
      m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::log(const uint32_t nVal, const bool bNewLine)
{
  if (m_fileLog)
  {
    m_fileLog.write(nVal);
    if (bNewLine)
      m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::log(const int32_t nVal, const bool bNewLine)
{
  if (m_fileLog)
  {
    m_fileLog.write(nVal);
    if (bNewLine)
      m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(nVal);
  if (bNewLine)
    m_pSerial->println();
}

void CDebug::log(IPAddress IPAddr, const bool bNewLine)
{
  if (m_fileLog)
  {
    m_fileLog.write(IPAddr);
    if (bNewLine)
      m_fileLog.writeLine(F(""));
  }
  m_pSerial->print(IPAddr);
  if (bNewLine)
    m_pSerial->println();
}

#ifdef __SAM3X8E__
  void CDebug::logRuntimeError(const char* strFileName, const int16_t nLineNum)
  {
    if (m_fileLog)
    {
      m_fileLog.write(F("RUNTIME ERROR: "));
      m_fileLog.write(strFileName);
      m_fileLog.write(F(" line number "));
      m_fileLog.writeLine(nLineNum);  
    }
    m_pSerial->print(F("RUNTIME ERROR: "));
    m_pSerial->print(strFileName);
    m_pSerial->print(F(" line number "));
    m_pSerial->println(nLineNum);  
  }
  void CDebug::logFileFoundError(const char* strFileName)
  {
    if (m_fileLog)
    {
      m_fileLog.write(F("File '"));
      m_fileLog.write(strFileName);
      m_fileLog.writeLine(F("' was not found!"));  
    }
    m_pSerial->print(F("File '"));
    m_pSerial->print(strFileName);
    m_pSerial->println(F("' was not found!"));  
  }
  void CDebug::logFileFoundError(const __FlashStringHelper* strFileName)
  {
    if (m_fileLog)
    {
      m_fileLog.write(F("File '"));
      m_fileLog.write(strFileName);
      m_fileLog.writeLine(F("' was not found!"));  
    }
    m_pSerial->print(F("File '"));
    m_pSerial->print(strFileName);
    m_pSerial->println(F("' was not found!"));  
  }
  void CDebug::logFileOpenError(const char* strFileName)
  {
    if (m_fileLog)
    {
      m_fileLog.print(F("File '"));
      m_fileLog.print(strFileName);
      m_fileLog.println(F("' could not be opened!"));  
    }
    m_pSerial->print(F("File '"));
    m_pSerial->print(strFileName);
    m_pSerial->println(F("' could not be opened!"));  
  }
  void CDebug::logFileOpenError(const __FlashStringHelper* strFileName)
  {
    if (m_fileLog)
    {
      m_fileLog.write(F("File '"));
      m_fileLog.write(strFileName);
      m_fileLog.writeLine(F("' could not be opened!"));  
    }
    m_pSerial->print(F("File '"));
    m_pSerial->print(strFileName);
    m_pSerial->println(F("' could not be opened!"));  
  }
#else
  void CDebug::logRuntimeError(const __FlashStringHelper* strFileName, const int16_t nLineNum)
  {
    if (m_fileLog)
    {
      m_fileLog.write(F("RUNTIME ERROR: "));
      m_fileLog.write(strFileName);
      m_fileLog.write(F(" line number "));
      m_fileLog.writeLine(nLineNum);  
    }
    m_pSerial->print(F("RUNTIME ERROR: "));
    m_pSerial->print(strFileName);
    m_pSerial->print(F(" line number "));
    m_pSerial->println(nLineNum);  
  }
  void CDebug::logFileFoundError(const char* strFileName)
  {
    if (m_fileLog)
    {
      m_fileLog.write(F("File '"));
      m_fileLog.write(strFileName);
      m_fileLog.writeLine(F("' was not found!"));  
    }
    m_pSerial->print(F("File '"));
    m_pSerial->print(strFileName);
    m_pSerial->println(F("' was not found!"));  
  }
  void CDebug::logFileFoundError(const __FlashStringHelper* strFileName)
  {
    if (m_fileLog)
    {
    }
    m_pSerial->print(F("File '"));
    m_pSerial->print(strFileName);
    m_pSerial->println(F("' was not found!"));  
  }
  void CDebug::logFileOpenError(const char* strFileName)
  {
    if (m_fileLog)
    {
      m_fileLog.write(F("File '"));
      m_fileLog.write(strFileName);
      m_fileLog.writeLine(F("' could not be opened!"));  
    }
    m_pSerial->print(F("File '"));
    m_pSerial->print(strFileName);
    m_pSerial->println(F("' could not be opened!"));  
  }
  void CDebug::logFileOpenError(const __FlashStringHelper* strFileName)
  {
    if (m_fileLog)
    {
      m_fileLog.write(F("File '"));
      m_fileLog.write(strFileName);
      m_fileLog.writeLine(F("' could not be opened!"));  
    }
    m_pSerial->print(F("File '"));
    m_pSerial->print(strFileName);
    m_pSerial->println(F("' could not be opened!"));  
  }
#endif

CDebug debug(&Serial);




