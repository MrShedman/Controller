
#include <Arduino.h>
#include "LCD.h"

LCD display;

#define swap_int16_t(a, b) \
	{                      \
		int16_t c = b;     \
		b = a;             \
		a = c;             \
	}

LCD::LCD()
{
	// pinMode(cs_pin, OUTPUT);
	// pinMode(cd_pin, OUTPUT);
	// pinMode(wr_pin, OUTPUT);
	// pinMode(rd_pin, OUTPUT);
	// pinMode(rst_pin, OUTPUT);
	// pinMode(lite_pin, OUTPUT);

	// digitalWriteFast(cs_pin, HIGH);
	// digitalWriteFast(cd_pin, HIGH);
	// digitalWriteFast(wr_pin, HIGH);
	// digitalWriteFast(rd_pin, HIGH);

	// setWriteDir(); // Set up LCD data port(s) for WRITE operations

	// setBrightness(100);

	// rotation  = 0;
	// _width    = TFTWIDTH;
	// _height   = TFTHEIGHT;
	// window = Rect(0, 0, TFTWIDTH, TFTHEIGHT);
}

void LCD::begin(uint8_t cs, uint8_t cd, uint8_t wr, uint8_t rd, uint8_t rst, uint8_t lite)
{
	cs_pin = cs;
	cd_pin = cd;
	wr_pin = wr;
	rd_pin = rd;
	rst_pin = rst;
	lite_pin = lite;

	pinMode(cs_pin, OUTPUT);
	pinMode(cd_pin, OUTPUT);
	pinMode(wr_pin, OUTPUT);
	pinMode(rd_pin, OUTPUT);
	pinMode(rst_pin, OUTPUT);
	pinMode(lite_pin, OUTPUT);

	digitalWriteFast(cs_pin, HIGH);
	digitalWriteFast(cd_pin, HIGH);
	digitalWriteFast(wr_pin, HIGH);
	digitalWriteFast(rd_pin, HIGH);

	setWriteDir(); // Set up LCD data port(s) for WRITE operations

	setBrightness(100);

	rotation = 0;
	_width = TFTWIDTH;
	_height = TFTHEIGHT;
	window = Rect(0, 0, TFTWIDTH, TFTHEIGHT);

	reset();

	if (readID() == 0x9341)
	{
		Serial.println(F("Found ILI9341 LCD driver"));
	}

	CS_ACTIVE();
	writeRegister8(ILI9341_SOFTRESET, 0);
	delay(50);
	writeRegister8(ILI9341_DISPLAYOFF, 0);

	writeRegister8(ILI9341_POWERCONTROL1, 0x23);
	writeRegister8(ILI9341_POWERCONTROL2, 0x10);
	writeRegister16(ILI9341_VCOMCONTROL1, 0x2B2B);
	writeRegister8(ILI9341_VCOMCONTROL2, 0xC0);
	writeRegister8(ILI9341_MEMCONTROL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
	writeRegister8(ILI9341_PIXELFORMAT, 0x55);
	writeRegister16(ILI9341_FRAMECONTROL, 0x001B);

	writeRegister8(ILI9341_ENTRYMODE, 0x07);
	//writeRegister32(ILI9341_DISPLAYFUNC, 0x0A822700);

	writeRegister8(ILI9341_SLEEPOUT, 0);
	delay(120);
	writeRegister8(ILI9341_DISPLAYON, 0);

	//delay(500);
	//delay(150);
	//setAddrWindow(window);
}

void LCD::reset(void)
{
	CS_IDLE();
	//CD_DATA;

	WR_IDLE();
	RD_IDLE();

	if (rst_pin)
	{
		digitalWriteFast(rst_pin, LOW);
		delay(2);
		digitalWriteFast(rst_pin, HIGH);
	}

	// Data transfer sync
	CS_ACTIVE();
	CD_COMMAND();
	write8(0x00);
	for (uint8_t i = 0; i < 3; i++)
		WR_STROBE(); // Three extra 0x00s
	CS_IDLE();
}

// Draw a circle outline
void LCD::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0, y0 + r, color);
	drawPixel(x0, y0 - r, color);
	drawPixel(x0 + r, y0, color);
	drawPixel(x0 - r, y0, color);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

void LCD::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x4)
		{
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2)
		{
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8)
		{
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1)
		{
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

void LCD::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
	drawFastVLine(x0, y0 - r, 2 * r + 1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void LCD::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1)
		{
			drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
		}
		if (cornername & 0x2)
		{
			drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
		}
	}
}

