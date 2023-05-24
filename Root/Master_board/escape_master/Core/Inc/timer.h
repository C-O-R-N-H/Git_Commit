

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

#include "button_interrupt.h"
#include "challenge_2.h"
#include "stm32f303xc.h"

#define PRESCALER 47999  // Prescaler such that at a clock frequency of 48MHz, the period of a count is 1 millisecond

// Values to be controlled through serial
#define button_hold_time 3000
//#define target_distance 500

// Threshold lidar
#define LIDAR_THRESHOLD 75



extern bool unlocked;
extern uint16_t last_period;
extern uint16_t dist_I2C;
//uint16_t button_start_time;

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

void set_next_compare_time(Timer *timer, uint16_t time_ms);

void set_compare_if_vel_not_zero(Timer *timer, int8_t horizontal_vel, int8_t vertical_vel, uint16_t time_ms);


void check_condition();

void TIM4_IRQHandler();

// Handles timer 3 interrupts
void TIM3_IRQHandler();




#endif /* INC_TIMER_H_ */
