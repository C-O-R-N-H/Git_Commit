/*
 * challenge_4.c
 *
 *  Created on: May 18, 2023
 *      Author: richardgan
 */

#include "challenge_4.h"
#include "led.h"
#include "serial.h"
#include "serialise.h"
#include "accelerometer.h"

void enter_challenge_4()
{
	set_led(0b1111);
	extern int current_game_state;
	extern I2C_HandleTypeDef hi2c1;
	uint8_t buffer[BUFFER];

	// Initialise peripherals
	stm_accel_init(&hi2c1);

	int16_t zero_errors[] = {0, 0, 0};  // Zero errors in x, y, and z axes
	find_zero_errors(&hi2c1, zero_errors);

	int16_t accel_x, accel_y, accel_z;
	int8_t stm_pitch, stm_roll;
	int8_t ptu_vel_pitch, ptu_vel_yaw;


	while (1)
	{
		if (current_game_state != 4)
		{
			SerialOutputString("!", &USART1_PORT);
			enter_waiting();
		}

		// Actual challenge
		read_stm_accel_data(&hi2c1, &accel_x, &accel_y, &accel_z, zero_errors);

		if (get_orientation(accel_x, accel_y, accel_z, &stm_pitch, &stm_roll))
		{
			show_orientation_led(stm_pitch, stm_roll, 6);
			get_velocity(stm_pitch, stm_roll, &ptu_vel_pitch, &ptu_vel_yaw);

			// Make velocities positive for sending over terminal
			ptu_vel_yaw += 12;
			ptu_vel_pitch += 12;
		}

		// Pack and send the velocity data to the serial port
		sprintf(buffer, "AA554%2hu%2hu!", ptu_vel_yaw, ptu_vel_pitch);
		SerialOutputString(buffer, &USART1_PORT);

//		Data vel_data;
//		uint8_t velocity_packet_buffer[HEADER_SIZE + sizeof(Velocity)] = {0};
//		vel_data.velocity.horizontal_vel = ptu_vel_yaw;
//		vel_data.velocity.vertical_vel = ptu_vel_pitch;
//		uint16_t velocity_buffer_length = pack_buffer(velocity_packet_buffer, VELOCITY, &vel_data);
//		SerialOutputBuffer(velocity_packet_buffer, velocity_buffer_length, &USART1_PORT);

		HAL_Delay(10);
	}
}


void enter_waiting()
{
	set_led(0b11001100);
	extern int current_game_state;

	while (1)
	{
		if (current_game_state == 4)
		{
			enter_challenge_4();
		}
	}

}


void state_handler(char *state, int *game_state)
{
	int state_int = atoi(state);
	*game_state = state_int;
}




