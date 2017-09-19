
// IrrigationUploaderDlg.h : header file
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
#include "SerialPort.h"


// CIrrigationUploaderDlg dialog
class CIrrigationUploaderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIrrigationUploaderDlg)

	DECLARE_MESSAGE_MAP()

	public:
		// Construction
		CIrrigationUploaderDlg(CWnd* pParent = NULL);	// standard constructor

		// Dialog Data
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_IRRIGATIONUPLOADER_DIALOG };
#endif
	
	protected:
		// Data exchange and validation
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

		// Helpers
		void PositionChildWindows();
		void GetCOMPorts();
		void CenterChildWindow(CWnd* pParentWnd, CWnd* pChildWnd);
		void ChangeTab(const int nSelTab);

		// Implementation
		HICON m_hIcon;
		CCOMPortArray m_arrayCOMPorts;
		CCOMPort m_COMPort;

		// Controls
		CComboBoxEx m_comboboxCOMPort;
		CTabCtrl m_tabActivity;

		// Child windows
		CFileUploadDlg m_dlgFileUpload;
		CWifiUploadDlg m_dlgWifiUpload;
		CEmailUploadDlg m_dlgEmailUpload;
		CTimeUploadDlg m_dlgTimeUpload;
		CProgramUploadDlg m_dlgProgramUpload;
		CAlarmsUploadDlg m_dlgAlarms;
		CDownloadFileDialog m_dlgFileDownload;
		CStatic m_staticLoading;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		afx_msg void OnSelchangeComboboxComPort();
		afx_msg void OnSelchangeTabActivity(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnClickedButtonRefresh();
};
