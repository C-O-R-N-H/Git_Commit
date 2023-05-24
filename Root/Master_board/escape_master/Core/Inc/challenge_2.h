/*
 * challenge_2.h
 *
 *  Created on: May 13, 2023
 *      Author: will
 */

#ifndef INC_CHALLENGE_2_H_
#define INC_CHALLENGE_2_H_

#include "ptu_definitions.h"
#include "ptu_i2c.h"
#include <stdbool.h>
#include <stdint.h>

//uint16_t rise_time = 0;
//uint16_t last_period = 0;
//uint16_t last_capture = 0;
//bool unlocked = false;

void enter_challenge_2();

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#endif /* INC_CHALLENGE_2_H_ */
