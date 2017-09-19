#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"
#include "UploadDlg.h"


class CStationType
{
	public:
		CStationType();
		~CStationType()
		{
		};
		int GetStartMinute();
		int GetStartHour();
		int GetEndHour();
		int m_nRadioFreq, m_nSelFreq, m_nRunFreq;
		CString m_strDescription, m_strRunTime, m_strDateStart, m_strDateEnd, m_strTimeStart, m_strTimeEnd;
		CStringArray m_arrayTimes, m_arrayDates;
};




// CProgramUploadDlg dialog

class CProgramUploadDlg : public CUploadDlg
{
	DECLARE_DYNAMIC(CProgramUploadDlg)

	DECLARE_MESSAGE_MAP()

	public:
		// Construction & destruction
		CProgramUploadDlg(CWnd* pParent = NULL);   // standard constructor
		virtual ~CProgramUploadDlg();

		// Dialog Data
		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_DIALOG_PROGRAMS };
		#endif

		// Interface
		virtual void EnableControls(const bool bEnable);
		virtual void InitFields();

	protected:
		// Overrides
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual BOOL OnInitDialog();

		// Helpers
		int GetMaxDay(const int nMonth, const int nYear);
		bool IsLeapYear(const int nYear);
		int GetDeltaDay();
		int GetDeltaDay(const int nRadioFreq, const int nSelFreq);
		int GetRadioDateFreq();
		void SetRadioFreq(const int nRadioFreq);
		void SetSelectionFreq(const int nRadioFreq, const int nSelFreq);
		int GetSelectionFreq(const int nRadioFreq);
		int GetCurrStation();
		void RestoreStationSettings(const int nStation);
		void GetListContents(CStringArray& arrayList, CListBox& list);
		void SetListContents(CStringArray& arrayList, CListBox& list);
		void BuildRuntimeList(CStringArray& arrayTimes, int nStartHour, int nEndHour, const int nStartMinute, int nFrequency, LPCSTR lpszRuntime);
		void BuildDateList(CStringArray& arrayDates, const int nDayFreq);
		void SetDate(CString& strDate, CDateTimeCtrl& m_dateCtrl);
		void SetTime(CString& strTime, CDateTimeCtrl& m_timeCtrl);
		LPCSTR GetTimeStr(CTime &time);
		LPCSTR GetDateStr(CTime &time);
		void UpdateLists();
		void EnableUploadButton();
		void EnableFrequencyCombos();
		void EnableControls(const bool bEnable, const int nRadioFreq);

		// Generated message map functions
		afx_msg void OnClickedRadioMonthly();
		afx_msg void OnClickedRadioWeekly();
		afx_msg void OnClickedRadioDaily();
		afx_msg void OnClickedRadioStation1();
		afx_msg void OnClickedRadioStation2();
		afx_msg void OnClickedRadioStation3();
		afx_msg void OnClickedRadioStation4();
		afx_msg void OnClickedRadioStation5();
		afx_msg void OnClickedRadioStation6();
		afx_msg void OnClickedRadioStation7();
		afx_msg void OnClickedRadioStation8();
		afx_msg void OnChangeEditRunTime();
		afx_msg void OnClickedButtonBuildRuntimesList();
		afx_msg void OnClickedButtonStationOff();
		afx_msg void OnClickedButtonUpload();
		afx_msg void OnDatetimechangeTimeEnd(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDatetimechangeTimeStart(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDatetimechangeDateEnd(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDatetimechangeDateStart(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnSelchangeComboFrequency();
		afx_msg void OnSelchangeComboMonthly();
		afx_msg void OnSelchangeComboWeekly();
		afx_msg void OnChangeEditDescription();
		afx_msg void OnSelchangeComboDaily();

		// Controls
		CButton m_buttonUpload;
		CButton m_buttonStationOff;
		CComboBox m_comboDaily;
		CComboBox m_comboFrequency;
		CComboBox m_comboMonthly;
		CComboBox m_comboWeekly;
		CDateTimeCtrl m_datectrlEnd;
		CDateTimeCtrl m_datectrlStart;
		CEdit m_editRuntime;
		CEdit m_editDescription;
		CListBox m_listDates;
		CListBox m_listRuntimes;
		CButton m_radioMonthly;
		CButton m_radioWeekly;
		CButton m_radioDaily;
		CButton m_radioStation1;
		CButton m_radioStation2;
		CButton m_radioStation3;
		CButton m_radioStation4;
		CButton m_radioStation5;
		CButton m_radioStation6;
		CButton m_radioStation7;
		CButton m_radioStation8;
		CSpinButtonCtrl m_spinRuntime;
		CDateTimeCtrl m_timectrlEnd;
		CDateTimeCtrl m_timectrlStart;
		CStatic m_staticAnd;
		CStatic m_static_Between;
		CStatic m_staticEndDate;
		CStatic m_staticEvery;
		CStatic m_staticFor;
		CStatic m_staticMinutes;
		CStatic m_staticStartDate;
		CStatic m_staticStatus;

		// Data
		CString m_strStartTimes;
		int m_nLastStation;
		CStationType m_arrayStationSettings[8];
		bool m_bEnableChangeRuntime;
};
