// TimeUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploaderUDP.h"
#include "TimeUploadDlg.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"
#include "IrrigationSocket.h"

// CTimeUploadDlg dialog

IMPLEMENT_DYNAMIC(CTimeUploadDlg, CUploadDlg)

CTimeUploadDlg::CTimeUploadDlg(CWnd* pParent /*=NULL*/) : CUploadDlg(IDD_DIALOG_WIFI, pParent)
{
	m_nTimeZone = 0;
    m_bDaylightSavings = false;
}

CTimeUploadDlg::~CTimeUploadDlg()
{
}

void CTimeUploadDlg::InitFields()
{
	CString strDOW, strDOM, strMonth, strYear, strHour, strMinute, strSecond, strDaylightSavings, strID, strTimezone, strMsg;
	int nDaylightSavings = 0;
	CProgressDlg dlg(this);

	m_staticStatus.SetWindowTextA("Downloading current irrigation controller time...");
	if (!m_strIPAddr.IsEmpty() && (m_nPort > 0))
	{
		dlg.SetProgress(0, "Fetching current day of week...");
		m_pSocket->Request("dow", strDOW, m_strIPAddr, m_nPort);
		if (strDOW == "1")
			strDOW = "Sunday";
		else if (strDOW == "2")
			strDOW = "Monday";
		else if (strDOW == "3")
			strDOW = "Tuesday";
		else if (strDOW == "4")
			strDOW = "Wednesday";
		else if (strDOW == "5")
			strDOW = "Thursday";
		else if (strDOW == "6")
			strDOW = "Friday";
		else if (strDOW == "7")
			strDOW = "Saturday";
		dlg.SetProgress(10, "Fecthing current day of month...");
		GetDataItem("dom", strDOM);

		dlg.SetProgress(20, "Fecthing current month...");
		GetDataItem("month", strMonth);

		dlg.SetProgress(30, "Fecthing current year...");
		GetDataItem("year", strYear);

		dlg.SetProgress(40, "Fecthing current hour...");
		GetDataItem("hour", strHour);

		dlg.SetProgress(50, "Fecthing current minute...");
		GetDataItem("minute", strMinute);

		dlg.SetProgress(60, "Fecthing current second...");
		GetDataItem("second", strSecond);

		dlg.SetProgress(70, "Fecthing current daylight savings setting...");
		GetDataItem("daylightsavings", strDaylightSavings);

		dlg.SetProgress(80, "Fecthing current ID...");
		GetDataItem("id", strID);

		dlg.SetProgress(90, "Fecthing current timezone adjustment...");
		GetDataItem("timezone", strTimezone);
		dlg.Close();

		//CTime(int  nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1);
		CTime time;
		time = CTime::GetCurrentTime();

		m_datetimeDate.SetTime(&time);
		m_datetimeTime.SetTime(&time);
		m_editCurrentTime.SetWindowTextA(strDOW + " " + strDOM + "/" + strMonth + "/" + strYear + "  " + strHour + ":" + strMinute + ":" + strSecond);
		m_editID.SetWindowTextA(strID);
		m_editTimezone.SetWindowText(strTimezone);
		m_nTimeZone = atoi(strTimezone);
		m_checkboxSaylightSavings.SetCheck(atoi(strDaylightSavings) > 0);
		m_bDaylightSavings = atoi(strDaylightSavings) > 0;
		m_strID = strID;
		EnableControls(true);
		m_staticStatus.SetWindowTextA("Download successful...");
	}
}

void CTimeUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, m_buttonUpload);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATE, m_datetimeDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_TIME, m_datetimeTime);
	DDX_Control(pDX, IDC_EDIT_CURRENT_TIME, m_editCurrentTime);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_CHECK_DAYLIGHT_SAVINGS, m_checkboxSaylightSavings);
	DDX_Control(pDX, IDC_SPIN_TIMEZONE, m_spinTimezone);
	DDX_Control(pDX, IDC_EDIT_TIMEZONE, m_editTimezone);
}


BEGIN_MESSAGE_MAP(CTimeUploadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CTimeUploadDlg::OnClickedButtonUpload)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_DATE, &CTimeUploadDlg::OnDatetimechangeDatetimepickerDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_TIME, &CTimeUploadDlg::OnDatetimechangeDatetimepickerTime)
	ON_EN_CHANGE(IDC_EDIT_TIMEZONE, &CTimeUploadDlg::OnChangeEditTimezone)
	ON_EN_CHANGE(IDC_EDIT_ID, &CTimeUploadDlg::OnChangeEditId)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TIMEZONE, &CTimeUploadDlg::OnDeltaposSpinTimezone)
	ON_BN_CLICKED(IDC_CHECK_DAYLIGHT_SAVINGS, &CTimeUploadDlg::OnClickedCheckDaylightSavings)
