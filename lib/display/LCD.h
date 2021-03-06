
#pragma once

#include <Arduino.h>
#include "Rect.h"

#define ILI9341_SOFTRESET       0x01
#define ILI9341_SLEEPIN         0x10
#define ILI9341_SLEEPOUT        0x11
#define ILI9341_NORMALDISP      0x13
#define ILI9341_INVERTOFF       0x20
#define ILI9341_INVERTON        0x21
#define ILI9341_GAMMASET        0x26
#define ILI9341_DISPLAYOFF      0x28
#define ILI9341_DISPLAYON       0x29
#define ILI9341_COLADDRSET      0x2A
#define ILI9341_PAGEADDRSET     0x2B
#define ILI9341_MEMORYWRITE     0x2C
#define ILI9341_PIXELFORMAT     0x3A
#define ILI9341_FRAMECONTROL    0xB1
#define ILI9341_DISPLAYFUNC     0xB6
#define ILI9341_ENTRYMODE       0xB7
#define ILI9341_POWERCONTROL1   0xC0
#define ILI9341_POWERCONTROL2   0xC1
#define ILI9341_VCOMCONTROL1    0xC5
#define ILI9341_VCOMCONTROL2    0xC7
#define ILI9341_MEMCONTROL      0x36
#define ILI9341_MADCTL  		0x36
#define ILI9341_GMCTRP1			0xE0
#define ILI9341_GMCTRN1			0xE1

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

#define TFTWIDTH   240
#define TFTHEIGHT  320

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

class LCD 
{
public:

	enum Mode : uint8_t
	{
		Full,
		Dimmed,
		Off
	};

	LCD();

	void begin(uint8_t cs_pin, uint8_t cd_pin, uint8_t wr_pin, uint8_t rd_pin, uint8_t rst_pin, uint8_t lite_pin);
	
	void drawPixel(int16_t x, int16_t y, uint16_t color, bool clip = true);
	
	void drawFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
	void drawFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	
	void drawRect(const Rect& r, uint16_t color);
	void fillRect(const Rect& r, uint16_t c, bool clip = true);
	void drawRoundRect(const Rect& r, int16_t radius, uint16_t color);
	void fillRoundRect(const Rect& r, int16_t radius, uint16_t color);
		
	void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

	void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	
	void fillScreen(uint16_t color);
	void reset();

	void setRotation(uint8_t x);
	   // These methods are public in order for BMP examples to work:
	void setAddrWindow(const Rect& w);
	void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

	void drawBitmapFromAtlas(const uint16_t *atlas, int16_t id, int16_t x, int16_t y);
	
	uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
	uint32_t readID();
	uint32_t readReg(uint8_t r);

	// Return the size of the display (per current rotation)
	int16_t width() const 
	{
		return window.w;
	}

	int16_t height() const 
	{
		return window.h;
	}

	void invert(bool flag)
	{
		writeRegister8(flag ? ILI9341_INVERTON : ILI9341_INVERTOFF, 0);
	}

	void setMode(Mode mode)
	{
		if (m_mode == mode) return;
		
		m_mode = mode;

		if (m_mode == Full)
		{
			updateBrightness(m_brightness);
		}
		else if (m_mode == Dimmed)
		{
			updateBrightness(m_dimmed_brightness);
		}
		else if (m_mode == Off)
		{
			updateBrightness(0);
		}
	}

	void setBrightness(uint8_t percent)
	{
		m_brightness = percent;

		if (m_brightness > m_dimmed_brightness)
		{
			m_mode = Full;
		}
		else if (m_brightness > 0)
		{
			m_mode = Dimmed;
		}
		else
		{
			m_mode = Off;
		}

		updateBrightness(m_brightness);
	}

	uint8_t getBrightness() const
	{
		return m_brightness;
	}

	Mode getMode() const
	{
		return m_mode;
	}
	
private:

	void updateBrightness(uint8_t level)
	{
		uint16_t exp = powf(1.05697667, float(level)) - 1;
		analogWrite(lite_pin, exp);
	}

