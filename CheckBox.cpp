#include "CheckBox.h"
#include "LCD.h"
#include "TextGFX.h"

void CheckBox::draw(bool force_draw)
{
	if (m_should_draw || force_draw)
	{
		int16_t dy = m_shape.y + m_shape.h / 2 - (m_boxShape.h + 4) / 2;

		atlas_id id = checkbox_unchecked;

		if (isPressed)
		{
			id = checkbox_pressed;
		}
		else if (state)
		{
			id = checkbox_checked;
		}

		display.drawBitmapFromAtlas(bitmap_atlas, id, m_shape.x, dy);

		if (m_text.length() > 0)
		{
			textgfx.setCursor(m_shape.x + m_boxShape.w + 8, m_shape.y + m_shape.h / 2 - 7);
			textgfx.setTextSize(2);
			textgfx.print(m_text);
		}

		m_should_draw = false;
	}
}