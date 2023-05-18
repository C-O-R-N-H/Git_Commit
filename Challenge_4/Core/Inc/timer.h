/*
 * timer.h
 *
 *  Created on: 11/05/2023
 *      Author: richardgan
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

#include "stm32f303xc.h"

#define PRESCALER 47999  // Prescaler such that at a clock frequency of 48MHz, the period of a count is 1 millisecond


typedef struct
{
	volatile uint32_t *ControlRegister1;
	volatile uint32_t *StatusRegister;
	volatile uint32_t *TimerCount;
	volatile uint32_t *EventGenerationRegister;
	volatile uint32_t *CompareEnableRegister;
	volatile uint32_t *ClockEnableRegister;
	volatile uint32_t ClockEnableMask;
	volatile uint32_t *PrescalerRegister;
	volatile uint32_t PrescalerValue;
	volatile uint32_t *AutoReloadRegister;
	volatile uint32_t *CompareRegister1;
	volatile uint32_t *InterruptEnableRegister;
	volatile uint32_t InterruptPriority;
	volatile uint32_t TimerIRQn;
} Timer;


// Declare timers as extern
extern Timer TIMER3;
extern Timer TIMER4;

void enable_timer_interrupt(Timer *timer);

// TimerInitialise - Initialise the timer (valid for TIM2/3/4/6/7)
void TimerInitialise(Timer *timer);

// Set the compare time `time_ms` milliseconds ahead of the current time
void set_next_compare_time(Timer *timer, uint16_t time_ms);

// Set the next compare time equal to current time + time_ms if the velocity is not zero
void set_compare_if_vel_not_zero(Timer *timer, int8_t horizontal_vel, int8_t vertical_vel, uint16_t time_ms);

// Handles timer 3 interrupts
void TIM3_IRQHandler();

// Handles timer 4 interrupts
void TIM4_IRQHandler();


#endif /* INC_TIMER_H_ */
