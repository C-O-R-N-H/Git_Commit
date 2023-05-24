/*
 * timer.c
 *
 *  Created on: 15 May 2023
 *      Author: hirom
 */

#include "timer.h"
#include "led.h"
#include "challenge_2.h"
#include "serial.h"
#include "ptu.h"
#include <stdbool.h>

#include "serial.h"

char test_buffer[100];
extern int current_game_state;

extern int target_distance;

extern uint16_t button_start_time;

Timer TIMER3 = {&(TIM3->CR1),
		&(TIM3->SR),
		&(TIM3->CNT),
		&(TIM3->EGR),
		&(TIM3->CCER),
		&(RCC->APB1ENR),
		RCC_APB1ENR_TIM3EN,
		&(TIM3->PSC),
		PRESCALER,
		&(TIM3->ARR),
		&(TIM3->CCR1),
		&(TIM3->DIER),
		5,
		TIM3_IRQn};

Timer TIMER4 = {&(TIM4->CR1),
		&(TIM4->SR),
		&(TIM4->CNT),
		&(TIM4->EGR),
		&(TIM4->CCER),
		&(RCC->APB1ENR),
		RCC_APB1ENR_TIM4EN,
		&(TIM4->PSC),
		PRESCALER,
		&(TIM4->ARR),
		&(TIM4->CCR1),
		&(TIM4->DIER),
		4,
		TIM4_IRQn};

void enable_timer_interrupt(Timer *timer)
{
	// Disable the interrupts while messing around with the settings
	__disable_irq();

	// Enable compare interrupt
	*(timer->InterruptEnableRegister) |= TIM_DIER_CC1IE;

	// Tell the NVIC module that the timer interrupts should be handled
	NVIC_SetPriority(timer->TimerIRQn, timer->InterruptPriority);  // set priority
	NVIC_EnableIRQ(timer->TimerIRQn);

	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}


// TimerInitialise - Initialise the timer (timer 3)
void TimerInitialise(Timer *timer)
{
	// Enable the clock for the timer
	*(timer->ClockEnableRegister) |= timer->ClockEnableMask;

	// Set the prescaler
	*(timer->PrescalerRegister) = timer->PrescalerValue;

	// Trigger the prescaler value
	*(timer->EventGenerationRegister) |= TIM_EGR_UG;

	// Enable interrupts
	enable_timer_interrupt(timer);

	// Start the timer counter
	*(timer->ControlRegister1) |= TIM_CR1_CEN;
}


void set_next_compare_time(Timer *timer, uint16_t time_ms)
{
	uint16_t current_time = *(timer->TimerCount);
	uint16_t compare_time = (current_time + time_ms) % *(timer->AutoReloadRegister);
	*(timer->CompareRegister1) = compare_time;
}

void set_compare_if_vel_not_zero(Timer *timer, int8_t horizontal_vel, int8_t vertical_vel, uint16_t time_ms)
{
	if (horizontal_vel != 0 || vertical_vel != 0)
	{
		set_next_compare_time(timer, time_ms);
	}
}

void check_condition()
{
	if (GPIOA->IDR & 0x01)  // Button is pressed
	{
		// TESTING
		//sprintf(test_buffer, "last_period: %hu\r\n", last_period);
		//SerialOutputString(test_buffer, &USART1_PORT);

//		sprintf(test_buffer, "%d!\r\n", target_distance);
//		SerialOutputString(test_buffer, &USART1_PORT);


		// Check if LIDAR is in range
		if (abs(last_period - target_distance) <= LIDAR_THRESHOLD)
		{
			uint16_t current_time = TIM3->CNT;
			uint16_t button_end_time = (button_start_time + button_hold_time) % (TIM3->ARR);

			if ((button_end_time > button_start_time) && (current_time >= button_end_time))  // i.e no wraparound
			{
				unlocked = true;
			}
			else if ((button_end_time < button_start_time) && (current_time >= button_end_time) && (current_time < button_start_time))  // i.e. wraparound
			{
				unlocked = true;
			}
			else
			{
				button_timer();
			}
		}
		else
		{
			TIM3->DIER &= ~TIM_DIER_CC1IE;
			set_led(0);
		}
	}
	else
	{
		TIM3->DIER &= ~TIM_DIER_CC1IE;
		set_led(0);
	}
}


void TIM3_IRQHandler()
{

	if ((TIM3->SR & TIM_SR_CC1IF) != 0)
	{
		if (current_game_state == 2){
			chase_led();
			check_condition();
		}
		if (current_game_state == 4){
			check_ptu_pos();
		}
		TIM3->SR &= ~TIM_SR_CC1IF;
	}

//	if (current_game_state == 2){
//		chase_led();
//
//			// If the interrupt was due to the channel 1 compare flag being raised
//		if ((TIM3->SR & TIM_SR_CC1IF) != 0)
//		{
//			check_condition();
//			// Reset channel 1 compare flag
//			TIM3->SR &= ~TIM_SR_CC1IF;
//		}
//	}
//	if (current_game_state == 4){
//		// If the interrupt was due to the channel 1 compare flag being raised
//		if ((TIM3->SR & TIM_SR_CC1IF) != 0)
//		{
//			check_ptu_pos();
//
//			// Reset channel 1 compare flag
//			TIM3->SR &= ~TIM_SR_CC1IF;
//		}
//	}


}

void TIM4_IRQHandler()
{
	// If the interrupt was due to the channel 1 compare flag being raised
	if ((TIM4->SR & TIM_SR_CC1IF) != 0)
	{
		if (current_game_state == 4){
			flash_led(0, 0, 0, 0, &TIMER4);  // Only the last argument is important, the rest are stored as static variables

			// Reset channel 1 compare flag

		}
		TIM4->SR &= ~TIM_SR_CC1IF;
	}
}
