#pragma once

#include "Container.h"
#include "BitmapButton.h"

class NavigationBar : public Container<3>
{
public:

	NavigationBar();

private:

	BitmapButton b_back, b_home, b_settings;
};

extern NavigationBar navBar;