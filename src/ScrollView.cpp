#include "ScrollView.h"
#include "LCD.h"

ScrollView::ScrollView()
{
	int w = 200;
	int h = 10;

	for (int i = 0; i < size; ++i)
	{
		rects[i] = Rect(20, i*h + 50, w, h);
	}
}

float vel1;

bool ScrollView::handleTouch(const Touch& t)
{
	bool used_event = false;

	if (t.gesture != Touch::Gesture::none)
	{
		//display.fillRect(rect, BLACK);

		const int16_t m = abs(t.gestureVel);
		vel1 = t.gestureVel*3.0f;

		if (t.gesture == Touch::Gesture::swipe_down ||
			t.gesture == Touch::Gesture::swipe_up)
		{
			for (int i = 0; i < size; ++i)
			{
				rects[i].move(0, vel1);
			}
		}
	}

	return used_event;
}

void ScrollView::draw(bool force_draw)
{
	const Rect window = Rect(20, 50, 200, 200);
	
	const float decay = 0.95f;
	vel1 *= decay;

	if (abs(vel1) > 1.0f)
	{
		for (int i = 0; i < size; ++i)
		{
			rects[i].move(0, vel1);
		}

		m_should_draw = true;
	}

	if (m_should_draw || force_draw)
	{
		display.fillRect(window, BLACK);

		for (int i = 0; i < size; ++i)
		{
			if (i % 2)
			{
				display.fillRect(rects[i].getClip(window), 0x1F1F);
			}
			else
			{
				display.fillRect(rects[i].getClip(window), 0x07E0);
			}
		}

		m_should_draw = false;
	}
}