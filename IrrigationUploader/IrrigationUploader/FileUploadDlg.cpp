
// IrrigationUploaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploader.h"
#include "FileUploadDlg.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CFileUploadDlg::CFileUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IRRIGATIONUPLOADER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileUploadDlg::EnableControls(const bool bEnable)
{
	m_editContents.EnableWindow(bEnable);
	m_browseFile.EnableWindow(bEnable);
	m_comboBlank.EnableWindow(bEnable);

	CString strFilename;
	m_browseFile.GetWindowTextA(strFilename);
	if (strFilename.IsEmpty() && (m_comboBlank.GetCurSel() != CB_ERR))
		m_comboBlank.GetLBText(m_comboBlank.GetCurSel(), strFilename);
	m_editContents.EnableWindow(!strFilename.IsEmpty());

	CString strContents;
	m_editContents.GetWindowTextA(strContents);
	m_buttonUpload.EnableWindow(!strContents.IsEmpty());
	m_buttonSave.EnableWindow(!strContents.IsEmpty());
	m_buttonDiscard.EnableWindow(!strContents.IsEmpty());
}

void CFileUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_FILE, m_browseFile);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, m_buttonUpload);
	DDX_Control(pDX, IDC_EDIT_FILE_CONTENTS, m_editContents);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
	DDX_Control(pDX, IDC_BUTTON_DISCARD, m_buttonDiscard);
	DDX_Control(pDX, IDC_COMBO_BLANK, m_comboBlank);
}

BEGIN_MESSAGE_MAP(CFileUploadDlg, CDialogEx)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_FILE, &CFileUploadDlg::OnChangeMfceditbrowseFile)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CFileUploadDlg::OnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DISCARD, &CFileUploadDlg::OnClickedButtonDiscard)
	ON_EN_UPDATE(IDC_MFCEDITBROWSE_FILE, &CFileUploadDlg::OnUpdateMfceditbrowseFile)
	ON_CBN_SELCHANGE(IDC_COMBO_BLANK, &CFileUploadDlg::OnSelchangeComboBlank)
	ON_CBN_EDITCHANGE(IDC_COMBO_BLANK, &CFileUploadDlg::OnEditchangeComboBlank)
	ON_COMMAND(IDC_BUTTON_UPLOAD, &CFileUploadDlg::OnButtonUpload)
END_MESSAGE_MAP()


// CFileUploadDlg message handlers

BOOL CFileUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_browseFile.EnableFileBrowseButton(_T("txt"), _T("Text Files: (*.txt)|*.txt|Html Files: (*.htm)|*.htm||"));
	ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileUploadDlg::OnChangeMfceditbrowseFile()
{
	CStdioFile file;
	CString strContents, strLine, strFilename;

	m_browseFile.GetWindowTextA(strFilename);
	if (strFilename.GetLength() > 0)
	{
		if (file.Open(strFilename, CFile::modeRead | CFile::typeText))
		{
			UpdateData(true);
			while (file.ReadString(strLine) > 0)
			{
				strContents += strLine + "\r\n";
			}
			m_editContents.SetWindowTextA(strContents);
			file.Close();
			m_editContents.SetReadOnly(strFilename.Find("htm") >= 0);
			m_buttonSave.EnableWindow(strFilename.Find("htm") < 0);
			m_buttonDiscard.EnableWindow(strFilename.Find("htm") < 0);
			m_comboBlank.SetCurSel(-1);
		}
	}
	EnableControls(true);
}


