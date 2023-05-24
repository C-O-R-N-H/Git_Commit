/*
 * ptu.c
 *
 *  Created on: 13/05/2023
 *      Author: richardgan
 */

#include "ptu.h"
#include "led.h"

char test_buffer[100];


void move_ptu(TIM_TypeDef *tim, volatile uint16_t *horizontal_PWM, volatile uint16_t *vertical_PWM,
		int16_t ptu_vel_yaw, int16_t ptu_vel_pitch)
{
	const uint16_t PWM_min = 600;
	const uint16_t PWM_max = 2400;

	*horizontal_PWM += ptu_vel_yaw;
	*vertical_PWM += ptu_vel_pitch;

	if (*horizontal_PWM > PWM_max)
	{
		*horizontal_PWM = PWM_max;
	}

	if (*horizontal_PWM < PWM_min)
	{
		*horizontal_PWM = PWM_min;
	}

	if (*vertical_PWM > PWM_max)
	{
		*vertical_PWM = PWM_max;
	}

	if (*vertical_PWM < PWM_min)
	{
		*vertical_PWM = PWM_min;
	}

	tim->CCR1 = *horizontal_PWM;
	tim->CCR2 = *vertical_PWM;
}


bool ptu_pos_is_correct(const uint16_t *desired_position)
{
	const uint16_t tolerance = 100;  // The PTU position must lie within position +/- tolerance in both axes
	bool horizontal_pos_is_correct = (horizontal_PWM > desired_position[0] - tolerance) &&
								     (horizontal_PWM < desired_position[0] + tolerance);
	bool vertical_pos_is_correct = (vertical_PWM > desired_position[1] - tolerance) &&
		     	 	 	 	 	   (vertical_PWM < desired_position[1] + tolerance);

	return horizontal_pos_is_correct && vertical_pos_is_correct;
}


void check_ptu_pos()
{
	// Array containing the positions (horizontal_PWM, vertical_PWM) required to open the lock
	const uint16_t positions[3][2] = {
			{800, 1800},
			{2000, 600},
			{1500, 1500}
	};
	const uint8_t NUM_POSITIONS = 3;

	static uint8_t current_position = 0;

	if (ptu_pos_is_correct(positions[current_position]))
	{
		current_position++;
		if (current_position == NUM_POSITIONS)
		{
			flash_led(GREEN, 3, 800, 400, &TIMER4);
			delay = 3200;

			// Disable compare interrupts
			TIM3->DIER &= TIM_DIER_CC1IE;

			solved = true;
		}

		else
		{
			flash_led(GREEN, 1, 800, 0, &TIMER4);
			delay = 800;
		}

	}

	else  // The position is incorrect and the lock is reset
	{
		current_position = 0;
		flash_led(RED, 2, 800, 400, &TIMER4);
		delay = 2000;
	}
}












void read_ptu_gyro_data(I2C_HandleTypeDef *i2c, int16_t *pitch_rate, int16_t *roll_rate, int16_t *yaw_rate)
{
	uint8_t xMSB = 0x00;
	HAL_I2C_Mem_Read(i2c, gyro_rd, L3G4200D_OUT_X_H, 1, &xMSB, 1, 10);
	uint8_t xLSB = 0x00;
	HAL_I2C_Mem_Read(i2c, gyro_rd, L3G4200D_OUT_X_L, 1, &xLSB, 1, 10);
	*yaw_rate = ((xMSB << 8) | xLSB);

	uint8_t yMSB = 0x00;
	HAL_I2C_Mem_Read(i2c, gyro_rd, L3G4200D_OUT_Y_H, 1, &yMSB, 1, 10);
	uint8_t yLSB = 0x00;
	HAL_I2C_Mem_Read(i2c, gyro_rd, L3G4200D_OUT_Y_L, 1, &yLSB, 1, 10);
	*pitch_rate = ((yMSB << 8) | yLSB);

	uint8_t zMSB = 0x00;
	HAL_I2C_Mem_Read(i2c, gyro_rd, L3G4200D_OUT_Z_H, 1, &zMSB, 1, 10);
	uint8_t zLSB = 0x00;
	HAL_I2C_Mem_Read(i2c, gyro_rd, L3G4200D_OUT_Z_L, 1, &zLSB, 1, 10);
	*roll_rate = ((zMSB << 8) | zLSB);
}



