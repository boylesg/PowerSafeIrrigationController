#include "Control.h"
#include "Devices.h"
#include "DialogBox.h"
#include "Debug.h"




CControl::CControl(Adafruit_TFTLCD *pLCD)
{
	m_pParent = NULL;
	m_pLCD = pLCD;
	m_bEnabled = true;
	m_bFocused = false;
  m_nOrientation = 0;
}

CControl::CControl(CControl &control): m_rectEnabled(control.getLeft(), control.getTop(), control.getRight(), control.getBottom(), control.getCornerRadius(), control.getFillColor(), control.getOutlineColor())
{
  set(control);
  setOrientation(control.getOrientation());
}

CControl::~CControl()
{
}

void CControl::hide()
{
  if (m_pLCD && m_pParent)
  {
    if (m_pParent->isControl())
      m_pLCD->fillRect(getLeft(), getTop(), getWidth(), getHeight(), ((CControl*)m_pParent)->getFillColor());
    else if (m_pParent->isDialog())
      m_pLCD->fillRect(getLeft(), getTop(), getWidth(), getHeight(), ((CDialogBox*)m_pParent)->getBGColor());
  }
}

void CControl::processResponse(CControl *pCtrl)
{
}

void CControl::dump()
{
    debug.log(F("CControl object"));
    debug.log(F("---------------"));
    debug.dump(F("m_pParent"), (uint32_t)m_pParent);
    debug.dump(F("m_pParent"), (uint32_t)m_pParent);
    debug.dump(F("m_pLCD"), (uint32_t)m_pLCD);
    debug.dump(F("m_bEnabled"), m_bEnabled);
    debug.dump(F("m_bFocused"), m_bFocused);
    debug.dump(F("m_nOrientation"), m_nOrientation);
    debug.log(F("m_rectDisabled = "));
    m_rectDisabled.dump();
    debug.log(F("m_rectEnabled = "));
    m_rectEnabled.dump();
}

void CControl::set(CControl &control)
{
  CBase::set(control);
  m_rectEnabled = control.m_rectEnabled;
  m_pParent = control.m_pParent;
  m_pLCD = control.m_pLCD;
  m_bEnabled = control.m_bEnabled;
  m_bFocused = control.m_bFocused;
  m_rectDisabled = control.m_rectDisabled;
  m_nOrientation = control.m_nOrientation;

}

bool CControl::isLandscape()
{
  return (m_nOrientation == LANDSCAPE1) || (m_nOrientation == LANDSCAPE2);
}

bool CControl::isPortrait()
{
  return (m_nOrientation == PORTRAIT1) || (m_nOrientation == PORTRAIT2);
}

uint8_t CControl::getOrientation()
{
  return m_nOrientation;
}

void CControl::setOrientation(const uint8_t nOrientation)
{
  m_nOrientation = nOrientation;
}

CControl &CControl::operator =(CControl &control)
{
  set(control);
	return *this;
}

CBase *CControl::getParent()
{
	return m_pParent;
}

void CControl::setParent(CBase *pParent)
{
	m_pParent = pParent;
}

const bool CControl::isEnabled()
{
	return m_bEnabled;
}

void CControl::setEnabled(const bool bEnable)
{
	m_bEnabled = bEnable;
}

const bool CControl::isFocused()
{
	return m_bFocused;;
}

void CControl::setFocus(const bool bFocus)
{
	m_bFocused = bFocus;
}

uint16_t CControl::getCornerRadius()
{
  return m_rectEnabled.getCornerRadius();
}

uint16_t CControl::getFillColor()
{
  return m_rectEnabled.getFillColor();
}

uint16_t CControl::getOutlineColor()
{
  return m_rectEnabled.getOutlineColor();
}

uint16_t CControl::getWidth()
{
  return m_rectEnabled.getWidth();
}

uint16_t CControl::getHeight()
{
  return m_rectEnabled.getHeight();
}

CPoint &CControl::getPos()
{
  return m_rectEnabled.getPos();
}

int16_t CControl::getLeft()
{

  return m_rectEnabled.getLeft();
}

int16_t CControl::getRight()
{
  return m_rectEnabled.getRight();
}

int16_t CControl::getTop()
{
  return m_rectEnabled.getTop();
}

