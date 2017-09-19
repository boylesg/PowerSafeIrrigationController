#include "MainMenu.h"
#include "Devices.h"
#include "Debug.h"
#include "Common.h"
#include "StationSettings.h"
#include "ControllerSettings.h"
#include "ProbeSettings.h"
#include "WifiSettings.h"




CMainMenu::CMainMenu(CTFTLCDScreen &tftlcd, CWifiManager *pWifiManager): CDialogBox(&tftlcd.m_LCD), m_buttonControllerSettings(&tftlcd.m_LCD), m_buttonProbeSettings(&tftlcd.m_LCD), 
                                                                          m_strIPAddr(m_buffIPAddr), m_buttonWifiSettings(&tftlcd.m_LCD), m_buttonStationSettings(&tftlcd.m_LCD), m_tftlcd(tftlcd)
{
	setTitle(F("Main menu"), WHITE, 3);
	setBGColor(RGB(0, 70, 55));
	setBorderColor(RGB(0, 90, 75));
	setCornerRadius(10);
	setBorderWidth(5);
	setOrientation(LANDSCAPE2);
	m_pWifiManager = pWifiManager;
}


CMainMenu::~CMainMenu()
{
}

void CMainMenu::setIPAddr(IPAddress IPAddr)
{
  m_strIPAddr = fromUint(IPAddr[0], 10);
  m_strIPAddr += F(".");
  m_strIPAddr += fromUint(IPAddr[1], 10);
  m_strIPAddr += F(".");
  m_strIPAddr += fromUint(IPAddr[2], 10);
  m_strIPAddr += F(".");
  m_strIPAddr += fromUint(IPAddr[3], 10);
}

void CMainMenu::restore()
{
  if (m_pDlgBox && (m_pDlgBox != this))
    delete m_pDlgBox;
  m_pDlgBox = NULL;
  show();
}

void CMainMenu::begin()
{  
  uint8_t nFontSize = 2, nButtonSpacing = MARGIN;
  uint16_t nButtonTextColor = WHITE, nButtonTextDisabledColor = LIGHTGREY,
		   nWidth = 250, nHeight = 35, nCornerRadius = nHeight / 2, nX = (m_pLCD->height() - nWidth) / 2, nY = 50;

  m_buttonControllerSettings.setPos(nX, nY);
  m_buttonControllerSettings.setSize(nWidth, nHeight);
  m_buttonControllerSettings.setColors(RGB(0, 70, 80), RGB(0, 90, 100));
  m_buttonControllerSettings.setDisabledColors(RGB(0, 40, 60), RGB(0, 60, 80));
  m_buttonControllerSettings.setText(F("Select Controller"));
  m_buttonControllerSettings.setFont(nFontSize, nButtonTextColor, nButtonTextDisabledColor);
  m_buttonControllerSettings.setCornerRadius(nCornerRadius);
  m_buttonControllerSettings.setParent(this);
  add(&m_buttonControllerSettings);

  nY += nHeight + nButtonSpacing;
  m_buttonStationSettings.setPos(nX, nY);
  m_buttonStationSettings.setSize(nWidth, nHeight);
  m_buttonStationSettings.setColors(RGB(80, 10, 10), RGB(95, 10, 10));
  m_buttonStationSettings.setDisabledColors(RGB(80, 20, 20), RGB(95, 20, 20));
  m_buttonStationSettings.setText(F("Station Settings"));
  m_buttonStationSettings.setFont(nFontSize, nButtonTextColor, nButtonTextDisabledColor);
  m_buttonStationSettings.setCornerRadius(nCornerRadius);
  m_buttonStationSettings.setParent(this);
  add(&m_buttonStationSettings);

  nY += nHeight + nButtonSpacing;
  m_buttonProbeSettings.setPos(nX, nY);
  m_buttonProbeSettings.setSize(nWidth, nHeight);
  m_buttonProbeSettings.setColors(RGB(80, 40, 60), RGB(95, 40, 85));
  m_buttonProbeSettings.setDisabledColors(RGB(40, 40, 30), RGB(60, 40, 50));
  m_buttonProbeSettings.setText(F("Probe Settings"));
  m_buttonProbeSettings.setFont(nFontSize, nButtonTextColor, nButtonTextDisabledColor);
  m_buttonProbeSettings.setCornerRadius(nCornerRadius);
  m_buttonProbeSettings.setParent(this);
  add(&m_buttonProbeSettings);

  nY += nHeight + nButtonSpacing;
  m_buttonWifiSettings.setPos(nX, nY);
  m_buttonWifiSettings.setSize(nWidth, nHeight);
  m_buttonWifiSettings.setColors(RGB(90, 40, 1), RGB(100, 40, 1));
  m_buttonWifiSettings.setDisabledColors(RGB(70, 40, 10), RGB(80, 40, 10));
  m_buttonWifiSettings.setText(F("Wifi Settings"));
  m_buttonWifiSettings.setFont(nFontSize, nButtonTextColor, nButtonTextDisabledColor);
  m_buttonWifiSettings.setCornerRadius(nCornerRadius);
  m_buttonWifiSettings.setParent(this);
  add(&m_buttonWifiSettings);
}


