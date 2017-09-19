#pragma once

#include <Arduino.h>





typedef enum{eNone, eButton, eText, eCheckBox, eKeyboard, eLabel, eDialog} ControlType;

class CControl;

class CBase
{
	public:
		// Construction, destruction & initialisation
		CBase();
		virtual ~CBase();

		// Overrides
    virtual void processResponse(CControl *pCtrl) = 0;
    virtual CString *getBuddyStr() = 0;

    // Interface
    bool isText();
    bool isButton();
    bool isKeyboard();
    bool isLabel();
    bool isCheckbox();
    bool isDialog();
    bool isControl();
    void set(CBase &base);

    // Operators
    CBase &operator =(CBase &base);

  protected:
    ControlType m_eCtrlType;
};

