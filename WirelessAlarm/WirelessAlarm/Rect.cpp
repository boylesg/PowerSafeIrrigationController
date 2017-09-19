#include "Rect.h"
#include "debug.h"



CRect::CRect()
{
	m_nCornerRadius = 0;
	m_nFillColor = m_nOutlineColor = 0;
}

CRect::CRect(const int16_t nX, const int16_t nY, const uint16_t nWidth, const uint16_t nHeight)
{
  set(nX, nY, nWidth, nHeight, 0, 0, 0);
}

CRect::CRect(CPoint &point, CSize &size)
{
  set(point, size, 0, 0, 0);
}

CRect::CRect(CRect &rect)
{
	set(rect.m_point, rect.m_size, rect.m_nCornerRadius, rect.m_nFillColor, rect.m_nOutlineColor);
}

CRect::CRect(const int16_t nX, const int16_t nY, const uint16_t nWidth, const uint16_t nHeight, const uint8_t nCornerRadius, const uint16_t nFillColor, const uint16_t nOutlineColor)
{
	set(nX, nY, nWidth, nHeight, nCornerRadius, nFillColor, nOutlineColor);
}

CRect::CRect(CPoint &point, CSize &size, const uint8_t nCornerRadius, const uint16_t nFillColor, const uint16_t nOutlineColor)
{
	set(point, size, nCornerRadius, nFillColor, nOutlineColor);
}

CRect::~CRect()
{
}

void CRect::set(CRect &rect)
{
  set(rect.getLeft(), rect.getTop(), rect.getWidth(), rect.getHeight(), rect.getCornerRadius(), rect.getFillColor(), rect.getOutlineColor());
}

void CRect::dump()
{
  debug.log(F("CRect object"));
  debug.log(F("---------------"));
  debug.dump(F("m_nCornerRadius"), m_nCornerRadius);
  debug.dump(F("m_nFillColor"), m_nFillColor);
  debug.dump(F("m_nOutlineColor"), m_nOutlineColor);
  debug.log(F("m_point = "));
  m_point.dump();
  debug.log(F("m_size = "));
  m_size.dump();
}

void CRect::set(const int16_t nX, const int16_t nY, const uint16_t nWidth, const uint16_t nHeight, const uint8_t nCornerRadius, const uint16_t nFillColor, const uint16_t nOutlineColor)
{
	m_point.set(nX, nY);
	m_size.set(nWidth, nHeight);
	m_nCornerRadius = nCornerRadius;
	m_nFillColor = nFillColor;
	m_nOutlineColor = nOutlineColor;
}

void CRect::set(CPoint &point, CSize &size, const uint8_t nCornerRadius, const uint16_t nFillColor, const uint16_t nOutlineColor)
{
  set(point.getX(), point.getY(), size.getWidth(), size.getHeight(), nCornerRadius, nFillColor, nOutlineColor);
}

void CRect::moveX(const int16_t nAmount)
{
  m_point.moveX(nAmount);
}

void CRect::moveY(const int16_t nAmount)
{
  m_point.moveY(nAmount);
}

void CRect::moveXY(const int16_t nAmountX, const int16_t nAmountY)
{
  m_point.moveXY(nAmountX, nAmountY);
}

void CRect::setCornerRadius(const uint8_t nCornerRadius)
{
	m_nCornerRadius = nCornerRadius;
}

void CRect::setFillColor(const uint16_t nFillColor)
{
  m_nFillColor = nFillColor;
}

void CRect::setOutlineColor(const uint16_t nOutlineColor)
{
  m_nOutlineColor = nOutlineColor;
}

void CRect::setColors(const uint16_t nFillColor, const uint16_t nOutlineColor)
{
	m_nFillColor = nFillColor;
	m_nOutlineColor = nOutlineColor;
}

void CRect::setPos(const int16_t nX, const int16_t nY)
{
	m_point.set(nX, nY);
}

void CRect::setSize(const uint16_t nWidth, const uint16_t nHeight)
{
	m_size.set(nWidth, nHeight);
}

void CRect::setPos(CPoint &point)
{
	m_point = point; 
}

void CRect::setSize(CSize &size)
{
	m_size = size;
}

uint8_t CRect::getCornerRadius()
{
	return m_nCornerRadius;
}

uint16_t CRect::getFillColor()
{
	return m_nFillColor;
}

uint16_t CRect::getOutlineColor()
{
	return m_nOutlineColor;
}

CPoint &CRect::getPos()
{
	return m_point;
}

CSize &CRect::getSize()
{
	return m_size;
}

int16_t CRect::getLeft()
{
	return m_point.getX();
}

int16_t CRect::getTop()
{
	return m_point.getY();
}

int16_t CRect::getRight()
{
	return m_point.getX() + m_size.getWidth();
}

int16_t CRect::getBottom()
{
	return m_point.getY() + m_size.getHeight();
}

int16_t CRect::getWidth()
{
	return m_size.getWidth();
}

int16_t CRect::getHeight()
{
	return m_size.getHeight();
}

CRect &CRect::operator =(CRect &rect)
{
  set(rect);
	return *this;
}

bool CRect::contains(TSPoint &point)
{
  return contains(point.x, point.y);
}

bool CRect::contains(CPoint &point)
{
  return contains(point.getX(), point.getY());
}

bool CRect::contains(const int16_t nX, const int16_t nY)
{
  return (nX >= m_point.getX()) && (nX <= m_point.getX() + m_size.getWidth()) && (nY >= m_point.getY()) && (nY <= m_point.getY() + m_size.getHeight());
}



