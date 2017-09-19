#include "DialogBox.h"
#include "Debug.h"




CDialogBox *CDialogBox::m_pVisible = NULL;

CDialogBox::CDialogBox(Adafruit_TFTLCD *pLCD) : m_strTitle(m_buff)
{
	for (uint8_t nI = 0; nI < MAX_CTRLS; nI++)
	{
		m_arrayCtrls[nI] = NULL;
	}
	m_nNumCtrls = m_nOrientation = 0;
	m_nBGColor = m_nBorderColor = m_nBorderWidth = m_nTitleColor = m_nTitleSize = 0;
	m_pLCD = pLCD;
	m_strButtonID = NULL;
  m_nLastCtrl = 0;
  m_eCtrlType = eDialog;
  m_pParent = NULL;
}


CDialogBox::~CDialogBox()
{
}

void CDialogBox::drawBorder(Adafruit_TFTLCD *pLCD)
{
	uint8_t nI = 0;
	uint16_t nWidth = pLCD->width(), nHeight = pLCD->height();

	for (nI = 0; nI < m_nBorderWidth; nI++)
	{
		pLCD->drawRoundRect(nI, nI, nWidth - (2 * nI), nHeight - (2 * nI), m_nCornerRadius, m_nBorderColor);
	}
}

void CDialogBox::hide()
{
  m_nLastCtrl = -1;
}

void CDialogBox::restore()
{
}

void CDialogBox::show()
{
	if (m_pLCD)
	{
		m_pLCD->setRotation(m_nOrientation);
		m_pLCD->fillScreen(BLACK);
		m_pLCD->fillRoundRect(0, 0, m_pLCD->width(), m_pLCD->height(), m_nCornerRadius, m_nBGColor);
		drawBorder(m_pLCD);
		m_pLCD->setTextSize(m_nTitleSize);
		m_pLCD->setTextColor(m_nTitleColor);
		m_pLCD->setCursor(15, 15);
		m_pLCD->print(m_strTitle);
	}
	for (uint8_t nI = 0; nI < m_nNumCtrls; nI++)
	{
		if (m_arrayCtrls[nI])
			m_arrayCtrls[nI]->show();
	}
	m_pVisible = this;
}

CString *CDialogBox::getBuddyStr()
{
  // Must be implemented but not used by most dialog boxes
}

bool CDialogBox::isVisible()
{
	return m_pVisible == this;
}


void CDialogBox::add(CControl *pControl)
{
	if ((m_nNumCtrls + 1) < MAX_CTRLS)
		m_arrayCtrls[m_nNumCtrls++] = pControl;
}

uint16_t CDialogBox::getCenter(const uint16_t nScreen)
{
	float fHalfScreen = (float)nScreen / 2;
	uint16_t nCenter = (uint16_t)(fHalfScreen);

	return nCenter;
}

void CDialogBox::setCornerRadius(const uint16_t nRadius)
{
	m_nCornerRadius = nRadius;
}

void CDialogBox::setBGColor(const uint16_t nColor)
{
	m_nBGColor = nColor;
}

void CDialogBox::setBorderColor(const uint16_t nColor)
{
	m_nBorderColor = nColor;
}


void CDialogBox::setBorderWidth(const uint16_t nWidth)
{
	m_nBorderWidth = nWidth;
}


void CDialogBox::setTitle(const __FlashStringHelper *strTitle, const uint16_t nTitleColor, const uint16_t nTitleSize)
{
	m_strTitle = strTitle;
	m_nTitleColor = nTitleColor;
	m_nTitleSize = nTitleSize;
}

void CDialogBox::setTitle(const char *strTitle, const uint16_t nTitleColor, const uint16_t nTitleSize)
{
	m_strTitle = strTitle;
	m_nTitleColor = nTitleColor;
	m_nTitleSize = nTitleSize;
}

void CDialogBox::setParent(CDialogBox *pParent)
{
  m_pParent = pParent;
}

CDialogBox *CDialogBox::getParent()
{
  return m_pParent;
}

uint8_t CDialogBox::getOrientation()
{
	return m_nOrientation;
}

void CDialogBox::setOrientation(const uint8_t nOrientation)
{
	m_nOrientation = nOrientation;
}

uint16_t CDialogBox::getBGColor()
{
	return m_nBGColor;
}


uint16_t CDialogBox::getBorderColor()
{
	return m_nBorderColor;
}

uint16_t CDialogBox::getBorderWidth()
{
	return m_nBorderWidth;
}

const char *CDialogBox::getTitle()
{
	return m_strTitle;
}

bool CDialogBox::isLandscape()
{
	return (m_nOrientation == LANDSCAPE1) || (m_nOrientation == LANDSCAPE2);
}

bool CDialogBox::isPortrait()
{
	return (m_nOrientation == PORTRAIT1) || (m_nOrientation == PORTRAIT2);
}

void CDialogBox::calibratePoint(TSPoint &point)
{
	const uint16_t nMinX = 209, nMaxX = 887, nMinY = 195, nMaxY = 904;

	if (m_nOrientation == LANDSCAPE1)
	{
		point.x = map(point.x, nMinX, nMaxX, 0, m_pLCD->height());
		point.y = map(point.y, nMinY, nMaxY, 0, m_pLCD->width());
		swapCoords(point);
		point.x = m_pLCD->width() - point.x;
	}
	else if (m_nOrientation == LANDSCAPE2)
	{
		point.x = map(point.x, nMinX, nMaxX, 0, m_pLCD->height());
		point.y = map(point.y, nMinY, nMaxY, 0, m_pLCD->width());
		swapCoords(point);
		point.y = m_pLCD->height() - point.y;
	}
	else if (m_nOrientation == PORTRAIT1)
	{
		point.x = map(point.x, nMinX, nMaxX, 0, m_pLCD->width());
		point.y = map(point.y, nMinY, nMaxY, 0, m_pLCD->height());
		point.x = m_pLCD->width() - point.x;
		point.y = m_pLCD->height() - point.y;
	}
	else if (m_nOrientation == PORTRAIT2)
	{
		point.x = map(point.x, nMinX, nMaxX, 0, m_pLCD->width());
		point.y = map(point.y, nMinY, nMaxY, 0, m_pLCD->height());
	}
}

void CDialogBox::swapCoords(TSPoint &point)
{

	uint16_t nTemp = point.x;
	point.x = point.y;
	point.y = nTemp;
}

bool CDialogBox::processTouch(TSPoint &point, const uint16_t nScreenWidth, const uint16_t nScreenHight)
{
	bool bResult = false;
	uint8_t nI = 0;

	if (isVisible())
	{
		calibratePoint(point);

		for (nI = 0; nI < m_nNumCtrls; nI++)
		{
			if (m_arrayCtrls[nI])
			{
				if (m_arrayCtrls[nI]->processTouch(point))
				{
          if ((m_nLastCtrl >= 0) && (m_nLastCtrl < m_nNumCtrls) && (m_nLastCtrl != nI))
          {
            m_arrayCtrls[m_nLastCtrl]->onLoseFocus();
          }
					m_nLastCtrl = nI;
					bResult = true;
					break;
				}
			}
		}
	}
	return bResult;
}
