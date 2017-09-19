#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "UploadDlg.h"


// CTimeUploadDlg dialog

class CTimeUploadDlg : public CUploadDlg
{
	DECLARE_DYNAMIC(CTimeUploadDlg)

	DECLARE_MESSAGE_MAP()

	public:
		CTimeUploadDlg(CWnd* pParent = NULL);   // standard constructor
		virtual ~CTimeUploadDlg();

		// Dialog Data
		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_DIALOG_TIME };
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
		CButton m_buttonUpload;
		CStatic m_staticStatus;
		CDateTimeCtrl m_datetimeDate;
		CDateTimeCtrl m_datetimeTime;
		CEdit m_editCurrentTime;
		CEdit m_editID;
		CButton m_checkboxSaylightSavings;
		CSpinButtonCtrl m_spinTimezone;
		CEdit m_editTimezone;

		// Data
		CString m_strID;
		short m_nTimeZone;
		BOOL m_bDaylightSavings;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnClickedButtonUpload();
		afx_msg void OnDatetimechangeDatetimepickerDate(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDatetimechangeDatetimepickerTime(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnChangeEditTimezone();
		afx_msg void OnChangeEditId();
		afx_msg void OnDeltaposSpinTimezone(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnClickedCheckDaylightSavings();
};
