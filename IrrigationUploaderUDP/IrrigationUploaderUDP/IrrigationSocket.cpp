// IrrigationSocket.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploaderUDP.h"
#include "IrrigationSocket.h"


// CIrrigationSocket

IMPLEMENT_DYNAMIC(CIrrigationSocket, CSocket);

LPCSTR CIrrigationSocket::m_lpszDelim = "`";
const DWORD CIrrigationSocket::m_nWait = 5000;

CIrrigationSocket::CIrrigationSocket()
{
	m_nTimeoutTrigger = 0;
}

CIrrigationSocket::~CIrrigationSocket()
{
}

bool CIrrigationSocket::Create()
{
	BOOL bResult = CSocket::Create(10004, SOCK_DGRAM, NULL);
	if (!bResult)
		DisplayError(GetLastError());

	return bResult == TRUE;
}

BOOL CIrrigationSocket::OnMessagePending()
{
	BOOL bResult = FALSE;

	if (GetTickCount() > m_nTimeoutTrigger)
	{
		CancelBlockingCall();
		bResult = TRUE;
	}
	return bResult;
}

bool CIrrigationSocket::Request(LPCSTR lpszDataItemName, const byte nI, int &nDataItem, LPCSTR lpszIPAddr, const UINT nPort)
{
	CString strDataItem;
	bool bResult = false;

	bResult = Request(lpszDataItemName, nI, strDataItem, lpszIPAddr, nPort);
	nDataItem = atoi(strDataItem);

	return bResult;
}

bool CIrrigationSocket::Request(LPCSTR lpszDataItemName, const byte nI, bool &bDataItem, LPCSTR lpszIPAddr, const UINT nPort)
{
	CString strDataItem;
	bool bResult = false;

	bResult = Request(lpszDataItemName, nI, strDataItem, lpszIPAddr, nPort);
	bDataItem = atoi(strDataItem) == 1;

	return bResult;
}

bool CIrrigationSocket::Request(LPCSTR lpszDataItemName, const byte nI, CString &strDataItem, LPCSTR lpszIPAddr, const UINT nPort)
{
	CString strData;

	strData.Format("%s%s%d", lpszDataItemName, m_lpszDelim, nI);
	return Request(strData, strDataItem, lpszIPAddr, nPort);
}

bool CIrrigationSocket::Request(LPCSTR lpszDataItemName, UINT &nDataItem, LPCSTR lpszIPAddr, const UINT nPort)
{
	CString strDataItem;
	bool bResult = false;

	bResult = Request(lpszDataItemName, strDataItem, lpszIPAddr, nPort);
	nDataItem = atoi(strDataItem);

	return bResult;
}

bool CIrrigationSocket::Request(LPCSTR lpszDataItemName, DWORD &nDataItem, LPCSTR lpszIPAddr, const UINT nPort)
{
	CString strDataItem;
	bool bResult = false;

	bResult = Request(lpszDataItemName, strDataItem, lpszIPAddr, nPort);
	nDataItem = atol(strDataItem);

	return bResult;
}

bool CIrrigationSocket::Request(LPCSTR lpszDataItemName, bool &bDataItem, LPCSTR lpszIPAddr, const UINT nPort)
{
	CString strDataItem;
	bool bResult = false;

	bResult = Request(lpszDataItemName, strDataItem, lpszIPAddr, nPort);
	bDataItem = atoi(strDataItem) == 1;

	return bResult;
}

bool CIrrigationSocket::Request(LPCSTR lpszDataItemName, CString &strDataItem, LPCSTR lpszIPAddr, const UINT nPort)
{
	bool bResult = false;
	CString strIPAddrSender;
	UINT nPortSender = 0;

	strDataItem.Empty();
	if (Send(lpszDataItemName, lpszIPAddr, nPort))
	{
		if (Receive(strDataItem, strIPAddrSender, nPortSender))
		{
			if ((strIPAddrSender == lpszIPAddr) && (nPortSender == nPort))
			{
				bResult = true;
			}
		}
	}
	Sleep(250);
	return bResult;
}

