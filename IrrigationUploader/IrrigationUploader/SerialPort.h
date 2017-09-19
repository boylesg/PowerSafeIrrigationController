// Serial.h

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__




class CCOMPort
{
	public:
		CCOMPort();
		CCOMPort(LPCSTR lpszName, LPCSTR lpszCOM);
		virtual ~CCOMPort();
		CCOMPort& operator =(CCOMPort& rCOMPort);
		bool operator ==(CCOMPort& rCOMPort);
		bool operator !=(CCOMPort& rCOMPort);
		bool IsEmpty();
		void Empty();
		void Set(LPCSTR lpszName, LPCSTR lpszCOM);

		CString m_strCOM, m_strDesc, m_strName;
};

typedef CArray<CCOMPort, CCOMPort&> CCOMPortArray;




class CSerialPort: public CObject
{
	DECLARE_DYNAMIC(CSerialPort);

	protected:
		HANDLE m_commHandle;
		static UINT m_nTimeout;

		static bool isIrrigationController(CSerialPort &SerialCOMPort);
		CCOMPort m_COMPort;

		bool Close(HANDLE &commHandle);
		bool InitPort(HANDLE &commHandle, LPCSTR lpszCOMPort, const int nBaudRate);
		HANDLE OpenHandle(LPCSTR lpszCOMPortNum, const int nBaudRate);
		bool IsValid(HANDLE commHandle);

	public:

		CSerialPort();
    
		CSerialPort(CCOMPort COMPort, const int nBaudRate = 9600)
		{
			Open(COMPort, nBaudRate);
		};

		CSerialPort(LPCSTR lpszCOMPort, const int nBaudRate = 9600)
		{
			Open(lpszCOMPort, nBaudRate);
		};

		virtual ~CSerialPort();
    
		operator bool()
		{
			return (m_commHandle != INVALID_HANDLE_VALUE);
		};

		static void GetCOMPorts(CCOMPortArray &arrayCOMPorts, CWnd *pWnd);

		bool Close();
		bool Open(LPCSTR lpszCOMPort, const int nBaudRate = 9600);
		bool Open(CCOMPort COMPort, const int nBaudRate = 9600);

		UINT Write(LPCSTR lpszBuffer);
		UINT Read(CString &rstrText);

		bool Send(LPCSTR lpszData);
		bool Receive(CString &strData);

		bool Request(LPCSTR lpszDataItemName, CString &strDataItem);
		bool Request(LPCSTR lpszDataItemName, UINT &nDataItem);
		bool Request(LPCSTR lpszDataItemName, DWORD &nDataItem);
		bool Request(LPCSTR lpszDataItemName, bool &bDataItem);

		bool Request(LPCSTR lpszDataItemName, const byte nI, CString &strDataItem);
		bool Request(LPCSTR lpszDataItemName, const byte nI, int &nDataItem);
		bool Request(LPCSTR lpszDataItemName, const byte nI, bool &bDataItem);

		LPCSTR GetDelim()
		{
			return m_lpszDelim;
		};

		void Flush();

	protected:
		// Data
		DWORD m_nTimeoutTrigger;
		static const DWORD m_dwWait;
		static LPCSTR m_lpszDelim;
};

#endif
