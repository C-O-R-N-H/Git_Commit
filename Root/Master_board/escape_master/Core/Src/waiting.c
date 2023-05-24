#include "challenge_2.h"
#include "challenge_1.h"
#include "challenge_3.h"
#include "challenge_4.h"
#include "login.h"
#include "waiting.h"
#include "led.h"
#include "servo.h"
#include "data_handler.h"
#include "stm32f303xc.h"
#include "serial.h"

void enter_waiting(){
	set_led(0b11001100);
	set_pitch(1000);
	set_yaw(1000);

	extern int current_game_state;
	while(1){
		if (current_game_state == 1){
			//SerialOutputString("!", &USART1_PORT);
			enter_challenge_1();
		}
		else if (current_game_state == 2){
			//SerialOutputString("!", &USART1_PORT);
			enter_challenge_2();
		}
		else if (current_game_state == 3){
			//SerialOutputString("!", &USART1_PORT);
			enter_challenge_3();
		}
		else if (current_game_state == 4){
			//SerialOutputString("!", &USART1_PORT);
			enter_challenge_4();
		}
		else if (current_game_state == 5){
			//SerialOutputString("!", &USART1_PORT);
			enter_login();
		}
	}
}
