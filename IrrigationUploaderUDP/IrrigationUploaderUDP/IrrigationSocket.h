#pragma once

// CIrrigationSocket command target
class CIrrigationSocket : public CSocket
{
	DECLARE_DYNAMIC(CIrrigationSocket);

	public:
		// Construction & destruction
		CIrrigationSocket();
		virtual ~CIrrigationSocket();

		// Interface
		bool Create();
		bool Send(LPCSTR lpszData, LPCSTR lpszIPAddr, const UINT nPort);
		bool Receive(CString &strData, CString &strIPAddr, UINT &nPort);

		bool Request(LPCSTR lpszDataItemName, CString &strDataItem, LPCSTR lpszIPAddr, const UINT nPort);
		bool Request(LPCSTR lpszDataItemName, UINT &nDataItem, LPCSTR lpszIPAddr, const UINT nPort);
		bool Request(LPCSTR lpszDataItemName, DWORD &nDataItem, LPCSTR lpszIPAddr, const UINT nPort);
		bool Request(LPCSTR lpszDataItemName, bool &bDataItem, LPCSTR lpszIPAddr, const UINT nPort);

		bool Request(LPCSTR lpszDataItemName, const byte nI, CString &strDataItem, LPCSTR lpszIPAddr, const UINT nPort);
		bool Request(LPCSTR lpszDataItemName, const byte nI, int &nDataItem, LPCSTR lpszIPAddr, const UINT nPort);
		bool Request(LPCSTR lpszDataItemName, const byte nI, bool &bDataItem, LPCSTR lpszIPAddr, const UINT nPort);
		LPCSTR GetDelim()
		{
			return m_lpszDelim;
		};

	protected:
		// Overrides
		virtual BOOL OnMessagePending();

		// Data
		DWORD m_nTimeoutTrigger;
		static const DWORD m_nWait;
		static LPCSTR m_lpszDelim;

		// Helpers
		void DisplayError(const UINT nError);

};
