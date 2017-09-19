// ProgramUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploader.h"
#include "ProgramUploadDlg.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"


CStationType::CStationType()
{
	m_nRadioFreq = m_nSelFreq = m_nRunFreq = 0;
}

int CStationType::GetStartMinute()
{
	int nPos = m_strTimeStart.Find(":");
	CString strMin = m_strTimeStart.Right(m_strTimeStart.GetLength() - nPos - 1);
	return atoi(strMin);
}

int CStationType::GetStartHour()
{
	int nPos = m_strTimeStart.Find(":");
	CString strHour = m_strTimeStart.Left(nPos);
	return atoi(strHour);
}

int CStationType::GetEndHour()
{
	int nPos = m_strTimeEnd.Find(":");
	CString strHour = m_strTimeEnd.Left(nPos);
	return atoi(strHour);
}

int m_nRadioFreq, m_nSelFreq, m_nRunFreq;
CString m_strDescription, m_strRunTime, m_strDateStart, m_strDateEnd, m_strTimeStart, m_strTimeEnd;





// CProgramUploadDlg dialog

IMPLEMENT_DYNAMIC(CProgramUploadDlg, CDialogEx)

CProgramUploadDlg::CProgramUploadDlg(CWnd* pParent /*=NULL*/): CDialogEx(IDD_DIALOG_PROGRAMS, pParent)
{
	m_nLastStation = 0;
	m_bEnableChangeRuntime = true;
}

CProgramUploadDlg::~CProgramUploadDlg()
{
}

void CProgramUploadDlg::EnableFrequencyCombos()
{
	if (m_radioMonthly.GetCheck())
	{
		m_comboMonthly.EnableWindow(true);
		m_comboWeekly.EnableWindow(false);
		m_comboDaily.EnableWindow(false);
	}
	else if (m_radioWeekly.GetCheck())
	{
		m_comboMonthly.EnableWindow(false);
		m_comboWeekly.EnableWindow(true);
		m_comboDaily.EnableWindow(false);
	}
	else if (m_radioDaily.GetCheck())
	{
		m_comboMonthly.EnableWindow(false);
		m_comboWeekly.EnableWindow(false);
		m_comboDaily.EnableWindow(true);
	}
}

void CProgramUploadDlg::EnableControls(const bool bEnable, const int nRadioFreq)
{
	m_radioStation1.EnableWindow(bEnable);
	m_radioStation2.EnableWindow(bEnable);
	m_radioStation3.EnableWindow(bEnable);
	m_radioStation4.EnableWindow(bEnable);
	m_radioStation5.EnableWindow(bEnable);
	m_radioStation6.EnableWindow(bEnable);
	m_radioStation7.EnableWindow(bEnable);
	m_radioStation8.EnableWindow(bEnable);
	m_staticStartDate.EnableWindow(bEnable);
	m_datectrlStart.EnableWindow(bEnable);
	m_staticEndDate.EnableWindow(bEnable);
	m_datectrlEnd.EnableWindow(bEnable);
	m_radioMonthly.EnableWindow(bEnable);
	m_radioDaily.EnableWindow(bEnable);
	m_radioWeekly.EnableWindow(bEnable);
	m_static_Between.EnableWindow(bEnable);
	m_staticAnd.EnableWindow(bEnable);
	m_staticEvery.EnableWindow(bEnable);
	m_staticFor.EnableWindow(bEnable);
	m_staticMinutes.EnableWindow(bEnable);
	m_timectrlStart.EnableWindow(bEnable);
	m_timectrlEnd.EnableWindow(bEnable);
	m_comboFrequency.EnableWindow(bEnable);
	m_editRuntime.EnableWindow(bEnable);
	m_editDescription.EnableWindow(bEnable);
	m_spinRuntime.EnableWindow(bEnable);
	m_listDates.EnableWindow(bEnable);
	m_listRuntimes.EnableWindow(bEnable);
	m_buttonStationOff.EnableWindow(bEnable);
	SetRadioFreq(nRadioFreq);
	EnableFrequencyCombos();
	EnableUploadButton();
}

void CProgramUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, m_buttonUpload);
	DDX_Control(pDX, IDC_BUTTON_STATION_OFF, m_buttonStationOff);
	DDX_Control(pDX, IDC_COMBO_DAILY, m_comboDaily);
	DDX_Control(pDX, IDC_COMBO_FREQUENCY, m_comboFrequency);
	DDX_Control(pDX, IDC_COMBO_MONTHLY, m_comboMonthly);
	DDX_Control(pDX, IDC_COMBO_WEEKLY, m_comboWeekly);
	DDX_Control(pDX, IDC_DATE_END, m_datectrlEnd);
	DDX_Control(pDX, IDC_DATE_START, m_datectrlStart);
	DDX_Control(pDX, IDC_EDIT_RUN_TIME, m_editRuntime);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_LIST_DATES, m_listDates);
	DDX_Control(pDX, IDC_LIST_RUNTIMES, m_listRuntimes);
	DDX_Control(pDX, IDC_RADIO_MONTHLY, m_radioMonthly);
	DDX_Control(pDX, IDC_RADIO_WEEKLY, m_radioWeekly);
	DDX_Control(pDX, IDC_RADIO_DAILY, m_radioDaily);
	DDX_Control(pDX, IDC_RADIO_STATION1, m_radioStation1);
	DDX_Control(pDX, IDC_RADIO_STATION2, m_radioStation2);
	DDX_Control(pDX, IDC_RADIO_STATION3, m_radioStation3);
	DDX_Control(pDX, IDC_RADIO_STATION4, m_radioStation4);
	DDX_Control(pDX, IDC_RADIO_STATION5, m_radioStation5);
	DDX_Control(pDX, IDC_RADIO_STATION6, m_radioStation6);
	DDX_Control(pDX, IDC_RADIO_STATION7, m_radioStation7);
	DDX_Control(pDX, IDC_RADIO_STATION8, m_radioStation8);
	DDX_Control(pDX, IDC_SPIN_RUN_TIME, m_spinRuntime);
	DDX_Control(pDX, IDC_TIME_END, m_timectrlEnd);
	DDX_Control(pDX, IDC_TIME_START, m_timectrlStart);
	DDX_Control(pDX, IDC_STATIC_AND, m_staticAnd);
	DDX_Control(pDX, IDC_STATIC_BETWEEN, m_static_Between);
	DDX_Control(pDX, IDC_STATIC_END_DATE, m_staticEndDate);
	DDX_Control(pDX, IDC_STATIC_EVERY, m_staticEvery);
	DDX_Control(pDX, IDC_STATIC_FOR, m_staticFor);
	DDX_Control(pDX, IDC_STATIC_MINUTES, m_staticMinutes);
	DDX_Control(pDX, IDC_STATIC_START_DATE, m_staticStartDate);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
}


