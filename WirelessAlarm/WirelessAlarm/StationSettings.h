#pragma once

#include "DialogBox.h"
#include "WifiManager.h"
#include "Button.h"
#include "Text.h"
#include "Keyboard.h"
#include "Label.h"
#include "CheckBox.h"




class CStationSettings: public CDialogBox
{
	public:
		// Construction, destruction and initialisation
		CStationSettings(Adafruit_TFTLCD *pLCD, CWifiManager *pWifiManager);
		virtual ~CStationSettings();

		// Overrides
    virtual void processResponse(CControl *pCtrl);
    virtual void begin();

	protected:
		// Data
		CLabel<21> m_labelStationNum, m_labelStationDesc;
		CText<2> m_textStationNum;
		CText<25> m_textStationDesc;
		CKeyboard<100> m_keyboard;
		CButton<5> m_buttonSave, m_buttonBack;
		CPoint m_point1, m_point2;
		CWifiManager *m_pWifiManager;

		// Helpers
		void moveKeyboard(CPoint &point);
		void doSave();
};

