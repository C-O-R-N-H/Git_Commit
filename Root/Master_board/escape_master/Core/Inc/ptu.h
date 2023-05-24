/*
 * ptu.h
 *
 *  Created on: 13/05/2023
 *      Author: richardgan
 */

#ifndef INC_PTU_H_
#define INC_PTU_H_

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "serial.h"


#include "stm32f3xx_hal.h"

#define NUM_POSITIONS 3
#define NUM_AXES 2
#define TOLERANCE 100  // The PTU position must lie within position +/- tolerance in both axes


extern bool solved;  // Flag indicating if the puzzle has been solved
extern uint16_t delay;
extern const uint16_t positions[NUM_POSITIONS][NUM_AXES];
extern uint8_t curr_pos;  // Current position in positions

// Declare PTU position global variables
extern uint16_t vertical_PWM;
extern uint16_t horizontal_PWM;

// Moves the PTU with vertical velocity determined by `ptu_pitch_vel` and horizontal velocity determined by `ptu_yaw_vel`
void move_ptu(TIM_TypeDef *tim, int16_t ptu_vel_yaw, int16_t ptu_vel_pitch);

// Returns whether the actual position of the PTU is within the range of the desired position given
// the tolerance defined in the function
bool ptu_pos_is_correct();

// This function is called from TIM3_IRQHandler when the PTU velocity is zero for 2 seconds
// If the position is correct, the user proceeds to the next position, otherwise the lock resets
void check_ptu_pos();


#endif /* INC_PTU_H_ */
