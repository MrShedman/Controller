#pragma once

#include "Container.h"
#include "BitmapButton.h"
#include "Icons.h"
#include "Beeper.h"
#include "StateStack.h"

extern StateStack stateStack;

class NavigationBar : public Container<3>
{
public:

	NavigationBar()
	{
		b_back.setShape(Rect(15, 280, 66, 40));
		b_back.setBitmap(back);

		b_back.setCallback([]()
		{
			beeper(BEEPER_SHORT);
			stateStack.popState();
		});

		b_home.setShape(Rect(87, 280, 66, 40));
		b_home.setBitmap(home);

		b_home.setCallback([]()
		{
			beeper(BEEPER_SHORT);
			stateStack.pushState(State::Home);
		});

		b_settings.setShape(Rect(159, 280, 66, 40));
		b_settings.setBitmap(settings);

		b_settings.setCallback([]()
		{
			beeper(BEEPER_SHORT);
		});

		pack(&b_back);
		pack(&b_home);
		pack(&b_settings);
	}

private:

	BitmapButton b_back, b_home, b_settings;
};