// EmailUploadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploaderUDP.h"
#include "EmailUploadDlg.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"
#include "IrrigationSocket.h"


// CEmailUploadDlg dialog

IMPLEMENT_DYNAMIC(CEmailUploadDlg, CUploadDlg)

CEmailUploadDlg::CEmailUploadDlg(CWnd* pParent /*=NULL*/) : CUploadDlg(IDD_DIALOG_WIFI, pParent)
{
}

CEmailUploadDlg::~CEmailUploadDlg()
{
}

void CEmailUploadDlg::InitFields()
{
	CString strEmailAddress, strMailServer, strUsername, strPassword, strMsg;
	CProgressDlg dlg(this);

	m_staticStatus.SetWindowTextA("Uploading...");
	if (!m_strIPAddr.IsEmpty() && (m_nPort > 0))
	{
		dlg.SetProgress(0, "Fetching current email address...");
		GetDataItem("emailaddress", strEmailAddress);
		dlg.SetProgress(25, "Fetching current mail server...");
		GetDataItem("mailserver", strMailServer);
		dlg.SetProgress(50, "Fetching current mail server username...");
		GetDataItem("username", strUsername);
		dlg.SetProgress(75, "Fetching current mail server password...");
		GetDataItem("password", strPassword);

		strMsg = "";
		int nLength = strMsg.GetLength();

		if ((strEmailAddress.GetLength() == 0) || (strMailServer.GetLength() == 0) || (strUsername.GetLength() == 0) || (strPassword.GetLength() == 0))
			strMsg += "Error retrieving data!";
		else
			strMsg += "Data retrived successfully!";

		m_staticStatus.SetWindowTextA(strMsg);
		dlg.Close();

		m_editEmailAddress.SetWindowTextA(strEmailAddress);
		m_editMailServer.SetWindowTextA(strMailServer);
		m_editUsername.SetWindowTextA(strUsername);
		m_editPassword.SetWindowTextA(strPassword);
	}
}

void CEmailUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, m_buttonUpload);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDC_EDIT_EMAIL_ADDRESS, m_editEmailAddress);
	DDX_Control(pDX, IDC_EDIT_MAIL_SERVER, m_editMailServer);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUsername);
}


BEGIN_MESSAGE_MAP(CEmailUploadDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CEmailUploadDlg::OnClickedButtonUpload)
	ON_EN_CHANGE(IDC_EDIT_USERNAME, &CEmailUploadDlg::OnChangeEditUsername)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CEmailUploadDlg::OnChangeEditPassword)
	ON_EN_CHANGE(IDC_EDIT_MAIL_SERVER, &CEmailUploadDlg::OnChangeEditMailServer)
	ON_EN_CHANGE(IDC_EDIT_EMAIL_ADDRESS, &CEmailUploadDlg::OnChangeEditEmailAddress)
END_MESSAGE_MAP()


// CEmailUploadDlg message handlers


void CEmailUploadDlg::EnableUploadButton()
{
	CString strEmailAddress, strMailServer, strUsername, strPassword;

	m_editEmailAddress.GetWindowText(strEmailAddress);
	m_editMailServer.GetWindowText(strMailServer);
	m_editPassword.GetWindowText(strUsername);
	m_editUsername.GetWindowText(strPassword);

	m_buttonUpload.EnableWindow(!strEmailAddress.IsEmpty() && !strMailServer.IsEmpty() && !strUsername.IsEmpty() && !strPassword.IsEmpty());
}

void CEmailUploadDlg::EnableControls(const bool bEnable)
{
	m_editEmailAddress.EnableWindow(bEnable);
	m_editMailServer.EnableWindow(bEnable);
	m_editPassword.EnableWindow(bEnable);
	m_editUsername.EnableWindow(bEnable);
}

BOOL CEmailUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_editEmailAddress.SetLimitText(40);
	m_editMailServer.SetLimitText(40);
	m_editPassword.SetLimitText(40);
	m_editUsername.SetLimitText(40);
	ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CEmailUploadDlg::OnClickedButtonUpload()
{
	CString strEmailAddress, strMailServer, strUsername, strPassword, strReq, strResp;

	if (!m_strIPAddr.IsEmpty() && (m_nPort > 0))
	{
		m_editEmailAddress.GetWindowTextA(strEmailAddress);
		m_editMailServer.GetWindowTextA(strMailServer);
		m_editPassword.GetWindowTextA(strUsername);
		m_editUsername.GetWindowTextA(strPassword);

		strReq = CString("emailsettings") + m_pSocket->GetDelim() + strEmailAddress + m_pSocket->GetDelim() + strMailServer + m_pSocket->GetDelim() + strUsername + m_pSocket->GetDelim() + strPassword;
		if (m_pSocket->Request(strReq, strResp, m_strIPAddr, m_nPort))
			m_staticStatus.SetWindowTextA(strResp);
		else
			m_staticStatus.SetWindowTextA("Upload was unsuccessful!");
	}
}


void CEmailUploadDlg::OnChangeEditUsername()
{
	EnableUploadButton();
}


void CEmailUploadDlg::OnChangeEditPassword()
{
	EnableUploadButton();
}


void CEmailUploadDlg::OnChangeEditMailServer()
{
	EnableUploadButton();
}


void CEmailUploadDlg::OnChangeEditEmailAddress()
{
	EnableUploadButton();
}
