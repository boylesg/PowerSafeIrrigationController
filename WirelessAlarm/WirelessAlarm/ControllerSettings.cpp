#include "ControllerSettings.h"
#include "Devices.h"
#include "Debug.h"





CControllerSettings::CControllerSettings(Adafruit_TFTLCD *pLCD, CWifiManager *pWifiManager):
	CDialogBox(pLCD), m_labelIP(pLCD), m_labelID(pLCD), m_labelDot1(pLCD), m_labelDot2(pLCD), m_labelDot3(pLCD),
	m_textIP1(pLCD), m_textIP2(pLCD), m_textIP3(pLCD), m_textIP4(pLCD), m_textID(pLCD), m_keyboard(pLCD), m_buttonSave(pLCD), m_buttonBack(pLCD)
{
  setTitle(F("Select Controller"), WHITE, 2);
  setOrientation(LANDSCAPE2);
  setBGColor(RGB(0, 70, 80));
  setBorderColor(RGB(0, 90, 100));
  setCornerRadius(10);
  setBorderWidth(5);
  m_nLastCtrl = 1;
  m_pWifiManager = pWifiManager;
}

CControllerSettings::~CControllerSettings()
{
}

void CControllerSettings::begin()
{
  CPoint point, pointDot;
  uint8_t nHeight = 24, nRadius = 5, nFontSize = 2, nIPWidth = 60, nIPGap = 45, nDotGap = 2, nX = 15;
 
  point.set(nX, 57);
  m_labelIP.setPos(point); 
  m_labelIP.setSize(120, nHeight);  
  m_labelIP.setText(F("IP address")); 
  m_labelIP.setFontColors(WHITE, LIGHTGREY); 
  m_labelIP.setColors(getBGColor(), getBGColor()); 
  m_labelIP.setCornerRadius(nRadius);
  m_labelIP.setFontSize(nFontSize); 
  m_labelIP.setParent(this);
  add(&m_labelIP);

  point.moveY(m_labelIP.getHeight());
  m_textIP1.setPos(point);
  m_textIP1.setSize(nIPWidth, nHeight);
  m_textIP1.setFont(nFontSize, BLACK, GREY);
  m_textIP1.setColors(WHITE, GREY);
  m_textIP1.setText(m_pWifiManager->getIP(0));
  m_textIP1.setDisabledColors(LIGHTGREY, GREY);
  m_textIP1.setCornerRadius(nRadius);
  m_textIP1.setFocus(true);
  m_textIP1.setParent(this);
  add(&m_textIP1);
  m_point1.set(m_textIP1.getLeft(), m_textIP1.getTop());
  m_point1.moveY(m_textIP1.getHeight() + MARGIN);

  pointDot.set(m_textIP1.getRight() + nDotGap, m_textIP1.getBottom() - 6);
  m_labelDot1.setPos(pointDot);
  m_labelDot1.setText(".");
  m_labelDot1.setFontColors(WHITE, LIGHTGREY); 
  m_labelDot1.setColors(getBGColor(), getBGColor()); 
  m_labelDot1.setFontSize(2);
  m_labelDot1.setParent(this);
  add(&m_labelDot1);

  m_textIP4 = m_textIP3 = m_textIP2 = m_textIP1;
  m_labelDot3 = m_labelDot2 = m_labelDot1;
  
  point.moveX(nIPWidth / 2 + nIPGap);
  m_textIP2.setPos(point);
  m_textIP2.setFocus(false);
  m_textIP2.setText(m_pWifiManager->getIP(1));
  add(&m_textIP2);

  pointDot.set(m_textIP2.getRight() + nDotGap, m_textIP2.getBottom() - 6);
  m_labelDot2.setPos(pointDot);
  add(&m_labelDot2);

  point.moveX(nIPWidth / 2 + nIPGap);
  m_textIP3.setPos(point);
  m_textIP3.setFocus(false);
  m_textIP3.setText(m_pWifiManager->getIP(2));
  add(&m_textIP3);
  
  pointDot.set(m_textIP3.getRight() + nDotGap, m_textIP3.getBottom() - 6);
  m_labelDot3.setPos(pointDot);
  add(&m_labelDot3);

  point.moveX(nIPWidth / 2 + nIPGap);
  m_textIP4.setPos(point);
  m_textIP4.setFocus(false);
  m_textIP4.setText(m_pWifiManager->getIP(3));
  add(&m_textIP4);

  point.set(nX, 145);
  m_labelID.setPos(point); 
  m_labelID.setSize(120, nHeight);  
  m_labelID.setText(F("Controller ID")); 
  m_labelID.setFontColors(WHITE, LIGHTGREY); 
  m_labelID.setColors(getBGColor(), getBGColor()); 
  m_labelID.setCornerRadius(nRadius);
  m_labelID.setFontSize(nFontSize); 
  m_labelID.setParent(this);
  add(&m_labelID);
  
  point.moveY(m_labelID.getHeight());
  m_textID.setPos(point);
  m_textID.setSize(200, nHeight);
  m_textID.setFont(nFontSize, BLACK, GREY);
  m_textID.setColors(WHITE, GREY);
  m_textID.setText(m_pWifiManager->getControllerID());
  m_textID.setDisabledColors(LIGHTGREY, GREY);
  m_textID.setCornerRadius(nRadius);
  m_textID.setFocus(false);
  m_textID.setParent(this);
  add(&m_textID);
  m_point2.set(m_textID.getLeft(), m_textID.getTop());
  m_point2.moveY(m_textID.getHeight() + MARGIN);

  m_keyboard.setPos(m_point1);
  m_keyboard.setSize(0, 30);
  m_keyboard.setChars(F("0123456789"));
  m_keyboard.setColors(RGB(100, 65, 20), RGB(100, 85, 25));
  m_keyboard.setDisabledColors(RGB(80, 45, 20), RGB(80, 65, 25));
  m_keyboard.setFontColors(WHITE, LIGHTGREY);
  m_keyboard.setFontSize(3);
  m_keyboard.setCornerRadius(nRadius);
  m_keyboard.setParent(this);
  m_keyboard.setBuddy(&m_textIP1, m_textIP1.getBuddyStr());
  m_keyboard.init();
  add(&m_keyboard);

  m_buttonSave.setText(F("Save"));
  m_buttonSave.setSize(90, 30);
  m_buttonSave.setPos(nX + m_textID.getWidth() + MARGIN, m_textID.getTop());
  m_buttonSave.setColors(RGB(100, 65, 20), RGB(100, 85, 25));
  m_buttonSave.setDisabledColors(RGB(80, 45, 20), RGB(80, 65, 25));
  m_buttonSave.setFont(2, WHITE, LIGHTGREY);
  m_buttonSave.setFontSize(2);
  m_buttonSave.setCornerRadius(nRadius);
  m_buttonSave.setParent(this);
  add(&m_buttonSave);

  m_buttonBack = m_buttonSave;
  m_buttonBack.setText(F("Back"));
  m_buttonBack.setPos(nX + m_textID.getWidth() + MARGIN, m_buttonSave.getTop() + m_buttonSave.getHeight() + MARGIN);
  add(&m_buttonBack);
}

void CControllerSettings::moveKeyboard(CPoint &point)
{
  m_keyboard.setPos(point);
  show();
}

void CControllerSettings::doSave()
{
	if (m_pWifiManager)
	{
		IPAddress ipaddr(toUint(m_textIP1.getText(), 10), toUint(m_textIP2.getText(), 10), toUint(m_textIP3.getText(), 10), toUint(m_textIP4.getText(), 10));
		m_pWifiManager->saveControllerDetails(m_textID.getText(), ipaddr);
	}
}
void CControllerSettings::processResponse(CControl *pCtrl)
{
  if (pCtrl == &m_textID)
  {
    m_keyboard.hide();
    m_keyboard.setPos(m_point2);
    m_keyboard.setChars(F("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789"));
    m_keyboard.init();
    m_keyboard.show();
  }
  else if ((pCtrl == &m_textIP1) || (pCtrl == &m_textIP2) || (pCtrl == &m_textIP3) || (pCtrl == &m_textIP4))
  {
    m_keyboard.hide();
    m_keyboard.setPos(m_point1);
    m_keyboard.setChars(F("0123456789"));
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

