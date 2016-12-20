
#include "TextGFX.h"
#include "glcdfont.c"
#include "LCD.h"

TextGFX textgfx;

TextGFX::TextGFX() : cursor()
{
	textsize  = 1;
	textcolor = textbgcolor = 0xFFFF;
	wrap      = true;
	m_alignment = Left;
}

void TextGFX::drawChar(uint8_t c)
{
	drawChar(cursor.x, cursor.y, c, textcolor, textbgcolor, textsize);
}

size_t TextGFX::write(uint8_t c)
{
	if (c == '\n')
	{
		cursor.y += textsize * 8;
		cursor.x = 0;
	}
	else if (c == '\r')
	{
		// skip em
	}
	else
	{
		if (wrap && ((cursor.x + textsize * 6) >= display.width())) // Heading off edge?
		{
			cursor.x = 0;            // Reset x to zero
			cursor.y += textsize * 8; // Advance y one line
		}

		drawChar(c);
		cursor.x += textsize * 6;
	}

	return 1;
}

size_t TextGFX::write(const uint8_t *buffer, size_t size)
{
	uint8_t count = 0;

	if (m_alignment != Left)
	{
		bool temp_wrap = wrap;
		wrap = false;
		Rect bounds = getTextBounds((char*)buffer, cursor.x, cursor.y);
		wrap = temp_wrap;

		if (m_alignment == Middle) cursor.x -= bounds.w / 2;
		if (m_alignment == Right) cursor.x -= (bounds.w - textsize * 6);
	}

	while (size--)
	{
		uint8_t c = buffer[count];

		count += write(c);
	}
	
	return count;
}

// Draw a character
void TextGFX::drawChar(int16_t x, int16_t y, const unsigned char c, 
	const uint16_t color, const uint16_t bg, const uint8_t size)
 {
    if((x >= display.width())            || // Clip right
       (y >= display.height())           || // Clip bottom
       ((x + 6 * size - 1) < 0) || // Clip left
       ((y + 8 * size - 1) < 0))   // Clip top
      return;

    for(int8_t i=0; i<6; i++ ) 
	{
		uint8_t line;
		if(i < 5) line = pgm_read_byte(font+(c*5)+i);
		else      line = 0x0;
		
		for(int8_t j=0; j<8; j++, line >>= 1) 
		{
			if(line & 0x1) 
			{
				if(size == 1) display.drawPixel(x+i, y+j, color, false);
				else          display.fillRect(Rect(x+(i*size), y+(j*size), size, size), color, false);
			} 
			else if(bg != color) 
			{
				if(size == 1) display.drawPixel(x+i, y+j, bg, false);
				else          display.fillRect(Rect(x+i*size, y+j*size, size, size), bg, false);
			}
		}
    }
}

void TextGFX::setCursor(int16_t x, int16_t y, Alignment a)
{
	cursor = Point(x, y);
	m_alignment = a;
}

Point TextGFX::getCursor() const 
{
	return cursor;
}

void TextGFX::setTextSize(uint8_t s) 
{
	textsize = (s > 0) ? s : 1;
}

void TextGFX::setTextColor(uint16_t c) 
{
  // For 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

void TextGFX::setTextColor(uint16_t c, uint16_t b) 
{
	textcolor   = c;
	textbgcolor = b;
}

void TextGFX::setTextWrap(bool w) 
{
	wrap = w;
}

void TextGFX::setAlignment(Alignment a)
{
	m_alignment = a;
}

// Pass string and a cursor position, returns UL corner and W,H.
Rect TextGFX::getTextBounds(char *str, int16_t x, int16_t y) 
{
	uint8_t c; // Current character

	Rect bounds(x, y, 0, 0);

	uint16_t lineWidth = 0, maxWidth = 0; // Width of current, all lines

	while((c = *str++)) 
	{
		if(c != '\n') 
		{ 
			// Not a newline
			if(c != '\r') 
			{ 
				// Not a carriage return, is normal char
				if(wrap && ((x + textsize * 6) >= display.width())) 
				{
					x  = 0;            // Reset x to 0
					y += textsize * 8; // Advance y by 1 line
					if(lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
					lineWidth  = textsize * 6; // First char on new line
				} 
				else // No line wrap, just keep incrementing X
				{
					lineWidth += textsize * 6; // Includes interchar x gap
					if (lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
				}
			} // Carriage return = do nothing
		} 
		else 
		{ 
			// Newline
			x  = 0;            // Reset x to 0
			y += textsize * 8; // Advance y by 1 line
			if(lineWidth > maxWidth) maxWidth = lineWidth; // Save widest line
			lineWidth = 0;     // Reset lineWidth for new line
		}
	}

	// End of string
	if(lineWidth) y += textsize * 8; // Add height of last (or only) line
 
	bounds.w = maxWidth - 1;               // Don't include last interchar x gap
	bounds.h = y - bounds.y;

	return bounds;
}