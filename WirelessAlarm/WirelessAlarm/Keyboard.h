#pragma once

#include "DialogBox.h"
#include "Control.h"
#include "CString.h"
#include "Devices.h"
#include "Debug.h"




template<uint16_t nBuffSize>
class CKeyboard: public CControl
{
	public:
		// Construction, destruction & initialisation
		CKeyboard(Adafruit_TFTLCD *pLCD);
		virtual ~CKeyboard();

		// Interface
		void setFontColor(const uint16_t nTextColor);
		uint16_t getFontColor();
		void setDisabledFontColor(const uint16_t nDisabledColor);
		uint16_t getDisabledFontColor();
		void setBuddy(const CControl *pBuddyText, const CString *pBuddyStr);
		CControl *getBuddyText();
		CString *getBuddyStr();
		const char *getChars();
		void setChars(const char *strChars);
		void setChars(const __FlashStringHelper *strChars);
		void setFontSize(const uint16_t nTextSize);
		uint16_t getFontSize();
		void setFontColors(const uint16_t nTextColor, const uint16_t nDisabledColor);
    void setFillColor(const uint16_t nFillColor);
    void setBorderColor(const uint16_t nBorderColor);
    void setColors(const uint16_t nFillColor, const uint16_t nBorderColor);
    void setDisabledFillColor(const uint16_t nFillColor);
    void setDisabledBorderColor(const uint16_t nBorderColor);
    void setDisabledFontColors(const uint16_t nFillColor, const uint16_t nBorderColor);
    void init();

		// Overrides
		virtual void show();
		virtual void onTouch();
		virtual void processResponse(CControl *pCtrl);
    virtual void onLoseFocus();
    virtual bool processTouch(TSPoint &point);
		//virtual bool contains(TSPoint &point);

	protected:
		// Data
		uint16_t m_nFontColor, m_nDisabledFontColor, m_nTextSize;
		CControl *m_pBuddyText;
		CBuff<nBuffSize> m_buff;
		CString *m_pBuddyStr, m_strChars;
		int8_t m_nCharIndex;
  
		CButton<2> m_buttonLeft, m_buttonRight, m_buttonChar;
		CButton<11> m_buttonBackSpace;
};




template<uint16_t nBuffSize>
CKeyboard<nBuffSize>::CKeyboard(Adafruit_TFTLCD *pLCD): CControl(pLCD), m_strChars(m_buff), m_buttonLeft(pLCD), m_buttonRight(pLCD), m_buttonChar(pLCD), m_buttonBackSpace(pLCD)
{
	m_pBuddyText = NULL;
	m_pBuddyStr = NULL;
	m_nTextSize = 0;
	m_nFontColor = m_nDisabledFontColor = 0;
	m_nCharIndex = 0;
  m_eCtrlType = eKeyboard;
}