END_MESSAGE_MAP()


// CTimeUploadDlg message handlers


void CTimeUploadDlg::EnableUploadButton()
{
	CString strDate, strTime, strID, strTimezone;

	m_datetimeDate.GetWindowText(strDate);
	m_datetimeTime.GetWindowText(strTime);
	m_editID.GetWindowText(strID);
	m_editTimezone.GetWindowText(strTimezone);

	m_buttonUpload.EnableWindow(!strDate.IsEmpty() && !strTime.IsEmpty() && !strID.IsEmpty() && !strTimezone.IsEmpty());
}

void CTimeUploadDlg::EnableControls(const bool bEnable)
{
	m_datetimeDate.EnableWindow(bEnable);
	m_datetimeTime.EnableWindow(bEnable);
	m_editID.EnableWindow(bEnable);
	m_checkboxSaylightSavings.EnableWindow(bEnable);
	m_editTimezone.EnableWindow(bEnable);
	m_spinTimezone.EnableWindow(bEnable);
}


BOOL CTimeUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_editID.SetLimitText(12);
	m_editTimezone.SetLimitText(3);
	ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTimeUploadDlg::OnClickedButtonUpload()
{
	CTime timeDateTime;
	CString strIReq, strResp, strID, strDOM, strMonth, strYear, strHour, strMinute, strSecond, strDaylightSavings, strTimezoneAdjust;
	bool bDaylightSavings = m_checkboxSaylightSavings.GetCheck() == TRUE;

	if (!m_strIPAddr.IsEmpty() && (m_nPort > 0))
	{
		m_datetimeDate.GetTime(timeDateTime);
		m_datetimeTime.GetTime(timeDateTime);
		m_editID.GetWindowTextA(strID);
		m_editTimezone.GetWindowTextA(strTimezoneAdjust);

		strDOM.Format("%d", timeDateTime.GetDay());
		strMonth.Format("%d", timeDateTime.GetMonth());
		strYear.Format("%d", timeDateTime.GetYear());
		strHour.Format("%d", timeDateTime.GetHour());
		strMinute.Format("%d", timeDateTime.GetMinute());
		strSecond.Format("%d", timeDateTime.GetSecond() + 1);
		strDaylightSavings.Format("%d", bDaylightSavings);

		m_staticStatus.SetWindowTextA("Uploading new system time to irrigation controller!");
		strIReq = CString("datetime") + m_pSocket->GetDelim() + strDOM + m_pSocket->GetDelim() + strMonth + m_pSocket->GetDelim() + strYear + m_pSocket->GetDelim() +
			strHour + m_pSocket->GetDelim() + strMinute + m_pSocket->GetDelim() + strSecond + m_pSocket->GetDelim() + strDaylightSavings + m_pSocket->GetDelim() +
			strTimezoneAdjust + m_pSocket->GetDelim() + strID;
		if (m_pSocket->Request(strIReq, strResp, m_strIPAddr, m_nPort))
			m_staticStatus.SetWindowTextA(strResp);
		else
			m_staticStatus.SetWindowTextA("Upload was unsuccessful!");
	}
}


void CTimeUploadDlg::OnDatetimechangeDatetimepickerDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	EnableUploadButton();
	*pResult = 0;
}


void CTimeUploadDlg::OnDatetimechangeDatetimepickerTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	EnableUploadButton();
	*pResult = 0;
}


void CTimeUploadDlg::OnChangeEditTimezone()
{
	EnableUploadButton();
	CString str;
	m_editTimezone.GetWindowTextA(str);
	m_nTimeZone = atoi(str);
}


void CTimeUploadDlg::OnChangeEditId()
{
	EnableUploadButton();
	m_editID.GetWindowTextA(m_strID);
}


void CTimeUploadDlg::OnDeltaposSpinTimezone(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_nTimeZone += -pNMUpDown->iDelta;
	if (m_nTimeZone < -12)
		m_nTimeZone = 12;
	else if (m_nTimeZone > 12)
		m_nTimeZone = -12;
	CString str;
	str.Format("%d", m_nTimeZone);
	m_editTimezone.SetWindowTextA(str);
	*pResult = 0;
}


void CTimeUploadDlg::OnClickedCheckDaylightSavings()
{
	m_bDaylightSavings = m_checkboxSaylightSavings.GetCheck();
}
