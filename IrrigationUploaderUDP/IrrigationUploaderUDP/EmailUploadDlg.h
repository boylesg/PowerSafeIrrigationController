#pragma once
#include "afxwin.h"
#include "UploadDlg.h"


// CEmailUploadDlg dialog

class CEmailUploadDlg : public CUploadDlg
{
	DECLARE_DYNAMIC(CEmailUploadDlg)

	DECLARE_MESSAGE_MAP()

	public:
		CEmailUploadDlg(CWnd* pParent = NULL);   // standard constructor
		virtual ~CEmailUploadDlg();

		// Dialog Data
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_DIALOG_EMAIL };
	#endif

		// Interface
		virtual void EnableControls(const bool bEnable);
		virtual void InitFields();

	protected:

		// Overrides
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual BOOL OnInitDialog();

		// Helpers
		void EnableUploadButton();

		// Controls
		CButton m_buttonUpload;
		CStatic m_staticStatus;
		CEdit m_editEmailAddress;
		CEdit m_editMailServer;
		CEdit m_editPassword;
		CEdit m_editUsername;

		// Generated message map functions
		afx_msg void OnChangeEditUsername();
		afx_msg void OnChangeEditPassword();
		afx_msg void OnChangeEditMailServer();
		afx_msg void OnChangeEditEmailAddress();
		afx_msg void OnClickedButtonUpload();
};
