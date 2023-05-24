#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "challenge_2.h"
#include "challenge_1.h"
#include "challenge_3.h"
#include "challenge_4.h"
#include "login.h"
#include "led.h"
#include "waiting.h"
#include "serial.h"
#include "servo.h"
#include "timer.h"
#include "ptu.h"
#include "data_handler.h"
#include "stm32f303xc.h"

#define HOLD_TIME 2500

int pitch_vel = 0;
int yaw_vel = 0;
bool solved = false;
uint16_t delay = 0;

extern uint16_t vertical_PWM;
extern uint16_t horizontal_PWM;

void enter_challenge_4(){
	set_led(0b1111);
	extern int current_game_state;
	uint8_t string_to_send[64] = "This is a string !\r\n";
	set_pitch(1500);
	set_yaw(1500);
	vertical_PWM = 1500;
	horizontal_PWM = 1500;
	solved = false;


	while(1){
		if (current_game_state == 1){
			SerialOutputString("!", &USART1_PORT);
			enter_challenge_1();
		}
		else if (current_game_state == 3){
			SerialOutputString("!", &USART1_PORT);
			enter_challenge_3();
		}
		else if (current_game_state == 2){
			SerialOutputString("!", &USART1_PORT);
			enter_challenge_2();
		}
		else if (current_game_state == 5){
			SerialOutputString("!", &USART1_PORT);
			enter_login();
		}
		else if (current_game_state == 0){
			SerialOutputString("!", &USART1_PORT);
			enter_waiting();
		}

		//sprintf(string_to_send, "AA550%d%d!", pitch_vel, yaw_vel);
		//SerialOutputString(string_to_send, &USART1_PORT);
//
//
//		if (pitch_vel == 12 && yaw_vel == 12){
//			set_led(0b11111111);
//		}
//		else{
//			set_led(0b0);
//		}

//		sprintf(string_to_send, "AA550, %d, %d!", horizontal_PWM, vertical_PWM);
//		SerialOutputString(string_to_send, &USART1_PORT);


		//set and check
		if (!solved)
		{

			show_correct_pos_led();  // Display 4 LEDs if the position is correct
//			if (horizontal_PWM < 100 + 800 && horizontal_PWM > 800 - 100){
//				set_led(0b11111111);
//			}

			if (delay)
			{
				HAL_Delay(delay);
				delay = 0;
				set_next_compare_time(&TIMER3, HOLD_TIME);  // After recording a value, set compare time
				// Without the above function, the velocity may be 0 for > HOLD_TIME without timer compare triggering
			}

			// Move the PTU -- For some reason, this doesn't work inside the function `move_ptu`
			const uint16_t PWM_min = 600;
			const uint16_t PWM_max = 2000;

			horizontal_PWM += (yaw_vel - 12);
			vertical_PWM += (pitch_vel - 12);

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

			TIM2->CCR1 = vertical_PWM;
			TIM2->CCR2 = horizontal_PWM;

			set_compare_if_vel_not_zero(&TIMER3, yaw_vel - 12, pitch_vel - 12, HOLD_TIME);  // Keep PTU still for 2000 ms to record value

			HAL_Delay(10);
		}

		else
		{
			set_led(0b11111111);
			SerialOutputString("AA5521!", &USART1_PORT);
		}


	}
}