bool CIrrigationSocket::Receive(CString &strData, CString &strIPAddr, UINT &nPort)
{
	int nResult = 0, nPos = 0;
	const UINT nSize = 129;
	char szBuff[nSize];
	memset(szBuff, 0, nSize);
	
	m_nTimeoutTrigger = GetTickCount() + m_nWait;

	if ((nResult = ReceiveFrom(szBuff, nSize - 1, strIPAddr, nPort)) == SOCKET_ERROR)
	{
		DisplayError(GetLastError());
		nResult = 0;
	}
	else if (nResult > 0)
	{
		strData = szBuff;
		if ((nPos = strData.Find(m_lpszDelim)) > -1)
		{
			strData.Delete(nPos, strlen(m_lpszDelim));
		}
	}
	return nResult > 0;
}

bool CIrrigationSocket::Send(LPCSTR lpszData, LPCSTR lpszIPAddr, const UINT nPort)
{
	int nResult = 0;
	CString strData = CString(lpszData) + m_lpszDelim;

	if (strlen(lpszIPAddr) > 0)
	{
		m_nTimeoutTrigger = GetTickCount() + m_nWait;

		if ((nResult = SendToEx(strData, strData.GetLength(), nPort, lpszIPAddr)) == SOCKET_ERROR)
		{
			DisplayError(GetLastError());
			nResult = 0;
		}
	}
	return nResult > 0;
}

void CIrrigationSocket::DisplayError(const UINT nError)
{
	CString strError, strNum;

	strNum.Format("%d", nError);
	switch (nError)
	{
		case WSANOTINITIALISED: strError = "AfxSocketInit() must occur before using this API!"; break;
		case WSAENETDOWN: strError = "The Windows Sockets implementation detected that the network subsystem failed!"; break;
		case WSAEAFNOSUPPORT: strError = "The specified address family is not supported!"; break;
		case WSAEINPROGRESS: strError = "A blocking Windows Sockets operation is in progress!"; break;
		case WSAEMFILE: strError = "No more file descriptors are available!"; break;
		case WSAENOBUFS: strError = "No buffer space is available - the socket cannot be created!"; break;
		case WSAEPROTONOSUPPORT: strError = "The specified port is not supported!"; break;
		case WSAEPROTOTYPE: strError = "The specified port is the wrong type for this socket!"; break;
		case WSAESOCKTNOSUPPORT: strError = "The specified socket type is not supported in this address family!"; break;
		case WSAEADDRINUSE: strError = "An attempt has been made to listen on an address in use!"; break;
		case WSAEINVAL: strError = "The socket has not been bound with Bind or is already connected!"; break;
		case WSAEISCONN: strError = "The socket is already connected!"; break;
		case WSAENOTSOCK: strError = "The descriptor is not a socket!"; break;
		case WSAEOPNOTSUPP: strError = "The referenced socket is not of a type that supports the Listen operation!"; break;
		case WSAEADDRNOTAVAIL: strError = "The specified address is not available from the local machine!"; break;
		case WSAECONNREFUSED: strError = "The attempt to connect was rejected!"; break;
		case WSAEDESTADDRREQ: strError = "A destination address is required!"; break;
		case WSAEFAULT: strError = "The nSockAddrLen argument is incorrect!"; break;
		case WSAENETUNREACH: strError = "The network cannot be reached from this host at this time!"; break;
		case WSAETIMEDOUT: strError = "Attempt to connect timed out without establishing a connection!"; break;
		case WSAEWOULDBLOCK: strError = "The socket is marked as nonblocking and the connection cannot be completed immediately!"; break;
		case WSAENOTCONN: strError = "The socket is not connected!"; break;
		case WSAENOPROTOOPT: strError = "The option is unknown or unsupported.In particular, SO_BROADCAST is not supported on sockets of type SOCK_STREAM, while SO_ACCEPTCONN, SO_DONTLINGER, SO_KEEPALIVE, SO_LINGER, and SO_OOBINLINE are not supported on sockets of type SOCK_DGRAM!"; break;
		case WSAEMSGSIZE: strError = "Message too long!"; break;
		case WSAEINTR: strError = ""; break;

		default: strError = "Unknon error: " + strNum + "!"; break;
	}
	if (strError.GetLength() > 0)
		AfxMessageBox(strError, MB_OK);
}
