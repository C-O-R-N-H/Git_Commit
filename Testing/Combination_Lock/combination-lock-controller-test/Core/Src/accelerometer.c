/*
 * accelerometer.c
 *
 *  Created on: May 7, 2023
 *      Author: richardgan
 */

#include <math.h>
#include <stdlib.h>

#include "accelerometer.h"
#include "LSM303DLHC_definitions.h"


void stm_accel_init(I2C_HandleTypeDef *i2c)
{
	// Set the sampling rate
	uint8_t reg_1 = CTRL_REG1_A_LOW_POWER_MODE_100_HZ | CTRL_REG1_A_Xen | CTRL_REG1_A_Yen | CTRL_REG1_A_Zen;
	HAL_I2C_Mem_Write(i2c, accel_wr, LSM303DLHC_CTRL_REG1_A, 1, &reg_1, 1, 10);

//	// Enable block data update: output registers only updated once read
//	// Enable high-resolution output
//	uint8_t reg_4 = CTRL_REG4_A_BDU | CTRL_REG4_A_HR;
//	HAL_I2C_Mem_Write(i2c, accel_wr, LSM303DLHC_CTRL_REG4_A, 1, &reg_4, 1, 10);
}


void find_zero_errors(I2C_HandleTypeDef *i2c, int16_t *zero_errors)
{

	const uint16_t n_samples = 400;
	const int16_t G = 16384;

	int32_t sum_x = 0, sum_y = 0, sum_z = 0;
	int16_t accel_x, accel_y, accel_z;
	for (uint16_t i = 0; i < n_samples; i++)
	{
		read_stm_accel_data(i2c, &accel_x, &accel_y, &accel_z, zero_errors);
		sum_x += accel_x;
		sum_y += accel_y;
		sum_z += accel_z - G;
	}

	zero_errors[0] = (int16_t)(sum_x / (int16_t)n_samples);
	zero_errors[1] = (int16_t)(sum_y / (int16_t)n_samples);
	zero_errors[2] = (int16_t)(sum_z / (int16_t)n_samples);
}


void read_stm_accel_data(I2C_HandleTypeDef *i2c, int16_t *accel_x, int16_t *accel_y, int16_t *accel_z, int16_t *zero_errors)
{
	// Initialise windows for filtering
	static Window window_x, window_y, window_z;
	static bool windows_initialised = false;
	if (!windows_initialised)
	{
		window_init(&window_x);
		window_init(&window_y);
		window_init(&window_z);
		windows_initialised = true;
	}

	// Read acceleration in the x-axis
	uint8_t accel_x_MSB = 0x00;
	HAL_I2C_Mem_Read(i2c, accel_rd, OUT_X_H_A, 1, &accel_x_MSB, 1, 10);
	uint8_t accel_x_LSB = 0x00;
	HAL_I2C_Mem_Read(i2c, accel_rd, OUT_X_L_A, 1, &accel_x_LSB, 1, 10);
	*accel_x = ((accel_x_MSB << 8) | accel_x_LSB) - zero_errors[0];

	// Read acceleration in the y-axis
	uint8_t accel_y_MSB = 0x00;
	HAL_I2C_Mem_Read(i2c, accel_rd, OUT_Y_H_A, 1, &accel_y_MSB, 1, 10);
	uint8_t accel_y_LSB = 0x00;
	HAL_I2C_Mem_Read(i2c, accel_rd, OUT_Y_L_A, 1, &accel_y_LSB, 1, 10);
	*accel_y = ((accel_y_MSB << 8) | accel_y_LSB) - zero_errors[1];

	// Read acceleration in the z-axis
	uint8_t accel_z_MSB = 0x00;
	HAL_I2C_Mem_Read(i2c, accel_rd, OUT_Z_H_A, 1, &accel_z_MSB, 1, 10);
	uint8_t accel_z_LSB = 0x00;
	HAL_I2C_Mem_Read(i2c, accel_rd, OUT_Z_L_A, 1, &accel_z_LSB, 1, 10);
	*accel_z = ((accel_z_MSB << 8) | accel_z_LSB) - zero_errors[2];

	// Compute moving average of data
	*accel_x = moving_average(&window_x, *accel_x);
	*accel_y = moving_average(&window_y, *accel_y);
	*accel_z = moving_average(&window_z, *accel_z);
}


bool get_orientation(int16_t accel_x, int16_t accel_y, int16_t accel_z, int8_t *pitch, int8_t *roll)
{
	const float G = 16384;  // (2^16 / 4) since the full-scale of 16 bits is +/- 2g
	const int16_t valid_threshold = 5000;

	*pitch = (int8_t)(atan((float)accel_y / (float)accel_z) * (180 / M_PI));  // pitch from horizontal in degrees
	*roll = (int8_t)(asin((float)accel_x / G) * (180 / M_PI));  // roll from horizontal in degrees

	return accel_z >= valid_threshold ? true : false;  // If accel_z < threshold then STM is nearly upside down so data is invalid
}


void get_velocity(int8_t pitch, int8_t roll, int8_t *ptu_vel_pitch, int8_t *ptu_vel_yaw)
{
	*ptu_vel_pitch = calculate_velocity(pitch);
	*ptu_vel_yaw = calculate_velocity(roll);
}


int8_t calculate_velocity(int8_t angle)
{
	int8_t sign = angle >= 0 ? 1 : -1;
	int8_t return_vel = 0;

	const int8_t threshold_1 = 6;
	const int8_t threshold_2 = 12;
	const int8_t threshold_3 = 20;
	const int8_t threshold_4 = 29;
	const int8_t threshold_5 = 40;

	const int8_t vel_1 = 1;
	const int8_t vel_2 = 2;
	const int8_t vel_3 = 3;
	const int8_t vel_4 = 6;
	const int8_t vel_5 = 12;

	if ((abs(angle) >= threshold_1) && (abs(angle) < threshold_2))
	{
		return_vel = vel_1;
	}
	else if ((abs(angle) >= threshold_2) && (abs(angle) < threshold_3))
	{
		return_vel = vel_2;
	}
	else if ((abs(angle) >= threshold_3) && (abs(angle) < threshold_4))
	{
		return_vel = vel_3;
	}
	else if ((abs(angle) >= threshold_4) && (abs(angle) < threshold_5))
	{
		return_vel = vel_4;
	}
	else if (abs(angle) >= threshold_5)
	{
		return_vel = vel_5;
	}

	return return_vel * sign;
}









