#include "ProbeSettings.h"
#include "Devices.h"
#include "Debug.h"
#include "Text.h"





CProbeSettings::CProbeSettings(Adafruit_TFTLCD *pLCD, CWifiManager *pWifiManager):
	CDialogBox(pLCD), m_labelDryTime(pLCD), m_labelOnTime(pLCD), m_labelThresh(pLCD), m_textDryTime(pLCD), m_textOnTime(pLCD), m_textThresh(pLCD), m_keyboard(pLCD), m_buttonSave(pLCD), 
	m_buttonBack(pLCD), m_checkbox(pLCD)
{
  setTitle(F("Probe Settings"), WHITE, 2);
  setOrientation(PORTRAIT2);
  setBGColor(RGB(80, 40, 60));
  setBorderColor(RGB(95, 40, 85));
  setCornerRadius(10);
  setBorderWidth(5);
  m_nLastCtrl = 1;
  m_pWifiManager = pWifiManager;
}

CProbeSettings::~CProbeSettings()
{
}

void CProbeSettings::begin()
{
  CPoint point;
  uint8_t nHeight = 24, nWidth = 60, nButtonWidth = 70, nRadius = 5, nFontSize = 2, nIPWidth = 60, nIPGap = 45, nDotGap = 31, nX = 10;
 
  m_keyboard.setSize(0, 30);
  m_keyboard.setChars(F("0123456789"));
  m_keyboard.setColors(RGB(0, 90, 40), RGB(0, 100, 50));
  m_keyboard.setDisabledColors(RGB(80, 45, 20), RGB(80, 65, 25));
  m_keyboard.setFontColors(WHITE, LIGHTGREY);
  m_keyboard.setFontSize(3);
  m_keyboard.setParent(this);
  m_keyboard.setBuddy(&m_textDryTime, m_textDryTime.getBuddyStr());
  add(&m_keyboard);

  point.set(nX, 50);
  m_labelDryTime.setPos(point); 
  m_labelDryTime.setSize(nWidth * 2, nHeight);  
  m_labelDryTime.setText(F("Dry time")); 
  m_labelDryTime.setFontColors(WHITE, LIGHTGREY); 
  m_labelDryTime.setColors(getBGColor(), getBGColor()); 
  m_labelDryTime.setCornerRadius(nRadius);
  m_labelDryTime.setFontSize(nFontSize); 
  m_labelDryTime.setParent(this);
  add(&m_labelDryTime);
  
  point.moveY(m_labelDryTime.getHeight() + MARGIN / 2 - 4);
  m_textDryTime.setPos(point);
  m_textDryTime.setSize(nWidth, nHeight);  
  m_textDryTime.setText(m_pWifiManager->getDryTimeStr());
  m_textDryTime.setFont(nFontSize, BLACK, GREY);
  m_textDryTime.setColors(WHITE, GREY);
  m_textDryTime.setDisabledColors(LIGHTGREY, GREY);
  m_textDryTime.setCornerRadius(nRadius);
  m_textDryTime.setFocus(true);
  m_textDryTime.setParent(this);
  add(&m_textDryTime);
  
  point.moveY(m_textDryTime.getHeight() + MARGIN);
  m_point1 = point;
  m_keyboard.setPos(m_point1);
  m_keyboard.init();
  point.set(m_textDryTime.getLeft(), m_textDryTime.getBottom() + m_keyboard.getHeight() + MARGIN);
  m_labelOnTime.setPos(point); 
  m_labelOnTime.setSize(nWidth * 2, nHeight);  
  m_labelOnTime.setText(F("On time")); 
  m_labelOnTime.setColors(getBGColor(), getBGColor()); 
  m_labelOnTime.setFontColors(WHITE, LIGHTGREY); 
  m_labelOnTime.setCornerRadius(nRadius);
  m_labelOnTime.setFontSize(nFontSize); 
  m_labelOnTime.setParent(this);
  add(&m_labelOnTime);

  point.moveY(m_labelOnTime.getHeight() + MARGIN / 2 - 4);
  m_textOnTime.setPos(point);
  m_textOnTime.setSize(nWidth, nHeight);  
  m_textOnTime.setFont(nFontSize, BLACK, GREY);
  m_textOnTime.setText(m_pWifiManager->getOnTimeStr());
  m_textOnTime.setColors(WHITE, GREY);
  m_textOnTime.setDisabledColors(LIGHTGREY, GREY);
  m_textOnTime.setCornerRadius(nRadius);
  m_textOnTime.setFocus(false);
  m_textOnTime.setParent(this);
  add(&m_textOnTime);

  point.moveY(m_textOnTime.getHeight() + MARGIN);
  m_point2 = point;
  point.moveY(m_keyboard.getHeight() + MARGIN);
  m_labelThresh.setPos(point);
  m_labelThresh.setSize(146, nHeight);
  m_labelThresh.setText(F("Threshold"));
  m_labelThresh.setFontColors(WHITE, LIGHTGREY);
  m_labelThresh.setColors(getBGColor(), getBGColor());
  m_labelThresh.setCornerRadius(nRadius);
  m_labelThresh.setFontSize(nFontSize);
  m_labelThresh.setParent(this);
  add(&m_labelThresh);
  
  point.moveY(m_labelThresh.getHeight() + MARGIN / 2 - 4);
  m_textThresh.setPos(point);
  m_textThresh.setSize(146, nHeight);
  m_textThresh.setFont(nFontSize, BLACK, GREY);
  m_textThresh.setText(fromUint(m_pWifiManager->getThreshold(), 10));
  m_textThresh.setColors(WHITE, GREY);
  m_textThresh.setDisabledColors(LIGHTGREY, GREY);
  m_textThresh.setCornerRadius(nRadius);
  m_textThresh.setFocus(false);
  m_textThresh.setParent(this);
  add(&m_textThresh);

  point.moveY(m_textThresh.getHeight() + MARGIN);
  m_point3 = point;

  m_buttonSave.setText(F("Save"));
  m_buttonSave.setSize(nButtonWidth, nHeight);
  m_buttonSave.setPos(m_pLCD->height() - m_buttonSave.getWidth() - MARGIN - 2, m_textThresh.getTop());
  m_buttonSave.setColors(RGB(100, 65, 20), RGB(100, 85, 25));
  m_buttonSave.setDisabledColors(RGB(80, 45, 20), RGB(80, 65, 25));
  m_buttonSave.setFont(2, WHITE, LIGHTGREY);
  m_buttonSave.setFontSize(2);
  m_buttonSave.setCornerRadius(nRadius);
  m_buttonSave.setParent(this);
  add(&m_buttonSave);

  m_buttonBack = m_buttonSave;
  m_buttonBack.setText(F("Back"));
  m_buttonBack.setPos(m_buttonSave.getLeft(), m_buttonSave.getBottom() + MARGIN);
  add(&m_buttonBack);

  m_checkbox.setLabel(F("Auto"));
  m_checkbox.setSize(nButtonWidth, nHeight);
  point.set(m_textOnTime.getRight() + 70, m_textOnTime.getTop());
  m_checkbox.setPos(point);
  m_checkbox.setColors(WHITE, BLACK);
  m_checkbox.setCheck(m_pWifiManager->getAuto());
  m_checkbox.setDisabledColors(LIGHTGREY, BLACK);
  m_checkbox.setFont(2, WHITE, GREY);
  m_checkbox.setFontSize(2);
  m_checkbox.setCornerRadius(nRadius);
  m_checkbox.setParent(this);
  m_checkbox.init();
  add(&m_checkbox);

  m_nLastCtrl = 2;
}

