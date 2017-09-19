#pragma once

#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include "Point.h"
#include "Size.h"




class CRect
{
	public:
		// Construction, destruction & initialisation
		CRect();
    CRect(const int16_t nX, const int16_t nY, const uint16_t nWidth, const uint16_t nHeight, const uint8_t nCornerRadius, 
        const uint16_t nFillColor, const uint16_t nOutlineColor);
    CRect(CPoint &point, CSize &size, const uint8_t nCornerRadius, const uint16_t nFillColor, const uint16_t nOutlineColor);
    CRect(const int16_t nX, const int16_t nY, const uint16_t nWidth, const uint16_t nHeight);
    CRect(CPoint &point, CSize &size);
		CRect(CRect &rect);
		virtual ~CRect();

		// Interface
    void set(const int16_t nX, const int16_t nY, const uint16_t nWidth, const uint16_t nHeight, const uint8_t nCornerRadius, 
             const uint16_t nFillColor, const uint16_t nOutlineColor);
    void set(CRect &rect);
		void setPos(const int16_t nX, const int16_t nY);
		void setSize(const uint16_t nWidth, const uint16_t nHeight);
		void setPos(CPoint &point);
		void setSize(CSize &size);
		void set(CPoint &point, CSize &size, const uint8_t nCornerRadius, const uint16_t nFillColor, const uint16_t nOutlineColor);
		void setCornerRadius(const uint8_t nCornerRadius);
    void setColors(const uint16_t nFillColor, const uint16_t nOutlineColor);
    void setFillColor(const uint16_t nFillColor);
    void setOutlineColor(const uint16_t nOutlineColor);
    void moveX(const int16_t nAmount);
    void moveY(const int16_t nAmount);
    void moveXY(const int16_t nAmountX, const int16_t nAmountY);
		CPoint &getPos();
		CSize &getSize();
		int16_t getLeft();
		int16_t getTop();
		int16_t getRight();
		int16_t getBottom();
		int16_t getWidth();
		int16_t getHeight();
		uint8_t getCornerRadius();
		uint16_t getFillColor();
		uint16_t getOutlineColor();
    virtual bool contains(TSPoint &point);
    virtual bool contains(CPoint &point);
    virtual bool contains(const int16_t nX, const int16_t nY);
    void dump();

		// Operators
		CRect &operator =(CRect &rect);

	protected:
		CPoint m_point;
		CSize m_size;
		uint8_t m_nCornerRadius;
		uint16_t m_nFillColor, m_nOutlineColor;
};

