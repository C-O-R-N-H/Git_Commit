/*
 * ptu.h
 *
 *  Created on: 13/05/2023
 *      Author: richardgan
 */

#ifndef INC_PTU_H_
#define INC_PTU_H_

#include <stdbool.h>

#include "ptu_definitions.h"
#include "stm32f3xx_hal.h"

extern bool solved;
extern uint16_t delay;

// Declare PTU position global variables
extern volatile uint16_t vertical_PWM;
extern volatile uint16_t horizontal_PWM;

// Moves the PTU with vertical velocity determined by `ptu_pitch_vel` and horizontal velocity determined by `ptu_yaw_vel`
void move_ptu(TIM_TypeDef *tim, volatile uint16_t *horizontal_PWM, volatile uint16_t *vertical_PWM,
		int16_t ptu_vel_yaw, int16_t ptu_vel_pitch);

// Returns whether the actual position of the PTU is within the range of the desired position given
// the tolerance defined in the function
bool ptu_pos_is_correct(const uint16_t *desired_position);

// This function is called from TIM3_IRQHandler when the PTU velocity is zero for 2 seconds
// If the position is correct, the user proceeds to the next position, otherwise the lock resets
void check_ptu_pos();

// Reads data from the PTU gyroscope
void read_ptu_gyro_data(I2C_HandleTypeDef *i2c, int16_t *pitch_rate, int16_t *roll_rate, int16_t *yaw_rate);


#endif /* INC_PTU_H_ */
