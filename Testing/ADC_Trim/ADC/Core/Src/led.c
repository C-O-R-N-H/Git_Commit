#include "led.h"


#include "stm32f303xc.h"

bool flashing = false;

void set_led(uint8_t led_mask){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = led_mask;
}
