#include <Arduino.h>
#include <MySD.h>
#include "Common.h"
#include "debug.h"
#include "Devices.h"
#include "MainMenu.h"
#include "TextFile.h"




#define TS_YP A3  // must be an analog pin, use "An" notation!
#define TS_XM A2  // must be an analog pin, use "An" notation!
#define TS_YM 9   // can be a digital pin
#define TS_XP 8   // can be a digital pin

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

#define SD_CS 10
#define SD_MOSI 11
#define SD_MISO 12
#define SD_SCK 13




CTFTLCDScreen::CTFTLCDScreen(const uint8_t nSD_CS, const uint8_t nSD_MOSI, const uint8_t nSD_MISO, const uint8_t nSD_SCK,
	const uint8_t nLCD_CS, const uint8_t nLCD_CD, const uint8_t nLCD_WR, const uint8_t nLCD_RD, const uint8_t nLCD_RESET,
	const uint8_t nTFT_XP, const uint8_t nTFT_YP, const uint8_t nTFT_XM, const uint8_t nTFT_YM, const uint16_t nTFT_RX):
	m_LCD(nLCD_CS, nLCD_CD, nLCD_WR, nLCD_RD, nLCD_RESET), m_TS(nTFT_XP, nTFT_YP, nTFT_XM, nTFT_YM, nTFT_RX)
{
}

CTFTLCDScreen::~CTFTLCDScreen()
{
}

bool CTFTLCDScreen::beginSD()
{
	bool bResult = false;
    
  debug.log(F("Initializing SD card..."), false);
  pinMode(SD.getSCLK(), OUTPUT);

	if (!SD.begin(SD_CS, SD_MOSI, SD_MISO, SD_SCK))
		debug.log(F("failed!"));
	else
	{
		debug.log(F("done!"));
		bResult = true;
	}
	return bResult;
}

bool CTFTLCDScreen::begin()
{
	bool bResult = false;
 
	if (beginSD())
    bResult = true;
  else
		debug.logRuntimeError(F("TFTLCDShield.cpp"), __LINE__);

	m_LCD.begin();

	return bResult;
}

void CTFTLCDScreen::turnOff()
{
	const uint8_t nMode = INPUT;

	SD.end();
	pinMode(2, nMode);
	pinMode(3, nMode);
	pinMode(4, nMode);
	pinMode(5, nMode);
	pinMode(6, nMode);
	pinMode(7, nMode);
	pinMode(8/*TS_XP*/, nMode);
	pinMode(9/*TS_YM*/, nMode);
	pinMode(10/*SD_CS*/, nMode);
	pinMode(11/*SD_MOSI*/, nMode);
	pinMode(12/*SD_MISO*/, nMode);
	pinMode(13/*SD_SCK*/, nMode);
	pinMode(A0/*LCD_RD*/, nMode);
	pinMode(A1/*LCD_WR*/, nMode);
	pinMode(A2/*TS_XM, LCD_CD*/, nMode);
	pinMode(A3/*TS_YP, LCD_CS*/, nMode);
  pinMode(A4/*LCD_RESET*/, nMode);
  pinMode(A5, nMode);
  pinMode(A6, nMode);
	CDevice::turnOff();
}

void CTFTLCDScreen::turnOn()
{
  extern CMainMenu dlgMainMenu;

  CDevice::turnOn();
  begin();
  dlgMainMenu.show();
}

uint16_t CTFTLCDScreen::rgb(const uint8_t nRedPercent, const uint8_t nGreenpercent, const uint8_t nBluePercent)
{
	// RRRR RGGG GGGB BBBB
	// 0000 0000 0000 0000
	const uint16_t nFullSatRB = 0b0000000000011111,
		             nFullSatG = 0b0000000000111111;
	uint16_t nRed = (nRedPercent * nFullSatRB + 50) / 100,
		       nGreen = (nGreenpercent * nFullSatG + 50) / 100,
		       nBlue = (nBluePercent  * nFullSatRB + 50) / 100,
		        nColor = (nRed << 11) | (nGreen << 5) | nBlue;
	return nColor;
}

bool CTFTLCDScreen::pollTouch(TSPoint &point)
{
	bool bResult = false;

  //debug.log(F("1111111111"));
  
	if (m_TS.isTouching())
	{
	  //debug.log(F("2222222222"));

	  bResult = true;
		digitalWrite(SD.getSCLK(), HIGH);
		point = m_TS.getPoint();
		digitalWrite(SD.getSCLK(), LOW);

		// If sharing pins, you'll need to fix the directions of the touchscreen pins
		pinMode(m_TS.getXM(), OUTPUT);
		pinMode(m_TS.getYP(), OUTPUT);
	}
	return bResult;
}

CTFTLCDScreen TFTLCD(SD_CS, SD_MOSI, SD_MISO, SD_SCK, LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET, TS_XP, TS_YP, TS_XM, TS_YM, 300);
CArduino Arduino;
CEsp8266 Esp8266;

