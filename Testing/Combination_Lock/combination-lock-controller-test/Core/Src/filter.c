/*
 * filter.c
 *
 *  Created on: May 8, 2023
 *      Author: richardgan
 */


#include "filter.h"


void window_init(Window *window)
{
	window->idx = 0;
	window->is_full = false;
	memset(window->arr, 0, WINDOW_SIZE * sizeof(int16_t));
}


int16_t moving_average(Window *window, int16_t new_value)
{
	update_window(window, new_value);
	return mean(window);
}


void update_window(Window *window, int16_t new_value)
{
	if (window->idx == WINDOW_SIZE - 1)
	{
		window->is_full = true;
	}

	window->arr[window->idx] = new_value;
	window->idx = (window->idx + 1) % WINDOW_SIZE;
}


int16_t mean(Window *window)
{
	int32_t sum = 0;
	for (uint8_t i = 0; i < WINDOW_SIZE; i++)
	{
		sum += window->arr[i];
	}

	int8_t n_samples = window->is_full ? WINDOW_SIZE : window->idx;  // If the window is not yet full,
																	 // the number of samples is the idx

	return sum / n_samples;  // Error associated with integer division is negligible
}