void CProgramUploadDlg::InitFields(CCOMPort COMPort)
{
	CSerialPort SerialPort;
	CProgressDlg dlg(GetParent());
	CString str, strStartTime, strEndTime, strStation;
	int nProgress = 0, nI = 0, nStartHour = 0, nEndHour = 0, nStartMinute = 0, nPos = 0;

	if (m_COMPort != COMPort)
	{
		m_COMPort = COMPort;
		if (SerialPort.Open(m_COMPort))
		{
			for (nI = 0; nI < 8; nI++)
			{
				strStation.Format("%d", nI + 1);
				dlg.SetProgress(nProgress, "Fetching current station " + strStation + " settings...");

				SerialPort.Request("radfrq", nI + 1, m_arrayStationSettings[nI].m_nRadioFreq);
				nProgress += 2;
				dlg.SetProgress(nProgress);

				SerialPort.Request("selfrq", nI + 1, m_arrayStationSettings[nI].m_nSelFreq);
				nProgress += 1;
				dlg.SetProgress(nProgress);

				SerialPort.Request("runfrq", nI + 1, m_arrayStationSettings[nI].m_nRunFreq);
				nProgress += 2;
				dlg.SetProgress(nProgress);

				SerialPort.Request("runtme", nI + 1, m_arrayStationSettings[nI].m_strRunTime);
				nProgress += 1;
				dlg.SetProgress(nProgress);

				SerialPort.Request("stme", nI + 1, m_arrayStationSettings[nI].m_strTimeStart);
				nProgress += 2;
				dlg.SetProgress(nProgress);

				SerialPort.Request("etme", nI + 1, m_arrayStationSettings[nI].m_strTimeEnd);
				nProgress += 1;
				dlg.SetProgress(nProgress);

				SerialPort.Request("sdte", nI + 1, m_arrayStationSettings[nI].m_strDateStart);
				nProgress += 2;
				dlg.SetProgress(nProgress);

				SerialPort.Request("edte", nI + 1, m_arrayStationSettings[nI].m_strDateEnd);
				nProgress += 1;
				dlg.SetProgress(nProgress);

				SerialPort.Request("desc", nI + 1, m_arrayStationSettings[nI].m_strDescription);
				nProgress += 1;
				dlg.SetProgress(nProgress);
			}
			dlg.Close();
			for (nI = 0; nI < 8; nI++)
			{
				if (m_arrayStationSettings[nI].m_strRunTime != "0")
				{
					nPos = m_arrayStationSettings[nI].m_strTimeStart.Find(":");
					str = m_arrayStationSettings[nI].m_strTimeStart.Left(nPos);
					nStartHour = atoi(str);
					str = m_arrayStationSettings[nI].m_strTimeStart.Right(m_arrayStationSettings[nI].m_strDateStart.GetLength() - nPos - 1);
					nStartMinute = atoi(str);
					nPos = m_arrayStationSettings[nI].m_strTimeEnd.Find(":");
					str = m_arrayStationSettings[nI].m_strTimeEnd.Left(nPos);
					nEndHour = atoi(str);
					BuildRuntimeList(m_arrayStationSettings[nI].m_arrayTimes, nStartHour, nEndHour, nStartMinute, m_arrayStationSettings[nI].m_nRunFreq, m_arrayStationSettings[nI].m_strRunTime);
					BuildDateList(m_arrayStationSettings[nI].m_arrayDates, GetDeltaDay(m_arrayStationSettings[nI].m_nRadioFreq, m_arrayStationSettings[nI].m_nSelFreq));
				}
			}
			EnableControls(true, m_arrayStationSettings[0].m_nRadioFreq);
			RestoreStationSettings(0);
		}
	}
}

BEGIN_MESSAGE_MAP(CProgramUploadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_MONTHLY, &CProgramUploadDlg::OnClickedRadioMonthly)
	ON_BN_CLICKED(IDC_RADIO_DAILY, &CProgramUploadDlg::OnClickedRadioDaily)
	ON_BN_CLICKED(IDC_RADIO_WEEKLY, &CProgramUploadDlg::OnClickedRadioWeekly)
	ON_BN_CLICKED(IDC_RADIO_STATION1, &CProgramUploadDlg::OnClickedRadioStation1)
	ON_BN_CLICKED(IDC_RADIO_STATION2, &CProgramUploadDlg::OnClickedRadioStation2)
	ON_BN_CLICKED(IDC_RADIO_STATION3, &CProgramUploadDlg::OnClickedRadioStation3)
	ON_BN_CLICKED(IDC_RADIO_STATION4, &CProgramUploadDlg::OnClickedRadioStation4)
	ON_BN_CLICKED(IDC_RADIO_STATION5, &CProgramUploadDlg::OnClickedRadioStation5)
	ON_BN_CLICKED(IDC_RADIO_STATION6, &CProgramUploadDlg::OnClickedRadioStation6)
	ON_BN_CLICKED(IDC_RADIO_STATION7, &CProgramUploadDlg::OnClickedRadioStation7)
	ON_BN_CLICKED(IDC_RADIO_STATION8, &CProgramUploadDlg::OnClickedRadioStation8)
	ON_EN_CHANGE(IDC_EDIT_RUN_TIME, &CProgramUploadDlg::OnChangeEditRunTime)
	ON_BN_CLICKED(IDC_BUTTON_STATION_OFF, &CProgramUploadDlg::OnClickedButtonStationOff)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CProgramUploadDlg::OnClickedButtonUpload)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIME_END, &CProgramUploadDlg::OnDatetimechangeTimeEnd)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TIME_START, &CProgramUploadDlg::OnDatetimechangeTimeStart)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_END, &CProgramUploadDlg::OnDatetimechangeDateEnd)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_START, &CProgramUploadDlg::OnDatetimechangeDateStart)
	ON_CBN_SELCHANGE(IDC_COMBO_FREQUENCY, &CProgramUploadDlg::OnSelchangeComboFrequency)
	ON_CBN_SELCHANGE(IDC_COMBO_MONTHLY, &CProgramUploadDlg::OnSelchangeComboMonthly)
	ON_CBN_SELCHANGE(IDC_COMBO_WEEKLY, &CProgramUploadDlg::OnSelchangeComboWeekly)
	ON_EN_CHANGE(IDC_EDIT_DESCRIPTION, &CProgramUploadDlg::OnChangeEditDescription)
	ON_CBN_SELCHANGE(IDC_COMBO_DAILY, &CProgramUploadDlg::OnSelchangeComboDaily)
