/*
 * button_interrupt.h
 *
 *  Created on: 15 May 2023
 *      Author: hirom
 */

#ifndef INC_BUTTON_INTERRUPT_H_
#define INC_BUTTON_INTERRUPT_H_

#define poll_interval 100



void enable_button_interrupt();

void button_timer();

void EXTI0_IRQHandler();

#endif /* INC_BUTTON_INTERRUPT_H_ */
