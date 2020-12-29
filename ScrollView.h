#pragma once

#include "Component.h"
#include "Rect.h"

class ScrollView : public Component
{
public:

	ScrollView();

	virtual bool handleTouch(const Touch& t) override;

	virtual void draw(bool force_draw = false) override;

private:

	const static int size = 10;

	Rect rects[size];
};