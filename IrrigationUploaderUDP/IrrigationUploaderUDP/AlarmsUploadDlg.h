#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "UploadDlg.h"


class CStationAlarmDetails
{
	public:
		CStationAlarmDetails();

		bool m_bInstalled, m_bAuto;
		int m_nDryTime, m_nOnTime, m_nThreshold;
		CString m_strDescription;
};

// CAlarmsUploadDlg dialog

class CAlarmsUploadDlg : public CUploadDlg
{
	DECLARE_DYNAMIC(CAlarmsUploadDlg)

	DECLARE_MESSAGE_MAP()

	public:
		// Construction & destruction
		CAlarmsUploadDlg(CWnd* pParent = NULL);   // standard constructor
		virtual ~CAlarmsUploadDlg();

		// Dialog Data
		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_DIALOG_WALARMS };
		#endif

		// Interface
		virtual void EnableControls(const bool bEnable);
		virtual void InitFields();

	protected:

		// Helpers
		void SaveAlarmDetails(const int nStation);
		void RestoreAlarmDetails(const int nStation);
		bool ValidateFields(CString& strMsg);
		int GetStation();
		void SetStation(const int nStation);
		void CAlarmsUploadDlg::EnableDryThresh(const bool bEnable);
		void CAlarmsUploadDlg::EnableOn(const bool bEnable);

		// Overrides
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual BOOL OnInitDialog();

		// Generated message maps
		afx_msg void OnClickedCheckAuto();
		afx_msg void OnClickedCheckInstalled();
		afx_msg void OnClickedRadioStation1();
		afx_msg void OnClickedRadioStation2();
		afx_msg void OnClickedRadioStation3();
		afx_msg void OnClickedRadioStation4();
		afx_msg void OnClickedRadioStation5();
		afx_msg void OnClickedRadioStation6();
		afx_msg void OnClickedRadioStation7();
		afx_msg void OnClickedRadioStation8();
		afx_msg void OnClickedButtonUpload();
		afx_msg void OnDeltaposSpinOntime(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDeltaposSpinProbeThreshold(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDeltaposSpinDrytime(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnChangeEditDrytime();
		afx_msg void OnChangeEditOntime();
		afx_msg void OnChangeEditProbeThreshold();

		// Controls
		CButton m_checkAuto;
		CButton m_checkInstalled;
		CEdit m_editDesc;
		CEdit m_editDrytime;
		CEdit m_editOnTime;
		CEdit m_editThresh;
		CSpinButtonCtrl m_spinDryTime;
		CSpinButtonCtrl m_spinOnTime;
		CSpinButtonCtrl m_spinThresh;
		CButton m_radioStation1;
		CButton m_radioStation2;
		CButton m_radioStation3;
		CButton m_radioStation4;
		CButton m_radioStation5;
		CButton m_radioStation6;
		CButton m_radioStation7;
		CButton m_radioStation8;
		CStatic m_staticStatus;
		CButton m_buttonUpload;

		// Data
		CStationAlarmDetails m_arrayAlarmDetails[8];
		int m_nLastStation;
};
