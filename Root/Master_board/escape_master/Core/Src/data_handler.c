#include "data_handler.h"
#include "challenge_1.h"
#include "challenge_2.h"
#include "challenge_3.h"
#include "challenge_4.h"
#include "login.h"
#include "waiting.h"
#include "stm32f303xc.h"
#include "led.h"

void state_handler(char *state, int *game_state){
	int state_int = atoi(state);
	*game_state = state_int;
//	if (state_int == 01){
//		enter_challenge_1();
//	}
//	else if(state_int == 02){
//		enter_challenge_2();
//	}

	//set_led(0b10101010);
}

void check_state(int game_state){
	if (game_state == 1){
		enter_challenge_1();
	}
	else if (game_state == 2){
		enter_challenge_2();
	}
	else if (game_state == 3){
		enter_challenge_3();
	}
	else if (game_state == 4){
		enter_challenge_4();
	}
	else if (game_state == 5){
		enter_login();
	}
	else if (game_state == 0){
		enter_waiting();
	}
}
