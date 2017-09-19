#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "SerialPort.h"


// CTimeUploadDlg dialog

class CTimeUploadDlg : public CDialogEx
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
		CCOMPort m_COMPort;
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
