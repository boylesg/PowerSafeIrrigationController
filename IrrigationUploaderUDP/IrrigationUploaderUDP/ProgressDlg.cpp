// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploaderUDP.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"
#include "IrrigationSocket.h"

// CProgressDlg dialog

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/): CDialogEx(IDD_DIALOG_PROGRESS, pParent)
{
	Create(IDD_DIALOG_PROGRESS, pParent);
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::SetProgress(byte nPercentProgress, LPCSTR lpszMessage)
{
	if (lpszMessage != NULL)
		m_staticMessage.SetWindowTextA(lpszMessage);

	if (nPercentProgress > 100)
		nPercentProgress = 100;
	m_progress.SetPos(nPercentProgress);
}

void CProgressDlg::Close(byte nPercentProgress, LPCSTR lpszMessage)
{
	SetProgress(nPercentProgress, lpszMessage);
	Sleep(1000);
	DestroyWindow();
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_staticMessage);
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
END_MESSAGE_MAP()


// CProgressDlg message handlers


BOOL CProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ShowWindow(SW_NORMAL);
	CenterWindow(GetParent());
	m_progress.SetRange(0, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

