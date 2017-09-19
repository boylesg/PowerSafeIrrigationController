// AlarmsUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploader.h"
#include "AlarmsUploadDlg.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"


CStationAlarmDetails::CStationAlarmDetails()
{
	m_bInstalled = m_bAuto = false;
	m_nDryTime = m_nOnTime = m_nThreshold = 0;
}





// CAlarmsUploadDlg dialog

IMPLEMENT_DYNAMIC(CAlarmsUploadDlg, CDialogEx)

CAlarmsUploadDlg::CAlarmsUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_WALARMS, pParent)
{
	m_nLastStation = 0;
}

CAlarmsUploadDlg::~CAlarmsUploadDlg()
{
}

void CAlarmsUploadDlg::SaveAlarmDetails(const int nStation)
{
	if ((nStation >= 0) && (nStation < 8))
	{
		m_arrayAlarmDetails[nStation].m_bInstalled = m_checkInstalled.GetCheck() == TRUE;
		m_arrayAlarmDetails[nStation].m_bAuto = m_checkAuto.GetCheck() == TRUE;
		m_arrayAlarmDetails[nStation].m_nDryTime = m_spinDryTime.GetPos();
		m_arrayAlarmDetails[nStation].m_nOnTime = m_spinOnTime.GetPos();
		m_arrayAlarmDetails[nStation].m_nThreshold = m_spinThresh.GetPos32();
	}
}

void CAlarmsUploadDlg::RestoreAlarmDetails(const int nStation)
{
	if ((nStation >= 0) && (nStation < 8))
	{
		CString str;

		m_editDesc.SetWindowTextA(m_arrayAlarmDetails[nStation].m_strDescription);
		m_checkInstalled.SetCheck(m_arrayAlarmDetails[nStation].m_bInstalled);
		m_checkAuto.SetCheck(m_arrayAlarmDetails[nStation].m_bAuto);
		m_spinDryTime.SetPos(m_arrayAlarmDetails[nStation].m_nDryTime);
		str.Format("%d", m_arrayAlarmDetails[nStation].m_nDryTime);
		m_editDrytime.SetWindowTextA(str);
		m_spinOnTime.SetPos(m_arrayAlarmDetails[nStation].m_nOnTime);
		str.Format("%d", m_arrayAlarmDetails[nStation].m_nOnTime);
		m_editOnTime.SetWindowTextA(str);
		m_spinThresh.SetPos(m_arrayAlarmDetails[nStation].m_nThreshold);
		str.Format("%d", m_arrayAlarmDetails[nStation].m_nThreshold);
		m_editThresh.SetWindowTextA(str);
		EnableDryThresh(m_arrayAlarmDetails[nStation].m_bInstalled ? TRUE : FALSE);
		EnableOn(m_arrayAlarmDetails[nStation].m_bAuto ? TRUE : FALSE);
	}
}

void CAlarmsUploadDlg::EnableDryThresh(const bool bEnable)
{
	m_editDrytime.EnableWindow(bEnable);
	m_editThresh.EnableWindow(bEnable);
	m_spinDryTime.EnableWindow(bEnable);
	m_spinThresh.EnableWindow(bEnable);
	m_checkAuto.EnableWindow(bEnable);
}

void CAlarmsUploadDlg::EnableOn(const bool bEnable)
{
	m_editOnTime.EnableWindow(bEnable);
	m_spinOnTime.EnableWindow(bEnable);
}

void CAlarmsUploadDlg::EnableControls(const bool bEnable)
{
	m_checkInstalled.EnableWindow(bEnable);
	m_editDesc.EnableWindow(bEnable);
	m_radioStation1.EnableWindow(bEnable);
	m_radioStation2.EnableWindow(bEnable);
	m_radioStation3.EnableWindow(bEnable);
	m_radioStation4.EnableWindow(bEnable);
	m_radioStation5.EnableWindow(bEnable);
	m_radioStation6.EnableWindow(bEnable);
	m_radioStation7.EnableWindow(bEnable);
	m_radioStation8.EnableWindow(bEnable);
}

