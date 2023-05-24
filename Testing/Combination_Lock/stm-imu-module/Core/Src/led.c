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


void flash_led(Colour colour, uint8_t n_times, uint16_t on_time, uint16_t off_time, Timer *timer)
{
	static Colour static_colour;
	static uint8_t n_flashes_left;
	static uint16_t static_on_time;
	static uint16_t static_off_time;
	static uint8_t original_led_register;
	static bool led_on;
	LEDRegister *led_register = (LEDRegister *)(((uint8_t *)&(GPIOE->ODR)) + 1);

	if (n_times)  // n_times will always be > 0 when called from main and 0 when called from TIM4_IRQHandler
	{			  // We are only interested in the arguments when flash_led is called main
		// Save these arguments for future calls
		static_colour = colour;
		n_flashes_left = n_times;
		static_on_time = on_time;
		static_off_time = off_time;
		original_led_register = led_register->all_leds;
		led_register->all_leds = 0;
		led_on = false;

		// Enable compare interrupt
		*(timer->InterruptEnableRegister) |= TIM_DIER_CC1IE;
	}


	if (!led_on)
	{
		// Turn on LEDs according to colour specified
		switch (static_colour)
		{
			case RED:
				led_register->colour.red_0 = 1;
				led_register->colour.red_1 = 1;
				break;

			case GREEN:
				led_register->colour.green_0 = 1;
				led_register->colour.green_1 = 1;
				break;

			case BLUE:
				led_register->colour.blue_0 = 1;
				led_register->colour.blue_1 = 1;
				break;

			case ORANGE:
				led_register->colour.orange_0 = 1;
				led_register->colour.orange_1 = 1;
				break;
		}

		led_on = true;

		// Set the compare value so that the interrupt is called after `on_time`
		*(timer->CompareRegister1) = static_on_time;
	}

	else
	{
		// Turn off the LEDs
		led_register->all_leds = 0;
		led_on = false;

		// Set the compare value so that the interrupt is called after `off_time`
		*(timer->CompareRegister1) = static_off_time;

		n_flashes_left--;
		if (n_flashes_left == 0)
		{
			// Disable compare interrupt
			*(timer->InterruptEnableRegister) &= ~TIM_DIER_CC1IE;

			// Change the LEDs back to the way they were before the first function call
			led_register->all_leds = original_led_register;
			return;
		}
	}

	// Reset the timer counter so that the interrupt is triggered again
	*(timer->TimerCount) = 0;
}












