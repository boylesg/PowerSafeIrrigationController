/*
  CString.cpp - Lightweight printable string class
  Copyright (c) 2009 Mikal Hart.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/




#include <Arduino.h>
#include <MyBase64.h>
#include "CString.h"
#include "Common.h"
#include "debug.h"




//****************************************************************
//* Construction and destuction
//****************************************************************

char* CString::m_strStartPtr = NULL; 
char* CString::m_strEndPtr = NULL;

// So you can declare a global CString variable, but make sure you call init(...) before using it.
CString::CString()
{ 
  m_pBuff = NULL;
  m_strStartPtr = m_strEndPtr = NULL;

}

CString::CString(CString& strOther)
{
  init(*strOther.m_pBuff);
}

void CString::init(CBuffBase &buff, const bool bClear)
{
  if (bClear)
    buff.empty();
  m_pBuff = &buff;
  m_strEndPtr = m_pBuff->m_strBuffer + strlen(m_pBuff->m_strBuffer) - 1;
  m_strStartPtr = m_pBuff->m_strBuffer;
}
//****************************************************************
//* Concatenation operator functions
//****************************************************************
void CString::operator +=(const char cCh)
{
  uint16_t nLen = strlen(m_pBuff->m_strBuffer);
  if ((nLen + 2) <= m_pBuff->capacity())
  {
	  m_pBuff->m_strBuffer[nLen] = cCh;
	  m_pBuff->m_strBuffer[nLen + 1] = 0;
  }
}
  
void CString::operator +=(CString& str)
{
  if ((strlen(m_pBuff->m_strBuffer) + str.length() + 1) <= m_pBuff->capacity())
    strcat(m_pBuff->m_strBuffer, str.c_str());
}
  
void CString::operator +=(const char* str)
{
  if ((strlen(m_pBuff->m_strBuffer) + strlen(str) + 1) <= m_pBuff->capacity())
    strcat(m_pBuff->m_strBuffer, str);
}
  
#ifndef __SAM3X8E__
  void CString::operator +=(const __FlashStringHelper* str)
  {
    if ((strlen(m_pBuff->m_strBuffer) + strlen_P((PGM_P)str) + 1) <= m_pBuff->capacity())
      strcat_P(m_pBuff->m_strBuffer, (PGM_P)str);
  }
#endif
  
//****************************************************************
//* Assignment operator functions
//****************************************************************
CString& CString::operator =(const char cCh)
{
  if (m_pBuff->capacity() >= 2)
  {
    m_pBuff->m_strBuffer[0] = cCh;
    m_pBuff->m_strBuffer[1] = 0;
  }
  return *this;
}
  
CString& CString::operator =(const char* str)
{
  if ((strlen(str) + 1) <= m_pBuff->capacity())
  {
    strncpy(m_pBuff->m_strBuffer, str, m_pBuff->capacity());
    m_pBuff->m_strBuffer[strlen(str)] = 0;
  }
  return *this;
}
  
CString& CString::operator =(CString& str)
{
  *this = str.c_str();
  return *this;
}
  
const char* CString::fromIPAddr(IPAddress ip)
{
  char strTemp[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, strDot[2] = ".";
  memset(strTemp, 0, 16);
  strcat(strTemp, fromUint(ip[0]));
  strcat(strTemp, strDot);
  strcat(strTemp, fromUint(ip[1]));
  strcat(strTemp, strDot);
  strcat(strTemp, fromUint(ip[2]));
  strcat(strTemp, strDot);
  strcat(strTemp, fromUint(ip[3]));

  if (strlen(strTemp) < capacity())
    *this = strTemp;

  return m_pBuff->m_strBuffer;
}

#ifndef __SAM3X8E__
  CString& CString::operator =(const __FlashStringHelper* str)
  {
    if ((strlen_P((PGM_P)str) + 1) <= m_pBuff->capacity())
    {
      strncpy_P(m_pBuff->m_strBuffer, (PGM_P)str, m_pBuff->capacity());
      m_pBuff->m_strBuffer[strlen_P((PGM_P)str)] = 0;
    }
    return *this;
  }
#endif
  
//****************************************************************
//* Comparaison operator functions
//****************************************************************
bool CString::operator ==(const char *str) 
{ 
  return (m_pBuff->capacity() > 0) && (strcmp(m_pBuff->m_strBuffer, str) == 0); 
}

#ifndef __SAM3X8E__
  bool CString::operator ==(const __FlashStringHelper *str) 
  { 
    return (m_pBuff->capacity() > 0) && (strcmp_P(m_pBuff->m_strBuffer, (PGM_P)str) == 0); 
  }
#endif

bool CString::operator >=(const char *str)
{
  return (m_pBuff->capacity() > 0) && (strcmp(m_pBuff->m_strBuffer, str) >= 0);   
}

#ifndef __SAM3X8E__
  bool CString::operator >=(const __FlashStringHelper *str)
  {
    return (m_pBuff->capacity() > 0) && (strcmp_P(m_pBuff->m_strBuffer, (PGM_P)str) >= 0); 
  }
#endif

bool CString::operator >(const char *str)
{
  return (m_pBuff->capacity() > 0) && (strcmp(m_pBuff->m_strBuffer, str) > 0);   
}

#ifndef __SAM3X8E__
  bool CString::operator >(const __FlashStringHelper *str)
  {
    return (m_pBuff->capacity() > 0) && (strcmp_P(m_pBuff->m_strBuffer, (PGM_P)str) > 0); 
  }
#endif

bool CString::operator <=(const char *str)
{
  return (m_pBuff->capacity() > 0) && (strcmp(m_pBuff->m_strBuffer, str) <= 0);   
}

#ifndef __SAM3X8E__
  bool CString::operator <=(const __FlashStringHelper *str)
  {
    return (m_pBuff->capacity() > 0) && (strcmp_P(m_pBuff->m_strBuffer, (PGM_P)str) <= 0); 
  }
#endif

bool CString::operator <(const char *str)
{
  return (m_pBuff->capacity() > 0) && (strcmp(m_pBuff->m_strBuffer, str) < 0);   
}

#ifndef __SAM3X8E__
  bool CString::operator <(const __FlashStringHelper *str)
  {
    return (m_pBuff->capacity() > 0) && (strcmp_P(m_pBuff->m_strBuffer, (PGM_P)str) < 0); 
  }
#endif

bool CString::operator !=(const char *str)
{
  return (m_pBuff->capacity() > 0) && (strcmp(m_pBuff->m_strBuffer, str) != 0);   
}

#ifndef __SAM3X8E__
  bool CString::operator !=(const __FlashStringHelper *str)
  {
    return (m_pBuff->capacity() > 0) && (strcmp_P(m_pBuff->m_strBuffer, (PGM_P)str) != 0); 
  }
#endif

//****************************************************************
//* Miscellaneous functions
//****************************************************************

const char* CString::substring(uint16_t nStartPos, uint16_t nEndPos)
{
  const uint16_t nBuffSize = 512;
  static char str[nBuffSize];
  uint16_t nI = 0, nSize = 0;

  if ((nEndPos >= nStartPos) && (nStartPos >= 0) && (nStartPos < strlen(m_pBuff->m_strBuffer)) && (nEndPos >= 0) && (nEndPos <= strlen(m_pBuff->m_strBuffer)))
  {
    nSize = nEndPos - nStartPos;
    memset(str, 0, nBuffSize);
  
    if (nEndPos == strlen(m_pBuff->m_strBuffer))
      nEndPos = strlen(m_pBuff->m_strBuffer) - 1;
      
    m_strStartPtr = m_pBuff->m_strBuffer + nStartPos;
    m_strEndPtr = m_pBuff->m_strBuffer + nEndPos;

    while (m_strStartPtr <= m_strEndPtr)
    {
      str[nI++] = *m_strStartPtr;
      m_strStartPtr++;
    }
  }
  else
    str[0] = 0;
    
  return str;
}

//****************************************************************
//* Search functions functions
//****************************************************************

int16_t CString::findOneOf(const char* strCharList, const uint16_t nPos)
{
  char *strPos = NULL, strCh[] = {0, 0};
  int16_t nFindPos = -1;
  
  if ((nPos >= 0) && (nPos < length()))
  {
    for (uint16_t nI = 0; nI < strlen(strCharList); nI++)
    {
      strCh[0] = strCharList[nI];
      strPos = strstr(m_pBuff->m_strBuffer + nPos, strCh);
      if (strPos != NULL)
      {
        nFindPos = strPos - m_pBuff->m_strBuffer;
        break;
      }
    }
  }
  return nFindPos;
}

int16_t CString::reverseFindOneOf(const char* strCharList, const uint16_t nPos)
{
  char *strPos = NULL;
  int16_t nFindPos = -1;
  
  if ((nPos >= 0) && (nPos < length()))
  {
    for (uint16_t nI = strlen(strCharList) - 1; nI >= 0 ; nI--)
    {
      strPos = strstr(m_pBuff->m_strBuffer + nPos, strCharList + nI);
      if (strPos != NULL)
      {
        nFindPos = strPos - m_pBuff->m_strBuffer;
        break;
      }
    }
  }
  return nPos;
}

#ifndef __SAM3X8E__
    int16_t CString::reverseFindOneOf(const __FlashStringHelper* strCharList, const uint16_t nPos)
    {
      uint16_t nSize = strlen_P((PGM_P)strCharList) + 1;
      char *strTemp = new char[nSize];
      memset(strTemp, 0, nSize);
      int16_t nFindPos = reverseFindOneOf(strTemp, nPos);
      delete strTemp;
      return nFindPos;
    }

    int16_t CString::findOneOf(const __FlashStringHelper* strCharList, const uint16_t nPos)
    {
      uint16_t nSize = strlen_P((PGM_P)strCharList) + 1;
      char *strTemp = new char[nSize];
      memset(strTemp, 0, nSize);
      strcpy_P(strTemp, (PGM_P)strCharList);
      int16_t nFindPos = findOneOf(strTemp, nPos);
      delete strTemp;
      return nFindPos;
    }
#endif

int16_t CString::indexOf(const char cCh, const int16_t nPos)
{
  int16_t nI = 0;
	  
  m_strStartPtr = m_pBuff->m_strBuffer;
  if (nPos < length())
  {
    m_strStartPtr = m_pBuff->m_strBuffer + nPos;
    nI = nPos;
  }
  while ((m_pBuff->m_strBuffer[nI] != cCh) && (nI < m_pBuff->capacity()) && (nI < strlen(m_pBuff->m_strBuffer)))
	  nI++;
  if ((nI >= strlen(m_pBuff->m_strBuffer)) && (nI < m_pBuff->capacity()))
	  nI = -1;
  
  return nI;
}
 
int16_t CString::indexOf(const char* str, const int16_t nPos)
{
  int16_t nI = -1;
	  
  m_strStartPtr = m_pBuff->m_strBuffer;
  if (nPos < length())
    m_strStartPtr = m_pBuff->m_strBuffer + nPos;

  m_strStartPtr = strstr(m_strStartPtr, str);
  if (m_strStartPtr)
	  nI = m_strStartPtr - m_pBuff->m_strBuffer;
  
  return nI;
}

#ifndef __SAM3X8E__
  int16_t CString::indexOf(const __FlashStringHelper* str, const int16_t nPos)
  {
    int16_t nI = -1;

    m_strStartPtr = m_pBuff->m_strBuffer + nPos;
    m_strStartPtr = strstr_P(m_strStartPtr, (PGM_P)str);
    if (m_strStartPtr)
  	  nI = m_strStartPtr - m_pBuff->m_strBuffer;

    return nI;
  }
#endif

int16_t CString::reverseIndexOf(const char cCh, const int16_t nPos)
{
  int16_t nI = 0;

  if (nPos == -1)
    nI = strlen(m_pBuff->m_strBuffer) - 1;
  else
    nI = nPos;
    
  while ((m_pBuff->m_strBuffer[nI] != cCh) && (nI >= 0))
	  nI--;
  
  return nI;
}

int16_t CString::reverseIndexOf(const char* str, const int16_t nPos)
{
  int16_t nI = -1;
  
  if ((nPos >= 0) && (nPos < length()))
    m_strStartPtr = m_pBuff->m_strBuffer + nPos;
  else
    m_strStartPtr = m_pBuff->m_strBuffer + strlen(m_pBuff->m_strBuffer) - 1;
	  
  while (m_strStartPtr >= m_pBuff->m_strBuffer)
  {
	  if (strstr(m_strStartPtr, str) != NULL)
	  {
		  nI = m_strStartPtr - m_pBuff->m_strBuffer;
		  break;
	  }
	  m_strStartPtr--;
  }
  return nI;
}

#ifndef __SAM3X8E__
  int16_t CString::reverseIndexOf(const __FlashStringHelper* str, const int16_t nPos)
  {
    int16_t nI = -1;
    
    if ((nPos >= 0) && (nPos < length()))
      m_strStartPtr = m_pBuff->m_strBuffer + nPos;
    else
      m_strStartPtr = m_pBuff->m_strBuffer + strlen(m_pBuff->m_strBuffer) - 1;
  	  
    while (m_strStartPtr >= m_pBuff->m_strBuffer)
    {
  	  if (strstr_P(m_strStartPtr, (PGM_P)str) != NULL)
  	  {
  		  nI = m_strStartPtr - m_pBuff->m_strBuffer;
  		  break;
  	  }
  	  m_strStartPtr--;
    }
    return nI;
  }
#endif
  
//****************************************************************
//* Formatting and conversion functions
//****************************************************************
#ifndef __SAM3X8E__
  void CString::format(const __FlashStringHelper *str, ...)
  {
    const uint16_t nSize = strlen_P((PGM_P)str) + 1;
    char *strTemp = new char[nSize];
    memset(strTemp, 0, nSize);
    strcpy_P(strTemp, (PGM_P)str);
    
    va_list argptr;  
    va_start(argptr, str); 
    int nRet = vsnprintf(m_pBuff->m_strBuffer, m_pBuff->capacity() - 1, strTemp, argptr);
    if (nRet == 0)
      debug.logRuntimeError(F("CString.cpp"), __LINE__);
    delete strTemp;
  }
#endif

void CString::format(const char *str, ...) 
{ 
  va_list argptr;  
  va_start(argptr, str); 
  int nRet = vsnprintf(m_pBuff->m_strBuffer, m_pBuff->capacity() - 1, str, argptr);
  if (nRet == 0)
    debug.logRuntimeError(F("CString.cpp"), __LINE__);
}

const char* CString::fromBool(const bool bVal)
{
  static char strTemp[6] = {0,0,0,0,0,0};

  if (m_pBuff->capacity() >= 6)
  {
    memset(strTemp, 0, 6);
    if (bVal)
      strcpy_P(strTemp, (PGM_P)F("true"));
    else
      strcpy_P(strTemp, (PGM_P)F("false"));
  } 
  return strTemp;
}

void CString::convertHex2Bin(char *strHex, const uint8_t nCapacity)
{
  char strTemp[nCapacity];

  memset(strTemp, 0, nCapacity);
  for (uint16_t nI = 0; nI < strlen(strHex); nI++)
  {
  	if ((strlen(strTemp) + 4) <= nCapacity)
  	{
  	  switch (strHex[nI])
  	  {
  	    case '0': strcat_P(strTemp, (PGM_P)F("0000")); break;
  	    case '1': strcat_P(strTemp, (PGM_P)F("0001")); break;
  	    case '2': strcat_P(strTemp, (PGM_P)F("0010")); break;
  	    case '3': strcat_P(strTemp, (PGM_P)F("0011")); break;
  	    case '4': strcat_P(strTemp, (PGM_P)F("0100")); break;
  	    case '5': strcat_P(strTemp, (PGM_P)F("0101")); break;
  	    case '6': strcat_P(strTemp, (PGM_P)F("0110")); break;
  	    case '7': strcat_P(strTemp, (PGM_P)F("0111")); break;
  	    case '8': strcat_P(strTemp, (PGM_P)F("1000")); break;
  	    case '9': strcat_P(strTemp, (PGM_P)F("1001")); break;
  	    case 'A':  
  	    case 'a': strcat_P(strTemp, (PGM_P)F("1010")); break;
  	    case 'B':  
  	    case 'b': strcat_P(strTemp, (PGM_P)F("1011")); break;
  	    case 'C':  
  	    case 'c': strcat_P(strTemp, (PGM_P)F("1100")); break;
  	    case 'D':  
  	    case 'd': strcat_P(strTemp, (PGM_P)F("1101")); break;
  	    case 'E':  
  	    case 'e': strcat_P(strTemp, (PGM_P)F("1110")); break;
        case 'F':  
  		  case 'f': strcat_P(strTemp, (PGM_P)F("1111")); break;
  	  };
  	}
	}
  strncpy(strHex, strTemp, nCapacity);
}

const char* CString::fromUint(const uint32_t nNum, const uint8_t nBase)
{
  if (((uint16_t)log10(nNum) + 1) <= m_pBuff->capacity())
  {
    memset(m_pBuff->m_strBuffer, 0, m_pBuff->capacity());
    utoa(nNum, m_pBuff->m_strBuffer, nBase);
  }
  return m_pBuff->m_strBuffer;
}

const char* CString::fromInt(const int32_t nNum, const uint8_t nBase)
{
  if (((uint16_t)log10(nNum) + 2) <= m_pBuff->capacity())
  {
    memset(m_pBuff->m_strBuffer, 0, m_pBuff->capacity());
    itoa(nNum, m_pBuff->m_strBuffer, nBase);
  }
  return m_pBuff->m_strBuffer;
}

const char* CString::fromReal(const double dNum, const uint8_t nDecimalPlaces)
{
  if (((uint16_t)log10(floor(dNum)) + nDecimalPlaces + 2) <= m_pBuff->capacity())
  {
    memset(m_pBuff->m_strBuffer, 0, m_pBuff->capacity());
  
    #ifdef __SAM3X8E__
      dtostrf(dNum, (nDecimalPlaces + 2), nDecimalPlaces, m_pBuff->m_strBuffer);
    #else
      dtostrf(dNum, (nDecimalPlaces + 2), nDecimalPlaces, m_pBuff->m_strBuffer);
    #endif
  }
  return m_pBuff->m_strBuffer;
}

uint32_t CString::toUint(const uint8_t nBase)
{
	return ::toUint(m_pBuff->m_strBuffer, nBase);
}

int32_t CString::toInt(const uint8_t nBase)
{
	return ::toInt(m_pBuff->m_strBuffer, nBase);
}

double CString::toReal(const uint8_t nBase)
{
	return ::toReal(m_pBuff->m_strBuffer, nBase);
}

//****************************************************************
//* Modification functions
//****************************************************************

void CString::encodeBase64()
{
  const uint16_t nBuffSize = 256;
  char strTempBuff[nBuffSize];
  memset(strTempBuff, 0, nBuffSize);
  base64_decode(strTempBuff, m_pBuff->m_strBuffer, m_pBuff->capacity() - 1);
  *this = strTempBuff;
}

void CString::decodeBase64()
{
  const uint16_t nBuffSize = 256;
  char strTempBuff[nBuffSize];
  memset(strTempBuff, 0, nBuffSize);
  base64_decode(strTempBuff, m_pBuff->m_strBuffer, m_pBuff->capacity() - 1);
  *this = strTempBuff;
}

void CString::insert(const char cCh, const uint16_t nBeforePos)
{
  char str[2] = {cCh, 0};
  insert(str, nBeforePos);
}

#ifndef __SAM3X8E__
  void CString::insert(const __FlashStringHelper* str, const uint16_t nBeforePos)
  {
    uint16_t nBuffSize = 256;
    char strTemp[nBuffSize];
    memset(strTemp, 0, nBuffSize);
    insert(strTemp, nBeforePos);
  }
#endif

void CString::insert(const char* str, const uint16_t nBeforePos)
{
  uint16_t nBuffSize = 256, nLength = 0;
  char strTemp[nBuffSize];
  
  if (((strlen(m_pBuff->m_strBuffer) + strlen(str) + 1) <= m_pBuff->capacity()) && (nBeforePos >= 0) && (nBeforePos < strlen(m_pBuff->m_strBuffer)))
  {
    m_strStartPtr = m_pBuff->m_strBuffer + nBeforePos;
    nLength = strlen(m_pBuff->m_strBuffer);
    memset(strTemp, 0, nBuffSize);
    memmove(strTemp, m_strStartPtr, nBuffSize);
    *m_strStartPtr = 0;
    strcat(m_pBuff->m_strBuffer, str);
    strcat(m_pBuff->m_strBuffer, strTemp);
    m_pBuff->m_strBuffer[nLength + strlen(str)] = 0;
  }
}

void CString::replace(const char cFind, const char cReplace, const uint16_t nStartFrom)
{
  for (uint16_t nI = nStartFrom, nLen = strlen(m_pBuff->m_strBuffer); nI < nLen; nI++)
  {
    if (m_pBuff->m_strBuffer[nI] == cFind)
    {
      m_pBuff->m_strBuffer[nI] = cReplace;
      break;
    }
  }
}

void CString::removeCh(const char cFind)
{
  char* strTempPtr = NULL;
  uint16_t nLengthMove = 0, nEnd = 0;
  
  for (m_strStartPtr = m_pBuff->m_strBuffer, m_strEndPtr = m_pBuff->m_strBuffer + strlen(m_pBuff->m_strBuffer) - 1; m_strStartPtr <= m_strEndPtr; m_strStartPtr++)
  {
    if (*m_strStartPtr == cFind)
    {
      strTempPtr = m_strStartPtr + 1;
      nLengthMove = strlen(m_pBuff->m_strBuffer) - (strTempPtr - m_pBuff->m_strBuffer);
      nEnd = strlen(m_pBuff->m_strBuffer) - 1;
      memmove(m_strStartPtr, strTempPtr, nLengthMove);
      m_pBuff->m_strBuffer[nEnd] = 0;
    }
  }
}

#ifndef __SAM3X8E__
  void CString::remove(const __FlashStringHelper* strFind)
  {
    uint16_t nBuffSize = 256;
    char strTemp[nBuffSize];
    memset(strTemp, 0, nBuffSize);
    strcpy_P(strTemp, (char*)strFind);
    remove(strTemp);
  }
#endif

void CString::remove(const char* strFind)
{
  uint16_t nLengthMove = 0, nLength = 0;
  
  while (m_strStartPtr = strstr(m_pBuff->m_strBuffer, strFind))
  {
    m_strEndPtr = m_strStartPtr + strlen(strFind);
    nLengthMove = strlen(m_pBuff->m_strBuffer) - (m_strEndPtr - m_pBuff->m_strBuffer);
    nLength = strlen(m_pBuff->m_strBuffer);
    memmove(m_strStartPtr, m_strEndPtr, nLengthMove);
    m_pBuff->m_strBuffer[nLength - strlen(strFind)] = 0;
  }
}

#ifndef __SAM3X8E__
  void CString::replace(const __FlashStringHelper* strFind, const __FlashStringHelper* strReplace, const uint16_t nStartFrom)
  {
  uint16_t nLengthMove = 0, nNewLength = 0;
  const uint16_t nLength = 512;
  char strTempBuff[nLength];

  memset(strTempBuff, 0, nLength);
  if ((strlen(m_pBuff->m_strBuffer) - strlen_P((PGM_P)strFind) + strlen_P((PGM_P)strReplace) + 1) <= m_pBuff->capacity())
  {
    if (nStartFrom < strlen(m_pBuff->m_strBuffer))
    {
      m_strStartPtr = m_pBuff->m_strBuffer + nStartFrom;
      m_strEndPtr = strstr_P(m_strStartPtr, (PGM_P)strFind);
  
      if (m_strEndPtr)
      {
        nNewLength = strlen(m_pBuff->m_strBuffer) + strlen_P((PGM_P)strReplace) - strlen_P((PGM_P)strFind);
        
        if (nNewLength < m_pBuff->capacity())
        {
          memset(strTempBuff, 0, nLength);
          strncpy(strTempBuff, m_pBuff->m_strBuffer, m_strEndPtr - m_strStartPtr);
          strcat_P(strTempBuff, (PGM_P)strReplace);
          strcat(strTempBuff, m_strEndPtr + strlen_P((PGM_P)strFind));
          memset(m_pBuff->m_strBuffer, 0, m_pBuff->capacity());
          strcpy(m_pBuff->m_strBuffer, strTempBuff);
        }
      }
    }
  }
  }

  void CString::replace(const char* strFind, const __FlashStringHelper* strReplace, const uint16_t nStartFrom)
  {
    uint16_t nLengthMove = 0, nNewLength = 0;
    const uint16_t nLength = 512;
    char strTempBuff[nLength];
  
    if ((strlen(m_pBuff->m_strBuffer) - strlen(strFind) + strlen_P((PGM_P)strReplace) + 1) <= m_pBuff->capacity())
    {
      if (nStartFrom < strlen(m_pBuff->m_strBuffer))
      {
        m_strStartPtr = m_pBuff->m_strBuffer + nStartFrom;
        m_strEndPtr = strstr(m_strStartPtr, strFind);
    
        if (m_strEndPtr)
        {
          nNewLength = strlen(m_pBuff->m_strBuffer) + strlen_P((PGM_P)strReplace) - strlen(strFind);
          
          if (nNewLength < m_pBuff->capacity())
          {
            memset(strTempBuff, 0, nLength);
            strncpy(strTempBuff, m_pBuff->m_strBuffer, m_strEndPtr - m_strStartPtr);
            strcat_P(strTempBuff, (PGM_P)strReplace);
            strcat(strTempBuff, m_strEndPtr + strlen(strFind));
            memset(m_pBuff->m_strBuffer, 0, m_pBuff->capacity());
            strcpy(m_pBuff->m_strBuffer, strTempBuff);
          }
        }
      }
    }
  }

  void CString::replace(const __FlashStringHelper* strFind, const char* strReplace, const uint16_t nStartFrom)
  {
    uint16_t nLengthMove = 0, nNewLength = 0;
    const uint16_t nLength = 512;
    char strTempBuff[nLength];
  
    if ((strlen(m_pBuff->m_strBuffer) - strlen_P((PGM_P)strFind) + strlen(strReplace) + 1) <= m_pBuff->capacity())
    {
      if (nStartFrom < strlen(m_pBuff->m_strBuffer))
      {
        m_strStartPtr = m_pBuff->m_strBuffer + nStartFrom;
        m_strEndPtr = strstr_P(m_strStartPtr, (PGM_P)strFind);
    
        if (m_strEndPtr)
        {
          nNewLength = strlen(m_pBuff->m_strBuffer) + strlen(strReplace) - strlen_P((PGM_P)strFind);
          
          if (nNewLength < m_pBuff->capacity())
          {
            memset(strTempBuff, 0, nLength);
            strncpy(strTempBuff, m_pBuff->m_strBuffer, m_strEndPtr - m_strStartPtr);
            strcat(strTempBuff, strReplace);
            strcat(strTempBuff, m_strEndPtr + strlen_P((PGM_P)strFind));
            memset(m_pBuff->m_strBuffer, 0, m_pBuff->capacity());
            strcpy(m_pBuff->m_strBuffer, strTempBuff);
          }
        }
      }
    }
  }
#endif

void CString::replace(const char* strFind, const char* strReplace, const uint16_t nStartFrom)
{
  uint16_t nLengthMove = 0, nNewLength = 0;
  const uint16_t nLength = 512;
  char strTempBuff[nLength];

  if ((strlen(m_pBuff->m_strBuffer) - strlen(strFind) + strlen(strReplace) + 1) <= m_pBuff->capacity())
  {
    if (nStartFrom < strlen(m_pBuff->m_strBuffer))
    {
      m_strStartPtr = m_pBuff->m_strBuffer + nStartFrom;
      m_strEndPtr = strstr(m_strStartPtr, strFind);
  
      if (m_strEndPtr)
      {
        nNewLength = strlen(m_pBuff->m_strBuffer) + strlen(strReplace) - strlen(strFind);
        
        if (nNewLength < m_pBuff->capacity())
        {
          memset(strTempBuff, 0, nLength);
          strncpy(strTempBuff, m_pBuff->m_strBuffer, m_strEndPtr - m_strStartPtr);
          strcat(strTempBuff, strReplace);
          strcat(strTempBuff, m_strEndPtr + strlen(strFind));
          memset(m_pBuff->m_strBuffer, 0, m_pBuff->capacity());
          strcpy(m_pBuff->m_strBuffer, strTempBuff);
        }
      }
    }
  }
}

void CString::remove(const uint16_t nPos, uint16_t nCount)
{
  if ((nPos >= 0) && (nPos < strlen(m_pBuff->m_strBuffer)) && ((nPos + nCount) <= strlen(m_pBuff->m_strBuffer)))
  {
    m_strStartPtr = m_pBuff->m_strBuffer + nPos;
    m_strEndPtr = m_pBuff->m_strBuffer + nPos + nCount;
    uint8_t nLength = strlen(m_pBuff->m_strBuffer), nLengthMove = strlen(m_pBuff->m_strBuffer) - (m_strEndPtr - m_pBuff->m_strBuffer) + 1;
    memmove(m_strStartPtr, m_strEndPtr, nLengthMove);
    m_pBuff->m_strBuffer[nLength - nCount] = 0;
  }
}

void CString::toLower()
{
  for (uint16_t nI = 0; nI < strlen(m_pBuff->m_strBuffer); nI++)
  {
    if ((m_pBuff->m_strBuffer[nI] >= 'A') && (m_pBuff->m_strBuffer[nI] <= 'Z'))
      m_pBuff->m_strBuffer[nI] += 32;
  }
}

void CString::toUpper()
{
  for (uint16_t nI = 0; nI < strlen(m_pBuff->m_strBuffer); nI++)
  {
    if ((m_pBuff->m_strBuffer[nI] >= 'a') && (m_pBuff->m_strBuffer[nI] <= 'z'))
      m_pBuff->m_strBuffer[nI] -= 32;
  }
}

void CString::concat(const char *strStart, const char* strEnd, char* strDest, const uint16_t nSize)
{
	uint16_t nI = strlen(strDest);

	while ((strStart <= strEnd) && (nI < nSize))
	{
		strDest[nI] = strStart[0];
		strDest[nI + 1] = 0;
		strStart++;
		nI++;
	}
}

void CString::copy(const char *strStart, const char* strEnd, char* strDest, const uint16_t nSize)
{
  uint16_t nI = 0;

  memset(strDest, 0, nSize);
  while ((strStart <= strEnd) && (nI < nSize))
  {
    strDest[nI] = *strStart;
    strStart++;
    nI++;
  }
}

void CString::trimLeft()
{
  m_strStartPtr = m_pBuff->m_strBuffer;
  m_strEndPtr = m_pBuff->m_strBuffer + strlen(m_pBuff->m_strBuffer) - 1;
  while (isWhitespace(*m_strStartPtr) && (m_strStartPtr <= m_strEndPtr))
	  m_strStartPtr++;

  uint16_t nMoveLength = m_strEndPtr - m_strStartPtr + 1;
  memmove(m_pBuff->m_strBuffer, m_strStartPtr, nMoveLength);
  m_pBuff->m_strBuffer[nMoveLength] = 0;
}

void CString::trimRight()
{
  m_strStartPtr = m_pBuff->m_strBuffer;
  m_strEndPtr = m_pBuff->m_strBuffer + strlen(m_pBuff->m_strBuffer) - 1;
  while (isWhitespace(*m_strEndPtr) && (m_strEndPtr >= m_strStartPtr))
	  m_strEndPtr--;
  m_strEndPtr++;
  *m_strEndPtr = 0;
}

void CString::trim()
{
  trimLeft();
  trimRight();
}

void CString::padLeft(const char cCh, const uint8_t nLength)
{
  while ((strlen(m_pBuff->m_strBuffer) < nLength) && (strlen(m_pBuff->m_strBuffer) < m_pBuff->capacity()))
  {
    insert(cCh, 0); 
  }
}

void CString::padRight(const char cCh, const uint8_t nLength)
{
  while ((strlen(m_pBuff->m_strBuffer) < nLength) && (strlen(m_pBuff->m_strBuffer) < m_pBuff->capacity()))
  {
    m_pBuff->m_strBuffer[strlen(m_pBuff->m_strBuffer)] = cCh;
    m_pBuff->m_strBuffer[strlen(m_pBuff->m_strBuffer) + 1] = 0;
  }
  
}






//#define DTOSTR_ALWAYS_SIGN 0x01
//#define DTOSTR_PLUS_SIGN   0x02
//#define DTOSTR_UPPERCASE   0x04








