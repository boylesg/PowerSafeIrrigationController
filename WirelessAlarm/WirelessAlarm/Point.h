#pragma once

#include <Arduino.h>
#include "Debug.h"




class CPoint
{
	public:
		// Construction, destruction & initialisation
		CPoint();
		CPoint(const int16_t nX, const int16_t nY);
		CPoint(CPoint &point);
		virtual ~CPoint();

		// Interface
		void set(const int16_t nX, const int16_t nY);
		int16_t getX();
		int16_t getY();
		void moveX(const int16_t nAmount);
		void moveY(const int16_t nAmount);
		void moveXY(const int16_t nAmountX, const int16_t nAmountY);
    void dump();
    
		// Operators
		CPoint &operator =(CPoint &point);

	protected:
		int16_t m_nX, m_nY;
};

