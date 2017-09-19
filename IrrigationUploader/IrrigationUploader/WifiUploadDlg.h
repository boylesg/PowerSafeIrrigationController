#pragma once
#include "afxwin.h"
#include "SerialPort.h"




// CWifiUploadDlg dialog

class CWifiUploadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWifiUploadDlg)

	DECLARE_MESSAGE_MAP()

	public:
		CWifiUploadDlg(CWnd* pParent = NULL);   // standard constructor
		virtual ~CWifiUploadDlg();

		// Dialog Data
		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_DIALOG_WIFI };
		#endif

		// Interface
		void EnableControls(const bool bEnable);
		void SetCOMPort(CCOMPort COMPort)
		{
			m_COMPort = COMPort;
		};
		void InitEditFields(CCOMPort COMPort);

	protected:

		// Data exchange & validation
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		// Helpers
		void EnableUploadButton();

		// Controls
		CEdit m_editNetworkKey;
		CEdit m_editNetworkName;
		CButton m_buttonUpload;
		CStatic m_staticStatus;

		// Data
		CCOMPort m_COMPort;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnChangeEditWfiNetworkKey();
		afx_msg void OnChangeEditWfiNetworkName();
		afx_msg void OnClickedButtonUpload();

};
