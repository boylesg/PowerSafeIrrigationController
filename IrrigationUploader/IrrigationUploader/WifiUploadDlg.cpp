// WifiUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploader.h"
#include "WifiUploadDlg.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"


// CWifiUploadDlg dialog

IMPLEMENT_DYNAMIC(CWifiUploadDlg, CDialogEx)

CWifiUploadDlg::CWifiUploadDlg(CWnd* pParent /*=NULL*/): CDialogEx(IDD_DIALOG_WIFI, pParent)
{

}

CWifiUploadDlg::~CWifiUploadDlg()
{
}

void CWifiUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WFI_NETWORK_KEY, m_editNetworkKey);
	DDX_Control(pDX, IDC_EDIT_WFI_NETWORK_NAME, m_editNetworkName);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, m_buttonUpload);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
}


BEGIN_MESSAGE_MAP(CWifiUploadDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_WFI_NETWORK_KEY, &CWifiUploadDlg::OnChangeEditWfiNetworkKey)
	ON_EN_CHANGE(IDC_EDIT_WFI_NETWORK_NAME, &CWifiUploadDlg::OnChangeEditWfiNetworkName)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CWifiUploadDlg::OnClickedButtonUpload)
END_MESSAGE_MAP()


// CWifiUploadDlg message handlers


void CWifiUploadDlg::EnableUploadButton()
{
	CString strNetworkKey, strNetworkName;

	m_editNetworkKey.GetWindowTextA(strNetworkKey);
	m_editNetworkName.GetWindowTextA(strNetworkName);
	m_buttonUpload.EnableWindow(!strNetworkKey.IsEmpty() && !strNetworkName.IsEmpty());
}

void CWifiUploadDlg::EnableControls(const bool bEnable)
{
	m_editNetworkKey.EnableWindow(bEnable);
	m_editNetworkName.EnableWindow(bEnable);
}


void CWifiUploadDlg::InitEditFields(CCOMPort COMPort)
{
	CString strNetworkKey, strNetworkName;
	CSerialPort SerialPort;
	CProgressDlg dlg(GetParent());
	CString strMsg;

	m_staticStatus.SetWindowTextA("Uploading...");
	if (m_COMPort != COMPort)
	{
		m_COMPort = COMPort;
		if (SerialPort.Open(m_COMPort))
		{
			dlg.SetProgress(0, "Fetching current network name...");
			SerialPort.Request("networkname", strNetworkName);
			dlg.SetProgress(50, "Fetching current network key...");
			SerialPort.Request("networkkey", strNetworkKey);
			dlg.Close();
			SerialPort.Close();

			if ((strNetworkName.GetLength() > 0) || (strNetworkKey.GetLength() > 0))
				strMsg = "Network name and key retrieved successfully!";
			else if ((strNetworkName.GetLength() > 0) || (strNetworkKey.GetLength() == 0))
				strMsg = "Could not retrieve network key!";
			else if ((strNetworkName.GetLength() == 0) || (strNetworkKey.GetLength() > 0))
				strMsg = "Could not retrieve network name!";
			else
				strMsg = "Could not retrieve network name and key!";

			m_staticStatus.SetWindowText(strMsg);
			m_editNetworkKey.SetWindowText(strNetworkKey);
			m_editNetworkName.SetWindowText(strNetworkName);
		}
	}
}


BOOL CWifiUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_editNetworkKey.SetLimitText(40);
	m_editNetworkName.SetLimitText(40);
	ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CWifiUploadDlg::OnChangeEditWfiNetworkKey()
{
	EnableUploadButton();
}


void CWifiUploadDlg::OnChangeEditWfiNetworkName()
{
	EnableUploadButton();
}


void CWifiUploadDlg::OnClickedButtonUpload()
{
	CSerialPort SerialPort;
	CString strNetworkKey, strNetworkName, strMsg;

	if (SerialPort.Open(m_COMPort))
	{
		m_editNetworkKey.GetWindowTextA(strNetworkKey);
		m_editNetworkName.GetWindowTextA(strNetworkName);

		SerialPort.Write(CString("wifi") + SerialPort.GetDelim() + strNetworkName + SerialPort.GetDelim() + strNetworkKey);
		SerialPort.Receive(strMsg);
		m_staticStatus.SetWindowTextA(strMsg);
		SerialPort.Close();
	}
}
