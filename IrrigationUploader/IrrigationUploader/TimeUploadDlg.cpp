// TimeUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploader.h"
#include "TimeUploadDlg.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"


// CTimeUploadDlg dialog

IMPLEMENT_DYNAMIC(CTimeUploadDlg, CDialogEx)

CTimeUploadDlg::CTimeUploadDlg(CWnd* pParent /*=NULL*/) : CDialogEx(IDD_DIALOG_WIFI, pParent)
{

	m_nTimeZone = 0;
    m_bDaylightSavings = false;
}

CTimeUploadDlg::~CTimeUploadDlg()
{
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


void CTimeUploadDlg::InitEditFields(CCOMPort COMPort)
{
	CSerialPort SerialPort;
	CString strDOW, strDOM, strMonth, strYear, strHour, strMinute, strSecond, strDaylightSavings, strID, strTimezone, strMsg;
	int nDaylightSavings = 0;
	CProgressDlg dlg(this);

	if (m_COMPort != COMPort)
	{
		m_COMPort = COMPort;
		m_staticStatus.SetWindowTextA("Downloading current irrigation controller time...");

		if (SerialPort.Open(m_COMPort))
		{
			dlg.SetProgress(0, "Fetching current day of week...");

			SerialPort.Request("dow", strDOW);
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
			SerialPort.Request("dom", strDOM);

			dlg.SetProgress(20, "Fecthing current month...");
			SerialPort.Request("month", strMonth);

			dlg.SetProgress(30, "Fecthing current year...");
			SerialPort.Request("year", strYear);

			dlg.SetProgress(40, "Fecthing current hour...");
			SerialPort.Request("hour", strHour);

			dlg.SetProgress(50, "Fecthing current minute...");
			SerialPort.Request("minute", strMinute);

			dlg.SetProgress(60, "Fecthing current second...");
			SerialPort.Request("second", strSecond);

			dlg.SetProgress(70, "Fecthing current daylight savings setting...");
			SerialPort.Request("daylightsavings", strDaylightSavings);

			dlg.SetProgress(80, "Fecthing current ID...");
			SerialPort.Request("id", strID);

			dlg.SetProgress(90, "Fecthing current timezone adjustment...");
			SerialPort.Request("timezone", strTimezone);
			dlg.Close();
			SerialPort.Close();

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
	CSerialPort SerialPort;
	CTime timeDateTime;
	CString strIReq, strResp, strID, strDOM, strMonth, strYear, strHour, strMinute, strSecond, strDaylightSavings, strTimezoneAdjust;
	bool bDaylightSavings = m_checkboxSaylightSavings.GetCheck() == TRUE;

	if (SerialPort.Open(m_COMPort))
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
		strIReq = CString("datetime") + SerialPort.GetDelim() + strDOM + SerialPort.GetDelim() + strMonth + SerialPort.GetDelim() + strYear + SerialPort.GetDelim() +
			strHour + SerialPort.GetDelim() + strMinute + SerialPort.GetDelim() + strSecond + SerialPort.GetDelim() + strDaylightSavings + SerialPort.GetDelim() +
			strTimezoneAdjust + SerialPort.GetDelim() + strID;
		if (SerialPort.Request(strIReq, strResp))
			m_staticStatus.SetWindowTextA(strResp);
		else
			m_staticStatus.SetWindowTextA("Upload was unsuccessful!");
		SerialPort.Close();
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
