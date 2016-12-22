#pragma once

#include "Container.h"
#include "BitmapButton.h"
#include "Icons.h"
#include "Beeper.h"
#include "StateStack.h"
#include "OptionButton.h"
#include "Numpad.h"

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
			if (OptionButton::isOpen())
			{
				stateStack.clearScreen();
				OptionButton::close();
			}
			else if (numpad.isOpen())
			{
				stateStack.clearScreen();
				numpad.close();
			}
			else
			{
				stateStack.popState();
			}

			beeper(BEEPER_SHORT);
		});

		b_home.setShape(Rect(87, 280, 66, 40));
		b_home.setBitmap(home);

		b_home.setCallback([]()
		{
			if (OptionButton::isOpen())
			{
				OptionButton::close();
			}
			if (numpad.isOpen())
			{
				numpad.close();
			}
			
			stateStack.pushState(State::Home);
			beeper(BEEPER_SHORT);
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