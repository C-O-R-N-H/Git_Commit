#include "led.h"
#include "timer.h"
#include "ptu.h"

#include "stm32f303xc.h"

bool flashing = false;

void set_led(uint8_t led_mask){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = led_mask;
}
/*void turn_on_led(uint8_t led_mask)
{
	uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;
	*led_register |= led_mask;
}*/


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

void completion_led()
{
	HAL_Delay(LED_DELAY);
	for (uint8_t i = 0; i < 8; i++)
	{
		uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;
		uint8_t old_led_register = *led_register;
		uint8_t last_led = *led_register & (1 << 7);
		uint8_t new_led_register = ((*led_register) << 1) | (last_led >> 7);
		*led_register = old_led_register | new_led_register;
		HAL_Delay(LED_DELAY);
	}
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
		flashing = true;
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
//			led_register->all_leds = original_led_register;
			flashing = false;
			return;
		}
	}

	// Reset the timer counter so that the interrupt is triggered again
	*(timer->TimerCount) = 0;
}


void show_correct_pos_led()
{
	// Show the LEDs only when LEDs are not being used for flashing
	if (!flashing)
	{
		if (ptu_pos_is_correct())
		{
			set_led(0b01010101);
		}
		else
		{
			set_led(0b0);
		}
	}
}
