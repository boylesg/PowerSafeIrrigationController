#include <Arduino.h>
#include "Debug.h"
#include "TextFile.h"


const char* CTextFile::m_strEOL = "\r\n";

CTextFile::CTextFile(): m_strFilename(m_strBuff, m_nBuffSize)
{
}

CTextFile::CTextFile(const char* strFilename, const uint8_t nMode): m_strFilename(m_strBuff, m_nBuffSize)
{
  open(strFilename, nMode);
}

CTextFile::~CTextFile()
{
  m_file.close();
}

bool CTextFile::eof()
{
  return (m_file.available() == 0);
}

#ifndef __SAM3X8E__
  bool CTextFile::open(const __FlashStringHelper* strFilename, const uint8_t nMode)
  {
    m_strFilename = strFilename;
    m_file = SD.open(m_strFilename.c_str(), nMode);
    return m_file;
  }
#endif

bool CTextFile::open(const char* strFilename, const uint8_t nMode)
{
  m_strFilename = strFilename;
  m_file = SD.open(m_strFilename.c_str(), nMode);
  return m_file;
}

bool CTextFile::read(CString &strData)
{
  char cChar = 0;

  strData.empty();
  if (m_file)
  {
    while (m_file.available() && (strData.length() < strData.maxLength()))
    {
      cChar = m_file.read();
      strData += cChar;
    }
  }
  return strData.length() > 0;
}

bool CTextFile::readLine(CString& strLine, const bool bIncludeEOL, const bool bDebug)
{
  char cChar = 0;
  bool bResult = false;

  strLine.empty();

  if (m_file)
  {
    while (m_file.available())
    {
      cChar = m_file.read();

      if ((cChar == '\n') || (cChar == '\r'))
      {
        if (bIncludeEOL)
          strLine += cChar;
        cChar = m_file.peek();

        if ((cChar == '\n') || (cChar == '\r'))
        {
          if (bIncludeEOL)
            strLine += cChar;
          m_file.read();
        }
        bResult = true;
        break;
      }
      else
      {
        if (bDebug)
        {
          debug.log(F("\""), false);
          debug.log(strLine, false);
          debug.log(F("\" + '"), false);
          debug.log(cChar, false);
          debug.log(F("' = \""), false);
        }
        strLine += cChar;
        if (bDebug)
        {
          debug.log(strLine, false);
          debug.log(F("\""), false);
        }
      }
    }
  }
  return bResult;
}

bool CTextFile::readWord(CString& strWord)
{
  strWord = F("");
  char cChar = 0;
  
  if (m_file)
  {
    while (m_file.available() && ((cChar >= ' ') && (cChar <= '~')))
    {
      cChar = m_file.read();
      strWord += cChar;
    }
  }
  return strWord.length() > 0;
}

bool CTextFile::readWord(CString& strWord, const char cDelim)
{
  strWord = F("");
  char cChar = 0;
  
  if (m_file)
  {
    while (m_file.available() && (cChar != cDelim))
    {
      cChar = m_file.read();
      strWord += cChar;
    }
  }
  return strWord.length() > 0;
}

#ifndef __SAM3X8E__
  void CTextFile::writeLine(const __FlashStringHelper* strText)
  {
    if (m_file)
    {
      char* strTemp = new char[strlen_P((char*)strText) + 1];
      memset(strTemp, 0, strlen_P((char*)strText) + 1);
      strcpy_P(strTemp, (char*)strText);
      m_file.write(strTemp, strlen(strTemp));
      delete strTemp;
      m_file.write(m_strEOL, strlen(m_strEOL));
    }
  }
#endif

void CTextFile::writeLine(const char* strText)
{
  if (m_file)
  {
    m_file.write(strText, strlen(strText));
    m_file.write(m_strEOL, strlen(m_strEOL));
  }
}

void CTextFile::writeln(const char cCh)
{
  if (m_file)
  {
    m_file.write(cCh);
    m_file.write(m_strEOL, strlen(m_strEOL));
  }
}

void CTextFile::write(const char cCh)
{
  if (m_file)
  {
    m_file.write(cCh);
  }
}

#ifndef __SAM3X8E__
  void CTextFile::write(const __FlashStringHelper* strText)
  {
    if (m_file)
    {
      char* strTemp = new char[strlen_P((char*)strText) + 1];
      memset(strTemp, 0, strlen_P((char*)strText) + 1);
      strcpy_P(strTemp, (char*)strText);
      m_file.write(strTemp, strlen(strTemp));
      delete strTemp;
    }
  }
#endif

void CTextFile::write(const char* strText)
{
  if (m_file)
  {
    m_file.write(strText, strlen(strText));
  }
}

bool CTextFile::seekRel(int32_t nPos)
{
  bool bResult = false;
  uint32_t nNewPos = m_file.position() + nPos;

  if ((nNewPos >= 0) && (nNewPos < m_file.size()))
  {
    bResult = true;
    m_file.seek(m_file.position() + nPos);
  }
  return bResult;
}

void CTextFile::dump(const char* strFilename)
{
  if (m_file)
  {
    close();

    open(strFilename, FILE_READ);
    if (m_file)
    {
      char strBuff[129];
      CString str(strBuff, 129);
      
      debug.log(F("=========================================================================")); 
      debug.log(m_strFilename);
      debug.log(F("=========================================================================")); 
      while (!eof())
      {
        readLine(str);
        debug.log(str);
      }
      debug.log(F("=========================================================================")); 
    }
    close();
  }
}