void CAlarmsUploadDlg::InitFields(CCOMPort COMPort)
{
	CSerialPort SerialPort;
	CProgressDlg dlg(GetParent());
	int nI = 0, nPos = 0, nProgress = 0;
	CString strStation, str;

	if (m_COMPort != COMPort)
	{
		m_COMPort = COMPort;
		if (SerialPort.Open(m_COMPort))
		{
			for (nI = 0; nI < 8; nI++)
			{
				strStation.Format("%d", nI + 1);
				dlg.SetProgress(nProgress, "Fetching current alarm settings for station " + strStation + "...");

				SerialPort.Request("desc", nI + 1, m_arrayAlarmDetails[nI].m_strDescription);
				nProgress += 2;
				dlg.SetProgress(nProgress);

				SerialPort.Request("inst", nI + 1, m_arrayAlarmDetails[nI].m_bInstalled);
				nProgress += 2;
				dlg.SetProgress(nProgress);

				SerialPort.Request("auto", nI + 1, m_arrayAlarmDetails[nI].m_bAuto);
				nProgress += 2;
				dlg.SetProgress(nProgress);

				SerialPort.Request("dry", nI + 1, m_arrayAlarmDetails[nI].m_nDryTime);
				nProgress += 2;
				dlg.SetProgress(nProgress);

				SerialPort.Request("on", nI + 1, m_arrayAlarmDetails[nI].m_nOnTime);
				nProgress += 2;
				dlg.SetProgress(nProgress);

				SerialPort.Request("thresh", nI + 1, m_arrayAlarmDetails[nI].m_nThreshold);
				nProgress += 2;
				dlg.SetProgress(nProgress);
			}
			dlg.Close();
			m_nLastStation = 0;
			RestoreAlarmDetails(0);
			SetStation(0);
			EnableControls(true);
		}
	}
}

void CAlarmsUploadDlg::SetStation(const int nStation)
{
	switch (nStation)
	{
	case 0: m_radioStation1.SetCheck(true);
		m_radioStation2.SetCheck(false);
		m_radioStation3.SetCheck(false);
		m_radioStation4.SetCheck(false);
		m_radioStation5.SetCheck(false);
		m_radioStation6.SetCheck(false);
		m_radioStation7.SetCheck(false);
		m_radioStation8.SetCheck(false);
		break;
	case 1: m_radioStation1.SetCheck(false);
		m_radioStation2.SetCheck(true);
		m_radioStation3.SetCheck(false);
		m_radioStation4.SetCheck(false);
		m_radioStation5.SetCheck(false);
		m_radioStation6.SetCheck(false);
		m_radioStation7.SetCheck(false);
		m_radioStation8.SetCheck(false);
		break;
	case 2: m_radioStation1.SetCheck(false);
		m_radioStation2.SetCheck(false);
		m_radioStation3.SetCheck(true);
		m_radioStation4.SetCheck(false);
		m_radioStation5.SetCheck(false);
		m_radioStation6.SetCheck(false);
		m_radioStation7.SetCheck(false);
		m_radioStation8.SetCheck(false);
		break;
	case 3: m_radioStation1.SetCheck(false);
		m_radioStation2.SetCheck(false);
		m_radioStation3.SetCheck(false);
		m_radioStation4.SetCheck(true);
		m_radioStation5.SetCheck(false);
		m_radioStation6.SetCheck(false);
		m_radioStation7.SetCheck(false);
		m_radioStation8.SetCheck(false);
		break;
	case 4: m_radioStation1.SetCheck(false);
		m_radioStation2.SetCheck(false);
		m_radioStation3.SetCheck(false);
		m_radioStation4.SetCheck(false);
		m_radioStation5.SetCheck(true);
		m_radioStation6.SetCheck(false);
		m_radioStation7.SetCheck(false);
		m_radioStation8.SetCheck(false);
		break;
	case 5: m_radioStation1.SetCheck(false);
		m_radioStation2.SetCheck(false);
		m_radioStation3.SetCheck(false);
		m_radioStation4.SetCheck(false);
		m_radioStation5.SetCheck(false);
		m_radioStation6.SetCheck(true);
		m_radioStation7.SetCheck(false);
		m_radioStation8.SetCheck(false);
		break;
	case 6: m_radioStation1.SetCheck(false);
		m_radioStation2.SetCheck(false);
		m_radioStation3.SetCheck(false);
		m_radioStation4.SetCheck(false);
		m_radioStation5.SetCheck(false);
		m_radioStation6.SetCheck(false);
		m_radioStation7.SetCheck(true);
		m_radioStation8.SetCheck(false);
		break;
	case 7: m_radioStation1.SetCheck(false);
		m_radioStation2.SetCheck(false);
		m_radioStation3.SetCheck(false);
		m_radioStation4.SetCheck(false);
		m_radioStation5.SetCheck(false);
		m_radioStation6.SetCheck(false);
		m_radioStation7.SetCheck(false);
		m_radioStation8.SetCheck(true);
		break;
	}
}