void CProbeSettings::moveButtons(const bool bDown)
{
  if (bDown)
  {
    m_buttonSave.hide();
    m_buttonSave.setPos(m_buttonBack.getPos());
    m_buttonBack.moveY(m_buttonSave.getHeight() + MARGIN);
    m_buttonSave.show();
    m_buttonBack.show();  
  }
  else
  {
    m_buttonBack.hide();
    m_buttonBack.setPos(m_buttonSave.getPos());
    m_buttonSave.moveY(-(m_buttonSave.getHeight() + MARGIN));
    m_buttonSave.show();
    m_buttonBack.show();
  }
}

void CProbeSettings::moveKeyboard(CPoint &point)
{
  m_keyboard.setPos(point);
  show();
}

void CProbeSettings::doSave()
{
	if (m_pWifiManager)
		m_pWifiManager->saveAlarmDetails(m_checkbox.getCheck(), toUint(m_textDryTime.getText(), 10), toUint(m_textOnTime.getText(), 10), toUint(m_textThresh.getText(), 10));
}

void CProbeSettings::processResponse(CControl *pCtrl)
{
  static bool bButtonsMoved = false;
  
  if (pCtrl == &m_textThresh)
  {
    m_keyboard.hide();
    m_keyboard.setPos(m_point3);
    m_keyboard.init();

    moveButtons(false);
    bButtonsMoved = true;
    m_keyboard.show();
  }
  else if (pCtrl == &m_textOnTime)
  {
    m_keyboard.hide();
    m_keyboard.setPos(m_point2);
    m_keyboard.init();
    m_keyboard.show();

    if (bButtonsMoved)
    {
      moveButtons(true);
      bButtonsMoved = false;
    }
  }
  else if ((pCtrl == &m_textDryTime) || (pCtrl == &m_textThresh))
  {
    m_keyboard.hide();
    m_keyboard.setPos(m_point1);
    m_keyboard.init();
    m_keyboard.show();

    if (bButtonsMoved)
    {
      moveButtons(true);
      bButtonsMoved = false;
    }
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