END_MESSAGE_MAP()


// CProgramUploadDlg message handlers


BOOL CProgramUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_spinRuntime.SetRange(0, 30);
	m_spinRuntime.SetPos(0);
	m_radioMonthly.SetCheck(true);
	m_radioStation1.SetCheck(true);
	m_editRuntime.SetWindowTextA("0");
	m_comboDaily.SetCurSel(0);
	m_comboFrequency.SetCurSel(0);
	m_comboMonthly.SetCurSel(0);
	m_comboWeekly.SetCurSel(0);
	CTime time(2000, 1, 1, 0, 0, 0);
	m_timectrlStart.SetTime(&time);
	m_timectrlStart.SetFormat("HH:mm:ss");
	m_timectrlEnd.SetTime(&time);
	m_timectrlEnd.SetFormat("HH:mm:ss");
	m_listDates.SetHorizontalExtent(640);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgramUploadDlg::OnClickedButtonStationOff()
{
	m_editRuntime.SetWindowTextA("0");
	m_arrayStationSettings[GetCurrStation()].m_strRunTime = "0";
	UpdateLists();
}

void CProgramUploadDlg::OnClickedRadioMonthly()
{
	EnableFrequencyCombos();
	m_arrayStationSettings[GetCurrStation()].m_nRadioFreq = 1;
	SetSelectionFreq(m_arrayStationSettings[GetCurrStation()].m_nRadioFreq, m_arrayStationSettings[GetCurrStation()].m_nSelFreq);
	OnSelchangeComboMonthly();
	UpdateLists();
}

void CProgramUploadDlg::OnClickedRadioWeekly()
{
	EnableFrequencyCombos();
	m_arrayStationSettings[GetCurrStation()].m_nRadioFreq = 2;
	SetSelectionFreq(m_arrayStationSettings[GetCurrStation()].m_nRadioFreq, m_arrayStationSettings[GetCurrStation()].m_nSelFreq);
	OnSelchangeComboWeekly();
	UpdateLists();
}

void CProgramUploadDlg::OnClickedRadioDaily()
{
	EnableFrequencyCombos();
	m_arrayStationSettings[GetCurrStation()].m_nRadioFreq = 3;
	SetSelectionFreq(m_arrayStationSettings[GetCurrStation()].m_nRadioFreq, m_arrayStationSettings[GetCurrStation()].m_nSelFreq);
	OnSelchangeComboDaily();
	UpdateLists();
}

void CProgramUploadDlg::OnChangeEditRunTime()
{
	CString str;
	int nVal = 0;

	if (m_spinRuntime.GetSafeHwnd() && m_bEnableChangeRuntime)
	{
		nVal = m_spinRuntime.GetPos();
		m_arrayStationSettings[GetCurrStation()].m_strRunTime.Format("%d", nVal);
		UpdateLists();
	}
}

void CProgramUploadDlg::BuildRuntimeList(CStringArray& arrayTimes, int nStartHour, int nEndHour, const int nStartMinute, int nFrequency, LPCSTR lpszRuntime)
{
	CString str, strLine, strRuntime = lpszRuntime;
	int nI = 0;

	arrayTimes.RemoveAll();
	m_strStartTimes.Empty();
	if (strRuntime != "0")
	{
		if (nStartHour == nEndHour)
			nEndHour += 24;

		switch (nFrequency)
		{
		case 0: nFrequency = 2; break;
		case 1: nFrequency = 3; break;
		case 2: nFrequency = 4; break;
		case 3: nFrequency = 6; break;
		case 4: nFrequency = 8; break;
		case 5: nFrequency = 12; break;
		case 6: nFrequency = 24; break;
		}
		if (nFrequency > 0)
		{
			nI = nStartHour;
			m_strStartTimes.Empty();
			do
			{
				if ((nI == nEndHour) && (nStartHour == (nEndHour % 24)))
					break;
				str.Format("%02d", nI >= 24 ? nI % 24 : nI);
				strLine = str + ":";
				str.Format("%02d", nStartMinute);
				strLine += str + "(" + strRuntime + ")";
				m_strStartTimes += strLine + ", ";
				arrayTimes.Add(strLine);
				nI += nFrequency;
			} while (nI <= nEndHour);
		}
		m_strStartTimes.Delete(m_strStartTimes.GetLength() - 2, 2);
	}
}