int CAlarmsUploadDlg::GetStation()
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

void CAlarmsUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_AUTO, m_checkAuto);
	DDX_Control(pDX, IDC_CHECK_INSTALLED, m_checkInstalled);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDesc);
	DDX_Control(pDX, IDC_EDIT_DRYTIME, m_editDrytime);
	DDX_Control(pDX, IDC_EDIT_ONTIME, m_editOnTime);
	DDX_Control(pDX, IDC_EDIT_PROBE_THRESHOLD, m_editThresh);
	DDX_Control(pDX, IDC_SPIN_DRYTIME, m_spinDryTime);
	DDX_Control(pDX, IDC_SPIN_PROBE_THRESHOLD, m_spinThresh);
	DDX_Control(pDX, IDC_SPIN_ONTIME, m_spinOnTime);
	DDX_Control(pDX, IDC_RADIO_STATION1, m_radioStation1);
	DDX_Control(pDX, IDC_RADIO_STATION2, m_radioStation2);
	DDX_Control(pDX, IDC_RADIO_STATION3, m_radioStation3);
	DDX_Control(pDX, IDC_RADIO_STATION4, m_radioStation4);
	DDX_Control(pDX, IDC_RADIO_STATION5, m_radioStation5);
	DDX_Control(pDX, IDC_RADIO_STATION6, m_radioStation6);
	DDX_Control(pDX, IDC_RADIO_STATION7, m_radioStation7);
	DDX_Control(pDX, IDC_RADIO_STATION8, m_radioStation8);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, m_buttonUpload);
}


BEGIN_MESSAGE_MAP(CAlarmsUploadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_AUTO, &CAlarmsUploadDlg::OnClickedCheckAuto)
	ON_BN_CLICKED(IDC_CHECK_INSTALLED, &CAlarmsUploadDlg::OnClickedCheckInstalled)
	ON_BN_CLICKED(IDC_RADIO_STATION1, &CAlarmsUploadDlg::OnClickedRadioStation1)
	ON_BN_CLICKED(IDC_RADIO_STATION2, &CAlarmsUploadDlg::OnClickedRadioStation2)
	ON_BN_CLICKED(IDC_RADIO_STATION3, &CAlarmsUploadDlg::OnClickedRadioStation3)
	ON_BN_CLICKED(IDC_RADIO_STATION4, &CAlarmsUploadDlg::OnClickedRadioStation4)
	ON_BN_CLICKED(IDC_RADIO_STATION5, &CAlarmsUploadDlg::OnClickedRadioStation5)
	ON_BN_CLICKED(IDC_RADIO_STATION6, &CAlarmsUploadDlg::OnClickedRadioStation6)
	ON_BN_CLICKED(IDC_RADIO_STATION7, &CAlarmsUploadDlg::OnClickedRadioStation7)
	ON_BN_CLICKED(IDC_RADIO_STATION8, &CAlarmsUploadDlg::OnClickedRadioStation8)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CAlarmsUploadDlg::OnClickedButtonUpload)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ONTIME, &CAlarmsUploadDlg::OnDeltaposSpinOntime)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PROBE_THRESHOLD, &CAlarmsUploadDlg::OnDeltaposSpinProbeThreshold)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DRYTIME, &CAlarmsUploadDlg::OnDeltaposSpinDrytime)
	ON_EN_CHANGE(IDC_EDIT_DRYTIME, &CAlarmsUploadDlg::OnChangeEditDrytime)
	ON_EN_CHANGE(IDC_EDIT_ONTIME, &CAlarmsUploadDlg::OnChangeEditOntime)
	ON_EN_CHANGE(IDC_EDIT_PROBE_THRESHOLD, &CAlarmsUploadDlg::OnChangeEditProbeThreshold)
