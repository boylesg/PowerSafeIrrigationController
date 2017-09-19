#include "StationSettings.h"
#include "Devices.h"
#include "Debug.h"





CStationSettings::CStationSettings(Adafruit_TFTLCD *pLCD, CWifiManager *pWifiManager): CDialogBox(pLCD), m_labelStationNum(pLCD), m_labelStationDesc(pLCD), m_textStationNum(pLCD), 
																						m_textStationDesc(pLCD), m_keyboard(pLCD), m_buttonSave(pLCD), m_buttonBack(pLCD)
{
  setTitle(F("Station Settings"), WHITE, 2);
  setOrientation(LANDSCAPE2);
  setBGColor(RGB(80, 10, 10));
  setBorderColor(RGB(95, 10, 10));
  setCornerRadius(10);
  setBorderWidth(5);
  m_nLastCtrl = 1;
  m_pWifiManager = pWifiManager;
}

CStationSettings::~CStationSettings()
{
}

void CStationSettings::begin()
{
  CPoint point;
  uint16_t nHeight = 24, nRadius = 5, nFontSize = 2, nWidth = 294, nX = 15;
 
  m_keyboard.setSize(0, 30);
  m_keyboard.setChars(F("0123456789"));
  m_keyboard.setColors(RGB(10, 80, 10), RGB(10, 95, 10));
  m_keyboard.setDisabledColors(RGB(20, 80, 20), RGB(20, 95, 20));
  m_keyboard.setFontColors(WHITE, LIGHTGREY);
  m_keyboard.setFontSize(3);
  m_keyboard.setCornerRadius(nRadius);
  m_keyboard.setParent(this);
  m_keyboard.setBuddy(&m_textStationNum, m_textStationNum.getBuddyStr());
  m_keyboard.init();
  add(&m_keyboard);

  point.set(nX, 57);
  m_labelStationNum.setPos(point); 
  m_labelStationNum.setSize(120, nHeight);
  m_labelStationNum.setText(F("Station Number"));
  m_labelStationNum.setFontColors(WHITE, LIGHTGREY); 
  m_labelStationNum.setColors(getBGColor(), getBGColor()); 
  m_labelStationNum.setCornerRadius(nRadius);
  m_labelStationNum.setFontSize(nFontSize);
  m_labelStationNum.setParent(this);
  add(&m_labelStationNum);

  point.moveY(m_labelStationNum.getHeight());
  m_textStationNum.setPos(point);
  m_textStationNum.setSize(nWidth, nHeight);
  m_textStationNum.setFont(nFontSize, BLACK, GREY);
  m_textStationNum.setText(fromUint(m_pWifiManager->getStationNum(), 10));
  m_textStationNum.setColors(WHITE, GREY);
  m_textStationNum.setDisabledColors(LIGHTGREY, GREY);
  m_textStationNum.setCornerRadius(nRadius);
  m_textStationNum.setFocus(true);
  m_textStationNum.setParent(this);
  add(&m_textStationNum);
  m_point1.set(m_textStationNum.getLeft(), m_textStationNum.getTop());
  m_point1.moveY(m_textStationNum.getHeight() + MARGIN);

  point.moveY(m_textStationNum.getHeight() + m_keyboard.getHeight() + MARGIN * 2);
  m_labelStationDesc.setPos(point);
  m_labelStationDesc.setText(F("Station Description"));
  m_labelStationDesc.setSize(120, nHeight);
  m_labelStationDesc.setFontColors(WHITE, LIGHTGREY); 
  m_labelStationDesc.setColors(getBGColor(), getBGColor()); 
  m_labelStationDesc.setCornerRadius(nRadius);
  m_labelStationDesc.setFontSize(nFontSize);
  m_labelStationDesc.setParent(this);
  add(&m_labelStationDesc);

  point.moveY(m_labelStationDesc.getHeight());
  m_textStationDesc.setPos(point);
  m_textStationDesc.setSize(nWidth, nHeight);
  m_textStationDesc.setFont(nFontSize, BLACK, GREY);
  m_textStationDesc.setColors(WHITE, GREY);
  m_textStationDesc.setText(m_pWifiManager->getStationDesc());     
  m_textStationDesc.setDisabledColors(LIGHTGREY, GREY);
  m_textStationDesc.setCornerRadius(nRadius);
  m_textStationDesc.setFocus(false);
  m_textStationDesc.setParent(this);
  add(&m_textStationDesc);

  m_point2.set(m_textStationDesc.getLeft(), m_textStationDesc.getTop());
  m_point2.moveY(m_textStationDesc.getHeight() + MARGIN);

  m_buttonSave.setText(F("Save"));
  m_buttonSave.setSize(90, 30);
  m_buttonSave.setPos(m_pLCD->width() - MARGIN * 2 - m_buttonSave.getWidth(), m_textStationNum.getTop() + MARGIN + m_textStationNum.getHeight());
  m_buttonSave.setColors(RGB(0, 65, 10), RGB(0, 85, 10));
  m_buttonSave.setDisabledColors(RGB(30, 65, 10), RGB(30, 85, 10));
  m_buttonSave.setFont(2, WHITE, LIGHTGREY);
  m_buttonSave.setFontSize(2);
  m_buttonSave.setCornerRadius(nRadius);
  m_buttonSave.setParent(this);
  add(&m_buttonSave);

  m_buttonBack = m_buttonSave;
  m_buttonBack.setText(F("Back"));
  m_buttonBack.setPos(m_buttonSave.getLeft(), m_textStationDesc.getTop() + MARGIN + m_textStationDesc.getHeight());
  add(&m_buttonBack);

  m_keyboard.setPos(m_point1);
  m_keyboard.init();

  m_nLastCtrl = 2;
}

void CStationSettings::moveKeyboard(CPoint &point)
{
  m_keyboard.setPos(point);
  show();
}

void CStationSettings::doSave()
{
	if (m_pWifiManager)
	{
		CBuff<5> buff;
		CString str(buff);
		str = m_textStationNum.getText();
		m_pWifiManager->saveStationDetails(str.toUint(), m_textStationDesc.getText());
	}
}
void CStationSettings::processResponse(CControl *pCtrl)
{
  if (pCtrl == &m_textStationNum)
  {
    m_keyboard.hide();
    m_keyboard.setPos(m_point1);
	m_keyboard.setChars(F("0123456789"));
    m_keyboard.init();
    m_keyboard.show();
  }
  else if (pCtrl == &m_textStationDesc)
  {
    m_keyboard.hide();
    m_keyboard.setPos(m_point2);
	m_keyboard.setChars(F("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789"));
    m_keyboard.init();
    m_keyboard.show();
  }
  else if (pCtrl == &m_buttonSave)
  {
    doSave();
  }
  else if ((pCtrl == &m_buttonBack) && m_pParent)
  {
    hide();
    m_pParent->restore();
  }
  m_keyboard.setBuddy(pCtrl, pCtrl->getBuddyStr());
}


