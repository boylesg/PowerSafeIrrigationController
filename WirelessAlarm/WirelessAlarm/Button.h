#pragma once

#include <Arduino.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include "Control.h"
#include "CString.h"
#include "Devices.h"
#include "Debug.h"




template<uint16_t nBuffSize>
class CButton: public CControl
{
	public:
		// Construction, destruction & initialisation
		CButton(Adafruit_TFTLCD *pLCD);
		CButton(CButton &button);
		virtual ~CButton();

		// Interface
		void set(CButton &button);
		void setText(const char *strText);
		void setText(const __FlashStringHelper *strText);
		void setFont(const uint8_t nFontSize, const uint16_t nFontColor, const uint16_t nFontDisabledColor, GFXfont *pFont = NULL);
    void setFontSize(const uint8_t nFontSize);
    void setFontColor(const uint16_t nFontColor);
    void setDisabledFontColor(const uint16_t nFontDisabledColor);
    void setFont(GFXfont *pFont);
		uint16_t getFontColor();
		uint16_t getFontDisabledColor();
		uint8_t getFontSize();
		GFXfont *getFont();
		const char *getText();
    void dump();

		// Operators
		CButton &operator =(CButton &button);

		// Overrides
		virtual void show();
		virtual void onTouch();
    virtual void onLoseFocus();

	protected:
		uint16_t m_nFontColor, m_nFontDisabledColor;
		uint8_t m_nFontSize;
		GFXfont *m_pFont;
		CBuff<nBuffSize> m_buff;
		CString m_strText;
};




template<uint16_t nBuffSize>
CButton<nBuffSize>::CButton(Adafruit_TFTLCD *pLCD): m_strText(m_buff), CControl(pLCD)
{
  m_nFontColor = m_nFontDisabledColor = 0;
  m_nFontSize = 0;
  m_pFont = NULL;
  m_eCtrlType = eButton;
}

template<uint16_t nBuffSize>
CButton<nBuffSize>::CButton(CButton &button): m_strText(m_buff), CControl(button)
{
  set(button);
  m_eCtrlType = eButton;
}

template<uint16_t nBuffSize>
CButton<nBuffSize>::~CButton()
{
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::dump()
{
    debug.log(F("CButton object"));
    debug.log(F("---------------"));
    debug.dump(F("m_nFontColor"), m_nFontColor);
    debug.dump(F("m_nFontDisabledColor"), m_nFontDisabledColor);
    debug.dump(F("m_nFontSize"), m_nFontSize);
    debug.dump(F("m_pFont"), (uint32_t)m_pFont);
    debug.dump(F("m_strText"), m_strText);
    CControl::dump();  
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::set(CButton &button)
{
  CControl::set(button);
  m_nFontColor = button.m_nFontColor;
  m_nFontDisabledColor = button.m_nFontDisabledColor;
  m_nFontSize = button.m_nFontSize;
  m_pFont = button.m_pFont;
  m_strText = button.m_strText;
}

template<uint16_t nBuffSize>
CButton<nBuffSize> &CButton<nBuffSize>::operator =(CButton<nBuffSize> &button)
{
  set(button);
  return *this;
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::setText(const char *strText)
{
  m_strText = strText;
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::setText(const __FlashStringHelper *strText)
{
  m_strText = strText;
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::setFont(GFXfont *pFont)
{
  m_pFont = pFont;
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::setFontSize(const uint8_t nFontSize)
{
  m_nFontSize = nFontSize;
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::setFontColor(const uint16_t nFontColor)
{
  m_nFontColor = nFontColor;
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::setDisabledFontColor(const uint16_t nFontDisabledColor)
{
  m_nFontDisabledColor = nFontDisabledColor;
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::setFont(const uint8_t nFontSize, const uint16_t nFontColor, const uint16_t nFontDisabledColor, GFXfont *pFont)
{
  m_nFontSize = nFontSize;
  m_nFontColor = nFontColor;
  m_pFont = pFont;
  m_nFontDisabledColor = nFontDisabledColor;
}

template<uint16_t nBuffSize>
uint16_t CButton<nBuffSize>::getFontColor()
{
  return m_nFontColor;
}

template<uint16_t nBuffSize>
uint16_t CButton<nBuffSize>::getFontDisabledColor()
{
  return m_nFontDisabledColor;
}

template<uint16_t nBuffSize>
uint8_t CButton<nBuffSize>::getFontSize()
{
  return m_nFontSize;
}

template<uint16_t nBuffSize>
GFXfont *CButton<nBuffSize>::getFont()
{
  return m_pFont;
}

template<uint16_t nBuffSize>
const char *CButton<nBuffSize>::getText()
{
  return m_strText;
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::show()
{
  Adafruit_GFX_Button button;

  if (m_bFocused)
    button.initButton(m_pLCD, m_rectEnabled.getLeft() + m_rectEnabled.getWidth() / 2, m_rectEnabled.getTop() + m_rectEnabled.getHeight() / 2, m_rectEnabled.getWidth(), m_rectEnabled.getHeight(), m_rectEnabled.getCornerRadius(), m_rectEnabled.getFillColor(), m_rectEnabled.getOutlineColor(), m_nFontColor, m_strText, m_nFontSize);
  else if (m_bEnabled)
    button.initButton(m_pLCD, m_rectEnabled.getLeft() + m_rectEnabled.getWidth() / 2, m_rectEnabled.getTop() + m_rectEnabled.getHeight() / 2, m_rectEnabled.getWidth(), m_rectEnabled.getHeight(), m_rectEnabled.getCornerRadius(), m_rectEnabled.getOutlineColor(), m_rectEnabled.getFillColor(), m_nFontColor, m_strText, m_nFontSize);
  else
    button.initButton(m_pLCD, m_rectEnabled.getLeft() + m_rectEnabled.getWidth() / 2, m_rectEnabled.getTop() + m_rectEnabled.getHeight() / 2, m_rectEnabled.getWidth(), m_rectEnabled.getHeight(), m_rectEnabled.getCornerRadius(), m_rectDisabled.getOutlineColor(), m_rectDisabled.getFillColor(), m_nFontDisabledColor, m_strText, m_nFontSize);

  button.drawButton();
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::onTouch()
{
	setFocus(true);
	show();
  delay(100);
  setFocus(false);
  show();
  if (m_pParent)
    m_pParent->processResponse(this);
}

template<uint16_t nBuffSize>
void CButton<nBuffSize>::onLoseFocus()
{
}

