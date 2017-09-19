#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CProgressDlg dialog

class CProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgressDlg)

	DECLARE_MESSAGE_MAP()
	
	public:
		CProgressDlg(CWnd* pParent = NULL);   // standard constructor
		virtual ~CProgressDlg();


		// Dialog Data
		#ifdef AFX_DESIGN_TIME
			enum { IDD = IDD_DIALOG_PROGRESS };
		#endif

		// Overrides
		virtual BOOL OnInitDialog();

		// Interface
		void SetProgress(byte nPercentProgress, LPCSTR lpszMessage = NULL);
		void Close(byte nPercentProgress = 100, LPCSTR lpszMessage = "Done!");

	protected:

		// Data exchange and validation
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		// Controls
		CProgressCtrl m_progress;
		CStatic m_staticMessage;

		// Data
		bool m_bMarquee;
//		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	CProgressCtrl m_progressMarquee;
};
