#include "BitmapButton.h"
#include "LCD.h"
#include "TextGFX.h"

void BitmapButton::draw(bool force_draw)
{
	if (m_should_draw || force_draw)
	{
		int16_t dx = m_shape.x + m_shape.w / 2 - m_bitmapShape.w / 2;

		//display.drawBitmapFromAtlas(bitmap_atlas, m_bitmap, dx, m_shape.y);

		if (m_text.length() > 0)
		{
			textgfx.setCursor(m_shape.x + 8, m_shape.y + m_shape.h / 2 - 7);
			textgfx.setTextSize(2);
			textgfx.print(m_text);
		}

		m_should_draw = false;
	}
}