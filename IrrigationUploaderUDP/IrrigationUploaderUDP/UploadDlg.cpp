#include "stdafx.h"
#include "IrrigationUploaderUDP.h"
#include "UploadDlg.h"
#include "afxdialogex.h"


// CUploadDlg dialog

IMPLEMENT_DYNAMIC(CUploadDlg, CDialogEx)

CUploadDlg::CUploadDlg(const UINT nDlgResID, CWnd* pParent /*=NULL*/): CDialogEx(nDlgResID, pParent)
{
	m_nPort = 0;
	m_pSocket = NULL;
}

CUploadDlg::~CUploadDlg()
{
}

void CUploadDlg::GetDataItem(LPCSTR lpszItemName, const int nStation, CString &strItemVal)
{
	byte nI = 0;

	for (nI = 1; nI < 10; nI++)
	{
		m_pSocket->Request(lpszItemName, nStation, strItemVal, m_strIPAddr, m_nPort);
		if (strItemVal.GetLength() > 0)
			break;
		Sleep(200);
	}
}

void CUploadDlg::GetDataItem(LPCSTR lpszItemName, const int nStation, int &nItemVal)
{
	CString strVal;
	GetDataItem(lpszItemName, nStation, strVal);
	nItemVal = atoi(strVal);
}

void CUploadDlg::GetDataItem(LPCSTR lpszItemName, const int nStation, bool &bItemVal)
{
	CString strVal;
	GetDataItem(lpszItemName, nStation, strVal);
	bItemVal = strVal == "1";
}

void CUploadDlg::GetDataItem(LPCSTR lpszItemName, CString &strItemVal)
{
	byte nI = 0;

	for (nI = 1; nI < 3; nI++)
	{
		m_pSocket->Request(lpszItemName, strItemVal, m_strIPAddr, m_nPort);
		if (strItemVal.GetLength() > 0)
			break;
	}
}

void CUploadDlg::GetDataItem(LPCSTR lpszItemName, int &nItemVal)
{
	CString strVal;
	GetDataItem(lpszItemName, strVal);
	nItemVal = atoi(strVal);
}

void CUploadDlg::GetDataItem(LPCSTR lpszItemName, bool &bItemVal)
{
	CString strVal;
	GetDataItem(lpszItemName, strVal);
	bItemVal = strVal == "1";
}

void CUploadDlg::SetSocket(CIrrigationSocket *pSocket)
{
	m_pSocket = pSocket;
}

void CUploadDlg::SetIPAddr(LPCSTR lpszIPAddr, const UINT nPort)
{
	m_strIPAddr = lpszIPAddr;
	m_nPort = nPort;
}

