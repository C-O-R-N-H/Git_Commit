

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

#include "button_interrupt.h"

#include "stm32f303xc.h"

#define PRESCALER 47999  // Prescaler such that at a clock frequency of 48MHz, the period of a count is 1 millisecond

// Values to be controlled through serial
#define button_hold_time 3000
#define target_distance 500

// Threshold lidar
#define LIDAR_THRESHOLD 150



extern bool unlocked;
extern uint16_t last_period;			// Last_period reading
extern uint16_t lidar_distance;

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

void enable_timer_interrupt(Timer *timer);

// TimerInitialise - Initialise the timer (valid for TIM2/3/4/6/7)
void TimerInitialise(Timer *timer);


// Handles timer 3 interrupts
void TIM3_IRQHandler();




#endif /* INC_TIMER_H_ */
