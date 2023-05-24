/*
 * led.h
 *
 *  Created on: May 13, 2023
 *      Author: will
 */

#ifndef INC_LED_H_
#define INC_LED_H_




#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "timer.h"

#define LED_DELAY 500

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

// Set the LED register according to `led_mask`
void set_led(uint8_t led_mask);

// Turn on the specified LEDs without affecting others
void turn_on_led(uint8_t led_mask);

// Turn off the specified LEDs without affecting others
void turn_off_led(uint8_t led_mask);

// Move each LED around clockwise
void chase_led();

void completion_led();

void flash_led(Colour colour, uint8_t n_times, uint16_t on_time, uint16_t off_time, Timer *timer);


void show_correct_pos_led();



#endif /* INC_LED_H_ */
