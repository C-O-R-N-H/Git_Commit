#include <stdint.h>
#include "stm32f303xc.h"
#include "button_interrupt.h"

#include "led.h"
#include "timer.h"

uint16_t button_start_time;



void button_timer()
{
	// Set the compare value on timer 3 so that we can check if the button is still pressed
	uint16_t current_time = TIM3->CNT;
	uint16_t compare_time = (current_time + poll_interval) % (TIM3->ARR);
	TIM3->CCR1 = compare_time;
}





