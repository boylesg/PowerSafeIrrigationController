
// IrrigationUploaderUDPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploaderUDP.h"
#include "IrrigationUploaderUDPDlg.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




CIrrigationUploaderUDPDlg::CIrrigationUploaderUDPDlg(CWnd* pParent /*=NULL*/): CDialogEx(IDD_IRRIGATIONUPLOADERUDP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nPort = 10002;
	m_bConnected = false;
}

void CIrrigationUploaderUDPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_buttonConnect);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDC_TAB_ACTIVITY, m_tabActivity);
	DDX_Control(pDX, IDC_IPADDRESS, m_ipaddrIP);
}

void CIrrigationUploaderUDPDlg::CenterChildWindow(CWnd* pParentWnd, CWnd* pChildWnd)
{
	CRect rectChildWnd;

	pChildWnd->CenterWindow(pParentWnd);
	pChildWnd->GetWindowRect(rectChildWnd);
	pParentWnd->ScreenToClient(rectChildWnd);
	pChildWnd->MoveWindow(rectChildWnd.left + 11, rectChildWnd.top + 52, rectChildWnd.Width(), rectChildWnd.Height());
}

void CIrrigationUploaderUDPDlg::PositionChildWindows()
{
	CenterChildWindow(&m_tabActivity, &m_dlgFileUpload);
	CenterChildWindow(&m_tabActivity, &m_dlgWifiUpload);
	CenterChildWindow(&m_tabActivity, &m_dlgEmailUpload);
	CenterChildWindow(&m_tabActivity, &m_dlgTimeUpload);
	CenterChildWindow(&m_tabActivity, &m_dlgProgramUpload);
	CenterChildWindow(&m_tabActivity, &m_dlgAlarms);
	CenterChildWindow(&m_tabActivity, &m_dlgFileDownload);
}