END_MESSAGE_MAP()


// CAlarmsUploadDlg message handlers


void CAlarmsUploadDlg::OnClickedCheckAuto()
{
	bool bEnable = m_checkAuto.GetCheck() == TRUE;
	EnableOn(bEnable);
	if (!bEnable)
	{
		m_spinOnTime.SetPos(0);
		SaveAlarmDetails(m_nLastStation);
	}
	else
	{
		m_arrayAlarmDetails[GetStation()].m_bAuto = bEnable == TRUE;
	}
}


void CAlarmsUploadDlg::OnClickedCheckInstalled()
{
	bool bEnable = m_checkInstalled.GetCheck() == TRUE;
	EnableDryThresh(bEnable);
	if (!bEnable)
	{
		m_spinDryTime.SetPos(0);
		m_spinThresh.SetPos(0);
		m_checkAuto.SetCheck(false);
		OnClickedCheckAuto();
		SaveAlarmDetails(m_nLastStation);
	}
	else
	{
		m_arrayAlarmDetails[GetStation()].m_bInstalled = bEnable == TRUE;
	}
	m_checkAuto.EnableWindow(bEnable);
}


BOOL CAlarmsUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_spinDryTime.SetRange(0, 120);
	m_spinDryTime.SetBuddy(&m_editDrytime);
	m_spinDryTime.SetPos(1);

	m_spinOnTime.SetRange(0, 30);
	m_spinOnTime.SetBuddy(&m_editOnTime);
	m_spinOnTime.SetPos(10);

	m_spinThresh.SetRange32(0, 2000);
	m_spinThresh.SetBuddy(&m_editThresh);
	m_spinThresh.SetPos32(740);

	m_radioStation1.SetCheck(true);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CAlarmsUploadDlg::OnClickedRadioStation1()
{
	SaveAlarmDetails(m_nLastStation);
	RestoreAlarmDetails(0);
	m_nLastStation = 0;
}


void CAlarmsUploadDlg::OnClickedRadioStation2()
{
	SaveAlarmDetails(m_nLastStation);
	RestoreAlarmDetails(1);
	m_nLastStation = 1;
}


void CAlarmsUploadDlg::OnClickedRadioStation3()
{
	SaveAlarmDetails(m_nLastStation);
	RestoreAlarmDetails(2);
	m_nLastStation = 2;
}


void CAlarmsUploadDlg::OnClickedRadioStation4()
{
	SaveAlarmDetails(m_nLastStation);
	RestoreAlarmDetails(3);
	m_nLastStation = 3;
}


void CAlarmsUploadDlg::OnClickedRadioStation5()
{
	SaveAlarmDetails(m_nLastStation);
	RestoreAlarmDetails(4);
	m_nLastStation = 4;
}


void CAlarmsUploadDlg::OnClickedRadioStation6()
{
	SaveAlarmDetails(m_nLastStation);
	RestoreAlarmDetails(5);
	m_nLastStation = 5;
}


void CAlarmsUploadDlg::OnClickedRadioStation7()
{
	SaveAlarmDetails(m_nLastStation);
	RestoreAlarmDetails(6);
	m_nLastStation = 6;
}


void CAlarmsUploadDlg::OnClickedRadioStation8()
{
	SaveAlarmDetails(m_nLastStation);
	RestoreAlarmDetails(7);
	m_nLastStation = 7;
}

