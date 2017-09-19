#pragma once

#include "DialogBox.h"
#include "Button.h"
#include "WifiManager.h"




class CMainMenu: public CDialogBox
{
	public:
		// Construction, destruction and initialisation
		CMainMenu(CTFTLCDScreen &tftlcd, CWifiManager *pWifiManager);
		virtual ~CMainMenu();

		// Overrides
		virtual void processResponse(CControl *pCtrl);
		virtual void begin();
		virtual void show();
		virtual void restore();

		// Interface
		bool pollTouch();
		void setIPAddr(IPAddress IPAddr);

	protected:
		// Data
		CButton<24> m_buttonControllerSettings, m_buttonStationSettings, m_buttonProbeSettings, m_buttonWifiSettings;
		CDialogBox *m_pDlgBox;
		CTFTLCDScreen &m_tftlcd;
		CWifiManager *m_pWifiManager;

		CBuff<16> m_buffIPAddr;
		CString m_strIPAddr;

		// Helpers
		bool displayFound(const uint16_t nIdentifier);
		CDialogBox *createControllerSettings();
		CDialogBox *createProbeSettings();
		CDialogBox *createWifiSettings();
		CDialogBox *createStationSettings();
};

