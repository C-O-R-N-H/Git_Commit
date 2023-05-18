/*
 * timer.c
 *
 *  Created on: 11/05/2023
 *      Author: richardgan
 */

#include "timer.h"
#include "led.h"
#include "ptu.h"

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
		3,
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


// TimerInitialise - Initialise the timer
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


void TIM3_IRQHandler()
{
	// If the interrupt was due to the channel 1 compare flag being raised
	if ((TIM3->SR & TIM_SR_CC1IF) != 0)
	{
		check_ptu_pos();

		// Reset channel 1 compare flag
		TIM3->SR &= ~TIM_SR_CC1IF;
	}
}


void TIM4_IRQHandler()
{
	// If the interrupt was due to the channel 1 compare flag being raised
	if ((TIM4->SR & TIM_SR_CC1IF) != 0)
	{
		flash_led(0, 0, 0, 0, &TIMER4);  // Only the last argument is important, the rest are stored as static variables

		// Reset channel 1 compare flag
		TIM4->SR &= ~TIM_SR_CC1IF;
	}
}














