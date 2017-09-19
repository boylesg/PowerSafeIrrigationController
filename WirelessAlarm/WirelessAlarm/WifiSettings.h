#pragma once

#include "DialogBox.h"
#include "WifiManager.h"
#include "Button.h"
#include "Text.h"
#include "Keyboard.h"
#include "Label.h"
#include "CheckBox.h"




class CWifiSettings: public CDialogBox
{
	public:
		// Construction, destruction and initialisation
		CWifiSettings(Adafruit_TFTLCD *pLCD, CWifiManager *pWifiManager);
		virtual ~CWifiSettings();

		// Overrides
    virtual void processResponse(CControl *pCtrl);
    virtual void begin();

	protected:
		// Data
		CLabel<16> m_labelName, m_labelKey;
		CText<25> m_textName, m_textKey;
		CKeyboard<100> m_keyboard;
		CButton<5> m_buttonSave, m_buttonBack;
		CPoint m_point1, m_point2;
		CWifiManager *m_pWifiManager;

		// Helpers
		void moveKeyboard(CPoint &point);
		void doSave();
};

