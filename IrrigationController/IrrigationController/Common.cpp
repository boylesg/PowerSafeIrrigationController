#include <Arduino.h>
#include "Common.h"
#include "CString.h"
#include "debug.h"




CBuff<32> buffCommon;




//*************************************************************************************************
// Replacement for SoftReset.h function for Due board
//*************************************************************************************************
      
void resetESP()
{
  uint8_t nPin = 22;
  pinMode(nPin, OUTPUT);
  digitalWrite(nPin, LOW);
}

void resetArd()
{
  uint8_t nPin = 24;
  pinMode(nPin, OUTPUT);
  digitalWrite(nPin, LOW);
}

#ifdef __SAM3X8E__

  char *dtostre(const double dVal, const uint8_t nWidth, const uint8_t nPrec, char *strOut) 
  {
     sprintf(strOut, "%*.*e", nWidth, nPrec, dVal);
     return strOut;
  }

  char *dtostrf(const double dVal, const uint8_t nWidth, const uint8_t nPrec, char *strOut) 
  {
     sprintf(strOut, "%*.*f", nWidth, nPrec, dVal);
     return strOut;
  }

#endif




//*************************************************************************************************
// Conversion functions
//*************************************************************************************************

const char* fromChar(const char cCh)
{
  buffCommon.empty();
  buffCommon[0] = cCh;

  return buffCommon;
}

const char* fromBool(const bool bVal)
{
  buffCommon.empty();

  if (bVal)
    strcpy_P(buffCommon, (char*)F("true"));
  else
    strcpy_P(buffCommon, (char*)F("false"));
    
  return buffCommon;
}

const char* fromUint(const uint32_t nNum, const uint8_t nBase)
{
  buffCommon.empty();
  utoa(nNum, buffCommon, nBase);

  return buffCommon;
}

const char* fromInt(const int32_t nNum, const uint8_t nBase)
{
  buffCommon.empty();
  itoa(nNum, buffCommon, nBase);

  return buffCommon;
}

const char* fromReal(const double dNum, const uint8_t nDecimalPlaces)
{
#ifdef __SAM3X8E__
  dtostrf(dNum, (nDecimalPlaces + 2), nDecimalPlaces, buffCommon);
#else
    dtostrf(dNum, (nDecimalPlaces + 2), nDecimalPlaces, buffCommon);
#endif

  return buffCommon;
}

const char* fromIP(IPAddress& ip)
{
  static CBuff<16> buffCommon;
  CString strIP(buffCommon);

  strIP.format(F("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);

  return strIP;
}

uint32_t toUint(const char *strNum, const uint8_t nBase)
{
	int8_t nI = 0;
	uint32_t nVal = 0, nMult = 0;

	if ((nBase >= 2) && (nBase <= 16))
	{
		for (nI = strlen(strNum) - 1, nMult = 1; nI >= 0; nI--, nMult *= nBase)
		{
			if ((strNum[nI] >= '0') && (strNum[nI] <= '9'))
				nVal += (strNum[nI] - '0') * nMult;
			else if ((strNum[nI] >= 'a') && (strNum[nI] <= 'z'))
				nVal += (strNum[nI] - 'W') * nMult;
			else if ((strNum[nI] >= 'A') && (strNum[nI] <= 'Z'))
				nVal += (strNum[nI] - '7') * nMult;
		}
	}
	return nVal;
}

int32_t toInt(const char *strNum, const uint8_t nBase)
{
	int8_t nI = 0;
	int32_t nVal = 0, nMult = 0;

	if ((nBase >= 2) && (nBase <= 16))
	{
		for (nI = strlen(strNum) - 1, nMult = 1; nI >= 0; nI--, nMult *= nBase)
		{
			if (strNum[nI] == '-')
				nVal *= -1;
			else if ((strNum[nI] >= '0') && (strNum[nI] <= '9'))
				nVal += (strNum[nI] - '0') * nMult;
			else if ((strNum[nI] >= 'a') && (strNum[nI] <= 'z'))
				nVal += (strNum[nI] - 'W') * nMult;
			else if ((strNum[nI] >= 'A') && (strNum[nI] <= 'Z'))
				nVal += (strNum[nI] - '7') * nMult;
		}
	}
	return nVal;
}

double toReal(const char *strNum, const uint8_t nBase)
{
	int8_t nI = 0, nSign = 0;
	uint8_t nPlaces = 0, nPos = 0, nLen = 0, nPow = 0, nMult = 0;
	double dVal = 0.0, dMult = 0;
	CString strNumber(buffCommon);
  strNumber = strNum;

	nPos = strNumber.indexOf(".");
	nPlaces = nPos - 1;
	nSign = 1;
	if (strNumber[0] == '-')
	{
		nSign = -1;
		strNumber[0] = '0';
	}
	else
		nSign = 1;

	// EXAMPLE: 123.456e-67
	if ((strstr(strNum, "e-") != NULL) || (strstr(strNum, "e+") != NULL) || (strstr(strNum, "E-") != NULL) || (strstr(strNum, "E+") != NULL))
	{
		for (nI = 0, nLen = strlen(strNum), dMult = pow(nBase, nPlaces); nI < nLen; nI++, dMult /= nBase)
		{
			if (strNumber[nI] == '.')
				dMult *= nBase;
			else if ((strNumber[nI] == 'e') || (strNumber[nI] == 'E'))
				break;
			else if ((strNumber[nI] >= '0') && (strNumber[nI] <= '9'))
				dVal += (strNumber[nI] - '0') * dMult;
			else if ((strNumber[nI] >= 'a') && (strNumber[nI] <= 'z'))
				dVal += (strNumber[nI] - 'W') * dMult;
			else if ((strNumber[nI] >= 'A') && (strNumber[nI] <= 'Z'))
				dVal += (strNumber[nI] - '7') * dMult;
		}
		dVal *= nSign;
		nI = nLen - 1;
		nMult = 1;
		while ((strNumber[nI] != '-') && (strNumber[nI] != '+'))
		{
			if ((strNumber[nI] >= '0') && (strNumber[nI] <= '9'))
				nPow += (strNumber[nI] - '0') * nMult;
			else if ((strNumber[nI] >= 'a') && (strNumber[nI] <= 'z'))
				nPow += (strNumber[nI] - 'W') * nMult;
			else if ((strNumber[nI] >= 'A') && (strNumber[nI] <= 'Z'))
				nPow += (strNumber[nI] - '7') * nMult;

			nMult *= nBase;
			nI--;
		}
		if (strNumber[nI] == '-')
			nSign = -1;
		else
			nSign = 1;

		dVal *= pow(nBase, nPow * nSign);
	}
	// EXAMPLE: 123.456
	else
	{
		for (nI = 0, nLen = strlen(strNum), dMult = pow(nBase, nPlaces); nI <= nLen; nI++, dMult /= nBase)
		{
			if (strNumber[nI] == '.')
				dMult *= nBase;
			else if ((strNumber[nI] >= '0') && (strNumber[nI] <= '9'))
				dVal += (strNumber[nI] - '0') * dMult;
			else if ((strNumber[nI] >= 'a') && (strNumber[nI] <= 'z'))
				dVal += (strNumber[nI] - 'W') * dMult;
			else if ((strNumber[nI] >= 'A') && (strNumber[nI] <= 'Z'))
				dVal += (strNumber[nI] - '7') * dMult;
		}
		dVal *= nSign;
	}
	return dVal;
}

IPAddress toIP(const char* strIP)
{
  IPAddress ip;
  ip.fromString(strIP);
  return ip;
}



