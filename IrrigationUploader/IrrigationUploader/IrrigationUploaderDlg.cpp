
// IrrigationUploaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploader.h"
#include "IrrigationUploaderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
	public:
		CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		// Implementation
		DECLARE_MESSAGE_MAP()
};




CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()




IMPLEMENT_DYNAMIC(CIrrigationUploaderDlg, CDialogEx)

CIrrigationUploaderDlg::CIrrigationUploaderDlg(CWnd* pParent /*=NULL*/): CDialogEx(IDD_IRRIGATIONUPLOADER_DIALOG, pParent), m_dlgFileUpload(this), m_dlgWifiUpload(this), m_dlgEmailUpload(this), 
																		m_dlgTimeUpload(this), m_dlgProgramUpload(this), m_dlgAlarms(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIrrigationUploaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOBOX_COM_PORT, m_comboboxCOMPort);
	DDX_Control(pDX, IDC_TAB_ACTIVITY, m_tabActivity);
	DDX_Control(pDX, IDC_STATIC_LOADING, m_staticLoading);
}

void CIrrigationUploaderDlg::GetCOMPorts()
{
	m_staticLoading.SetWindowText("Searching for irrigation controllers - please wait...");

	while (m_comboboxCOMPort.GetCount() > 0)
		m_comboboxCOMPort.DeleteItem(0);
	m_arrayCOMPorts.RemoveAll();

	CSerialPort::GetCOMPorts(m_arrayCOMPorts, this);
	COMBOBOXEXITEM  item;
	char strBuff[33];
	ZeroMemory(&item, sizeof(item));
	item.mask = CBEIF_TEXT;
	item.iItem = 0;
	
	for (int nI = 0; nI < m_arrayCOMPorts.GetSize(); nI++)
	{
		memset(strBuff, 0, 33);
		strcpy_s(strBuff, m_arrayCOMPorts[nI].m_strDesc);
		item.pszText = strBuff;
		m_comboboxCOMPort.InsertItem(&item);
		m_arrayCOMPorts[nI].m_strDesc.ReleaseBuffer();
	}
	if (m_arrayCOMPorts.GetCount() == 0)
		AfxMessageBox("No irrigation controllers were found. Please make sure that you have paired with the bluetooth device named 'IRRIGATION' in your computer's bluetooth settings.", MB_OK);
	m_staticLoading.SetWindowText("");
}

void CIrrigationUploaderDlg::CenterChildWindow(CWnd* pParentWnd, CWnd* pChildWnd)
{
	CRect rectChildWnd;

	pChildWnd->CenterWindow(pParentWnd);
	pChildWnd->GetWindowRect(rectChildWnd);
	pParentWnd->ScreenToClient(rectChildWnd);
	pChildWnd->MoveWindow(rectChildWnd.left + 11, rectChildWnd.top + 52, rectChildWnd.Width(), rectChildWnd.Height());
}

void CIrrigationUploaderDlg::PositionChildWindows()
{
	CenterChildWindow(&m_tabActivity, &m_dlgFileUpload);
	CenterChildWindow(&m_tabActivity, &m_dlgWifiUpload);
	CenterChildWindow(&m_tabActivity, &m_dlgEmailUpload);
	CenterChildWindow(&m_tabActivity, &m_dlgTimeUpload);
	CenterChildWindow(&m_tabActivity, &m_dlgProgramUpload);
	CenterChildWindow(&m_tabActivity, &m_dlgAlarms);
	CenterChildWindow(&m_tabActivity, &m_dlgFileDownload);
}

void CIrrigationUploaderDlg::ChangeTab(const int nSelTab)
{
	switch (nSelTab)
	{
	case 0:
		m_dlgWifiUpload.ShowWindow(SW_NORMAL);
		m_dlgWifiUpload.InitEditFields(m_COMPort);
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
		m_dlgEmailUpload.InitEditFields(m_COMPort);
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
		m_dlgTimeUpload.InitEditFields(m_COMPort);
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
		m_dlgProgramUpload.InitFields(m_COMPort);
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
		m_dlgAlarms.InitFields(m_COMPort);
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
		m_dlgFileUpload.SetCOMPort(m_COMPort);
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
		m_dlgFileDownload.SetCOMPort(m_COMPort);
		break;
	}
}

