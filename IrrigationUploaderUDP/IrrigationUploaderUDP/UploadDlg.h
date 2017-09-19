#pragma once
#include "IrrigationSocket.h"

// CUploadDlg dialog

class CUploadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUploadDlg)

	public:
		CUploadDlg(const UINT nDlgResID, CWnd* pParent = NULL);   // standard constructor
		virtual ~CUploadDlg();

		// Interface
		void SetSocket(CIrrigationSocket *pSocket);
		void SetIPAddr(LPCSTR lpszIPAddr, const UINT nPort);
		virtual void InitFields() = 0;
		virtual void EnableControls(const bool bEnable) = 0;

	protected:

		// Helpers
		void GetDataItem(LPCSTR lpszItemName, const int nStation, CString &strItemVal);
		void GetDataItem(LPCSTR lpszItemName, const int nStation, int &nItemVal);
		void GetDataItem(LPCSTR lpszItemName, const int nStation, bool &bItemVal);
		void GetDataItem(LPCSTR lpszItemName, CString &strItemVal);
		void GetDataItem(LPCSTR lpszItemName, int &nItemVal);
		void GetDataItem(LPCSTR lpszItemName, bool &bItemVal);

		// Data
		UINT m_nPort;
		CIrrigationSocket *m_pSocket;
		CString m_strIPAddr;
};
