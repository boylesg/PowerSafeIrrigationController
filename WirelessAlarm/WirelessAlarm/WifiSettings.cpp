#include "WifiSettings.h"
#include "Devices.h"
#include "Debug.h"





CWifiSettings::CWifiSettings(Adafruit_TFTLCD *pLCD, CWifiManager *pWifiManager): CDialogBox(pLCD), m_labelName(pLCD), m_labelKey(pLCD), m_textName(pLCD), 
																				m_textKey(pLCD), m_keyboard(pLCD), m_buttonSave(pLCD), m_buttonBack(pLCD)
{
  setTitle(F("Wifi Settings"), WHITE, 2);
  setOrientation(LANDSCAPE2);
  setBGColor(RGB(90, 40, 1));
  setBorderColor(RGB(100, 40, 1));
  setCornerRadius(10);
  setBorderWidth(5);
  m_nLastCtrl = 1;
  m_pWifiManager = pWifiManager;
}

CWifiSettings::~CWifiSettings()
{
}

void CWifiSettings::begin()
{
  CPoint point;
  uint16_t nHeight = 24, nRadius = 5, nFontSize = 2, nWidth = 294, nX = 15;
 
  m_keyboard.setSize(0, 30);
  m_keyboard.setChars(F("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789~`!@#$%^&*()-_=+{[}]|\\:;\"'<,>.?/"));
  m_keyboard.setColors(RGB(0, 65, 10), RGB(0, 85, 10));
  m_keyboard.setDisabledColors(RGB(30, 65, 10), RGB(30, 85, 10));
  m_keyboard.setFontColors(WHITE, LIGHTGREY);
  m_keyboard.setFontSize(3);
  m_keyboard.setCornerRadius(nRadius);
  m_keyboard.setParent(this);
  m_keyboard.setBuddy(&m_textName, m_textName.getBuddyStr());
  m_keyboard.init();
  add(&m_keyboard);

  point.set(nX, 57);
  m_labelName.setPos(point); 
  m_labelName.setSize(120, nHeight);
  m_labelName.setText(F("Network name"));
  m_labelName.setFontColors(WHITE, LIGHTGREY); 
  m_labelName.setColors(getBGColor(), getBGColor()); 
  m_labelName.setCornerRadius(nRadius);
  m_labelName.setFontSize(nFontSize);
  m_labelName.setParent(this);
  add(&m_labelName);

  point.moveY(m_labelName.getHeight());
  m_textName.setPos(point);
  m_textName.setSize(nWidth, nHeight);
  m_textName.setFont(nFontSize, BLACK, GREY);
  m_textName.setColors(WHITE, GREY);
  m_textName.setText(m_pWifiManager->getNetworkName());
  m_textName.setDisabledColors(LIGHTGREY, GREY);
  m_textName.setCornerRadius(nRadius);
  m_textName.setFocus(true);
  m_textName.setParent(this);
  add(&m_textName);
  m_point1.set(m_textName.getLeft(), m_textName.getTop());
  m_point1.moveY(m_textName.getHeight() + MARGIN);

  point.moveY(m_textName.getHeight() + m_keyboard.getHeight() + MARGIN * 2);
  m_labelKey.setPos(point);
  m_labelKey.setSize(120, nHeight);
  m_labelKey.setText(F("Network key"));
  m_labelKey.setFontColors(WHITE, LIGHTGREY); 
  m_labelKey.setColors(getBGColor(), getBGColor()); 
  m_labelKey.setCornerRadius(nRadius);
  m_labelKey.setFontSize(nFontSize);
  m_labelKey.setParent(this);
  add(&m_labelKey);

  point.moveY(m_labelKey.getHeight());
  m_textKey.setPos(point);
  m_textKey.setSize(nWidth, nHeight);
  m_textKey.setFont(nFontSize, BLACK, GREY);
  m_textKey.setColors(WHITE, GREY);
  m_textKey.setText(m_pWifiManager->getNetworkKey());
  m_textKey.setDisabledColors(LIGHTGREY, GREY);
  m_textKey.setCornerRadius(nRadius);
  m_textKey.setFocus(false);
  m_textKey.setParent(this);
  add(&m_textKey);

  m_point2.set(m_textKey.getLeft(), m_textKey.getTop());
  m_point2.moveY(m_textKey.getHeight() + MARGIN);

  m_buttonSave.setText(F("Save"));
  m_buttonSave.setSize(90, 30);
  m_buttonSave.setPos(m_pLCD->width() - MARGIN * 2 - m_buttonSave.getWidth(), m_textName.getTop() + MARGIN + m_textName.getHeight());
  m_buttonSave.setColors(RGB(0, 65, 10), RGB(0, 85, 10));
  m_buttonSave.setDisabledColors(RGB(30, 65, 10), RGB(30, 85, 10));
  m_buttonSave.setFont(2, WHITE, LIGHTGREY);
  m_buttonSave.setFontSize(2);
  m_buttonSave.setCornerRadius(nRadius);
  m_buttonSave.setParent(this);
  add(&m_buttonSave);

  m_buttonBack = m_buttonSave;
  m_buttonBack.setText(F("Back"));
  m_buttonBack.setPos(m_buttonSave.getLeft(), m_textKey.getTop() + MARGIN + m_textKey.getHeight());
  add(&m_buttonBack);

  m_keyboard.setPos(m_point1);
  m_keyboard.init();

  m_nLastCtrl = 2;
}

void CWifiSettings::moveKeyboard(CPoint &point)
{
  m_keyboard.setPos(point);
  show();
}

void CWifiSettings::doSave()
{
	if (m_pWifiManager)
		m_pWifiManager->saveWifiDetails(m_textName.getText(), m_textKey.getText());
}
void CWifiSettings::processResponse(CControl *pCtrl)
{
  if (pCtrl == &m_textName)
  {
    m_keyboard.hide();
    m_keyboard.setPos(m_point1);
    m_keyboard.init();
    m_keyboard.show();
  }
  else if (pCtrl == &m_textKey)
  {
    m_keyboard.hide();
    m_keyboard.setPos(m_point2);
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


