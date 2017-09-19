#pragma once
#include "afxwin.h"
#include "UploadDlg.h"


// CWifiUploadDlg dialog

class CWifiUploadDlg : public CUploadDlg
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
		virtual void EnableControls(const bool bEnable);
		virtual void InitFields();

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

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnChangeEditWfiNetworkKey();
		afx_msg void OnChangeEditWfiNetworkName();
		afx_msg void OnClickedButtonUpload();

};
