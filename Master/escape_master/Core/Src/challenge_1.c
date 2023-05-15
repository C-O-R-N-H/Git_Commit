#include "challenge_1.h"
#include "challenge_2.h"
#include "challenge_3.h"
#include "challenge_4.h"
#include "login.h"
#include "waiting.h"
#include "led.h"
#include "data_handler.h"
#include "serial.h"
#include "stm32f303xc.h"

void enter_challenge_1(){
	set_led(0b1);
	extern int current_game_state;
	while(1){
		if (current_game_state == 2){
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
		else if (current_game_state == 5){
			SerialOutputString("!", &USART1_PORT);
			enter_login();
		}
		else if (current_game_state == 0){
			SerialOutputString("!", &USART1_PORT);
			enter_waiting();
		}
	}
}