void CProgramUploadDlg::EnableUploadButton()
{
	bool bEnable = false;

	for (int nI = 0; nI < 8; nI++)
	{
		bEnable |= m_arrayStationSettings[nI].m_arrayDates.GetSize() > 0;
		if (bEnable)
			break;
	}
	m_buttonUpload.EnableWindow(bEnable && !m_COMPort.IsEmpty());
}

void CProgramUploadDlg::UpdateLists()
{
	int nStation = GetCurrStation(), nRadioFreq = GetRadioDateFreq(), nSelectFreq = GetSelectionFreq(nRadioFreq);
	CTime timeStart, timeEnd;
	CString strTimeStart, strTimeEnd, strRuntime;

	BuildRuntimeList(m_arrayStationSettings[nStation].m_arrayTimes, m_arrayStationSettings[nStation].GetStartHour(),
		m_arrayStationSettings[nStation].GetEndHour(), m_arrayStationSettings[nStation].GetStartMinute(),
		m_arrayStationSettings[nStation].m_nRunFreq, m_arrayStationSettings[nStation].m_strRunTime);
	SetListContents(m_arrayStationSettings[GetCurrStation()].m_arrayTimes, m_listRuntimes);
	BuildDateList(m_arrayStationSettings[GetCurrStation()].m_arrayDates, GetDeltaDay());
	SetListContents(m_arrayStationSettings[GetCurrStation()].m_arrayDates, m_listDates);
	EnableUploadButton();
}

bool CProgramUploadDlg::IsLeapYear(const int nYear)
{
	return (((nYear % 4) == 0) && ((nYear % 100) != 0)) || ((nYear % 400) == 0);
}

int CProgramUploadDlg::GetMaxDay(const int nMonth, const int nYear)
{
	int nMaxDay = 0;

	if (nMonth == 2)
	{
		if (IsLeapYear(nYear))
			nMaxDay = 29;
		else
			nMaxDay = 28;
	}
	else if ((nMonth == 4) || (nMonth == 6) || (nMonth == 9) || (nMonth == 11))
		nMaxDay = 30;
	else
		nMaxDay = 31;

	return nMaxDay;
}

int CProgramUploadDlg::GetDeltaDay(const int nRadioFreq, const int nSelFreq)
{
	int nDayFreq = 0;

	if (nRadioFreq == 1)
		nDayFreq = (nSelFreq + 1) * 30;
	else if (nRadioFreq == 2)
		nDayFreq = (nSelFreq + 1) * 7;
	else if (nRadioFreq == 3)
		nDayFreq = nSelFreq + 1;

	return nDayFreq;
}

int CProgramUploadDlg::GetDeltaDay()
{
	int nDayFreq = 0;

	if (m_radioMonthly.GetCheck())
		nDayFreq = GetDeltaDay(1, m_comboMonthly.GetCurSel());
	else if (m_radioWeekly.GetCheck())
		nDayFreq = GetDeltaDay(2, m_comboWeekly.GetCurSel());
	else if (m_radioDaily.GetCheck())
		nDayFreq = GetDeltaDay(3, m_comboDaily.GetCurSel());

	return nDayFreq;
}

void CProgramUploadDlg::BuildDateList(CStringArray& arrayDates, const int nDayFreq)
{
	int nDOM = 1, nMonth = 1, nMaxDay = 0;
	CString strLine, strNum;
	CTime time = CTime::GetCurrentTime();

	nMaxDay = GetMaxDay(nMonth, time.GetYear());
	arrayDates.RemoveAll();

	if ((nDayFreq > 0) && !m_strStartTimes.IsEmpty())
	{
		do
		{
			strNum.Format("%d", nDOM);
			strLine = strNum + "/";
			strNum.Format("%d", nMonth);
			strLine += strNum + " - " + m_strStartTimes;
			arrayDates.Add(strLine);
			nDOM += nDayFreq;
			if (nDOM > nMaxDay)
			{
				int nDiff = nDOM % nMaxDay;
				nMonth++;
				nMaxDay = GetMaxDay(nMonth, time.GetYear());
				nDOM = 1 + nDayFreq - nDiff;
			}
		} while (nMonth < 13);
	}
}

