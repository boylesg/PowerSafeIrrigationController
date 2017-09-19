#include "Size.h"
#include "Debug.h"



CSize::CSize()
{
	m_nWidth = m_nHeight = 0;
}

CSize::CSize(const uint16_t nWidth, const uint16_t nHeight)
{
	set(nWidth, nHeight);
}

CSize::CSize(CSize &size)
{
	set(size.m_nWidth, size.m_nHeight);
}

CSize::~CSize()
{
}

void CSize::dump()
{
  debug.log(F("CSize object"));
  debug.log(F("---------------"));
  debug.dump(F("m_nWidth"), m_nWidth);
  debug.dump(F("m_nHeight"), m_nHeight);
}

void CSize::set(const uint16_t nWidth, const uint16_t nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

uint16_t CSize::getWidth()
{
	return m_nWidth;
}

uint16_t CSize::getHeight()
{
	return m_nHeight;
}

CSize &CSize::operator =(CSize &size)
{
	m_nWidth = size.m_nWidth;
	m_nHeight = size.m_nHeight;
	return *this;
}