void CIrrigationUploaderUDPDlg::ChangeTab(const int nSelTab)
{
	if (m_bConnected)
	{
		m_dlgWifiUpload.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgEmailUpload.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgTimeUpload.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgProgramUpload.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgAlarms.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgFileUpload.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgFileDownload.SetIPAddr(m_strIPAddr, m_nPort);
	}
	switch (nSelTab)
	{
		case 0:
			m_dlgWifiUpload.ShowWindow(SW_NORMAL);
			if (m_bConnected)
				m_dlgWifiUpload.InitFields();
			m_dlgEmailUpload.ShowWindow(SW_HIDE);
			m_dlgTimeUpload.ShowWindow(SW_HIDE);
			m_dlgProgramUpload.ShowWindow(SW_HIDE);
			m_dlgAlarms.ShowWindow(SW_HIDE);
			m_dlgFileUpload.ShowWindow(SW_HIDE);
			m_dlgFileDownload.ShowWindow(SW_HIDE);
			break;
		case 1:
			m_dlgWifiUpload.ShowWindow(SW_HIDE);
			m_dlgEmailUpload.ShowWindow(SW_NORMAL);
			if (m_bConnected)
				m_dlgEmailUpload.InitFields();
			m_dlgTimeUpload.ShowWindow(SW_HIDE);
			m_dlgProgramUpload.ShowWindow(SW_HIDE);
			m_dlgAlarms.ShowWindow(SW_HIDE);
			m_dlgFileUpload.ShowWindow(SW_HIDE);
			m_dlgFileDownload.ShowWindow(SW_HIDE);
			break;
		case 2:
			m_dlgWifiUpload.ShowWindow(SW_HIDE);
			m_dlgEmailUpload.ShowWindow(SW_HIDE);
			m_dlgTimeUpload.ShowWindow(SW_NORMAL);
			if (m_bConnected)
				m_dlgTimeUpload.InitFields();
			m_dlgProgramUpload.ShowWindow(SW_HIDE);
			m_dlgAlarms.ShowWindow(SW_HIDE);
			m_dlgFileUpload.ShowWindow(SW_HIDE);
			m_dlgFileDownload.ShowWindow(SW_HIDE);
			break;
		case 3:
			m_dlgWifiUpload.ShowWindow(SW_HIDE);
			m_dlgEmailUpload.ShowWindow(SW_HIDE);
			m_dlgTimeUpload.ShowWindow(SW_HIDE);
			m_dlgProgramUpload.ShowWindow(SW_NORMAL);
			if (m_bConnected)
				m_dlgProgramUpload.InitFields();
			m_dlgAlarms.ShowWindow(SW_HIDE);
			m_dlgFileUpload.ShowWindow(SW_HIDE);
			m_dlgFileDownload.ShowWindow(SW_HIDE);
			break;
		case 4:
			m_dlgWifiUpload.ShowWindow(SW_HIDE);
			m_dlgEmailUpload.ShowWindow(SW_HIDE);
			m_dlgTimeUpload.ShowWindow(SW_HIDE);
			m_dlgProgramUpload.ShowWindow(SW_HIDE);
			m_dlgAlarms.ShowWindow(SW_SHOW);
			if (m_bConnected)
				m_dlgAlarms.InitFields();
			m_dlgFileUpload.ShowWindow(SW_HIDE);
			m_dlgFileDownload.ShowWindow(SW_HIDE);
			break;
		case 5:
			m_dlgWifiUpload.ShowWindow(SW_HIDE);
			m_dlgEmailUpload.ShowWindow(SW_HIDE);
			m_dlgTimeUpload.ShowWindow(SW_HIDE);
			m_dlgProgramUpload.ShowWindow(SW_HIDE);
			m_dlgAlarms.ShowWindow(SW_HIDE);
			m_dlgFileUpload.ShowWindow(SW_NORMAL);
			if (m_bConnected)
				m_dlgFileUpload.EnableControls(!m_strIPAddr.IsEmpty());
			m_dlgFileDownload.ShowWindow(SW_HIDE);
			break;
		case 6:
			m_dlgWifiUpload.ShowWindow(SW_HIDE);
			m_dlgEmailUpload.ShowWindow(SW_HIDE);
			m_dlgTimeUpload.ShowWindow(SW_HIDE);
			m_dlgProgramUpload.ShowWindow(SW_HIDE);
			m_dlgAlarms.ShowWindow(SW_HIDE);
			m_dlgFileUpload.ShowWindow(SW_HIDE);
			m_dlgFileDownload.ShowWindow(SW_NORMAL);
			if (m_bConnected)
				m_dlgFileDownload.EnableControls(!m_strIPAddr.IsEmpty());
			break;
	}
}

BEGIN_MESSAGE_MAP(CIrrigationUploaderUDPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ACTIVITY, &CIrrigationUploaderUDPDlg::OnSelchangeTabActivity)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CIrrigationUploaderUDPDlg::OnClickedButtonConnect)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS, &CIrrigationUploaderUDPDlg::OnFieldchangedIpaddress)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CIrrigationUploaderUDPDlg message handlers

BOOL CIrrigationUploaderUDPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_NORMAL);
	m_tabActivity.InsertItem(0, "Update Wifi Details");
	m_tabActivity.InsertItem(1, "Update Email Details");
	m_tabActivity.InsertItem(2, "Update Time Details");
	m_tabActivity.InsertItem(3, "Update Irrigation Programs");
	m_tabActivity.InsertItem(4, "Update Wired Alarms");
	m_tabActivity.InsertItem(5, "Upload Files");
	m_tabActivity.InsertItem(6, "Download Files");
	ChangeTab(0);
	PositionChildWindows();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIrrigationUploaderUDPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIrrigationUploaderUDPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIrrigationUploaderUDPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIrrigationUploaderUDPDlg::OnSelchangeTabActivity(NMHDR *pNMHDR, LRESULT *pResult)
{
	ChangeTab(m_tabActivity.GetCurSel());
	*pResult = 0;
}

