// DownloadFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "IrrigationUploader.h"
#include "DownloadFileDlg.h"
#include "afxdialogex.h"
#include "ProgressDlg.h"

// CDownloadFileDialog dialog

IMPLEMENT_DYNAMIC(CDownloadFileDialog, CDialogEx)

CDownloadFileDialog::CDownloadFileDialog(CWnd* pParent /*=NULL*/): CDialogEx(IDD_DIALOG_DOWNLOAD_FILE, pParent)
{

}

CDownloadFileDialog::~CDownloadFileDialog()
{
}

void CDownloadFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD, m_buttonDownload);
	DDX_Control(pDX, IDC_COMBO_FILENAME, m_comboFilename);
	DDX_Control(pDX, IDC_STATIC_CONTENTS, m_staticContents);
	DDX_Control(pDX, IDC_STATIC_FLE, m_staticFile);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDC_EDIT_FILE_CONTENTS, m_editFileContents);
}


BEGIN_MESSAGE_MAP(CDownloadFileDialog, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_FILENAME, &CDownloadFileDialog::OnSelchangeComboFilename)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CDownloadFileDialog::OnClickedButtonDownload)
END_MESSAGE_MAP()


// CDownloadFileDialog message handlers


BOOL CDownloadFileDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDownloadFileDialog::EnableControls(const bool bEnable)
{
	m_buttonDownload.EnableWindow(bEnable && (m_comboFilename.GetCurSel() >= 0));
	m_comboFilename.EnableWindow(bEnable);
	m_editFileContents.EnableWindow(bEnable);
	m_staticContents.EnableWindow(bEnable);
	m_staticFile.EnableWindow(bEnable);
	m_editFileContents.EnableWindow(bEnable);
}

void CDownloadFileDialog::OnSelchangeComboFilename()
{
	m_buttonDownload.EnableWindow(m_comboFilename.GetCurSel() >= 0);
}



void CDownloadFileDialog::OnClickedButtonDownload()
{
	CSerialPort SerialPort;
	CStdioFile file;
	CString strFilename, strReq, strResp, strContents, strFilePos;
	DWORD nFileSize = 0, nFilePos = 0;
	CProgressDlg dlg;
	int nProgress = 0, nRetries = 0;

	m_comboFilename.GetWindowTextA(strFilename);

	if (SerialPort.Open(m_COMPort))
	{
		m_comboFilename.GetWindowTextA(strFilename);
		m_editFileContents.SetWindowTextA("");
		m_staticStatus.SetWindowTextA("Downloading '" + strFilename + "'!");

		strReq = CString("filesize") + SerialPort.GetDelim() + strFilename;
		if (SerialPort.Request(strReq, nFileSize))
		{
			dlg.SetProgress(nProgress, "Downloading file '" + strFilename + "'");
			while (nFilePos < nFileSize)
			{
				strFilePos.Format("%d", nFilePos);
				strReq = CString("download") + SerialPort.GetDelim() + strFilename + SerialPort.GetDelim() + strFilePos;
				if (SerialPort.Request(strReq, strResp))
				{
					nRetries = 0;
					if (strResp.Find("<ERROR>") > -1)
					{
						strResp.Delete(0, 6);
						m_staticStatus.SetWindowTextA(strResp);
					}
					else
					{
						nFilePos += strResp.GetLength();
						m_editFileContents.GetWindowTextA(strContents);
						strContents += strResp;
						m_editFileContents.SetWindowTextA(strContents);
						nProgress = ((float)nFilePos / nFileSize) * 100;
						dlg.SetProgress(nProgress);

						if (nFilePos == nFileSize)
						{
							dlg.Close();
							m_staticStatus.SetWindowTextA("Download of '" + strFilename + "' was successful!");
						}
					}
				}
				else
				{
					if (nRetries < 100)
						nRetries++;
					else
					{
						m_staticStatus.SetWindowTextA("Download of '" + strFilename + "' was unsuccessful!");
						break;
					}
				}
			}
		}
		else
		{
			m_staticStatus.SetWindowTextA("Download of '" + strFilename + "' was unsuccessful!");
		}
	}
}
