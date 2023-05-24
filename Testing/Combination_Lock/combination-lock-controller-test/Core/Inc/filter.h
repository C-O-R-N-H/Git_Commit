/*
 * filter.h
 *
 *  Created on: May 8, 2023
 *      Author: richardgan
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define WINDOW_SIZE 5


// Define a sliding window struct
typedef struct
{
	int16_t arr[WINDOW_SIZE];
	uint8_t idx;
	bool is_full;
} Window;

// Initialise the sliding window
void window_init(Window *window);

// Adds `new_value` to `window` and computes the moving average
int16_t moving_average(Window *window, int16_t new_value);

// Adds a value to the window
void update_window(Window *window, int16_t new_value);

// Calculates the mean of the samples in `window`
int16_t mean(Window *window);


#endif /* INC_FILTER_H_ */
