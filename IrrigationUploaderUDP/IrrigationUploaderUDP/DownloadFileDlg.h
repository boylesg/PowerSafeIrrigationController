#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "UploadDlg.h"


// CDownloadFileDialog dialog

class CDownloadFileDialog : public CUploadDlg
{
	DECLARE_DYNAMIC(CDownloadFileDialog)
	DECLARE_MESSAGE_MAP()

	public:
		// Construction & destruction
		CDownloadFileDialog(CWnd* pParent = NULL);   // standard constructor
		virtual ~CDownloadFileDialog();

		// Dialog Data
		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_DIALOG_DOWNLOAD_FILE };
		#endif

		// Interface
		virtual void EnableControls(const bool bEmpty);
		virtual void InitFields(){};

		// Message map functions
		afx_msg void OnSelchangeComboFilename();
		afx_msg void OnClickedButtonDownload();

	protected:
		// Overrides
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual BOOL OnInitDialog();

		// Controls
		CButton m_buttonDownload;
		CComboBox m_comboFilename;
		CEdit m_editFileContents;
		CStatic m_staticContents;
		CStatic m_staticFile;
		CStatic m_staticStatus;
};
