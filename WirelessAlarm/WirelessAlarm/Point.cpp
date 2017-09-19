#include "Point.h"
#include "Debug.h"



CPoint::CPoint()
{
	m_nX = m_nY = 0;
}

CPoint::CPoint(const int16_t nX, const int16_t nY)
{
	set(nX, nY);
}

CPoint::CPoint(CPoint &point)
{
  set(point.m_nX, point.m_nY);
}

CPoint::~CPoint()
{
}

void CPoint::moveX(const int16_t nAmount)
{
	m_nX += nAmount;
}

void CPoint::moveY(const int16_t nAmount)
{
	m_nY += nAmount;
}

void CPoint::moveXY(const int16_t nAmountX, const int16_t nAmountY)
{
	m_nX += nAmountX;
	m_nY += nAmountY;
}

void CPoint::dump()
{
  debug.log(F("CPoint object"));
  debug.log(F("---------------"));
  debug.dump(F("m_nX"), m_nX);
  debug.dump(F("m_nY"), m_nY);
}

void CPoint::set(const int16_t nX, const int16_t nY)
{
	m_nX = nX;
	m_nY = nY;
}

int16_t CPoint::getX()
{
	return m_nX;
}

int16_t CPoint::getY()
{
	return m_nY;
}

CPoint &CPoint::operator =(CPoint &point)
{
	m_nX = point.m_nX;
	m_nY = point.m_nY;
	return *this;
}