	Mode m_mode;

	uint8_t m_brightness;
	const uint8_t m_dimmed_brightness = 40;

	Rect window;

	int16_t _width, _height, rotation;
	
	void flood(uint16_t color, uint32_t len);

	void setWriteDir();

	void setReadDir();
	
private:
	
	uint8_t cs_pin;
	uint8_t cd_pin;
	uint8_t wr_pin;
	uint8_t rd_pin;
	uint8_t rst_pin;
	uint8_t lite_pin;
	
	inline __attribute__((always_inline))
	void RD_ACTIVE()
	{
		digitalWriteFast(rd_pin, LOW);
	}

	inline __attribute__((always_inline))
	void RD_IDLE()
	{
		digitalWriteFast(rd_pin, HIGH);
	}
	
	inline __attribute__((always_inline))
	void WR_ACTIVE()
	{
		digitalWriteFast(wr_pin, LOW);
	}
	
	inline __attribute__((always_inline))
	void WR_IDLE()
	{
		digitalWriteFast(wr_pin, HIGH);
	}
		
	inline __attribute__((always_inline))
	void WR_STROBE()
	{
		WR_ACTIVE();
		WR_IDLE();
	}
	
	inline __attribute__((always_inline))
	void CD_COMMAND()
	{
		digitalWriteFast(cd_pin, LOW);
	}

	inline __attribute__((always_inline))
	void CD_DATA()
	{
		digitalWriteFast(cd_pin, HIGH);
	}

	inline __attribute__((always_inline))
	void CS_ACTIVE()
	{
		digitalWriteFast(cs_pin, LOW);
	}

	inline __attribute__((always_inline))
	void CS_IDLE()
	{
		digitalWriteFast(cs_pin, HIGH);
	}

	inline __attribute__((always_inline))
	uint8_t read8()
	{
		RD_ACTIVE();
		delayMicroseconds(1);
		uint8_t result = GPIOD_PDIR;
		RD_IDLE();
		return result;
	}

	inline __attribute__((always_inline))
	void write8(uint8_t value)
	{
		GPIOD_PDOR = value;
		WR_STROBE();
	}

	// Set value of TFT register: 8-bit address, 8-bit value
	inline __attribute__((always_inline))
	void writeRegister8(uint8_t reg, uint8_t value)
	{
		CD_COMMAND();
		write8(reg); 

		CD_DATA();
		write8(value);
	}

	// Set value of TFT register: 16-bit address, 16-bit value
	inline __attribute__((always_inline))
	void writeRegister16(uint16_t reg, uint16_t value)
	{
		CD_COMMAND();
		write8(reg >> 8); write8(reg);
 
		CD_DATA();
		write8(value >> 8); write8(value);
	}
	
	// Set value of TFT register: 8-bit address, 32-bit value
	inline __attribute__((always_inline))
	void writeRegister24(uint8_t r, uint32_t d) 
	{
		CS_ACTIVE();
		CD_COMMAND();
		write8(r);
		CD_DATA();
		write8(d >> 16); write8(d >> 8); write8(d);
		CS_IDLE();
	}

	// Set value of TFT register: 8-bit address, 32-bit value
	inline __attribute__((always_inline))
	void writeRegister32(uint8_t r, uint32_t d) 
	{
		CS_ACTIVE();
		CD_COMMAND();
		write8(r);
		CD_DATA();
		write8(d >> 24); write8(d >> 16); write8(d >> 8); write8(d);
		CS_IDLE();
	}

	// Set value of 2 TFT registers: Two 8-bit addresses (hi & lo), 16-bit value
	inline __attribute__((always_inline))
	void writeRegisterPair(uint8_t regHi, uint8_t regLo, uint16_t value)
	{
		CD_COMMAND();
		write8(regHi); 
		CD_DATA();
		write8(value >> 8);

		CD_COMMAND();
		write8(regLo); 
		CD_DATA();
		write8(value);
	}
};

extern LCD display;