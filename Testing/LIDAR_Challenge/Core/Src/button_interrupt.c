#include <stdint.h>
#include "stm32f303xc.h"
#include "button_interrupt.h"

#include "led.h"
#include "timer.h"

uint16_t button_start_time;

void enable_button_interrupt()
{
	// Disable the interrupts while messing around with the settings
	__disable_irq();

	// Enable the system configuration controller (SYSCFG in RCC)
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// External Interrupts details on large manual page 294
	// PA0 is on interrupt EXTI0 large manual - page 250
	// EXTI0 in SYSCFG_EXTICR1 needs to be 0x00 (SYSCFG_EXTICR1_EXTI0_PA)
	SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PA;

	// Select EXTI0 interrupt on rising edge
	EXTI->RTSR |= EXTI_RTSR_TR0;  // rising edge of EXTI line 0 (includes PA0)

	// set the interrupt from EXTI line 0 as 'not masked' - as in, enable it.
	EXTI->IMR |= EXTI_IMR_MR0;

	// Tell the NVIC module that EXTI0 interrupts should be handled
	NVIC_SetPriority(EXTI0_IRQn, 6);  // Set Priority
	NVIC_EnableIRQ(EXTI0_IRQn);

	// Re-enable all interrupts (now that we are finished)
	__enable_irq();
}

void button_timer()
{
	// Set the compare value on timer 3 so that we can check if the button is still pressed
	uint16_t current_time = TIM3->CNT;
	uint16_t compare_time = (current_time + poll_interval) % (TIM3->ARR);
	TIM3->CCR1 = compare_time;
}

void EXTI0_IRQHandler()
{
	set_led(0b1);
	button_start_time = TIM3->CNT;
	TIM3->DIER |= TIM_DIER_CC1IE;
	button_timer();
	// Reset the interrupt flag
	EXTI->PR |= EXTI_PR_PR0;
}




