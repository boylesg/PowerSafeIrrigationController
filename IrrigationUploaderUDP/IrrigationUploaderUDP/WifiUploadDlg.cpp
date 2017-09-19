// WifiUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploaderUDP.h"
#include "WifiUploadDlg.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"
#include "IrrigationSocket.h"

// CWifiUploadDlg dialog

IMPLEMENT_DYNAMIC(CWifiUploadDlg, CUploadDlg)

CWifiUploadDlg::CWifiUploadDlg(CWnd* pParent /*=NULL*/): CUploadDlg(IDD_DIALOG_WIFI, pParent)
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

	m_editNetworkKey.GetWindowText(strNetworkKey);
	m_editNetworkName.GetWindowText(strNetworkName);
	m_buttonUpload.EnableWindow(!strNetworkKey.IsEmpty() && !strNetworkName.IsEmpty());
}

void CWifiUploadDlg::EnableControls(const bool bEnable)
{
	m_editNetworkKey.EnableWindow(bEnable);
	m_editNetworkName.EnableWindow(bEnable);
}

void CWifiUploadDlg::InitFields()
{
	CString strNetworkKey, strNetworkName;
	CProgressDlg dlg(GetParent());
	CString strMsg;
	CString strIPAddr = m_strIPAddr;
	UINT nPort = m_nPort;

	m_staticStatus.SetWindowTextA("Uploading...");
	if (!m_strIPAddr.IsEmpty() && (m_nPort > 0))
	{
		dlg.SetProgress(0, "Fetching current network name...");
		GetDataItem("networkname", strNetworkName);
		dlg.SetProgress(50, "Fetching current network key...");
		GetDataItem("networkkey", strNetworkKey);
		dlg.SetProgress(100);
		dlg.Close();

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
	CString strNetworkKey, strNetworkName, strMsg, strIPAddr;
	UINT nPort = 0;

	if (m_pSocket)
	{
		m_editNetworkKey.GetWindowText(strNetworkKey);
		m_editNetworkName.GetWindowText(strNetworkName);

		m_pSocket->Request(CString("wifi") + m_pSocket->GetDelim() + strNetworkName + m_pSocket->GetDelim() + strNetworkKey, strMsg, m_strIPAddr, m_nPort);
		m_staticStatus.SetWindowText(strMsg);
	}
}
