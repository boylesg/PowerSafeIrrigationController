
// IrrigationUploaderUDPDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "FileUploadDlg.h"
#include "WifiUploadDlg.h"
#include "EmailUploadDlg.h"
#include "TimeUploadDlg.h"
#include "ProgramUploadDlg.h"
#include "AlarmsUploadDlg.h"
#include "DownloadFileDlg.h"
#include "IrrigationSocket.h"


// CIrrigationUploaderUDPDlg dialog
class CIrrigationUploaderUDPDlg : public CDialogEx
{
	// Construction
	public:
		// Construction & destruction
		CIrrigationUploaderUDPDlg(CWnd* pParent = NULL);	// standard constructor

		// Dialog Data
		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_IRRIGATIONUPLOADERUDP_DIALOG };
		#endif

	protected:
		// Overrides
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

		// Helpers
		void PositionChildWindows();
		void CenterChildWindow(CWnd* pParentWnd, CWnd* pChildWnd);
		void ChangeTab(const int nSelTab);
		bool TryConnect(LPCSTR lpszIPAddr);

		// Child windows
		CFileUploadDlg m_dlgFileUpload;
		CWifiUploadDlg m_dlgWifiUpload;
		CEmailUploadDlg m_dlgEmailUpload;
		CTimeUploadDlg m_dlgTimeUpload;
		CProgramUploadDlg m_dlgProgramUpload;
		CAlarmsUploadDlg m_dlgAlarms;
		CDownloadFileDialog m_dlgFileDownload;

		// Data
		HICON m_hIcon;
		CIrrigationSocket m_Socket;
		CString m_strIPAddr;
		UINT m_nPort;
		bool m_bConnected;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		afx_msg void OnSelchangeTabActivity(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnClickedButtonConnect();
		afx_msg void OnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		DECLARE_MESSAGE_MAP()

		// Controls
		CButton m_buttonConnect;
		CStatic m_staticStatus;
		CTabCtrl m_tabActivity;
		CIPAddressCtrl m_ipaddrIP;
};