template<uint16_t nBuffSize>
CKeyboard<nBuffSize>::~CKeyboard()
{
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setFillColor(const uint16_t nFillColor)
{
  CControl::setFillColor(nFillColor);
  m_buttonLeft.setFillColor(nFillColor);
  m_buttonRight.setFillColor(nFillColor);
  m_buttonChar.setFillColor(nFillColor);
  m_buttonBackSpace.setFillColor(nFillColor);
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setBorderColor(const uint16_t nBorderColor)
{
  CControl::setBorderColor(nBorderColor);
  m_buttonLeft.setBorderColor(nBorderColor);
  m_buttonRight.setBorderColor(nBorderColor);
  m_buttonChar.setBorderColor(nBorderColor);
  m_buttonBackSpace.setBorderColor(nBorderColor);
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setColors(const uint16_t nFillColor, const uint16_t nBorderColor)
{
  CControl::setColors(nFillColor, nBorderColor);
  m_buttonLeft.setColors(nFillColor, nBorderColor);
  m_buttonRight.setColors(nFillColor, nBorderColor);
  m_buttonChar.setColors(nFillColor, nBorderColor);
  m_buttonBackSpace.setColors(nFillColor, nBorderColor);
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setDisabledFillColor(const uint16_t nFillColor)
{
  CControl::setDisabledFillColor(nFillColor);
  m_buttonLeft.setDisabledFillColor(nFillColor);
  m_buttonRight.setDisabledFillColor(nFillColor);
  m_buttonChar.setDisabledFillColor(nFillColor);
  m_buttonBackSpace.setDisabledFillColor(nFillColor);
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setDisabledBorderColor(const uint16_t nBorderColor)
{
  CControl::setDisabledBorderColor(nBorderColor);
  m_buttonLeft.setDisabledBorderColor(nBorderColor);
  m_buttonRight.setDisabledBorderColor(nBorderColor);
  m_buttonChar.setDisabledBorderColor(nBorderColor);
  m_buttonBackSpace.setDisabledBorderColor(nBorderColor);
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setDisabledFontColors(const uint16_t nFillColor, const uint16_t nBorderColor)
{
  CControl::setDisabledColors(nFillColor, nBorderColor);
  m_buttonLeft.setDisabledColors(nFillColor, nBorderColor);
  m_buttonRight.setDisabledColors(nFillColor, nBorderColor);
  m_buttonChar.setDisabledColors(nFillColor, nBorderColor);
  m_buttonBackSpace.setDisabledColors(nFillColor, nBorderColor);
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setFontColors(const uint16_t nTextColor, const uint16_t nDisabledColor)
{
	m_nFontColor = nTextColor;
	m_nDisabledFontColor = nDisabledColor;
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setFontColor(const uint16_t nTextColor)
{
	m_nFontColor = nTextColor;
}

template<uint16_t nBuffSize>
uint16_t CKeyboard<nBuffSize>::getFontColor()
{
	return m_nFontColor;
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setFontSize(const uint16_t nTextSize)
{
	m_nTextSize = nTextSize;
}

template<uint16_t nBuffSize>
uint16_t CKeyboard<nBuffSize>::getFontSize()
{
	return m_nTextSize;
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setDisabledFontColor(const uint16_t nDisabledColor)
{
	m_nDisabledFontColor = nDisabledColor;
}

template<uint16_t nBuffSize>
uint16_t CKeyboard<nBuffSize>::getDisabledFontColor()
{
	return m_nDisabledFontColor;
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::init()
{
	char str[3] = { 0, 0, 0};
	uint16_t nTotalWidth = 0, nCharWidthMult = 10, nCharWidth = m_nTextSize * nCharWidthMult, nHeight = m_nTextSize * nCharWidthMult;
  CPoint point(getLeft(), getTop());

	m_buttonLeft.setPos(point);
	m_buttonLeft.setSize(nCharWidth, nHeight);
	m_buttonLeft.setText(F("-"));
  m_buttonLeft.setParent(this);
  m_buttonLeft.setFontSize(m_nTextSize);
  m_buttonLeft.setCornerRadius(m_rectEnabled.getCornerRadius());
	nTotalWidth += m_buttonLeft.getWidth();

  point.moveX(m_buttonLeft.getWidth() + MARGIN / 2);
  m_buttonChar.setPos(point);
  m_buttonChar.setSize(nCharWidth, nHeight);
  str[0] = m_strChars[m_nCharIndex];
  m_buttonChar.setText(str);
  m_buttonChar.setParent(this);
  m_buttonChar.setCornerRadius(m_rectEnabled.getCornerRadius());
  m_buttonChar.setFontSize(m_nTextSize);
  nTotalWidth += m_buttonChar.getWidth();

  point.moveX(m_buttonChar.getWidth() + MARGIN / 2);
  m_buttonRight.setPos(point);
  m_buttonRight.setSize(nCharWidth, nHeight);
  m_buttonRight.setText(F("+"));
  m_buttonRight.setParent(this);
  m_buttonRight.setCornerRadius(m_rectEnabled.getCornerRadius());
  m_buttonRight.setFontSize(m_nTextSize);
  nTotalWidth += m_buttonRight.getWidth();

  point.moveX(m_buttonRight.getWidth() + MARGIN / 2);
  m_buttonBackSpace.setPos(point);
  m_buttonBackSpace.setSize(nCharWidth * 2, nHeight);
  str[0] = char(174);
  str[1] = char(174);
  m_buttonBackSpace.setText(str);
  m_buttonBackSpace.setParent(this);
  m_buttonBackSpace.setCornerRadius(m_rectEnabled.getCornerRadius());
  m_buttonBackSpace.setFontSize(m_nTextSize);
  nTotalWidth += m_buttonBackSpace.getWidth();

  setSize(nTotalWidth + (MARGIN / 2) * 3, nHeight);
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::show()
{
  m_buttonLeft.show();
  m_buttonRight.show();
  m_buttonChar.show();
  m_buttonBackSpace.show();
}

template<uint16_t nBuffSize>
bool CKeyboard<nBuffSize>::processTouch(TSPoint &point)
{
  if (m_buttonLeft.processTouch(point))
  {
    m_nCharIndex--;
    if (m_nCharIndex < 0)
      m_nCharIndex = m_strChars.length() - 1;
    char str[2] = {0, 0};
    str[0] = m_strChars[m_nCharIndex];
    m_buttonChar.setText(str);
    m_buttonChar.show();
  }
  else if (m_buttonRight.processTouch(point))
  {
    m_nCharIndex++;
    if (m_nCharIndex >= m_strChars.length())
      m_nCharIndex = 0;
    char str[2] = {0, 0};
    str[0] = m_strChars[m_nCharIndex];
    m_buttonChar.setText(str);
    m_buttonChar.show();
  }
  else if (m_buttonChar.processTouch(point))
  {
    if (m_pBuddyStr && m_pBuddyText)
    {
      *m_pBuddyStr += m_strChars[m_nCharIndex];
      m_pBuddyText->show();
    }
  }
  else if (m_buttonBackSpace.processTouch(point))
  {
    m_pBuddyStr->remove(m_pBuddyStr->length() - 1, 1);
    m_pBuddyText->show();
  }
  return false;
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::processResponse(CControl *pCtrl)
{
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setBuddy(const CControl *pBuddyText, const CString *pBuddyStr)
{
	m_pBuddyText = (CControl*)pBuddyText;
	m_pBuddyStr = (CString*)pBuddyStr;
}

template<uint16_t nBuffSize>
CControl *CKeyboard<nBuffSize>::getBuddyText()
{
	return m_pBuddyText;
}

template<uint16_t nBuffSize>
CString *CKeyboard<nBuffSize>::getBuddyStr()
{
	return m_pBuddyStr;
}

template<uint16_t nBuffSize>
const char *CKeyboard<nBuffSize>::getChars()
{
	return m_strChars;
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setChars(const char *strChars)
{
	char str[2] = { 0, 0 };
	m_strChars = strChars;
	str[0] = m_strChars[0];
	m_buttonChar.setText(str);
	m_nCharIndex = 0;
}


template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::setChars(const __FlashStringHelper *strChars)
{
	char str[2] = { 0, 0 };
	m_strChars = strChars;
	str[0] = m_strChars[0];
	m_buttonChar.setText(str);
	m_nCharIndex = 0;
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::onTouch()
{
}

template<uint16_t nBuffSize>
void CKeyboard<nBuffSize>::onLoseFocus()
{
}