// Bresenham's algorithm - thx wikpedia
void LCD::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
				   uint16_t color)
{
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);

	if (steep)
	{
		swap_int16_t(x0, y0);
		swap_int16_t(x1, y1);
	}

	if (x0 > x1)
	{
		swap_int16_t(x0, x1);
		swap_int16_t(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (; x0 <= x1; x0++)
	{
		if (steep)
		{
			drawPixel(y0, x0, color);
		}
		else
		{
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}

// Draw a rectangle
void LCD::drawRect(const Rect &r, uint16_t color)
{
	drawFastHLine(r.x, r.y, r.w, color);
	drawFastHLine(r.x, r.y + r.h - 1, r.w, color);
	drawFastVLine(r.x, r.y, r.h, color);
	drawFastVLine(r.x + r.w - 1, r.y, r.h, color);
}

// Draw a rounded rectangle
void LCD::drawRoundRect(const Rect &r, int16_t rad, uint16_t color)
{
	// smarter version
	drawFastHLine(r.x + rad, r.y, r.w - 2 * rad, color);		   // Top
	drawFastHLine(r.x + rad, r.y + r.h - 1, r.w - 2 * rad, color); // Bottom
	drawFastVLine(r.x, r.y + rad, r.h - 2 * rad, color);		   // Left
	drawFastVLine(r.x + r.w - 1, r.y + rad, r.h - 2 * rad, color); // Right

	// draw four corners
	drawCircleHelper(r.x + rad, r.y + rad, rad, 1, color);
	drawCircleHelper(r.x + r.w - rad - 1, r.y + rad, rad, 2, color);
	drawCircleHelper(r.x + r.w - rad - 1, r.y + r.h - rad - 1, rad, 4, color);
	drawCircleHelper(r.x + rad, r.y + r.h - rad - 1, rad, 8, color);
}

// Fill a rounded rectangle
void LCD::fillRoundRect(const Rect &r, int16_t rad, uint16_t color)
{
	// smarter version
	fillRect(Rect(r.x + rad, r.y, r.w - 2 * rad, r.h), color);

	// draw four corners
	fillCircleHelper(r.x + r.w - rad - 1, r.y + rad, rad, 1, r.h - 2 * rad - 1, color);
	fillCircleHelper(r.x + rad, r.y + rad, rad, 2, r.h - 2 * rad - 1, color);
}

// Draw a triangle
void LCD::drawTriangle(int16_t x0, int16_t y0,
					   int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void LCD::fillTriangle(int16_t x0, int16_t y0,
					   int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1)
	{
		swap_int16_t(y0, y1);
		swap_int16_t(x0, x1);
	}
	if (y1 > y2)
	{
		swap_int16_t(y2, y1);
		swap_int16_t(x2, x1);
	}
	if (y0 > y1)
	{
		swap_int16_t(y0, y1);
		swap_int16_t(x0, x1);
	}

	// Handle awkward all-on-same-line case as its own thing
	if (y0 == y2)
	{
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		drawFastHLine(a, y0, b - a + 1, color);
		return;
	}

	int16_t
		dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1;
	int32_t
		sa = 0,
		sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1; // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++)
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b)
			swap_int16_t(a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y <= y2; y++)
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if (a > b)
			swap_int16_t(a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}
}
void LCD::setAddrWindow(const Rect &w)
{
	CS_ACTIVE();
	writeRegister32(ILI9341_COLADDRSET, (w.x << 16) | (w.x + w.w - 1));
	writeRegister32(ILI9341_PAGEADDRSET, (w.y << 16) | (w.y + w.h - 1));
	CS_IDLE();
}

// Sets the LCD address window.
// Relevant to rect/screen fills and H/V lines.  Input coordinates are
// assumed pre-sorted (e.g. x2 >= x1).
void LCD::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	CS_ACTIVE();
	writeRegister32(ILI9341_COLADDRSET, (x1 << 16) | x2);
	writeRegister32(ILI9341_PAGEADDRSET, (y1 << 16) | y2);
	CS_IDLE();
}

// Fast block fill operation for fillScreen, fillRect, H/V line, etc.
// Requires setAddrWindow() has previously been called to set the fill
// bounds.  'len' is inclusive, MUST be >= 1.
void LCD::flood(uint16_t color, uint32_t len)
{
	uint16_t blocks;
	uint8_t i, hi = color >> 8, lo = color;

	CS_ACTIVE();
	CD_COMMAND();
	write8(0x2C);
	// Write first pixel normally, decrement counter by 1
	CD_DATA();
	write8(hi);
	write8(lo);
	len--;

	blocks = (uint16_t)(len / 64); // 64 pixels/block
	if (hi == lo)
	{
		// High and low bytes are identical.  Leave prior data
		// on the port(s) and just toggle the write STROBE().
		while (blocks--)
		{
			i = 16; // 64 pixels/block / 4 pixels/pass
			do
			{
				WR_STROBE();
				WR_STROBE();
				WR_STROBE();
				WR_STROBE(); // 2 bytes/pixel
				WR_STROBE();
				WR_STROBE();
				WR_STROBE();
				WR_STROBE(); // x 4 pixels
			} while (--i);
		}
		// Fill any remaining pixels (1 to 64)
		for (i = (uint8_t)len & 63; i--;)
		{
			WR_STROBE();
			WR_STROBE();
		}
	}
	else
	{
		while (blocks--)
		{
			i = 16; // 64 pixels/block / 4 pixels/pass
			do
			{
				write8(hi);
				write8(lo);
				write8(hi);
				write8(lo);
				write8(hi);
				write8(lo);
				write8(hi);
				write8(lo);
			} while (--i);
		}
		for (i = (uint8_t)len & 63; i--;)
		{
			write8(hi);
			write8(lo);
		}
	}
	CS_IDLE();
}

void LCD::drawFastHLine(int16_t x, int16_t y, int16_t length, uint16_t color)
{
	Rect c = Rect(x, y, length, 1).getClip(window);

	if (c.area() >= 1)
	{
		setAddrWindow(c);
		flood(color, c.area());
	}
}

void LCD::drawFastVLine(int16_t x, int16_t y, int16_t length, uint16_t color)
{
	Rect c = Rect(x, y, 1, length).getClip(window);

	if (c.area() >= 1)
	{
		setAddrWindow(c);
		flood(color, c.area());
	}
}

void LCD::fillRect(const Rect &r, uint16_t fillcolor, bool clip)
{
	if (clip)
	{
		Rect c = r.getClip(window);

		if (c.area() >= 1)
		{
			setAddrWindow(c);
			flood(fillcolor, c.area());
		}
	}
	else
	{
		setAddrWindow(r);
		flood(fillcolor, r.area());
	}
}

void LCD::fillScreen(uint16_t color)
{
	setAddrWindow(window);
	flood(color, window.area());
}

void LCD::drawPixel(int16_t x, int16_t y, uint16_t color, bool clip)
{
	// Clip
	if (!clip || window.contains(x, y))
	{
		//if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

		setAddrWindow(x, y, x, y);
		CS_ACTIVE();
		CD_COMMAND();
		write8(0x2C);
		CD_DATA();
		write8(color >> 8);
		write8(color);
		CS_IDLE();
	}
}

void LCD::drawBitmapFromAtlas(const uint16_t *atlas, int16_t id, int16_t x, int16_t y)
{
	uint16_t color;

	const uint16_t atlas_width = atlas[0];
	const uint16_t offset = atlas[2] * 4 + 3;

	uint16_t x1 = atlas[id * 4 + 3 + 0];
	uint16_t y1 = atlas[id * 4 + 3 + 1];
	uint16_t x2 = atlas[id * 4 + 3 + 2];
	uint16_t y2 = atlas[id * 4 + 3 + 3];
	uint16_t w = x2 - x1;
	uint16_t h = y2 - y1;

	setAddrWindow(x, y, x + w, y + h);

	CS_ACTIVE();
	CD_COMMAND();
	write8(0x2C);
	CD_DATA();

	for (uint16_t iy = y1; iy <= y2; ++iy)
	{
		for (uint16_t ix = x1; ix <= x2; ++ix)
		{
			color = atlas[offset + ix + (iy * atlas_width)];
			write8(color >> 8);
			write8(color);
		}
	}

	CS_IDLE();
}

void LCD::setRotation(uint8_t x)
{
	CS_ACTIVE();

	uint16_t t = 0;

	switch (rotation)
	{
	case 1:
		t = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
		break;
	case 2:
		t = ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR;
		break;
	case 3:
		t = ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
		break;
	default:
		t = ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR;
	}
	writeRegister8(ILI9341_MADCTL, t); // MADCTL

	setAddrWindow(0, 0, _width - 1, _height - 1);
}

uint32_t LCD::readID(void)
{
	return readReg(0xD3);
}

uint32_t LCD::readReg(uint8_t r)
{
	CS_ACTIVE();
	CD_COMMAND();
	write8(r);
	setReadDir(); // Set up LCD data port(s) for READ operations
	CD_DATA();
	delayMicroseconds(50);

	uint32_t id = (read8() << 24) | (read8() << 16) | (read8() << 8) | read8();

	CS_IDLE();
	setWriteDir(); // Restore LCD data port(s) to WRITE configuration

	//Serial.print("Read $"); Serial.print(r, HEX);
	//Serial.print(":\t0x"); Serial.println(id, HEX);
	return id;
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t LCD::color565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void LCD::setWriteDir()
{
	pinMode(2, OUTPUT);
	pinMode(14, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(20, OUTPUT);
	pinMode(21, OUTPUT);
	pinMode(5, OUTPUT);
}

void LCD::setReadDir()
{
	pinMode(2, INPUT);
	pinMode(14, INPUT);
	pinMode(7, INPUT);
	pinMode(8, INPUT);
	pinMode(6, INPUT);
	pinMode(20, INPUT);
	pinMode(21, INPUT);
	pinMode(5, INPUT);
}
