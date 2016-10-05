#pragma once

class LCD;

template <class T>
class Drawable
{
public:

	typedef bool(*Checker)(void);

	Drawable(LCD* display)
	{

	}

	void draw()
	{
		bool should_draw = false;

		if ((micros() - prev_time) > period)
		{
			if (custom_checker != nullptr)
			{
				should_draw = custom_checker();
			}
			else if (*value != prev_value)
			{
				prev_value = *value;
				should_draw = true;
			}
		}


	}

private:

	uint32_t prev_time;
	uint32_t period;

	T* value;
	T prev_value;

	Checker custom_checker = nullptr;

	LCD* display;
};