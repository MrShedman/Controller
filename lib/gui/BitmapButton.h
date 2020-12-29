#pragma once

#include "Button.h"
#include "Icons.h"

class BitmapButton : public Button
{
public:

	void setBitmap(atlas_id bitmap)
	{
		//m_bitmap = bitmap;

		//m_bitmapShape = getBitmapRect(m_bitmap);
	}

	void draw(bool force_draw = false) override;

private:

	Rect m_bitmapShape;

	//atlas_id m_bitmap;
};
