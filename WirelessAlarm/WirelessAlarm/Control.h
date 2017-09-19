#pragma once

#include <Arduino.h>
#include <TouchScreen.h>
#include <gfxfont.h>
#include "Point.h"
#include "Rect.h"
#include "CString.h"
#include "Base.h"





class CControl: public CBase
{
	public:
		// Construction, destruction & initialisation
		CControl(Adafruit_TFTLCD *pLCD);
		CControl(CControl &control);
		virtual ~CControl();

		// Interface
		void set(CControl &control);
		void setParent(CBase *pParent);
		CBase *getParent();
		const bool isEnabled();
		void setEnabled(const bool bEnable);
		const bool isFocused();
		void setFocus(const bool bFocus);
		uint16_t getDisabledFillColor();
		uint16_t getDisabledOutlineColor();
		void setDisabledFillColor(const uint16_t nFillColor);
		void setDisabledBorderColor(const uint16_t nBorderColor);
		void setDisabledColors(const uint16_t nFillColor, const uint16_t nOutlinecolor);
		void setFillColor(const uint16_t nFillColor);
		void setBorderColor(const uint16_t nBorderColor);
		void setColors(const uint16_t nFillColor, const uint16_t nBorderColor);
		uint8_t getOrientation();
		void setOrientation(const uint8_t nOrientation);
		int16_t getLeft();
		int16_t getRight();
		int16_t getTop();
		int16_t getBottom();
		CPoint &getPos();
		void setCornerRadius(const uint8_t nCornerRadius);
		uint16_t getCornerRadius();
		uint16_t getFillColor();
		uint16_t getOutlineColor();
		uint16_t getWidth();
		uint16_t getHeight();
		void setPos(CPoint &pos);
		void setPos(const int16_t nX, const int16_t nY);
		void setSize(CSize &size);
		void setSize(const uint16_t nWidth, const uint16_t nHeight);
		void dump();
		void moveX(const int16_t nAmount);
		void moveY(const int16_t nAmount);
		void moveXY(const int16_t nAmountX, const int16_t nAmountY);

		// Operators
		CControl &operator =(CControl &control);

		// Overrides
		virtual void show() = 0;
		virtual bool processTouch(TSPoint &point);
		virtual void onTouch() = 0;
		virtual void onLoseFocus() = 0;
		virtual bool contains(TSPoint &point);
		virtual void processResponse(CControl *pCtrl);
		virtual CString *getBuddyStr();
		virtual void hide();
    
	public:
		// Data
		CBase *m_pParent;
		Adafruit_TFTLCD *m_pLCD;
		CRect m_rectDisabled, m_rectEnabled;
		bool m_bEnabled, m_bFocused;
		uint8_t m_nOrientation;

		//Helpers
		bool isLandscape();
		bool isPortrait();
};