int CProgramUploadDlg::GetCurrStation()
{
	int nStation = 0;

	if (m_radioStation1.GetCheck())
		nStation = 0;
	else if (m_radioStation2.GetCheck())
		nStation = 1;
	else if (m_radioStation3.GetCheck())
		nStation = 2;
	else if (m_radioStation4.GetCheck())
		nStation = 3;
	else if (m_radioStation5.GetCheck())
		nStation = 4;
	else if (m_radioStation6.GetCheck())
		nStation = 5;
	else if (m_radioStation7.GetCheck())
		nStation = 6;
	else if (m_radioStation8.GetCheck())
		nStation = 7;

	return nStation;
}

int CProgramUploadDlg::GetRadioDateFreq()
{
	int nFreq = 0;

	if (m_radioMonthly.GetCheck())
		nFreq = 1;
	else if (m_radioWeekly.GetCheck())
		nFreq = 2;
	else if (m_radioDaily.GetCheck())
		nFreq = 3;

	return nFreq;
}

void CProgramUploadDlg::SetRadioFreq(const int nRadioFreq)
{
	if (nRadioFreq == 1)
	{
		m_radioMonthly.SetCheck(true);
		m_radioWeekly.SetCheck(false);
		m_radioDaily.SetCheck(false);
	}
	else if (nRadioFreq == 2)
	{
		m_radioMonthly.SetCheck(false);
		m_radioWeekly.SetCheck(true);
		m_radioDaily.SetCheck(false);
	}
	else if (nRadioFreq == 3)
	{
		m_radioMonthly.SetCheck(false);
		m_radioWeekly.SetCheck(false);
		m_radioDaily.SetCheck(true);
	}
}

void CProgramUploadDlg::GetListContents(CStringArray& arrayList, CListBox& list)
{
	CString str;

	arrayList.RemoveAll();
	for (int nI = 0; nI < list.GetCount(); nI++)
	{
		list.GetText(nI, str);
		arrayList.Add(str);
	}
}

void CProgramUploadDlg::SetListContents(CStringArray& arrayList, CListBox& list)
{
	while (list.GetCount() > 0)
		list.DeleteString(0);

	for (int nI = 0; nI < arrayList.GetCount(); nI++)
	{
		list.AddString(arrayList[nI]);
	}
}

int CProgramUploadDlg::GetSelectionFreq(const int nRadioFreq)
{
	int nFreq = 0;

	if (nRadioFreq == 0)
		nFreq = m_comboMonthly.GetCurSel();
	else if (nRadioFreq == 1)
		nFreq = m_comboWeekly.GetCurSel();
	else if (nRadioFreq == 2)
		nFreq = m_comboDaily.GetCurSel();

	return nFreq;
}

LPCSTR CProgramUploadDlg::GetTimeStr(CTime &time)
{
	static CString strTime;

	strTime.Format("%02d:%02d", time.GetHour(), time.GetMinute());

	return strTime;
}

LPCSTR CProgramUploadDlg::GetDateStr(CTime &time)
{
	static CString strDate;

	strDate.Format("%d/%d", time.GetDay(), time.GetMonth());

	return strDate;
}

void CProgramUploadDlg::SetSelectionFreq(const int nRadioFreq, const int nSelFreq)
{
	if (nRadioFreq == 1)
		m_comboMonthly.SetCurSel(nSelFreq);
	else if (nRadioFreq == 2)
		m_comboWeekly.SetCurSel(nSelFreq);
	else if (nRadioFreq == 3)
		m_comboDaily.SetCurSel(nSelFreq);
}

void CProgramUploadDlg::SetDate(CString& strDate, CDateTimeCtrl& dateCtrl)
{
	CTime timeCurrent = CTime::GetCurrentTime();
	int nPos = strDate.Find("/"), nDOM = 0, nMonth = 0;
	CString strNum;

	dateCtrl.SetTime(&timeCurrent);
	if (!strDate.IsEmpty())
	{
		if (nPos >= 0)
		{
			strNum = strDate.Left(nPos);
			nDOM = atoi(strNum);
			strNum = strDate.Right(strDate.GetLength() - nPos - 1);
			nMonth = atoi(strNum);
			CTime time(timeCurrent.GetYear(), nMonth, nDOM, timeCurrent.GetHour(), timeCurrent.GetMinute(), timeCurrent.GetSecond());
			dateCtrl.SetTime(&time);
		}
	}
}

