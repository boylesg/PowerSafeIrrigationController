#pragma once

#include "Control.h"
#include "CString.h"
#include "Debug.h"




template<uint16_t nBuffSize>
class CText: public CControl
{
	public:
		// Construction, destruction & initialisation
		CText(Adafruit_TFTLCD *pLCD);
		CText(CText &text);
		virtual ~CText();

		// Interface
    void set(CText &text);
		const char *getText();
		void setText(const char *strText);
		void setText(const __FlashStringHelper *strText);
		void appendChar(const char cCh);
		void unappendChar();
		void setFont(const uint8_t nFontSize, const uint16_t nFontColor, const uint16_t nFontDisabledColor, GFXfont *pFont = NULL);
		uint16_t getFontColor();
		uint16_t getFontDisabledColor();
		uint8_t getFontSize();
		GFXfont *getFont();

		// Overrides
		virtual void show();
		virtual void onTouch();
    virtual CString *getBuddyStr();
    virtual void onLoseFocus();

    // Operators
    CText<nBuffSize> &operator =(CText<nBuffSize> &text);

	protected:
		// Data
		CBuff<nBuffSize> m_buff;
		CString m_strText;
		uint16_t m_nFontColor, m_nFontDisabledColor;
		uint8_t m_nFontSize;
		GFXfont *m_pFont;
};





template<uint16_t nBuffSize>
CText<nBuffSize>::CText(Adafruit_TFTLCD *pLCD): CControl(pLCD), m_strText(m_buff)
{
	m_nFontColor = m_nFontDisabledColor = 0;
	m_nFontSize = 0;
	m_pFont = NULL;
  m_eCtrlType = eText;
}

template<uint16_t nBuffSize>

CText<nBuffSize>::CText(CText &text): CControl(text), m_strText(m_buff)
{
  set(text);
  m_eCtrlType = eText;
}

template<uint16_t nBuffSize>
CText<nBuffSize>::~CText()
{
}

template<uint16_t nBuffSize>
void CText<nBuffSize>::set(CText<nBuffSize> &text)
{
  CControl::set(text);
  m_strText = text.m_strText;
  m_nFontColor = text.m_nFontColor;
  m_nFontDisabledColor = text.m_nFontDisabledColor;
  m_nFontSize = text.m_nFontSize;
  m_pFont = text.m_pFont;
  return *this;
}

template<uint16_t nBuffSize>
CText<nBuffSize> &CText<nBuffSize>::operator =(CText<nBuffSize> &text)
{
  set(text);
  return *this;
}

template<uint16_t nBuffSize>
CString *CText<nBuffSize>::getBuddyStr()
{
  return &m_strText;
}

template<uint16_t nBuffSize>
const char *CText<nBuffSize>::getText()
{
	return m_strText;
}

template<uint16_t nBuffSize>
void CText<nBuffSize>::setText(const char *strText)
{
	m_strText = strText;
}

template<uint16_t nBuffSize>
void CText<nBuffSize>::setText(const __FlashStringHelper *strText)
{
	m_strText = strText;
}

template<uint16_t nBuffSize>
void CText<nBuffSize>::appendChar(const char cCh)
{
	m_strText += cCh;
}

template<uint16_t nBuffSize>
void CText<nBuffSize>::unappendChar()
{
	m_strText.remove(m_strText.length() - 1, 1);
}

template<uint16_t nBuffSize>
void CText<nBuffSize>::setFont(const uint8_t nFontSize, const uint16_t nFontColor, const uint16_t nFontDisabledColor, GFXfont *pFont)
{
	m_nFontSize = nFontSize;
	m_nFontColor = nFontColor;
	m_pFont = pFont;
	m_nFontDisabledColor = nFontDisabledColor;
}

template<uint16_t nBuffSize>
uint16_t CText<nBuffSize>::getFontColor()
{
	return m_nFontColor;
}

template<uint16_t nBuffSize>
uint16_t CText<nBuffSize>::getFontDisabledColor()
{
	return m_nFontDisabledColor;
}

template<uint16_t nBuffSize>
uint8_t CText<nBuffSize>::getFontSize()
{
	return m_nFontSize;
}

template<uint16_t nBuffSize>
GFXfont *CText<nBuffSize>::getFont()
{
	return m_pFont;
}

template<uint16_t nBuffSize>
void CText<nBuffSize>::show()
{
	Adafruit_GFX_Button button;

	if (!m_bEnabled)
	{
		button.initButton(m_pLCD, m_rectEnabled.getLeft() + m_rectEnabled.getWidth() / 2, m_rectEnabled.getTop() + m_rectEnabled.getHeight() / 2, m_rectEnabled.getWidth(), m_rectEnabled.getHeight(), m_rectEnabled.getCornerRadius(), m_rectDisabled.getOutlineColor(), m_rectDisabled.getFillColor(), m_nFontDisabledColor, m_strText, m_nFontSize);
	}
	else if (m_bEnabled && m_bFocused)
	{
		button.initButton(m_pLCD, m_rectEnabled.getLeft() + m_rectEnabled.getWidth() / 2, m_rectEnabled.getTop() + m_rectEnabled.getHeight() / 2, m_rectEnabled.getWidth(), m_rectEnabled.getHeight(), m_rectEnabled.getCornerRadius(), m_rectEnabled.getOutlineColor(), m_nFontColor, m_rectEnabled.getFillColor(), m_strText, m_nFontSize);
	}
	else if (m_bEnabled && !m_bFocused)
	{
		button.initButton(m_pLCD, m_rectEnabled.getLeft() + m_rectEnabled.getWidth() / 2, m_rectEnabled.getTop() + m_rectEnabled.getHeight() / 2, m_rectEnabled.getWidth(), m_rectEnabled.getHeight(), m_rectEnabled.getCornerRadius(), m_rectEnabled.getOutlineColor(), m_rectEnabled.getFillColor(), m_nFontColor, m_strText, m_nFontSize);
	}
  button.alignLeft();
	button.drawButton();
}

template<uint16_t nBuffSize>
void CText<nBuffSize>::onTouch()
{
	setFocus(true);
	show();
  m_pParent->processResponse(this);
}

template<uint16_t nBuffSize>
void CText<nBuffSize>::onLoseFocus()
{
  setFocus(false);
  show();
}


