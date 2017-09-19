#pragma once

#include <Arduino.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include "Control.h"
#include "CString.h"
#include "Devices.h"
#include "Debug.h"
#include "Label.h"
#include "Button.h"




template<uint16_t nBuffSize>
class CCheckBox: public CControl
{
	public:
		// Construction, destruction & initialisation
		CCheckBox(Adafruit_TFTLCD *pLCD);
		CCheckBox(CCheckBox &checkbox);
		virtual ~CCheckBox();

		// Interface
		void set(CCheckBox &checkbox);
		void setLabel(const char *strLabel);
		void setLabel(const __FlashStringHelper *strLabel);
		void setFont(const uint8_t nFontSize, const uint16_t nFontColor, const uint16_t nFontDisabledColor, GFXfont *pFont = NULL);
		void setFontSize(const uint8_t nFontSize);
		void setFontColor(const uint16_t nFontColor);
		void setDisabledFontColor(const uint16_t nFontDisabledColor);
		uint16_t getFontColor();
		uint16_t getFontDisabledColor();
		uint8_t getFontSize();
		GFXfont *getFont();
		void dump();
		void alignLeft();
		void alignRight();
		void init();
		void setCheck(const bool bCheck);
		bool getCheck();

		// Operators
		CCheckBox &operator =(CCheckBox &checkbox);

		// Overrides
		virtual void show();
    virtual bool processTouch(TSPoint &point);
    virtual void processResponse(CControl *pCtrl);
    virtual void onTouch();
    virtual void onLoseFocus();

	protected:
		CButton<2> m_buttonCheck;
		CLabel<nBuffSize> m_labelText;
		bool m_bAlignLeft;
};




template<uint16_t nBuffSize>
CCheckBox<nBuffSize>::CCheckBox(Adafruit_TFTLCD *pLCD): CControl(pLCD), m_buttonCheck(pLCD), m_labelText(pLCD)
{
  m_eCtrlType = eCheckBox;
  m_bAlignLeft = true;
}

template<uint16_t nBuffSize>
CCheckBox<nBuffSize>::CCheckBox(CCheckBox &checkbox): CControl(checkbox)
{
  set(checkbox);
  m_eCtrlType = eCheckBox;
  m_bAlignLeft = true;
}

template<uint16_t nBuffSize>
CCheckBox<nBuffSize>::~CCheckBox()
{
}

