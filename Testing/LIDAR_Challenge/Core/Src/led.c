
#include "led.h"
#include "stm32f303xc.h"



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

void completion_led()
{
	HAL_Delay(LED_DELAY);
	for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++)
	{
		uint8_t *led_register = ((uint8_t *)&(GPIOE->ODR)) + 1;
		uint8_t old_led_register = *led_register;
		uint8_t last_led = *led_register & (1 << 7);
		uint8_t new_led_register = ((*led_register) << 1) | (last_led >> 7);
		*led_register = old_led_register | new_led_register;
		HAL_Delay(LED_DELAY);
	}
}