int16_t CControl::getBottom()
{
  return m_rectEnabled.getBottom();
}

uint16_t CControl::getDisabledFillColor()
{
  return m_rectDisabled.getFillColor();
}

uint16_t CControl::getDisabledOutlineColor()
{
  return m_rectDisabled.getOutlineColor();
}

void CControl::setDisabledFillColor(const uint16_t nFillColor)
{
  m_rectDisabled.setFillColor(nFillColor);
}

void CControl::setDisabledBorderColor(const uint16_t nBorderColor)
{
  m_rectDisabled.setOutlineColor(nBorderColor);
}

void CControl::setDisabledColors(const uint16_t nFillColor, const uint16_t nOutlinecolor)
{
	m_rectDisabled.setColors(nFillColor, nOutlinecolor);
}

void CControl::setPos(CPoint &pos)
{
  m_rectEnabled.setPos(pos);
  m_rectDisabled.setPos(pos);
}

void CControl::setPos(const int16_t nX, const int16_t nY)
{
  m_rectEnabled.setPos(nX, nY);
  m_rectDisabled.setPos(nX, nY);
}

void CControl::setSize(CSize &size)
{
  m_rectEnabled.setSize(size);
  m_rectDisabled.setSize(size);
}

void CControl::setSize(const uint16_t nWidth, const uint16_t nHeight)
{
  m_rectEnabled.setSize(nWidth, nHeight);
  m_rectDisabled.setSize(nWidth, nHeight);
}

void CControl::setCornerRadius(const uint8_t nCornerRadius)
{
  m_rectEnabled.setCornerRadius(nCornerRadius);
  m_rectDisabled.setCornerRadius(nCornerRadius);
}

void CControl::setFillColor(const uint16_t nFillColor)
{
  m_rectEnabled.setFillColor(nFillColor);
}

void CControl::setBorderColor(const uint16_t nBorderColor)
{
  m_rectEnabled.setOutlineColor(nBorderColor);
}

void CControl::setColors(const uint16_t nFillColor, const uint16_t nBorderColor)
{
  m_rectEnabled.setFillColor(nFillColor);
  m_rectEnabled.setOutlineColor(nBorderColor);
}

bool CControl::contains(TSPoint &point)
{
  bool bResult = false;
  int16_t nX1 = m_rectEnabled.getLeft(), nY1 = m_rectEnabled.getTop(), 
          nX2 = m_rectEnabled.getLeft() + m_rectEnabled.getWidth(), nY2 = m_rectEnabled.getTop() + m_rectEnabled.getHeight();
/*
  debug.log(F("==============================="));
  debug.dump(F("X"), point.x);
  debug.dump(F("Left"), nX1);
  debug.dump(F("Right"), nX2);
  debug.dump(F("Width"), m_rectEnabled.getWidth());
  debug.dump(F("\nY"), point.y);
  debug.dump(F("Top"), nY1);
  debug.dump(F("Bottom"), nY2);
  debug.dump(F("Height"), m_rectEnabled.getHeight());
  debug.log(F("==============================="));
*/
  bResult = (point.x >= nX1) && (point.x <= nX2) && (point.y >= nY1) && (point.y <= nY2);
  
  return bResult;
}

CString *CControl::getBuddyStr()
{
  // Must be implemented but not used by most controls
  return NULL;
}

bool CControl::processTouch(TSPoint &point)
{
  bool bResult = false;
  
	if ((point.z > MINPRESSURE) && (point.z < MAXPRESSURE))
	{
		if (contains(point) && m_pParent)
    {
		  onTouch();
			bResult = true;
	  }
	}
  return bResult;
}

void CControl::moveX(const int16_t nAmount)
{
  m_rectEnabled.moveX(nAmount);
  m_rectDisabled.moveX(nAmount);
}

void CControl::moveY(const int16_t nAmount)
{
  m_rectEnabled.moveY(nAmount);
  m_rectDisabled.moveY(nAmount);
}

void CControl::moveXY(const int16_t nAmountX, const int16_t nAmountY)
{
  m_rectEnabled.moveXY(nAmountX, nAmountY);
  m_rectDisabled.moveXY(nAmountX, nAmountY);
}



