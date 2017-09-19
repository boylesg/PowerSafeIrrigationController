#pragma once

#include "Control.h"
#include "Point.h"
#include "CString.h"
#include "DialogBox.h"




template<uint16_t nBuffSize>
class CLabel: public CControl
{
	public:
		// Construction, destruction & initialisation
		CLabel(Adafruit_TFTLCD *pLCD);
		CLabel(CLabel &label);
		virtual ~CLabel();

		// Interface
		void set(CLabel &label);
		void setText(const char *strText);
		void setText(const __FlashStringHelper *strText);
    void setColors(const uint16_t nFontColor, const uint16_t nFontDisabledColor);
    void setFontColor(const uint16_t nFontColor);
    void setDisabledFontColor(const uint16_t nFontDisabledColor);
    void setFontColors(const uint16_t nFontColor, const uint16_t nFontDisabledColor);
		void setFontSize(const uint8_t nFontSize);
    void setFont(GFXfont *pFont);
		const char *getText();
		uint16_t getFontColor();
		uint16_t getDisabledFontColor();
		uint8_t getFontSize();
    GFXfont *getFont();

		// Overrides
		virtual void show();
		virtual void onTouch();
    virtual void onLoseFocus();
    virtual bool processTouch(TSPoint &point);

    // Operators
    CLabel<nBuffSize> &operator =(CLabel<nBuffSize> &label);

	protected:
		// Data
		CBuff<nBuffSize> m_buff;
		CString m_strText;
    uint8_t m_nFontSize;
    GFXfont *m_pFont;
    uint16_t m_nFontColor, m_nDisabledFontColor;
};




template<uint16_t nBuffSize>
CLabel<nBuffSize>::CLabel(Adafruit_TFTLCD *pLCD): CControl(pLCD), m_strText(m_buff)
{
	m_nFontSize = 0;
  m_eCtrlType = eLabel;
  m_pFont = NULL;
  m_nFontColor = m_nDisabledFontColor = 0;
}

template<uint16_t nBuffSize>
CLabel<nBuffSize>::CLabel(CLabel &label): CControl(label), m_strText(m_buff)
{
	set(label);
  m_eCtrlType = eLabel;
}

template<uint16_t nBuffSize>
CLabel<nBuffSize>::~CLabel()
{
}

template<uint16_t nBuffSize>
CLabel<nBuffSize> &CLabel<nBuffSize>::operator =(CLabel<nBuffSize> &label)
{
  set(label);
  return *this;
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::set(CLabel &label)
{
  CControl::set(label);
  m_strText = label.m_strText;
  m_nFontSize = label.m_nFontSize;
  m_pFont = label.m_pFont;
  m_nFontColor = label.m_nFontColor;
  m_nDisabledFontColor = label.m_nDisabledFontColor;
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::setText(const char *strText)
{
	m_strText = strText;
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::setText(const __FlashStringHelper *strText)
{
	m_strText = strText;
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::setFont(GFXfont *pFont)
{
  m_pFont = pFont;
}

template<uint16_t nBuffSize>
GFXfont *CLabel<nBuffSize>::getFont()
{
  return m_pFont;
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::setFontColors(const uint16_t nFontColor, const uint16_t nFontDisabledColor)
{
  m_nFontColor = nFontColor;
  m_nDisabledFontColor = nFontDisabledColor;
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::setFontColor(const uint16_t nFontColor)
{
  m_nFontColor = nFontColor;
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::setDisabledFontColor(const uint16_t nFontDisabledColor)
{
  m_nDisabledFontColor = nFontDisabledColor;
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::setColors(const uint16_t nFontColor, const uint16_t nFontDisabledColor)
{
	m_rectEnabled.setColors(nFontColor, nFontDisabledColor);
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::setFontSize(const uint8_t nFontSize)
{
  m_nFontSize = nFontSize;
}

template<uint16_t nBuffSize>
const char *CLabel<nBuffSize>::getText()
{
	return m_strText;
}

template<uint16_t nBuffSize>
uint16_t CLabel<nBuffSize>::getFontColor()
{
	return m_rectEnabled.getFillColor();
}

template<uint16_t nBuffSize>
uint16_t CLabel<nBuffSize>::getDisabledFontColor()
{
	return m_rectDisabled.getFillColor();
}

template<uint16_t nBuffSize>
uint8_t CLabel<nBuffSize>::getFontSize()
{
	return m_nFontSize;
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::show()
{
	if (m_pLCD)
	{
    Adafruit_GFX_Button button;

    if (m_bFocused || m_bEnabled)
      button.initButton(m_pLCD, m_rectEnabled.getLeft() + m_rectEnabled.getWidth() / 2, m_rectEnabled.getTop() + m_rectEnabled.getHeight() / 2, m_rectEnabled.getWidth(), 
                        m_rectEnabled.getHeight(), m_rectEnabled.getCornerRadius(), getFillColor(), getOutlineColor(), m_nFontColor, m_strText, m_nFontSize);
    else
      button.initButton(m_pLCD, m_rectEnabled.getLeft() + m_rectEnabled.getWidth() / 2, m_rectEnabled.getTop() + m_rectEnabled.getHeight() / 2, m_rectEnabled.getWidth(), 
                         m_rectEnabled.getHeight(), m_rectEnabled.getCornerRadius(), getFillColor(), getOutlineColor(), m_nDisabledFontColor, m_strText, m_nFontSize);
    button.alignLeft();
    button.drawButton();
	}
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::onTouch()
{
}

template<uint16_t nBuffSize>
void CLabel<nBuffSize>::onLoseFocus()
{
}

template<uint16_t nBuffSize>
bool CLabel<nBuffSize>::processTouch(TSPoint &point)
{
  // Does not accept input.
  return false;
}


