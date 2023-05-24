

#include "filter.h"


void window_init(Window *window)
{
	window->idx = 0;
	window->is_full = false;
	memset(window->arr, 0, WINDOW_SIZE * sizeof(uint16_t));
}


uint16_t moving_average(Window *window, uint16_t new_value)
{
	update_window(window, new_value);
	return mean(window);
}


void update_window(Window *window, uint16_t new_value)
{
	if (window->idx == WINDOW_SIZE - 1)
	{
		window->is_full = true;
	}

	window->arr[window->idx] = new_value;
	window->idx = (window->idx + 1) % WINDOW_SIZE;
}


uint16_t mean(Window *window)
{
	int32_t sum = 0;
	for (uint16_t i = 0; i < WINDOW_SIZE; i++)
	{
		sum += window->arr[i];
	}

	int8_t n_samples = window->is_full ? WINDOW_SIZE : window->idx;  // If the window is not yet full,
																	 // the number of samples is the idx

	return sum / n_samples;  // Error associated with integer division is negligible
}



