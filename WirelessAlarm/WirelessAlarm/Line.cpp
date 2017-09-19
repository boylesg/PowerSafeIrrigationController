#include "Line.h"
#include "Debug.h"



CLine::CLine()
{
}

CLine::CLine(const int16_t nX1, const int16_t nY1, const int16_t nX2, const int16_t nY2)
{
  set(nX1, nY1, nX2, nY2);
}

CLine::CLine(CPoint &point1, CPoint &point2)
{
  set(point1, point2);
}


CLine::CLine(CLine &line)
{
  set(line);
}

CLine::~CLine()
{
}

void CLine::moveX(const int16_t nAmount)
{
  m_point1.moveX(nAmount);
  m_point2.moveX(nAmount);
}

void CLine::moveY(const int16_t nAmount)
{
  m_point1.moveY(nAmount);
  m_point2.moveY(nAmount);
}

void CLine::moveXY(const int16_t nAmountX, const int16_t nAmountY)
{
  m_point1.moveXY(nAmountX, nAmountX);
  m_point2.moveXY(nAmountX, nAmountY);
}

void CLine::dump()
{
  debug.log(F("CLine object"));
  debug.log(F("---------------"));
  debug.dump(F("m_point1.m_nX"), m_point1.getX());
  debug.dump(F("m_point1.m_nY"), m_point1.getY());
  debug.dump(F("m_point2.m_nX"), m_point2.getX());
  debug.dump(F("m_point2.m_nY"), m_point2.getY());
}

void CLine::set(const int16_t nX1, const int16_t nY1, const int16_t nX2, const int16_t nY2)
{
  m_point1.set(nX1, nY1);
  m_point2.set(nX2, nY2);
}

void CLine::set(CPoint &point1, CPoint &point2)
{
  m_point1 = point1;
  m_point2 = point2;
}


void CLine::set(CLine &line)
{
  m_point1 = line.m_point1;
  m_point2 = line.m_point2;
}

int16_t CLine::getX1()
{
	return m_point1.getX();
}

int16_t CLine::getY1()
{
	return m_point1.getY();
}

int16_t CLine::getX2()
{
  return m_point2.getX();
}

int16_t CLine::getY2()
{
  return m_point2.getY();
}

CPoint CLine::getPoint1()
{
  return m_point1;
}

CPoint CLine::getPoint2()
{
  return m_point2;
}

CLine &CLine::operator =(CLine &line)
{
  set(line);
  return *this;
}


