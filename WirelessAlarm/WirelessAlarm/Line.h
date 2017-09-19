#pragma once

#include <Arduino.h>
#include "point.h"
#include "Debug.h"




class CLine
{
	public:
		// Construction, destruction & initialisation
		CLine();
		CLine(const int16_t nX1, const int16_t nY1, const int16_t nX2, const int16_t nY2);
    CLine(CPoint &point1, CPoint &point2);
		CLine(CLine &line);
    virtual ~CLine();

		// Interface
		void set(const int16_t nX1, const int16_t nY1, const int16_t nX2, const int16_t nY2);
    void set(CPoint &point1, CPoint &point2);
    void set(CLine &line);
		int16_t getX1();
		int16_t getY1();
    int16_t getX2();
    int16_t getY2();
    CPoint getPoint1();
    CPoint getPoint2();
		void moveX(const int16_t nAmount);
		void moveY(const int16_t nAmount);
		void moveXY(const int16_t nAmountX, const int16_t nAmountY);
    void dump();
    
		// Operators
    CLine &operator =(CLine &line);

	protected:
		CPoint m_point1, m_point2;
};

