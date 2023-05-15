#include "servo.h"
#include "ptu_definitions.h"
#include "ptu_i2c.h"

void set_yaw(int value){
	TIM2->CCR2 = value;
}

void set_pitch(int value){
	TIM2->CCR1 = value;
}
