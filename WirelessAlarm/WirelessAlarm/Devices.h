#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include <MySD.h>
#include "Arduino.h"
#include "PowerControl.h"
#include "Common.h"




// Assign human-readable names to some common 16-bit color values:
#define	BLACK       0x0000 // 0b0000 0000 0000 0000
#define	BLUE        0x001F // 0b0000 0000 0001 1111
#define	GREEN       0x07E0 // 0b0000 0111 1110 0000
#define	RED         0xF800 // 0b1111 1000 0000 0000
#define CYAN        0x07FF // 0b0000 0111 1111 1111
#define MAGENTA     0xF81F // 0b1111 1000 0001 1111
#define YELLOW      0xFFE0 // 0b1111 1111 1110 0000
#define WHITE       0xFFFF // 0b1111 1111 1111 1111
#define LIGHTGREY   0xEEDA // 0b1111 1111 1101 1100
#define GREY        0xDF7D // 0b1101 1111 0111 1011
#define DARKGREY    0x85F7 // 0b1000 0101 1111 0111

#define PORTRAIT1 0
#define PORTRAIT2 2
#define LANDSCAPE1 1
#define LANDSCAPE2 3

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define MARGIN 6

#define RGB(nRedPercent, nGreenpercent, nBluePercent) TFTLCD.rgb(nRedPercent, nGreenpercent, nBluePercent)




class CTFTLCDScreen: public CDevice
{
	public:
		// Construction, destruction & initialisation
		CTFTLCDScreen(const uint8_t nSD_CS, const uint8_t nSD_MOSI, const uint8_t nSD_MISO, const uint8_t nSD_SCK,
						const uint8_t nLCD_CS = A3, const uint8_t nLCD_CD = A2, const uint8_t nLCD_WR = A1, const uint8_t nLCD_RD = A0, const uint8_t nLCD_RESET = A4,
						const uint8_t nTFT_XP = 9, const uint8_t nTFT_YP = A2, const uint8_t nTFT_XM = A3, const uint8_t nTFT_YM = 8, const uint16_t nTFT_RX = 300);
		virtual ~CTFTLCDScreen();
		bool begin();

		// Interface
		uint16_t rgb(const uint8_t nRedPercent, const uint8_t nGreenpercent, const uint8_t nBluePercent);
		bool pollTouch(TSPoint &point);

		// Overrides
		virtual void turnOn();
		virtual void turnOff();
    
		// Data
		Adafruit_TFTLCD m_LCD;
		TouchScreen m_TS;

	protected:
		// Helpers
		bool beginSD();

};

extern CTFTLCDScreen TFTLCD;




class CArduino: public CDevice
{
  public:
    // Construction, destruction & initialisation
    CArduino()
    {
    };
    virtual ~CArduino()
    {
    };
};

extern CArduino Arduino;




class CEsp8266: public CDevice
{
  public:
    // Construction, destruction & initialisation
    CEsp8266()
    {
    };
    virtual ~CEsp8266()
    {
    };
};

extern CEsp8266 Esp8266;

