#include "stdafx.h"
#include <iostream>
using namespace std;
#include "SerialPort.h"
#include "ProgressDlg.h"




CCOMPort::CCOMPort()
{
}

CCOMPort::CCOMPort(LPCSTR lpszName, LPCSTR lpszCOM)
{
	Set(lpszName, lpszCOM);
}

CCOMPort::~CCOMPort()
{
}

void CCOMPort::Set(LPCSTR lpszName, LPCSTR lpszCOM)
{
	m_strCOM = lpszCOM;
	m_strName = lpszName;
	m_strDesc = m_strName + " (" + m_strCOM + ")";
}

void CCOMPort::Empty()
{
	m_strCOM.Empty();
	m_strName.Empty();
	m_strDesc.Empty();
}

CCOMPort& CCOMPort::operator =(CCOMPort& rCOMPort)
{
	m_strCOM = rCOMPort.m_strCOM;
	m_strName = rCOMPort.m_strName;
	m_strDesc = rCOMPort.m_strDesc;
	return *this;
}

bool CCOMPort::operator ==(CCOMPort& rCOMPort)
{
	return m_strCOM == rCOMPort.m_strCOM;
}

bool CCOMPort::operator !=(CCOMPort& rCOMPort)
{
	return m_strCOM != rCOMPort.m_strCOM;
}

bool CCOMPort::IsEmpty()
{
	return m_strCOM.IsEmpty();
}





//typedef struct _COMMTIMEOUTS 
//{
//  DWORD ReadIntervalTimeout;
//  DWORD ReadTotalTimeoutMultiplier;
//  DWORD ReadTotalTimeoutConstant;
//  DWORD WriteTotalTimeoutMultiplier;
//  DWORD WriteTotalTimeoutConstant;
//} 
//COMMTIMEOUTS, *LPCOMMTIMEOUTS;

IMPLEMENT_DYNAMIC(CSerialPort, CObject)

LPCSTR CSerialPort::m_lpszDelim = "`";
const DWORD CSerialPort::m_dwWait = 10000;

CSerialPort::CSerialPort()
{
	m_commHandle = NULL;
}

CSerialPort::~CSerialPort()
{
	Close();
}

#define BUFF_SIZE 30

bool CSerialPort::isIrrigationController(CSerialPort &SerialCOMPort)
{
	CString strResp, strFound;
	bool bResult = false;

	if (SerialCOMPort.Request("irrigation", strResp))
	{
		bResult = strResp == "irrigation";
	}
	return bResult;
}

bool CSerialPort::IsValid(HANDLE commHandle)
{
	return (commHandle != INVALID_HANDLE_VALUE) && (commHandle != NULL);
}