bool CIrrigationUploaderUDPDlg::TryConnect(LPCSTR lpszIPAddr)
{
	bool bResult = false;
	CString strResp, strIPAddr = m_strIPAddr, strNum, strMsg = "Could not connect to '" + m_strIPAddr + "'";
	UINT nPort = m_nPort;
	CProgressDlg dlg(GetParent());

	dlg.SetProgress(25, "Checking if '" + m_strIPAddr + "' is an irrigation controller.");
	if (m_Socket.Request("irrigation", strResp, m_strIPAddr, m_nPort))
	{
		if (strResp == "irrigation")
		{
			dlg.SetProgress(50, "Fetching name from '" + m_strIPAddr + "'.");
			if (m_Socket.Request("id", strResp, m_strIPAddr, m_nPort))
			{
				dlg.SetProgress(75, "Connection to '" + m_strIPAddr + "' was successful!");
				strNum.Format("%d", m_nPort);
				strMsg = "Successfully connected to '" + strResp + "' at '" + m_strIPAddr + "' on port " + strNum + "!";
				dlg.Close();
				m_staticStatus.SetWindowTextA(strMsg);
				ChangeTab(m_tabActivity.GetCurSel());
				bResult = true;
			}
		}
	}
	m_staticStatus.SetWindowTextA(strMsg);

	return bResult;
}

void CIrrigationUploaderUDPDlg::OnClickedButtonConnect()
{
	if (!m_strIPAddr.IsEmpty())
	{
		m_staticStatus.SetWindowTextA("Attempting to connect to '" + m_strIPAddr + "'");

		m_dlgFileUpload.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgWifiUpload.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgEmailUpload.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgTimeUpload.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgProgramUpload.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgAlarms.SetIPAddr(m_strIPAddr, m_nPort);
		m_dlgFileDownload.SetIPAddr(m_strIPAddr, m_nPort);
		m_bConnected = true;
		if (!TryConnect(m_strIPAddr))
		{
			AfxMessageBox("Could not connect to '" + m_strIPAddr + "'!\nAre you sure this is the correct IP address of your irrigation controller?", MB_OK);
		}
	}
	else
		AfxMessageBox("Please enter the IP address of your irriigation controller!", MB_OK);
}


void CIrrigationUploaderUDPDlg::OnFieldchangedIpaddress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	m_ipaddrIP.GetWindowTextA(m_strIPAddr);
	m_bConnected = false;
	*pResult = 0;
}


int CIrrigationUploaderUDPDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	AfxInitRichEdit();

	m_Socket.Create();

	m_dlgFileUpload.Create(IDD_DIALOG_UPLOAD_FILE, this);
	m_dlgFileUpload.ShowWindow(SW_HIDE);
	m_dlgFileUpload.SetSocket(&m_Socket);

	m_dlgWifiUpload.Create(IDD_DIALOG_WIFI, this);
	m_dlgWifiUpload.ShowWindow(SW_HIDE);
	m_dlgWifiUpload.SetSocket(&m_Socket);

	m_dlgEmailUpload.Create(IDD_DIALOG_EMAIL, this);
	m_dlgEmailUpload.ShowWindow(SW_HIDE);
	m_dlgEmailUpload.SetSocket(&m_Socket);

	m_dlgTimeUpload.Create(IDD_DIALOG_TIME, this);
	m_dlgTimeUpload.ShowWindow(SW_HIDE);
	m_dlgTimeUpload.SetSocket(&m_Socket);

	m_dlgProgramUpload.Create(IDD_DIALOG_PROGRAMS, this);
	m_dlgProgramUpload.ShowWindow(SW_HIDE);
	m_dlgProgramUpload.SetSocket(&m_Socket);

	m_dlgAlarms.Create(IDD_DIALOG_WALARMS, this);
	m_dlgAlarms.ShowWindow(SW_HIDE);
	m_dlgAlarms.SetSocket(&m_Socket);

	m_dlgFileDownload.Create(IDD_DIALOG_DOWNLOAD_FILE, this);
	m_dlgFileDownload.ShowWindow(SW_HIDE);
	m_dlgFileDownload.SetSocket(&m_Socket);

	return 0;
}
