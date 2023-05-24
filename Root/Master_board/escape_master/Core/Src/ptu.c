/*
 * ptu.c
 *
 *  Created on: 13/05/2023
 *      Author: richardgan
 */

#include "ptu.h"
#include "led.h"
#include "timer.h"




// Array containing the positions (horizontal_PWM, vertical_PWM) required to open the lock
const uint16_t positions[3][2] = {
		{800, 1800},
		{2000, 600},
		{1500, 1500}
};

uint8_t curr_pos = 0;

extern uint8_t string_to_send[64];

void move_ptu(TIM_TypeDef *tim, int16_t ptu_vel_yaw, int16_t ptu_vel_pitch)
{
	const uint16_t PWM_min = 600;
	const uint16_t PWM_max = 2400;

	horizontal_PWM += ptu_vel_yaw;
	vertical_PWM += ptu_vel_pitch;

	if (horizontal_PWM > PWM_max)
	{
		horizontal_PWM = PWM_max;
	}

	if (horizontal_PWM < PWM_min)
	{
		horizontal_PWM = PWM_min;
	}

	if (vertical_PWM > PWM_max)
	{
		vertical_PWM = PWM_max;
	}

	if (vertical_PWM < PWM_min)
	{
		vertical_PWM = PWM_min;
	}

	tim->CCR1 = horizontal_PWM;
	tim->CCR2 = vertical_PWM;
}


bool ptu_pos_is_correct()
{
	//sprintf(string_to_send, "AA550, %d, %d!", horizontal_PWM, vertical_PWM);
	//SerialOutputString(string_to_send, &USART1_PORT);
	bool horizontal_pos_is_correct = abs(horizontal_PWM - positions[curr_pos][0]) < TOLERANCE;
	bool vertical_pos_is_correct = abs(vertical_PWM - positions[curr_pos][1]) < TOLERANCE;
	//bool horizontal_pos_is_correct = horizontal_PWM < TOLERANCE + 800 && horizontal_PWM > 800 - TOLERANCE;
	//bool vertical_pos_is_correct = vertical_PWM < TOLERANCE + 1800 && vertical_PWM > 1800 - TOLERANCE;
	return horizontal_pos_is_correct && vertical_pos_is_correct;
}


void check_ptu_pos()
{
	if (ptu_pos_is_correct())
	{
		curr_pos++;
		if (curr_pos == NUM_POSITIONS)
		{
//			flash_led(GREEN, 3, 800, 400, &TIMER4);
//			delay = 3200;

			// Disable compare interrupts
			TIM3->DIER &= TIM_DIER_CC1IE;

			solved = true;
		}

		else
		{
			flash_led(GREEN, 1, 800, 0, &TIMER4);
			delay = 800;
		}

	}

	else  // The position is incorrect and the lock is reset
	{
		curr_pos = 0;
		flash_led(RED, 2, 800, 400, &TIMER4);
		delay = 2000;
	}
}