void CSerialPort::GetCOMPorts(CCOMPortArray &arrayCOMPorts, CWnd *pWnd)
{
	CRegKey regkey;
	DWORD dwI = 0, nBuffSize = BUFF_SIZE;
	char szKeyName[BUFF_SIZE], szKeyVal[BUFF_SIZE];
	LONG nCode = ERROR_SUCCESS;
	CString strKeyVal, strResponse;
	UINT nCOMPort = 0, nProgress = 0, nBaud = 9600;
	CSerialPort SerialPort;
	CStringArray arrayPorts;
	CProgressDlg dlg(pWnd);
	
	memset(szKeyName, 0, BUFF_SIZE);
	memset(szKeyVal, 0, BUFF_SIZE);
	if (regkey.Open(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", KEY_READ) == ERROR_SUCCESS)
	{
		//LONG WINAPI RegEnumValue(_In_        HKEY    hKey,
		//							_In_        DWORD   dwIndex,
		//							_Out_       LPTSTR  lpValueName,
		//							_Inout_     LPDWORD lpcchValueName,
		//							_Reserved_  LPDWORD lpReserved,
		//							_Out_opt_   LPDWORD lpType,
		//							_Out_opt_   LPBYTE  lpData,
		//							_Inout_opt_ LPDWORD lpcbData);
		do
		{
			nCode = RegEnumValue(regkey.m_hKey, dwI, szKeyName, &nBuffSize, NULL, NULL, NULL, NULL);
			if (nCode != ERROR_NO_MORE_ITEMS)
			{
				nBuffSize = BUFF_SIZE;
				regkey.QueryStringValue(szKeyName, szKeyVal, &nBuffSize);
				nBuffSize = BUFF_SIZE;
				strKeyVal = szKeyVal;
				if (strKeyVal.Find("COM") >= 0)
				{
					if (strKeyVal != "COM1")
						arrayPorts.Add(strKeyVal);
				}
				dwI++;
			}
		} 
		while (nCode != ERROR_NO_MORE_ITEMS);

		if (arrayPorts.GetCount() > 0)
		{
			dlg.SetProgress(nProgress, "Searching for irrigation controllers - please be patient!");
			Sleep(1000);
			for (int nI = 0; nI < arrayPorts.GetSize(); nI++)
			{
				dlg.SetProgress(nProgress, "Searching for irrigation controllers - " + arrayPorts[nI]);
				if (SerialPort.Open(arrayPorts[nI], nBaud) && isIrrigationController(SerialPort))
				{
					SerialPort.Request("id#", strResponse);
					if (strResponse.GetLength() > 0)
						arrayCOMPorts.Add(CCOMPort(strResponse, arrayPorts[nI]));
					SerialPort.Close();
				}
				nProgress = (UINT)(((float)nI / arrayPorts.GetCount()) * 100);
			}
			dlg.SetProgress(100, "Search complete!");
			Sleep(1000);
		}
		else
			AfxMessageBox("No irrigation controllers found - please esnure you have paired with the device in your computer's bluetooth settings!", MB_OK);
	}
}

bool CSerialPort::Close(HANDLE &commHandle)
{
	bool bResult = false;

	if (IsValid(commHandle))
	{
		bResult = CloseHandle(commHandle) == TRUE;
		commHandle = NULL;
	}
	return bResult;
}

bool CSerialPort::InitPort(HANDLE &commHandle, LPCSTR lpszCOMPort, const int nBaudRate)
{
	bool bResult = false;
	CString strCOMPort = lpszCOMPort;

	if (IsValid(commHandle))
	{
		// set timeouts
		//COMMTIMEOUTS cto = {MAXDWORD, 0, 0, 500, 0};
		COMMTIMEOUTS cto = { MAXDWORD, 0, 0, 200, 0 };

		if (SetCommTimeouts(commHandle, &cto))
		{
			// set DCB
			DCB dcb;

			memset(&dcb, 0, sizeof(dcb));
			dcb.DCBlength = sizeof(dcb);
			dcb.BaudRate = nBaudRate;
			dcb.fBinary = true;
			dcb.fDtrControl = DTR_CONTROL_ENABLE;
			dcb.fRtsControl = RTS_CONTROL_ENABLE;
			dcb.Parity = NOPARITY;
			dcb.StopBits = ONESTOPBIT;
			dcb.ByteSize = 8;

			if (SetCommState(commHandle, &dcb))
				bResult = true;
			else
			{
				commHandle = NULL;
				AfxMessageBox(CString("Could not set COM state  on ") + strCOMPort + "...", MB_OK);
			}
		}
		else
		{
			commHandle = NULL;
			AfxMessageBox(CString("Could not set COM time outs on ") + strCOMPort + "...", MB_OK);
		}
	}
	else
	{
		commHandle = NULL;
	}
	return bResult;
}

bool CSerialPort::Open(LPCSTR lpszCOMPort, const int nBaudRate)
{
	bool bResult = false;

	Close(m_commHandle);
	
	m_COMPort.m_strCOM = lpszCOMPort;
	m_commHandle = OpenHandle(lpszCOMPort, nBaudRate);
	bResult = m_commHandle != INVALID_HANDLE_VALUE;

	return bResult;
}

bool CSerialPort::Open(CCOMPort COMPort, const int nBaudRate)
{
	bool bResult = false;

	Close(m_commHandle);

	m_COMPort = COMPort;
	m_commHandle = OpenHandle(COMPort.m_strCOM, nBaudRate);
	bResult = m_commHandle != INVALID_HANDLE_VALUE;

	return bResult;
}

HANDLE CSerialPort::OpenHandle(LPCSTR lpszCOMPortNum, const int nBaudRate)
{
	CString strComPort;
	HANDLE commHandle = NULL;

	strComPort = CString("\\\\.\\") + lpszCOMPortNum;
	commHandle = CreateFile(strComPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	InitPort(commHandle, lpszCOMPortNum, nBaudRate);

	return commHandle;
}

bool CSerialPort::Close()
{
	return Close(m_commHandle);
}

UINT CSerialPort::Write(LPCSTR lpszBuffer)
{
	DWORD dwNumWritten = 0;
	
	if (*this)
	{
		if (IsValid(m_commHandle))
			WriteFile(m_commHandle, lpszBuffer, strlen(lpszBuffer), &dwNumWritten, NULL);
	}
	return dwNumWritten;
}

UINT CSerialPort::Read(CString &rstrText)
{
	const int nBuffSize = 100;
	CString strTemp;
	LPSTR lpszBuff = strTemp.GetBuffer(nBuffSize);
	DWORD dwNumRead = 0, dwCurrentMillis = 0, dwStartMillis = 0;
	int nPos = 0, nLength = 0;

	if (IsValid(m_commHandle))
	{
		dwCurrentMillis = dwStartMillis = GetTickCount();
		rstrText.Empty();
		do
		{
			memset(lpszBuff, 0, nBuffSize);
			ReadFile(m_commHandle, lpszBuff, nBuffSize, &dwNumRead, NULL);
			rstrText += lpszBuff;
			nPos = rstrText.Find(m_lpszDelim);
			nLength = rstrText.GetLength();
			if ((nPos >= 0) && (nPos == (nLength - 1)))
			{
				rstrText.Delete(nPos, 1);
				break;
			}
			dwCurrentMillis = GetTickCount();
		} while (((dwCurrentMillis - dwStartMillis) < m_dwWait));

		strTemp.ReleaseBuffer();
	}
	return rstrText.GetLength();
}

void CSerialPort::Flush()
{
	const byte nBuffSize = 10;
	CString str;
	int nNumBytes = Read(str);
	
	while (nNumBytes > 0)
		nNumBytes = Read(str);
}

bool CSerialPort::Request(LPCSTR lpszDataItemName, const byte nI, int &nDataItem)
{
	CString strDataItem;
	bool bResult = false;

	bResult = Request(lpszDataItemName, nI, strDataItem);
	nDataItem = atoi(strDataItem);

	return bResult;
}

bool CSerialPort::Request(LPCSTR lpszDataItemName, const byte nI, bool &bDataItem)
{
	CString strDataItem;
	bool bResult = false;

	bResult = Request(lpszDataItemName, nI, strDataItem);
	bDataItem = atoi(strDataItem) == 1;

	return bResult;
}

bool CSerialPort::Request(LPCSTR lpszDataItemName, const byte nI, CString &strDataItem)
{
	CString strData;

	strData.Format("%s%s%d", lpszDataItemName, m_lpszDelim, nI);
	return Request(strData, strDataItem);
}

bool CSerialPort::Request(LPCSTR lpszDataItemName, UINT &nDataItem)
{
	CString strDataItem;
	bool bResult = false;

	bResult = Request(lpszDataItemName, strDataItem);
	nDataItem = atoi(strDataItem);

	return bResult;
}

bool CSerialPort::Request(LPCSTR lpszDataItemName, DWORD &nDataItem)
{
	CString strDataItem;
	bool bResult = false;

	bResult = Request(lpszDataItemName, strDataItem);
	nDataItem = atol(strDataItem);

	return bResult;
}

bool CSerialPort::Request(LPCSTR lpszDataItemName, bool &bDataItem)
{
	CString strDataItem;
	bool bResult = false;

	bResult = Request(lpszDataItemName, strDataItem);
	bDataItem = atoi(strDataItem) == 1;

	return bResult;
}

bool CSerialPort::Request(LPCSTR lpszDataItemName, CString &strDataItem)
{
	bool bResult = false;

	strDataItem.Empty();
	if (Send(lpszDataItemName))
	{
		if (Receive(strDataItem))
		{
			bResult = true;
		}
	}
	Sleep(250);
	return bResult;
}

bool CSerialPort::Receive(CString &strData)
{
	int nResult = 0, nPos = 0;
	const UINT nSize = 129;


	m_nTimeoutTrigger = GetTickCount() + m_dwWait;

	if ((nResult = Read(strData)) > 0)
	{
		// Do nothing
	}
	return nResult > 0;
}

bool CSerialPort::Send(LPCSTR lpszData)
{
	int nResult = 0;
	CString strData = CString(lpszData) + m_lpszDelim;

	m_nTimeoutTrigger = GetTickCount() + m_dwWait;

	if ((nResult = Write(strData)) == 0)
	{
		nResult = 0;
	}
	return nResult > 0;
}

