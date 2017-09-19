#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "UploadDlg.h"


// CFileUploadDlg dialog
class CFileUploadDlg : public CUploadDlg
{
	DECLARE_DYNAMIC(CFileUploadDlg)

	public:
		// Construction
		CFileUploadDlg(CWnd* pParent = NULL);	// standard constructor

		// Interface
		virtual void EnableControls(const bool bEnable);
		virtual void InitFields(){};

		// Dialog Data
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_IRRIGATIONUPLOADER_DIALOG };
#endif
	
	protected:
		// Data exchange and validation
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

		// Implementation
		HICON m_hIcon;

		// Controls
		CMFCEditBrowseCtrl m_browseFile;
		CButton m_buttonUpload;
		CEdit m_editContents;
		CButton m_buttonSave;
		CButton m_buttonDiscard;
		CComboBox m_comboBlank;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnSelchangeComboboxComPort();
		afx_msg void OnChangeMfceditbrowseFile();
		afx_msg void OnClickedButtonSave();
		afx_msg void OnClickedButtonDiscard();
		afx_msg void OnUpdateMfceditbrowseFile();
		afx_msg void OnSelchangeComboBlank();
		afx_msg void OnEditchangeComboBlank();
		afx_msg void OnClickedButtonCreate();
		afx_msg void OnButtonUpload();
		DECLARE_MESSAGE_MAP()
};