void CFileUploadDlg::OnClickedButtonSave()
{
	CStdioFile file;
	CString strContents, strLine, strFilename;
	int nPos = 0;

	m_browseFile.GetWindowTextA(strFilename);
	if (file.Open(strFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		m_editContents.GetWindowTextA(strContents);
		while (strContents.GetLength() > 0)
		{
			nPos = strContents.Find("\r\n");

			if ((nPos == -1) && (strContents.GetLength() > 0))
			{
				strLine = strContents;
				strContents.Empty();
			}
			else
			{
				strLine = strContents.Left(nPos);
				strContents = strContents.Mid(nPos + 2, strContents.GetLength());
			}
			file.WriteString(strLine + "\r\n");
		}
		file.Close();
	}
}


void CFileUploadDlg::OnClickedButtonDiscard()
{
	OnChangeMfceditbrowseFile();
}


void CFileUploadDlg::OnUpdateMfceditbrowseFile()
{
	OnChangeMfceditbrowseFile();
}


void CFileUploadDlg::OnSelchangeComboBlank()
{
	CString strFilename, strContents;

	m_comboBlank.GetLBText(m_comboBlank.GetCurSel(), strFilename);
	EnableControls(strFilename.GetLength() > 0);

	if (strFilename == "email.txt")
	{
		strContents = "<Change this to the email address that will receive alarm notifications>\r\n";
		strContents += "<Change this to your outgoing mail server>\r\n";
		strContents += "<Change this to the username for your mail server>\r\n";
		strContents += "<Change this to the password for your mail server>\r\n";
	}
	else if (strFilename == "program.txt")
	{
		strContents = "station 1\r\n";
		strContents += "\tdescription:<Any text up to 30 character>\r\n";
		strContents += "\tsuspend:<blank or start date, dash, end date (e.g. 1/5-1/8)>\r\n";
		strContents += "\t<Watering dates and times here. Any number of dates but a maximum of 12 start times per date. E.G. 1/1=10:00-10,13:00-8,14:30-6,...>";
		strContents += "station 2\r\n";
		strContents += "\tdescription:<Any text up to 30 character>\r\n";
		strContents += "\tsuspend:<blank or start date, dash, end date (e.g. 1/5-1/8)>\r\n";
		strContents += "\t<Watering dates and times here. Any number of dates but a maximum of 12 start times per date. E.G. 1/1=10:00-10,13:00-8,14:30-6,...>";
		strContents += "station 2\r\n";
		strContents += "\tdescription:<Any text up to 30 character>\r\n";
		strContents += "\tsuspend:<blank or start date, dash, end date (e.g. 1/5-1/8)>\r\n";
		strContents += "\t<Watering dates and times here. Any number of dates but a maximum of 12 start times per date. E.G. 1/1=10:00-10,13:00-8,14:30-6,...>";
		strContents += "station 3\r\n";
		strContents += "\tdescription:<Any text up to 30 character>\r\n";
		strContents += "\tsuspend:<blank or start date, dash, end date (e.g. 1/5-1/8)>\r\n";
		strContents += "\t<Watering dates and times here. Any number of dates but a maximum of 12 start times per date. E.G. 1/1=10:00-10,13:00-8,14:30-6,...>";
		strContents += "station 4\r\n";
		strContents += "\tdescription:<Any text up to 30 character>\r\n";
		strContents += "\tsuspend:<blank or start date, dash, end date (e.g. 1/5-1/8)>\r\n";
		strContents += "\t<Watering dates and times here. Any number of dates but a maximum of 12 start times per date. E.G. 1/1=10:00-10,13:00-8,14:30-6,...>";
		strContents += "station 5\r\n";
		strContents += "\tdescription:<Any text up to 30 character>\r\n";
		strContents += "\tsuspend:<blank or start date, dash, end date (e.g. 1/5-1/8)>\r\n";
		strContents += "\t<Watering dates and times here. Any number of dates but a maximum of 12 start times per date. E.G. 1/1=10:00-10,13:00-8,14:30-6,...>";
		strContents += "station 6\r\n";
		strContents += "\tdescription:<Any text up to 30 character>\r\n";
		strContents += "\tsuspend:<blank or start date, dash, end date (e.g. 1/5-1/8)>\r\n";
		strContents += "\t<Watering dates and times here. Any number of dates but a maximum of 12 start times per date. E.G. 1/1=10:00-10,13:00-8,14:30-6,...>";
		strContents += "station 7\r\n";
		strContents += "\tdescription:<Any text up to 30 character>\r\n";
		strContents += "\tsuspend:<blank or start date, dash, end date (e.g. 1/5-1/8)>\r\n";
		strContents += "\t<Watering dates and times here. Any number of dates but a maximum of 12 start times per date. E.G. 1/1=10:00-10,13:00-8,14:30-6,...>";
		strContents += "station 8\r\n";
		strContents += "\tdescription:<Any text up to 30 character>\r\n";
		strContents += "\tsuspend:<blank or start date, dash, end date (e.g. 1/5-1/8)>\r\n";
	}
	else if (strFilename == "timezone.txt")
	{
		strContents = "<Change this to the unique ID of this irrigation controller>\r\n";
		strContents += "<Change this to your timezone adjustment (+/- hours from Universal or Greenwich Mean Time>\r\n";
		strContents += "<Change this to 'yes' or 'no' for daylight savings adjustment>\r\n";
	}
	else if (strFilename == "wifi.txt")
	{
		strContents = "<Change this to your wifi network's name / SSID>\r\n";
		strContents += "<Change this to your wifi network's password / key>\r\n";
	}
	else if (strFilename == "walarms.txt")
	{
		strContents = "station 1\r\n";
		strContents += "\tprobe:<replace this with either 'blank' or 'installed'>\r\n";
		strContents += "\tallowed_dry_time:<replace this with either '0' or the number of minutes this station is permitted to be dry for>\r\n";
		strContents += "\tprobe_threshold_value:<replace this with either '0' or a value between '600' (bone dry soil) and '2000' (sopping wet soil)>\r\n";
		strContents += "\tprobe_auto:<replace this with either 'yes' or 'no' i.e. whether or not the controller should turn this station on if it remains dry for the specified number of minutes>\r\n";
		strContents += "\tauto_runtime:<replace this with either '0' or the number of minutes the station should be turned on for when a dry soil alarms is trigged>\r\n";
		strContents += "station 2\r\n";
		strContents += "\tprobe:<replace this with either 'blank' or 'installed'>\r\n";
		strContents += "\tallowed_dry_time:<replace this with either '0' or the number of minutes this station is permitted to be dry for>\r\n";
		strContents += "\tprobe_threshold_value:<replace this with either '0' or a value between '600' (bone dry soil) and '2000' (sopping wet soil)>\r\n";
		strContents += "\tprobe_auto:<replace this with either 'yes' or 'no' i.e. whether or not the controller should turn this station on if it remains dry for the specified number of minutes>\r\n";
		strContents += "\tauto_runtime:<replace this with either '0' or the number of minutes the station should be turned on for when a dry soil alarms is trigged>\r\n";
		strContents += "station 3\r\n";
		strContents += "\tprobe:<replace this with either 'blank' or 'installed'>\r\n";
		strContents += "\tallowed_dry_time:<replace this with either '0' or the number of minutes this station is permitted to be dry for>\r\n";
		strContents += "\tprobe_threshold_value:<replace this with either '0' or a value between '600' (bone dry soil) and '2000' (sopping wet soil)>\r\n";
		strContents += "\tprobe_auto:<replace this with either 'yes' or 'no' i.e. whether or not the controller should turn this station on if it remains dry for the specified number of minutes>\r\n";
		strContents += "\tauto_runtime:<replace this with either '0' or the number of minutes the station should be turned on for when a dry soil alarms is trigged>\r\n";
		strContents += "station 4\r\n";
		strContents += "\tprobe:<replace this with either 'blank' or 'installed'>\r\n";
		strContents += "\tallowed_dry_time:<replace this with either '0' or the number of minutes this station is permitted to be dry for>\r\n";
		strContents += "\tprobe_threshold_value:<replace this with either '0' or a value between '600' (bone dry soil) and '2000' (sopping wet soil)>\r\n";
		strContents += "\tprobe_auto:<replace this with either 'yes' or 'no' i.e. whether or not the controller should turn this station on if it remains dry for the specified number of minutes>\r\n";
		strContents += "\tauto_runtime:<replace this with either '0' or the number of minutes the station should be turned on for when a dry soil alarms is trigged>\r\n";
		strContents += "station 5\r\n";
		strContents += "\tprobe:<replace this with either 'blank' or 'installed'>\r\n";
		strContents += "\tallowed_dry_time:<replace this with either '0' or the number of minutes this station is permitted to be dry for>\r\n";
		strContents += "\tprobe_threshold_value:<replace this with either '0' or a value between '600' (bone dry soil) and '2000' (sopping wet soil)>\r\n";
		strContents += "\tprobe_auto:<replace this with either 'yes' or 'no' i.e. whether or not the controller should turn this station on if it remains dry for the specified number of minutes>\r\n";
		strContents += "\tauto_runtime:<replace this with either '0' or the number of minutes the station should be turned on for when a dry soil alarms is trigged>\r\n";
		strContents += "station 6\r\n";
		strContents += "\tprobe:<replace this with either 'blank' or 'installed'>\r\n";
		strContents += "\tallowed_dry_time:<replace this with either '0' or the number of minutes this station is permitted to be dry for>\r\n";
		strContents += "\tprobe_threshold_value:<replace this with either '0' or a value between '600' (bone dry soil) and '2000' (sopping wet soil)>\r\n";
		strContents += "\tprobe_auto:<replace this with either 'yes' or 'no' i.e. whether or not the controller should turn this station on if it remains dry for the specified number of minutes>\r\n";
		strContents += "\tauto_runtime:<replace this with either '0' or the number of minutes the station should be turned on for when a dry soil alarms is trigged>\r\n";
		strContents += "station 7\r\n";
		strContents += "\tprobe:<replace this with either 'blank' or 'installed'>\r\n";
		strContents += "\tallowed_dry_time:<replace this with either '0' or the number of minutes this station is permitted to be dry for>\r\n";
		strContents += "\tprobe_threshold_value:<replace this with either '0' or a value between '600' (bone dry soil) and '2000' (sopping wet soil)>\r\n";
		strContents += "\tprobe_auto:<replace this with either 'yes' or 'no' i.e. whether or not the controller should turn this station on if it remains dry for the specified number of minutes>\r\n";
		strContents += "\tauto_runtime:<replace this with either '0' or the number of minutes the station should be turned on for when a dry soil alarms is trigged>\r\n";
		strContents += "station 8\r\n";
		strContents += "\tprobe:<replace this with either 'blank' or 'installed'>\r\n";
		strContents += "\tallowed_dry_time:<replace this with either '0' or the number of minutes this station is permitted to be dry for>\r\n";
		strContents += "\tprobe_threshold_value:<replace this with either '0' or a value between '600' (bone dry soil) and '2000' (sopping wet soil)>\r\n";
		strContents += "\tprobe_auto:<replace this with either 'yes' or 'no' i.e. whether or not the controller should turn this station on if it remains dry for the specified number of minutes>\r\n";
		strContents += "\tauto_runtime:<replace this with either '0' or the number of minutes the station should be turned on for when a dry soil alarms is trigged>\r\n";
	}
	m_editContents.SetWindowTextA(strContents);
	m_browseFile.SetWindowTextA(strFilename);
}


void CFileUploadDlg::OnEditchangeComboBlank()
{
	OnSelchangeComboBlank();
}


void CFileUploadDlg::OnButtonUpload()
{
	CSerialPort SerialPort;
	CString strUploadFilename, strContents, strReq, strResp, strData, strByteCount;
	const DWORD nSize = 64;
	DWORD nByteCount = 0, nFileSize = 0;
	int nPos = 0, nRetries = 0, nProgress = 0;
	CProgressDlg dlg;

	m_browseFile.GetWindowTextA(strUploadFilename);
	if ((nPos = strUploadFilename.ReverseFind('\\')) >= 0)
		strUploadFilename = strUploadFilename.Right(strUploadFilename.GetLength() - nPos - 1);

	if (SerialPort.Open(m_COMPort))
	{
		m_editContents.GetWindowTextA(strContents);
		nFileSize = strContents.GetLength();
		dlg.SetProgress(nProgress, "Uploading file '" + strUploadFilename + "'");
		SetDlgItemText(IDC_STATIC_STATUS, "uploading");
		while (strContents.GetLength() > 0)
		{
			strByteCount.Format("%d", nByteCount);
			strReq = CString("upload") + SerialPort.GetDelim() + strUploadFilename + SerialPort.GetDelim() + strByteCount + SerialPort.GetDelim();
			strData = strContents.Mid(0, nSize);
			strReq += strData;
			if (SerialPort.Request(strReq, strResp))
			{
				if (strResp == "OK")
				{
					nRetries = 0;
					strContents = strContents.Mid(nSize, strContents.GetLength() - 1);
					nByteCount += nSize;
					nProgress = ((float)nByteCount / nFileSize) * 100;
					dlg.SetProgress(nProgress);
					if (strContents.IsEmpty())
						SetDlgItemText(IDC_STATIC_STATUS, "successful");
				}
				else
				{
					AfxMessageBox(strResp + " Please try again.", MB_OK);
					SetDlgItemText(IDC_STATIC_STATUS, "unsuccessful");
					break;
				}
			}
			else
			{
				if (nRetries <= 100)
					nRetries++;
				else
				{
					SetDlgItemText(IDC_STATIC_STATUS, "unsuccessful");
					break;
				}
			}
		}
	}
}
