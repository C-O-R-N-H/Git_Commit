/*
 * led.c
 *
 *  Created on: May 7, 2023
 *      Author: richardgan
 */

#include "led.h"
#include "stm32f303xc.h"


void enable_clocks()
{
	// Enable the clocks for GPIOA, C, and E
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOEEN;
}


void initialise_board()
{
	// Get a pointer to the second half word of the MODER register (for outputs pe8-15)
	uint16_t *led_mode_register = ((uint16_t *)&(GPIOE->MODER)) + 1;
	*led_mode_register = 0x5555;
}


void show_orientation_led(int8_t pitch, int8_t roll, int8_t sensitivity)
{
	LEDRegister *led_register = (LEDRegister *)(((uint8_t *)&(GPIOE->ODR)) + 1);
	led_register->all_leds = 0;

	if (pitch < -1 * sensitivity && abs(roll) < sensitivity)
	{
		led_register->direction.north = 1;
	}
	else if (pitch > sensitivity && abs(roll) < sensitivity)
	{
		led_register->direction.south = 1;
	}
	else if (roll < -1 * sensitivity && abs(pitch) < sensitivity)
	{
		led_register->direction.west = 1;
	}
	else if (roll > sensitivity && abs(pitch) < sensitivity)
	{
		led_register->direction.east = 1;
	}
	else if (pitch < -1 * sensitivity && roll < -1 * sensitivity)
	{
		led_register->direction.northwest = 1;
	}
	else if (pitch < -1 * sensitivity && roll > sensitivity)
	{
		led_register->direction.northeast = 1;
	}
	else if (pitch > sensitivity && roll > sensitivity)
	{
		led_register->direction.southeast = 1;
	}
	else if (pitch > sensitivity && roll < -1 * sensitivity)
	{
		led_register->direction.southwest = 1;
	}
}


void set_led(uint8_t led_mask)
{
	uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;
	*led_register = led_mask;
}


void turn_on_led(uint8_t led_mask)
{
	uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;
	*led_register |= led_mask;
}


void turn_off_led(uint8_t led_mask)
{
	uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;
	*led_register &= ~led_mask;
}


void chase_led()
{
	uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;
	uint8_t last_led = *led_register & (1 << 7);
	*led_register = ((*led_register) << 1) | (last_led >> 7);
}