CDialogBox *CMainMenu::createControllerSettings()
{
  CControllerSettings *pDlg = new CControllerSettings(m_pLCD, m_pWifiManager);
  if (pDlg)
  {
    pDlg->setParent(this);
    pDlg->begin();
  }
  return pDlg;
}

CDialogBox *CMainMenu::createProbeSettings()
{
  CProbeSettings *pDlg = new CProbeSettings(m_pLCD, m_pWifiManager);
  if (pDlg)
  {
    pDlg->setParent(this);
    pDlg->begin();
  }
  return pDlg;
}

CDialogBox *CMainMenu::createWifiSettings()
{
  CWifiSettings *pDlg = new CWifiSettings(m_pLCD, m_pWifiManager);
  if (pDlg)
  {
    pDlg->setParent(this);
    pDlg->begin();
  }
  return pDlg;
}

CDialogBox *CMainMenu::createStationSettings()
{
	CStationSettings *pDlg = new CStationSettings(m_pLCD, m_pWifiManager);
	if (pDlg)
	{
		pDlg->setParent(this);
		pDlg->begin();
	}
	return pDlg;
}

void CMainMenu::processResponse(CControl *pCtrl)
{
  if (pCtrl == &m_buttonControllerSettings)
	{
		m_pDlgBox = createControllerSettings();
	}
	else if (pCtrl == &m_buttonProbeSettings)
	{
		m_pDlgBox = createProbeSettings();
	}
	else if (pCtrl == &m_buttonWifiSettings)
	{
		m_pDlgBox = createWifiSettings();
	}
	else if (pCtrl == &m_buttonStationSettings)
	{
		m_pDlgBox = createStationSettings();
	}
  show();
}

bool CMainMenu::pollTouch()
{
  TSPoint point;
  bool bResult = false;

  if (m_tftlcd.pollTouch(point))
  {
    bResult = true;
    
    if (m_pDlgBox && m_pDlgBox->processTouch(point, m_pLCD->width(), m_pLCD->height()))
    {
      //debug.log(F("111111"));
      while (m_tftlcd.m_TS.isTouching());
    }
    else if (processTouch(point, m_pLCD->width(), m_pLCD->height()))
    {
      //debug.log(F("222222"));
    }
  }
  return bResult;
}

void CMainMenu::show()
{
  if (m_pDlgBox)
    m_pDlgBox->show();
  else
  {
    CDialogBox::show();
    m_pLCD->setCursor(10, 216);
    m_pLCD->setTextColor(1);
    m_pLCD->setTextColor(m_nTitleColor);
    m_pLCD->print(F("IP address: "));
    m_pLCD->print(m_strIPAddr);
  }
}

