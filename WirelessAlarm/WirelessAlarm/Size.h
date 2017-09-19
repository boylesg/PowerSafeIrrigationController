#pragma once

#include <Arduino.h>




class CSize
{
	public:
		// Construction, destruction & initialisation
		CSize();
		CSize(const uint16_t nWidth, const uint16_t nHeight);
		CSize(CSize &size);
		virtual ~CSize();

		// Interface
		void set(const uint16_t nWidth, const uint16_t nHeight);
		uint16_t getWidth();
		uint16_t getHeight();
    void dump();  

		// Operators
		CSize &operator =(CSize &size);

	protected:
		uint16_t m_nWidth, m_nHeight;
};

