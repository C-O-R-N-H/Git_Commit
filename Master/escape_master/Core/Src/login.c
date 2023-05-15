#include "challenge_2.h"
#include "challenge_1.h"
#include "challenge_3.h"
#include "challenge_4.h"
#include "led.h"
#include "waiting.h"
#include "data_handler.h"
#include "stm32f303xc.h"

void enter_login(){
	set_led(0b1100110011);
	extern int current_game_state;
	while(1){
		if (current_game_state == 1){
			enter_challenge_1();
		}
		else if (current_game_state == 2){
			enter_challenge_2();
		}
		else if (current_game_state == 3){
			enter_challenge_3();
		}
		else if (current_game_state == 4){
			enter_challenge_4();
		}
		else if (current_game_state == 0){
			enter_waiting();
		}
	}
}
