/*
 * accelerometer.h
 *
 *  Created on: May 7, 2023
 *      Author: richardgan
 */

#ifndef INC_ACCELEROMETER_H_
#define INC_ACCELEROMETER_H_

#include <stdint.h>
#include <stdbool.h>

#include "stm32f3xx_hal.h"
#include "filter.h"

// Initialises the LSM303DLHC IMU on the STM discovery board for acceleration data
void stm_accel_init(I2C_HandleTypeDef *i2c);

// Find the zero errors in the x, y, and z axes so that sensor readings can be zeroed
void find_zero_errors(I2C_HandleTypeDef *i2c, int16_t *zero_errors);

// Read filtered acceleration data in the x, y, and z axes on the STM discovery board
void read_stm_accel_data(I2C_HandleTypeDef *i2c, int16_t *accel_x, int16_t *accel_y, int16_t *accel_z, int16_t *zero_errors);

// Calculates the pitch and roll of the STM discovery board using acceleration data
// Returns whether the orientation is valid: STM is right side up => valid; STM is upside down => invalid
bool get_orientation(int16_t accel_x, int16_t accel_y, int16_t accel_z, int8_t *pitch, int8_t *roll);

// Converts the orientation of the STM board to velocities of the PTU
void get_velocity(int8_t pitch, int8_t roll, int8_t *ptu_vel_pitch, int8_t *ptu_vel_yaw);

// Calculates the velocity according to an angle, using set threshold values
int8_t calculate_velocity(int8_t angle);


#endif /* INC_ACCELEROMETER_H_ */
