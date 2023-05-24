#include "led.h"

#include "stm32f303xc.h"

void set_led(int set){
	uint8_t *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;
	*led_register = set;
}