template<uint16_t nBuffSize>
bool CCheckBox<nBuffSize>::getCheck()
{
	return strcmp_P(m_buttonCheck.getText(), (PGM_P)F("X")) == 0;
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::setCheck(const bool bCheck)
{
	if (bCheck)
		m_buttonCheck.setText(F("X"));
	else
		m_buttonCheck.setText(F(""));
	show();
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::alignLeft()
{
	m_bAlignLeft = true;
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::alignRight()
{
	m_bAlignLeft = false;
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::dump()
{
    debug.log(F("CCheckBox object"));
    debug.log(F("---------------"));
	  m_buttonCheck.dump();
	  m_labelText.dump();
    CControl::dump();  
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::init()
{
	if (m_bAlignLeft)
	{
		m_buttonCheck.setPos(m_rectEnabled.getLeft(), m_rectEnabled.getTop());
		m_buttonCheck.setSize(m_rectEnabled.getHeight(), m_rectEnabled.getHeight());
		m_labelText.setPos(m_buttonCheck.getRight() + MARGIN, m_rectEnabled.getTop());
		m_labelText.setSize(m_rectEnabled.getWidth() - m_rectEnabled.getHeight() - MARGIN, m_rectEnabled.getHeight());
	}
	else
	{
		m_labelText.setPos(m_rectEnabled.getLeft(), m_rectEnabled.getTop());
		m_labelText.setSize(m_rectEnabled.getWidth() - m_rectEnabled.getHeight() - MARGIN, m_rectEnabled.getHeight());
		m_buttonCheck.setPos(m_labelText.getRight() + MARGIN, m_rectEnabled.getTop());
		m_buttonCheck.setSize(m_rectEnabled.getHeight(), m_rectEnabled.getHeight());
	}
  uint16_t nColor = 0;

  if (m_pParent->isDialog())
    nColor = ((CDialogBox*)m_pParent)->getBGColor();
  else
    nColor = ((CControl*)m_pParent)->getOutlineColor();

  m_buttonCheck.setParent(this);
  m_labelText.setParent(this);
  m_labelText.setColors(nColor, nColor);
  m_labelText.setDisabledColors(nColor, nColor);
  m_labelText.setDisabledColors(nColor, nColor);
  m_buttonCheck.setColors(getFillColor(), getOutlineColor());
  m_buttonCheck.setDisabledColors(getDisabledFillColor(), getDisabledOutlineColor());
  m_buttonCheck.setCornerRadius(getCornerRadius());
  m_buttonCheck.setFontColor(getOutlineColor());
  m_buttonCheck.setDisabledFontColor(getDisabledOutlineColor());
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::set(CCheckBox &checkbox)
{
  CControl::set(checkbox);
  m_buttonCheck = checkbox.m_buttonCheck;
  m_labelText = checkbox.m_labelText;
}

template<uint16_t nBuffSize>
CCheckBox<nBuffSize> &CCheckBox<nBuffSize>::operator =(CCheckBox<nBuffSize> &checkbox)
{
  set(checkbox);
  return *this;
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::setLabel(const char *strLabel)
{
	m_labelText.setText(strLabel);
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::setLabel(const __FlashStringHelper *strLabel)
{
	m_labelText.setText(strLabel);
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::setFontSize(const uint8_t nFontSize)
{
	m_labelText.setFontSize(nFontSize);
	m_buttonCheck.setFontSize(nFontSize);
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::setFontColor(const uint16_t nFontColor)
{
	m_labelText.setFontColor(nFontColor);
	m_buttonCheck.setFontColor(nFontColor);
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::setDisabledFontColor(const uint16_t nFontDisabledColor)
{
	m_labelText.setFontDisabledColor(nFontDisabledColor);
	m_buttonCheck.setDisabledFontColor(nFontDisabledColor);
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::setFont(const uint8_t nFontSize, const uint16_t nFontColor, const uint16_t nFontDisabledColor, GFXfont *pFont)
{
	m_labelText.setFontSize(nFontSize);
	m_buttonCheck.setFontSize(nFontSize);

	m_labelText.setFontColor(nFontColor);
	m_buttonCheck.setFontColor(nFontColor);

	m_labelText.setDisabledFontColor(nFontDisabledColor);
	m_buttonCheck.setDisabledFontColor(nFontDisabledColor);

	m_labelText.setFont(pFont);
	m_buttonCheck.setFont(pFont);
}

template<uint16_t nBuffSize>
uint16_t CCheckBox<nBuffSize>::getFontColor()
{
  return m_labelText.getFontColor();
}

template<uint16_t nBuffSize>
uint16_t CCheckBox<nBuffSize>::getFontDisabledColor()
{
  return m_labelText.getFontDisabledColor();
}

template<uint16_t nBuffSize>
uint8_t CCheckBox<nBuffSize>::getFontSize()
{
  return m_labelText.getFontSize();
}

template<uint16_t nBuffSize>
GFXfont *CCheckBox<nBuffSize>::getFont()
{
  return m_labelText.getFont();
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::show()
{
	m_buttonCheck.show();
	m_labelText.show();
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::processResponse(CControl *pCtrl)
{
  if (pCtrl == &m_buttonCheck)
  {
    setCheck(!getCheck());
  }
}

template<uint16_t nBuffSize>
bool CCheckBox<nBuffSize>::processTouch(TSPoint &point)
{
  return m_buttonCheck.processTouch(point);
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::onTouch()
{
}

template<uint16_t nBuffSize>
void CCheckBox<nBuffSize>::onLoseFocus()
{
}


