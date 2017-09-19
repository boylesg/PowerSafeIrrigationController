#pragma once

#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include "CString.h"
#include "Control.h"
#include "Devices.h"
#include "Base.h"
#include "Debug.h"

#define MAX_CTRLS 32




class CDialogBox: public CBase
{
	public:
		// Construction, destruction and initialisation
		CDialogBox(Adafruit_TFTLCD *pLCD);
		virtual ~CDialogBox();

		// Interface
		void add(CControl *pControl);
		uint16_t getCenter(const uint16_t nScreen);
		bool isVisible();
		void setBGColor(const uint16_t nColor);
		void setBorderColor(const uint16_t nColor);
		void setBorderWidth(const uint16_t nWidth);
		void setCornerRadius(const uint16_t nRadius);
		void setTitle(const char *strTitle, const uint16_t nTitleColor, const uint16_t nTitleSize);
		void setTitle(const __FlashStringHelper *strTitle, const uint16_t nTitleColor, const uint16_t nTitleSize);
		void setOrientation(const uint8_t nOrientation);
    void setParent(CDialogBox *pParent);
		uint16_t getBGColor();
		uint16_t getBorderColor();
		uint16_t getBorderWidth();
		uint16_t getCornerRadius();
		const char *getTitle();
		uint8_t getOrientation();
    CDialogBox *getParent();

		// Overrides
		virtual void show();
		virtual void begin() = 0;
		virtual bool processTouch(TSPoint &point, const uint16_t nScreenWidth, const uint16_t nScreenHeight);
		virtual void processResponse(CControl *pCtrl) = 0;
    virtual CString *getBuddyStr();
    virtual void restore();
    virtual void hide();

	protected:
		// Data
		CControl *m_arrayCtrls[MAX_CTRLS];
    uint8_t m_nNumCtrls;
    int8_t m_nLastCtrl;
		static CDialogBox *m_pVisible;
		uint8_t m_nOrientation, m_nBorderWidth, m_nCornerRadius;
		uint16_t m_nBGColor, m_nBorderColor, m_nTitleColor, m_nTitleSize;
		CBuff<24> m_buff;
		CString m_strTitle;
		Adafruit_TFTLCD *m_pLCD;
		char *m_strButtonID;
    CDialogBox *m_pParent;

		// Helpers
		void calibratePoint(TSPoint &point);
		void swapCoords(TSPoint &point);
		void drawBorder(Adafruit_TFTLCD *pLCD);
		bool isLandscape();
		bool isPortrait();
};

