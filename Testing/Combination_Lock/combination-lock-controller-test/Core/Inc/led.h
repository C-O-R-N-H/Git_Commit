/*
 * led.h
 *
 *  Created on: May 7, 2023
 *      Author: richardgan
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Different ways to access the STM LEDs
typedef union {
    uint8_t all_leds;

    struct
	{
        uint8_t northwest : 1;
        uint8_t north 	  : 1;
        uint8_t northeast : 1;
        uint8_t east 	  : 1;
        uint8_t southeast : 1;
        uint8_t south 	  : 1;
        uint8_t southwest : 1;
        uint8_t west 	  : 1;
	} direction;

    struct
	{
        uint8_t led_0 : 1;
        uint8_t led_1 : 1;
        uint8_t led_2 : 1;
        uint8_t led_3 : 1;
        uint8_t led_4 : 1;
        uint8_t led_5 : 1;
        uint8_t led_6 : 1;
        uint8_t led_7 : 1;
    } led_bits;

    struct
	{
        uint8_t blue_0   : 1;
        uint8_t red_0    : 1;
        uint8_t orange_0 : 1;
        uint8_t green_0  : 1;
        uint8_t blue_1   : 1;
        uint8_t red_1    : 1;
        uint8_t orange_1 : 1;
        uint8_t green_1  : 1;
	} colour;
} LEDRegister;

// Colour of LED
typedef enum
{
	RED = 0,
	GREEN = 1,
	BLUE = 2,
	ORANGE = 3
} Colour;

// Enable the peripheral clocks on the STM
void enable_clocks();

// Enable the LEDs for output on the STM
void initialise_board();

// Set the LED register according to `led_mask`
void set_led(uint8_t led_mask);

// Turn on the specified LEDs without affecting others
void turn_on_led(uint8_t led_mask);

// Turn off the specified LEDs without affecting others
void turn_off_led(uint8_t led_mask);

// Move each LED around clockwise
void chase_led();

// Turn on one LED according to the orientation of the STM
// `sensitivity` is the angle in degrees to which `pitch` and `roll` are compared
void show_orientation_led(int8_t pitch, int8_t roll, int8_t sensitivity);

#endif /* INC_LED_H_ */







