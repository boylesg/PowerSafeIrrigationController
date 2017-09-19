#pragma once
#include "Arduino.h"
#include "Common.h"
#include "Debug.h"




class CDevice
{
	public:
		// Construction, destruction & initialisation
		CDevice();
		CDevice(const uint8_t nRelayPin, const uint8_t nResetPin);
		virtual ~CDevice();

		// Interface
		void setRelayPin(const uint8_t nPinNum);
		void setResetPin(const uint8_t nPinNum);
		void reset();

		// Overrides
		virtual void turnOn();
		virtual void turnOff();

	protected:
		// Data
		uint8_t m_nRelayPin, m_nResetPin;
};




#define MAX_DEVICES 5

class CPowerManager
{
	public:
		// Construction, destruction & initialisation
		CPowerManager();
		virtual ~CPowerManager();

		// Interface
		void addDevice(const CDevice *pDevice);
		void powerDown();
		void powerUp();
		void detachInterrupts();
		void turnOnDevices();
		void turnOffDevices();
		void resetDevices();
		void turnOnDevice(const uint8_t nI);
		void turnOffDevice(const uint8_t nI);
		void resetDevice(const uint8_t nI);
		void setPin18ISR(GlobalFunctionPtrType pISR, const uint8_t nMode = FALLING);
		void setPin19ISR(GlobalFunctionPtrType pISR, const uint8_t nMode = FALLING);
		void setPin20ISR(GlobalFunctionPtrType pISR, const uint8_t nMode = FALLING);
		void setPin21ISR(GlobalFunctionPtrType pISR, const uint8_t nMode = FALLING);

	protected:
		
		// Data
		CDevice *m_arrayDevices[MAX_DEVICES];
		uint8_t m_nNumDevices;
    const uint8_t m_nINT3Pin18 = 18, m_nINT2Pin19 = 19, m_nINT1Pin20 = 20, m_nINT0Pin21 = 21;
	  uint8_t m_nINT3Mode, m_nINT2Mode, m_nINT1Mode, m_nINT0Mode;
    GlobalFunctionPtrType m_pPin18ISR, m_pPin19ISR, m_pPin20ISR, m_pPin21ISR;
};

extern CPowerManager PowerManager;