bool CAlarmsUploadDlg::ValidateFields(CString& strMsg)
{
	bool bResult = true;

	for (int nI = 0; nI < 8; nI++)
	{
		if (m_arrayAlarmDetails[nI].m_bInstalled)
		{
			if ((m_arrayAlarmDetails[nI].m_nDryTime < 1) || (m_arrayAlarmDetails[nI].m_nDryTime > 120))
			{
				bResult = false;
				strMsg = "Dry time must be between 1 and 120 minutes";
			}
			else if ((m_arrayAlarmDetails[nI].m_nThreshold < 200) || (m_arrayAlarmDetails[nI].m_nThreshold > 2000))
			{
				bResult = false;
				strMsg = "Probe threshold value must be between 200 and 2000 minutes";
			}
		}
		if (m_arrayAlarmDetails[nI].m_bAuto)
		{
			if ((m_arrayAlarmDetails[nI].m_nOnTime < 1) || (m_arrayAlarmDetails[nI].m_nOnTime > 2000))
			{
				bResult = false;
				strMsg = "On time must be between 1 and 20 minutes";
			}
		}
	}
	return bResult;
}

void CAlarmsUploadDlg::OnClickedButtonUpload()
{
	CSerialPort SerialPort;
	CString strReq, strResp, strStation, str;
	bool bSuccess = false;
	CProgressDlg dlg(GetParent());
	int nProgress = 0;

	if (ValidateFields(str))
	{
		m_staticStatus.SetWindowTextA("Uploading...");
		if (SerialPort.Open(m_COMPort))
		{
			m_staticStatus.SetWindowTextA("Uploading alarm details...");
			strReq = CString("alarms") + SerialPort.GetDelim();
			for (int nI = 0; nI < 8; nI++)
			{
				strStation.Format("station %d", nI + 1);
				str = CString("Uploading alarm details for ") + strStation + "!";
				dlg.SetProgress(nProgress, str);
				strReq.Format("%s%d%s", strReq, nI + 1, SerialPort.GetDelim());
				strReq.Format("%s%d%s", strReq, m_arrayAlarmDetails[nI].m_bInstalled, SerialPort.GetDelim());
				strReq.Format("%s%d%s", strReq, m_arrayAlarmDetails[nI].m_bAuto, SerialPort.GetDelim());
				strReq.Format("%s%d%s", strReq, m_arrayAlarmDetails[nI].m_nDryTime, SerialPort.GetDelim());
				strReq.Format("%s%d%s", strReq, m_arrayAlarmDetails[nI].m_nThreshold, SerialPort.GetDelim());
				strReq.Format("%s%d", strReq, m_arrayAlarmDetails[nI].m_nOnTime);
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
	else
		AfxMessageBox(str, MB_OK);
}


void CAlarmsUploadDlg::OnDeltaposSpinOntime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_arrayAlarmDetails[GetStation()].m_nOnTime = m_spinOnTime.GetPos();
	*pResult = 0;
}

void CAlarmsUploadDlg::OnDeltaposSpinProbeThreshold(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_arrayAlarmDetails[GetStation()].m_nThreshold = m_spinThresh.GetPos32();
	*pResult = 0;
}


void CAlarmsUploadDlg::OnDeltaposSpinDrytime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_arrayAlarmDetails[GetStation()].m_nDryTime = m_spinDryTime.GetPos();
	*pResult = 0;
}


void CAlarmsUploadDlg::OnChangeEditDrytime()
{
	CString str;

	if (m_editDrytime.GetSafeHwnd())
	{
		m_editDrytime.GetWindowTextA(str);
		m_arrayAlarmDetails[GetStation()].m_nDryTime = atoi(str);
	}
}


void CAlarmsUploadDlg::OnChangeEditOntime()
{
	CString str;

	if (m_editOnTime.GetSafeHwnd())
	{
		m_editOnTime.GetWindowTextA(str);
		m_arrayAlarmDetails[GetStation()].m_nOnTime = atoi(str);
	}
}


void CAlarmsUploadDlg::OnChangeEditProbeThreshold()
{
	CString str;

	if (m_editThresh.GetSafeHwnd())
	{
		m_editThresh.GetWindowTextA(str);
		m_arrayAlarmDetails[GetStation()].m_nThreshold = atoi(str);
	}
}
