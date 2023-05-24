#include "data_handler.h"
#include "stm32f303xc.h"
#include "main.h"

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

	if (game_state == 5){
		enter_login();
	}
	if (game_state == 9){
		enter_pressure();
	}

}
