#include <avr/sleep.h>
#include <avr/power.h>
#include "Arduino.h"
#include "Common.h"
#include "debug.h"
#include "PowerControl.h"





CDevice::CDevice()
{
	m_nRelayPin = m_nResetPin = 100;

}

CDevice::CDevice(const uint8_t nRelayPin, const uint8_t nResetPin)
{
	m_nRelayPin = nRelayPin;
	m_nResetPin = nResetPin;
}

CDevice::~CDevice()
{
}

void CDevice::turnOff()
{
  if (m_nRelayPin != 100)
	  digitalWrite(m_nRelayPin, LOW);
}

void CDevice::turnOn()
{
  if (m_nRelayPin != 100)
    digitalWrite(m_nRelayPin, HIGH);
}

void CDevice::reset()
{
  if (m_nResetPin != 100)
  {
    pinMode(m_nResetPin, OUTPUT);
    digitalWrite(m_nResetPin, LOW);
  }
}

void CDevice::setRelayPin(const uint8_t nPinNum)
{
  m_nRelayPin = nPinNum;
  pinMode(m_nRelayPin, OUTPUT);
  digitalWrite(m_nRelayPin, HIGH);
}

void CDevice::setResetPin(const uint8_t nPinNum)
{
  m_nResetPin = nPinNum;
  pinMode(m_nResetPin, INPUT_PULLUP);
}




CPowerManager PowerManager;

CPowerManager::CPowerManager()
{
	m_nNumDevices = 0;
	m_pPin18ISR = m_pPin19ISR = m_pPin20ISR = m_pPin21ISR = NULL;
	m_nINT3Mode = m_nINT2Mode = m_nINT1Mode = m_nINT0Mode = 0;
}

CPowerManager::~CPowerManager()
{
}

void CPowerManager::turnOnDevices()
{
  for (uint8_t nI = 0; nI < m_nNumDevices; nI++)
    m_arrayDevices[nI]->turnOn();
}

void CPowerManager::turnOffDevices()
{
  for (uint8_t nI = 0; nI < m_nNumDevices; nI++)
    m_arrayDevices[nI]->turnOff();
}

void CPowerManager::resetDevices()
{
  for (uint8_t nI = 0; nI < m_nNumDevices; nI++)
    m_arrayDevices[nI]->reset();
}

void CPowerManager::turnOnDevice(const uint8_t nI)
{
  if ((nI >= 0) && (nI <= m_nNumDevices))
    m_arrayDevices[nI]->turnOn();
}

void CPowerManager::turnOffDevice(const uint8_t nI)
{
  if ((nI >= 0) && (nI <= m_nNumDevices))
    m_arrayDevices[nI]->turnOff();
}

void CPowerManager::resetDevice(const uint8_t nI)
{
  if ((nI >= 0) && (nI <= m_nNumDevices))
    m_arrayDevices[nI]->reset();
}

void CPowerManager::detachInterrupts()
{
  if (m_pPin18ISR)
    detachInterrupt(digitalPinToInterrupt(m_nINT3Pin18));
  if (m_pPin19ISR)
    detachInterrupt(digitalPinToInterrupt(m_nINT2Pin19));
  if (m_pPin20ISR)
    detachInterrupt(digitalPinToInterrupt(m_nINT1Pin20));
  if (m_pPin21ISR)
    detachInterrupt(digitalPinToInterrupt(m_nINT0Pin21));
}

void CPowerManager::setPin18ISR(GlobalFunctionPtrType pISR, const uint8_t nMode = FALLING)
{
	m_pPin18ISR = pISR;
	m_nINT3Mode = nMode;
}

void CPowerManager::setPin19ISR(GlobalFunctionPtrType pISR, const uint8_t nMode = FALLING)
{
	m_pPin19ISR = pISR;
	m_nINT2Mode = nMode;
}

void CPowerManager::setPin20ISR(GlobalFunctionPtrType pISR, const uint8_t nMode = FALLING)
{
	m_pPin20ISR = pISR;
	m_nINT1Mode = nMode;
}

void CPowerManager::setPin21ISR(GlobalFunctionPtrType pISR, const uint8_t nMode = FALLING)
{
	m_pPin21ISR = pISR;
	m_nINT0Mode = nMode;
}

void CPowerManager::powerDown()
{
	for (uint8_t nI = 0; nI < m_nNumDevices; nI++)
		m_arrayDevices[nI]->turnOff();

	// Sleep mode is set here
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
	// Enables the sleep bit in the mcucr register so sleep is possible - just a safety pin 
	sleep_enable();

  // Do not interrupt before we go to sleep, or the ISR will detach interrupts and we won't wake.
  noInterrupts();

  if (m_pPin18ISR)
  {
    attachInterrupt(digitalPinToInterrupt(m_nINT3Pin18), *m_pPin18ISR, m_nINT3Mode);
    EIFR = bit (INTF3);
  }
  if (m_pPin19ISR)
  {
    attachInterrupt(digitalPinToInterrupt(m_nINT2Pin19), *m_pPin19ISR, m_nINT2Mode);
    EIFR = bit (INTF2);
  }
  if (m_pPin20ISR)
  {
    attachInterrupt(digitalPinToInterrupt(m_nINT1Pin20), *m_pPin20ISR, m_nINT1Mode);
    EIFR = bit (INTF1);
  }
  if (m_pPin21ISR)
  {
    attachInterrupt(digitalPinToInterrupt(m_nINT0Pin21), *m_pPin21ISR, m_nINT0Mode);
    EIFR = bit (INTF0);
  }
  interrupts();
  
  // We are guaranteed that the sleep_cpu call will be done as the processor executes the next instruction after interrupts are turned on.
  interrupts ();  // one cycle
  sleep_cpu ();   // one cycle
}

void CPowerManager::powerUp()
{
	for (uint8_t nI = 0; nI < m_nNumDevices; nI++)
		m_arrayDevices[nI]->turnOn();
}

void CPowerManager::addDevice(const CDevice *pDevice)
{
	m_arrayDevices[m_nNumDevices++] = pDevice;
}







