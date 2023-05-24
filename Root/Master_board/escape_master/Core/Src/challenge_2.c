#include "challenge_2.h"
#include "challenge_1.h"
#include "challenge_3.h"
#include "challenge_4.h"
#include "login.h"
#include "waiting.h"
#include "led.h"
#include "serial.h"
#include "data_handler.h"
#include "filter.h"
#include "stm32f303xc.h"
#include "ptu_definitions.h"
#include "ptu_i2c.h"
#include "main.h"
#include <stdbool.h>


extern uint16_t rise_time;
extern uint16_t last_period;
uint16_t dist_I2C = 0;
extern uint16_t last_capture;
bool unlocked = false;

extern int target_distance;


void enter_challenge_2(){
	set_led(0b11);

	uint8_t string_to_send[64] = "This is a string !\r\n";
	extern uint16_t button_start_time;
	bool flashed = false;
	unlocked = false;

	HAL_StatusTypeDef return_value = 0x00;

	extern I2C_HandleTypeDef hi2c1;
	// Set vertical and horizontal orientation of LIDAR
	extern uint16_t vertical_PWM;
	extern uint16_t horizontal_PWM;



	extern int current_game_state;
	while(1){
		if (current_game_state == 1){
			SerialOutputString("!", &USART1_PORT);
			enter_challenge_1();
		}
		else if (current_game_state == 3){
			SerialOutputString("!", &USART1_PORT);
			enter_challenge_3();
		}
		else if (current_game_state == 4){
			SerialOutputString("!", &USART1_PORT);
			enter_challenge_4();
		}
		else if (current_game_state == 5){
			SerialOutputString("!", &USART1_PORT);
			enter_login();
		}
		else if (current_game_state == 0){
			SerialOutputString("!", &USART1_PORT);
			enter_waiting();
		}

		vertical_PWM = 1500;
		horizontal_PWM = 1500;
		TIM2->CCR1 = vertical_PWM;
		TIM2->CCR2 = horizontal_PWM;

		/*uint8_t lidar_value = 0x03;
		return_value = HAL_I2C_Mem_Write(&hi2c1, LIDAR_WR, 0x00, 1, &lidar_value, 1, 100);

		lidar_value = 0xff;

		uint8_t lidar_MSBa = 0x00;
		uint8_t lidar_LSBa = 0x00;

		volatile uint16_t lidar_distance = 0xff;

		uint16_t timeout;


		while ((lidar_value & 0x01) != 0x00) {
			return_value = HAL_I2C_Mem_Read(&hi2c1, LIDAR_RD, 0x01, 1, &lidar_value, 1, 100);
			return_value = HAL_I2C_Mem_Write(&hi2c1, LIDAR_WR, 0x00, 1, &lidar_value, 1, 100);

			return_value = HAL_I2C_Mem_Read(&hi2c1, LIDAR_RD, 0x0f, 1, &lidar_MSBa, 1, 100);
			return_value = HAL_I2C_Mem_Read(&hi2c1, LIDAR_RD, 0x10, 1, &lidar_LSBa, 1, 100);

			lidar_distance = ((lidar_MSBa << 8) | lidar_LSBa);
			timeout += 1;
			if (timeout > 0xff)
				break;
		}

		uint8_t lidar_ranges = lidar_distance / (100/4); // 100cm broken into 4 groups
			if (lidar_ranges > 3)
				lidar_ranges = 3;
			if (lidar_distance > 4000)
				lidar_distance = 5500;
		*/

//		while(1){
//			sprintf(string_to_send, "%d!", target_distance);
//			SerialOutputString(string_to_send, &USART1_PORT);
//		}

		read_LIDAR(&last_period, &dist_I2C);
		//main if
		if (unlocked)
		{

			if (!flashed)
			{
				set_led(0b10000000);
				completion_led();
				flashed = true;
			}

		}


		if (flashed == true){
			sprintf(string_to_send, "AA5521!");
			SerialOutputString(string_to_send, &USART1_PORT);
			//enter_waiting();
		}
		//ouput data for GUI to display
		sprintf(string_to_send, "AA555%04hu!", last_period);
		SerialOutputString(string_to_send, &USART1_PORT);



		// Serial string turned off as the data is being sent now through the serialiser
		//sprintf(string_to_send, "%hu,%hu,%hd,%hd,%hd\r\n", last_period, lidar_distance*10, roll_rate, pitch_rate, yaw_rate);
		//SerialOutputString(string_to_send, &USART1_PORT);

		//sprintf(string_to_send, "%hu!\r\n", last_period);
		//SerialOutputString(string_to_send, &USART1_PORT);






	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	// Instantiate sliding window for rolling average
	static Window last_period_window;
	static bool window_initialised = false;

	if (!window_initialised)
	{
		window_init(&last_period_window);
		window_initialised = true;
	}

	if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		uint16_t IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value

		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 1)
			rise_time = IC_Val1;
		else
		{
			uint16_t new_last_period = IC_Val1 - rise_time;
			last_period = moving_average(&last_period_window, new_last_period);
		}


		last_capture = IC_Val1;
	}
}



