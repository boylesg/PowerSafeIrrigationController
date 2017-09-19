#pragma once

#include "DialogBox.h"
#include "WifiManager.h"
#include "Button.h"
#include "Text.h"
#include "Keyboard.h"
#include "Label.h"
#include "CheckBox.h"




class CControllerSettings: public CDialogBox
{
	public:
		// Construction, destruction and initialisation
		CControllerSettings(Adafruit_TFTLCD *pLCD, CWifiManager *pWifiManager);
		virtual ~CControllerSettings();

		// Overrides
    virtual void processResponse(CControl *pCtrl);
    virtual void begin();

	protected:
		// Data
		CLabel<16> m_labelIP, m_labelID;
		CLabel<1> m_labelDot1, m_labelDot2, m_labelDot3;
		CText<3> m_textIP1, m_textIP2, m_textIP3, m_textIP4;
		CText<16> m_textID;
		CKeyboard<62> m_keyboard;
		CButton<5> m_buttonSave, m_buttonBack;
		CPoint m_point1, m_point2;
		CWifiManager *m_pWifiManager;

		// Helpers
		void moveKeyboard(CPoint &point);
		void doSave();
};

