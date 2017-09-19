#pragma once

#include "DialogBox.h"
#include "WifiManager.h"
#include "Button.h"
#include "Text.h"
#include "Keyboard.h"
#include "Label.h"
#include "CheckBox.h"





class CProbeSettings: public CDialogBox
{
	public:
		// Construction, destruction and initialisation
		CProbeSettings(Adafruit_TFTLCD *pLCD, CWifiManager *pWifiManager);
		virtual ~CProbeSettings();

		// Overrides
		virtual void processResponse(CControl *pCtrl);
		virtual void begin();

	protected:
		// Data
		CLabel<9> m_labelDryTime, m_labelOnTime;
		CLabel<11> m_labelThresh;
		CText<4> m_textDryTime, m_textOnTime;
		CText<5> m_textThresh;
		CKeyboard<11> m_keyboard;
		CButton<5> m_buttonSave, m_buttonBack;
		CCheckBox<5> m_checkbox;
		CPoint m_point1, m_point2, m_point3;
		CWifiManager *m_pWifiManager;

		// Helpers
		void moveKeyboard(CPoint &point);
		void moveButtons(const bool bDown);
		void doSave();
};

