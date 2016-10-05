
#pragma once

#include "Arduino.h"
#include "Print.h"
#include "Rect.h"
#include "Point.h"

class TextGFX : public Print
{
public:

	enum Alignment : uint8_t
	{
		Left,
		Middle,
		Right
	};

	TextGFX();

	void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
	void setCursor(int16_t x, int16_t y, Alignment a = Left);
	void setTextColor(uint16_t c);
	void setTextColor(uint16_t c, uint16_t bg);
	void setTextSize(uint8_t s);
	void setTextWrap(bool w);
	void setAlignment(Alignment a);
	void setRotation(uint8_t r);
	Rect getTextBounds(char *string, int16_t x, int16_t y);

	// get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
	Point getCursor() const;
	
protected:

	virtual size_t write(uint8_t c);

	virtual size_t write(const uint8_t *buffer, size_t size);

private:
	
	Point		cursor;
	uint16_t 	textcolor, textbgcolor;
	uint8_t		textsize, rotation;
	bool		wrap;
	Alignment	m_alignment;
};

extern TextGFX textgfx;