void CProgramUploadDlg::SetTime(CString& strTime, CDateTimeCtrl& timeCtrl)
{
	int nPos = strTime.Find(":"), nHour = 0, nMinute = 0;
	CString strNum;
	CTime timeCurrent = CTime::GetCurrentTime();

	timeCtrl.SetTime(&timeCurrent);
	if (!strTime.IsEmpty())
	{
		if (nPos >= 0)
		{
			strNum = strTime.Left(nPos);
			nHour = atoi(strNum);
			strNum = strTime.Right(strTime.GetLength() - nPos - 1);
			nMinute = atoi(strNum);

			CTime time(timeCurrent.GetYear(), timeCurrent.GetMonth(), timeCurrent.GetDay(), nHour, nMinute, 0);
			timeCtrl.SetTime(&time);
		}
	}
}

void CProgramUploadDlg::RestoreStationSettings(const int nStation)
{
	ASSERT((nStation >= 0) && (nStation < 8));
	SetRadioFreq(m_arrayStationSettings[nStation].m_nRadioFreq);
	SetSelectionFreq(m_arrayStationSettings[nStation].m_nRadioFreq, m_arrayStationSettings[nStation].m_nSelFreq);
	m_comboFrequency.SetCurSel(m_arrayStationSettings[nStation].m_nRunFreq);
	EnableFrequencyCombos();

	SetDate(m_arrayStationSettings[nStation].m_strDateStart, m_datectrlStart);
	SetDate(m_arrayStationSettings[nStation].m_strDateEnd, m_datectrlEnd);
	SetTime(m_arrayStationSettings[nStation].m_strTimeStart, m_timectrlStart);
	SetTime(m_arrayStationSettings[nStation].m_strTimeEnd, m_timectrlEnd);

	m_bEnableChangeRuntime = false;
	m_editRuntime.SetWindowTextA(m_arrayStationSettings[nStation].m_strRunTime);
	m_bEnableChangeRuntime = true;
	m_editDescription.SetWindowTextA(m_arrayStationSettings[nStation].m_strDescription);
	SetListContents(m_arrayStationSettings[nStation].m_arrayTimes, m_listRuntimes);
	SetListContents(m_arrayStationSettings[nStation].m_arrayDates, m_listDates);

	int nCount = m_arrayStationSettings[nStation].m_arrayDates.GetCount();
	m_buttonUpload.EnableWindow(nCount > 0);
}

void CProgramUploadDlg::OnClickedRadioStation1()
{
	RestoreStationSettings(m_nLastStation = GetCurrStation());
}

void CProgramUploadDlg::OnClickedRadioStation2()
{
	RestoreStationSettings(m_nLastStation = GetCurrStation());
}

void CProgramUploadDlg::OnClickedRadioStation3()
{
	RestoreStationSettings(m_nLastStation = GetCurrStation());
}

void CProgramUploadDlg::OnClickedRadioStation4()
{
	RestoreStationSettings(m_nLastStation = GetCurrStation());
}

void CProgramUploadDlg::OnClickedRadioStation5()
{
	RestoreStationSettings(m_nLastStation = GetCurrStation());
}

void CProgramUploadDlg::OnClickedRadioStation6()
{
	RestoreStationSettings(m_nLastStation = GetCurrStation());
}

void CProgramUploadDlg::OnClickedRadioStation7()
{
	RestoreStationSettings(m_nLastStation = GetCurrStation());
}

void CProgramUploadDlg::OnClickedRadioStation8()
{
	RestoreStationSettings(m_nLastStation = GetCurrStation());
}