BEGIN_MESSAGE_MAP(CIrrigationUploaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBOBOX_COM_PORT, &CIrrigationUploaderDlg::OnSelchangeComboboxComPort)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ACTIVITY, &CIrrigationUploaderDlg::OnSelchangeTabActivity)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CIrrigationUploaderDlg::OnClickedButtonRefresh)
END_MESSAGE_MAP()


// CIrrigationUploaderDlg message handlers

BOOL CIrrigationUploaderDlg::OnInitDialog()
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
	GetCOMPorts();
	m_tabActivity.InsertItem(0, "Update Wifi Details");
	m_tabActivity.InsertItem(1, "Update Email Details");
	m_tabActivity.InsertItem(2, "Update Time Details");
	m_tabActivity.InsertItem(3, "Update Irrigation Programs");
	m_tabActivity.InsertItem(4, "Update Wired Alarms");
	m_tabActivity.InsertItem(5, "Upload Files");
	m_tabActivity.InsertItem(6, "Download Files");
	ChangeTab(0);
	PositionChildWindows();

	//ShowWindow(SW_MAXIMIZE);
	ShowWindow(SW_NORMAL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIrrigationUploaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIrrigationUploaderDlg::OnPaint()
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
HCURSOR CIrrigationUploaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIrrigationUploaderDlg::OnSelchangeComboboxComPort()
{

	m_COMPort = m_arrayCOMPorts[m_comboboxCOMPort.GetCurSel()];

	m_dlgFileUpload.EnableControls(!m_arrayCOMPorts.IsEmpty());
	m_dlgWifiUpload.EnableControls(!m_arrayCOMPorts.IsEmpty());
	m_dlgEmailUpload.EnableControls(!m_arrayCOMPorts.IsEmpty());
	m_dlgTimeUpload.EnableControls(!m_arrayCOMPorts.IsEmpty());
	m_dlgAlarms.EnableControls(!m_arrayCOMPorts.IsEmpty());
	m_dlgFileDownload.EnableControls(!m_arrayCOMPorts.IsEmpty());
	ChangeTab(m_tabActivity.GetCurSel());
}


void CIrrigationUploaderDlg::OnSelchangeTabActivity(NMHDR *pNMHDR, LRESULT *pResult)
{
	ChangeTab(m_tabActivity.GetCurSel());
	*pResult = 0;
}


int CIrrigationUploaderDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	AfxInitRichEdit();
	m_dlgFileUpload.Create(IDD_DIALOG_UPLOAD_FILE, this);
	m_dlgFileUpload.ShowWindow(SW_HIDE);

	m_dlgWifiUpload.Create(IDD_DIALOG_WIFI, this);
	m_dlgWifiUpload.ShowWindow(SW_HIDE);

	m_dlgEmailUpload.Create(IDD_DIALOG_EMAIL, this);
	m_dlgEmailUpload.ShowWindow(SW_HIDE);

	m_dlgTimeUpload.Create(IDD_DIALOG_TIME, this);
	m_dlgTimeUpload.ShowWindow(SW_HIDE);

	m_dlgProgramUpload.Create(IDD_DIALOG_PROGRAMS, this);
	m_dlgProgramUpload.ShowWindow(SW_HIDE);

	m_dlgAlarms.Create(IDD_DIALOG_WALARMS, this);
	m_dlgAlarms.ShowWindow(SW_HIDE);

	m_dlgFileDownload.Create(IDD_DIALOG_DOWNLOAD_FILE, this);
	m_dlgFileDownload.ShowWindow(SW_HIDE);

	return 0;
}

void CIrrigationUploaderDlg::OnClickedButtonRefresh()
{
	GetCOMPorts();
}
