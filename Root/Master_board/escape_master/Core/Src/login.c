#include "challenge_2.h"
#include "challenge_1.h"
#include "challenge_3.h"
#include "challenge_4.h"
#include "led.h"
#include "main.h"
#include "waiting.h"
#include "serial.h"
#include "data_handler.h"
#include "stm32f303xc.h"
#include "servo.h"
#include <stdio.h>
#include <stdlib.h>


#define YAW 5
#define PITCH 8

void enter_login(){
	set_led(0b00110011);
	extern int current_game_state;
	int yaw_dir = 1;
	int pitch_dir = 1;
	int hor_pos = 1000;
	int vir_pos = 1000;

	extern int16_t yaw_rate;
	extern int16_t pitch_rate;
	extern int16_t roll_rate;
	extern uint8_t string_to_send[64];

	extern int yaw_vel;
	extern int pitch_vel;

	while(1){
		if (current_game_state == 1){
			SerialOutputString("!", &USART1_PORT);
			enter_challenge_1();
		}
		else if (current_game_state == 2){
			SerialOutputString("!", &USART1_PORT);
			enter_challenge_2();
		}
		else if (current_game_state == 3){
			SerialOutputString("!", &USART1_PORT);
			enter_challenge_3();
		}
		else if (current_game_state == 4){
			SerialOutputString("!", &USART1_PORT);
			enter_challenge_4();
		}
		else if (current_game_state == 0){
			SerialOutputString("!", &USART1_PORT);
			enter_waiting();
		}

		yaw_vel = yaw_vel - YAW;
		pitch_vel = pitch_vel - PITCH;

		if (yaw_dir == 1) {
			hor_pos += yaw_vel;
		}
		if (yaw_dir == 0) {
			hor_pos -= yaw_vel;
		}
		if (pitch_dir == 1) {
			vir_pos += pitch_vel;
		}
		else {
			vir_pos -= pitch_vel;
		}

		if (vir_pos > 2400) {
			vir_pos = 2400;
			pitch_dir = 0;
		}
		if (vir_pos < 800) {
			vir_pos = 800;
			pitch_dir = 1;
		}
		if (hor_pos > 2400) {
			hor_pos = 2400;
			yaw_dir = 0;
		}
		if (hor_pos < 800) {
			hor_pos = 800;
			yaw_dir = 1;
		}

		set_yaw(hor_pos);
		set_pitch(vir_pos);

		read_gyro(&yaw_rate, &pitch_rate, &roll_rate);
		roll_rate = abs(roll_rate);
		pitch_rate = abs(pitch_rate);
		yaw_rate = abs(yaw_rate);



		sprintf(string_to_send, "AA557%5i%5i%5i!", roll_rate, pitch_rate, yaw_rate);
		SerialOutputString(string_to_send, &USART1_PORT);


		HAL_Delay(10);


	}
}