void CProgramUploadDlg::OnClickedButtonUpload()
{
	CSerialPort SerialPort;
	CProgressDlg dlg(GetParent());
	int nProgress = 0, nI = 0;
	CString strReq, strResp, strStation, str;
	bool bSuccess = false;

	m_staticStatus.SetWindowTextA("Uploading...");
	if (!m_COMPort.IsEmpty())
	{
		if (SerialPort.Open(m_COMPort))
		{
			strReq = CString("program") + SerialPort.GetDelim();
			for (nI = 0; nI < 8; nI++)
			{
				strStation.Format("station %d", nI + 1);
				str = CString("Uploading alarm details for ") + strStation + "!";
				dlg.SetProgress(nProgress, str);

				strReq.Format("%s%d%s", strReq, nI + 1, SerialPort.GetDelim());
				strReq.Format("%s%s%s", strReq, m_arrayStationSettings[nI].m_strDescription, SerialPort.GetDelim());
				strReq.Format("%s%s%s", strReq, m_arrayStationSettings[nI].m_strDateStart, SerialPort.GetDelim());
				strReq.Format("%s%s%s", strReq, m_arrayStationSettings[nI].m_strDateEnd, SerialPort.GetDelim());
				strReq.Format("%s%d%s", strReq, m_arrayStationSettings[nI].m_nRadioFreq, SerialPort.GetDelim());
				strReq.Format("%s%d%s", strReq, m_arrayStationSettings[nI].m_nSelFreq, SerialPort.GetDelim());
				strReq.Format("%s%d%s", strReq, m_arrayStationSettings[nI].m_nRunFreq, SerialPort.GetDelim());
				strReq.Format("%s%s%s", strReq, m_arrayStationSettings[nI].m_strRunTime, SerialPort.GetDelim());
				strReq.Format("%s%s%s", strReq, m_arrayStationSettings[nI].m_strTimeStart, SerialPort.GetDelim());
				strReq.Format("%s%s", strReq, m_arrayStationSettings[nI].m_strTimeEnd);
				if (SerialPort.Request(strReq, strResp))
				{
					nProgress += 12;
					dlg.SetProgress(nProgress, strResp);
					bSuccess = true;
					strReq.Empty();
				}
				else
				{
					str = CString("Upload of alarm details for ") + strStation + " was unsuccessful!";
					dlg.Close(nProgress, str);
					bSuccess = false;
					break;
				}
			}
			if (bSuccess)
			{
				m_staticStatus.SetWindowTextA("Upload was successful!");
				dlg.Close();
			}
		}
	}
}


void CProgramUploadDlg::OnDatetimechangeTimeEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	
	CTime time;
	m_timectrlEnd.GetTime(time);
	m_arrayStationSettings[GetCurrStation()].m_strTimeEnd = GetTimeStr(time);
	UpdateLists();

	*pResult = 0;
}


void CProgramUploadDlg::OnDatetimechangeTimeStart(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	CTime time;
	m_timectrlStart.GetTime(time);
	m_arrayStationSettings[GetCurrStation()].m_strTimeStart = GetTimeStr(time);
	UpdateLists();

	*pResult = 0;
}


void CProgramUploadDlg::OnDatetimechangeDateEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	CTime time;
	m_datectrlEnd.GetTime(time);
	m_arrayStationSettings[GetCurrStation()].m_strDateEnd = GetDateStr(time);
	UpdateLists();

	*pResult = 0;
}


void CProgramUploadDlg::OnDatetimechangeDateStart(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);

	CTime time;
	m_datectrlStart.GetTime(time);
	m_arrayStationSettings[GetCurrStation()].m_strDateStart = GetDateStr(time);
	UpdateLists();

	*pResult = 0;
}


void CProgramUploadDlg::OnSelchangeComboFrequency()
{
	m_arrayStationSettings[GetCurrStation()].m_nRunFreq = m_comboFrequency.GetCurSel();
	UpdateLists();
}


void CProgramUploadDlg::OnSelchangeComboMonthly()
{
	m_arrayStationSettings[GetCurrStation()].m_nSelFreq = m_comboMonthly.GetCurSel();
	UpdateLists();
}


void CProgramUploadDlg::OnSelchangeComboWeekly()
{
	m_arrayStationSettings[GetCurrStation()].m_nSelFreq = m_comboWeekly.GetCurSel();
	UpdateLists();
}


void CProgramUploadDlg::OnSelchangeComboDaily()
{
	m_arrayStationSettings[GetCurrStation()].m_nSelFreq = m_comboDaily.GetCurSel();
	UpdateLists();
}

void CProgramUploadDlg::OnChangeEditDescription()
{
	m_editDescription.GetWindowTextA(m_arrayStationSettings[GetCurrStation()].m_strDescription);
}
