/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ptu_definitions.h"
#include "ptu_i2c.h"
#include "serial.h"
#include "serialise.h"
#include "board.h"
#include "servo.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "led.h"
#include "data_handler.h"
#include "challenge_1.h"
#include "challenge_2.h"
#include "challenge_3.h"
#include "challenge_4.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define BUFFER 100

typedef union {
	uint8_t all_leds;
	struct {
		uint8_t led_pair_1 : 2;
		uint8_t led_pair_2 : 2;
		uint8_t led_set_of_4 : 4;
	} led_groups;
} LedRegister;



uint16_t last_capture = 0;
uint16_t diff = 0;

uint16_t rise_time = 0;
uint16_t last_period = 0;

//encode the current state of the game
int current_game_state = 0;
char input_buffer[BUFFER] = {0}; //input data
int input_counter = 0;



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint8_t buffer[32];
	if (htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		uint16_t IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value

		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 1)
			rise_time = IC_Val1;
		else
			last_period = IC_Val1 - rise_time;

		diff = IC_Val1 - last_capture;
		last_capture = IC_Val1;
	}
}


typedef struct {
	uint8_t var1;
	uint32_t var2;
	uint8_t var3;
	uint32_t var4;
	uint8_t var5;
	uint8_t var6;
} struct_1;


typedef struct {
	uint32_t var2;
	uint32_t var4;
	uint8_t var1;
	uint8_t var3;
	uint8_t var5;
	uint8_t var6;
} struct_2;




/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	// look at the size difference between these structs,
	//  they are because the 32 bit microcontroller
	//  packs the data so the divisions are in groups
	//  of 4bytes (4bytes=32bit)
	uint8_t size_1 = sizeof(struct_1);
	uint8_t size_2 = sizeof(struct_2);


	//uint8_t string_to_send[64] = "This is a string !\r\n";

	enable_clocks();
	initialise_board();

	LedRegister *led_register = ((uint8_t*)&(GPIOE->ODR)) + 1;

	SerialInitialise(BAUD_115200, &USART1_PORT, 0x00);

	HAL_StatusTypeDef return_value = 0x00;

	volatile uint16_t vertical_PWM = 1000;
	volatile uint16_t horizontal_PWM = 1000;

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_SPI1_Init();
	MX_USB_PCD_Init();
	MX_TIM2_Init();
	MX_TIM1_Init();
	/* USER CODE BEGIN 2 */

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);

	// TIM 2 is setup with a prescaler that makes 1 count = 1 microsecond
	// Even with HAL, you can still set the values yourself
	TIM2->ARR = 20000; // 20000 = 20ms, which is the desired clock period for servos
	TIM2->CR1 |= TIM_CR1_ARPE; // this makes the timing not change until the next pulse is finished

	// note: for PWM if you continually change the clock period
	// you can get unexpected results. To remove this, set ARPE so that the
	// ARR settings are not activated until the next cycle.

	initialise_ptu_i2c(&hi2c1);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */


	// reset lidar board
	uint8_t reset_value = 0x00;
	return_value = HAL_I2C_Mem_Write(&hi2c1, LIDAR_WR, 0x00, 1, &reset_value, 1, 10);



	// delay for initialisation of the lidar
	HAL_Delay(100);
	enable_interrupts_RX();
	enable_interrupts_button();
	while (1)
	{

		while(1){
			check_state(current_game_state);
		}
















		vertical_PWM = 2500;
		horizontal_PWM = 1500;


		set_yaw(horizontal_PWM);
		set_pitch(vertical_PWM);


		//read gyro

		uint8_t xMSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c1,gyro_rd, 0x29, 1, &xMSB, 1, 10);
		uint8_t xLSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c1,gyro_rd, 0x28, 1, &xLSB, 1, 10);
		int16_t yaw_rate = ((xMSB << 8) | xLSB);

		uint8_t yMSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c1,gyro_rd, 0x2B, 1, &yMSB, 1, 10);
		uint8_t yLSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c1,gyro_rd, 0x2A, 1, &yLSB, 1, 10);
		int16_t pitch_rate = ((yMSB << 8) | yLSB);

		uint8_t zMSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c1,gyro_rd, 0x2D, 1, &zMSB, 1, 10);
		uint8_t zLSB = 0x00;
		HAL_I2C_Mem_Read(&hi2c1,gyro_rd, 0x2C, 1, &zLSB, 1, 10);
		int16_t roll_rate = ((zMSB << 8) | zLSB);



		//read LIDAR thru I2C
		/*
		uint8_t lidar_value = 0x03;
		return_value = HAL_I2C_Mem_Write(&hi2c1, LIDAR_WR, 0x00, 1, &lidar_value, 1, 100);

		lidar_value = 0xff;

		uint8_t lidar_MSBa = 0x00;
		uint8_t lidar_LSBa = 0x00;

		volatile uint16_t lidar_distance = 0xff;

		uint16_t timeout;

		while ((lidar_value & 0x01) != 0x00) {
			return_value = HAL_I2C_Mem_Read(&hi2c1, LIDAR_RD, 0x01, 1, &lidar_value, 1, 100);

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

		// set max value for lidar
		if (last_period > 4000)
			last_period = 5000;
		volatile uint16_t lidar_distance = 0;



		// Construct a sensor data packet and send over serial
		Data sensor_data;
		uint8_t sensor_data_packet_buffer[6 + sizeof(SensorData)] = {0}; // Header + SensorData

		// Fill sensor_data.sensor_data with your actual sensor readings
		sensor_data.sensor_data.acc_x = 12345;
		sensor_data.sensor_data.acc_y = 23456;
		sensor_data.sensor_data.acc_z = 34567;
		sensor_data.sensor_data.gyro_x = roll_rate;
		sensor_data.sensor_data.gyro_y = pitch_rate;
		sensor_data.sensor_data.gyro_z = yaw_rate;
		sensor_data.sensor_data.lidar_pwm = last_period;
		sensor_data.sensor_data.lidar_i2c = lidar_distance*10;

//char *test = "AAAA01010\n";
//		SerialOutputString(test, &USART1_PORT);
















		uint16_t sensor_data_buffer_length = pack_buffer(sensor_data_packet_buffer, SENSOR_DATA, &sensor_data);
		//SerialOutputBuffer(sensor_data_packet_buffer, sensor_data_buffer_length, &USART1_PORT); // Send the buffer over serial


		// Read a data packet from serial that has the LED state
		uint8_t data_packet_input_buffer[32] = {0};
		uint16_t data_packet_size = SerialInputPacketHeader(data_packet_input_buffer, &USART1_PORT);

		// copy the data to a header structure
		Header incoming_header = {0};
		memcpy(&incoming_header, data_packet_input_buffer, sizeof(Header));

		if (incoming_header.message_type == LED_STATE) {
			LEDState desired_led_state = {0};
			uint8_t success = SerialInputDataPacket(&desired_led_state, sizeof(desired_led_state), &USART1_PORT);

			if (success > 0) {
				//uint8_t tmp = desired_led_state.led_byte;
				led_register->all_leds = desired_led_state.led_byte;
			}
		}

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_I2C1
			|RCC_PERIPHCLK_TIM1;
	PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
	PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
	PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x2000090E;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void)
{

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 47;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 47;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 20000;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 2000;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

}

/**
 * @brief USB Initialization Function
 * @param None
 * @retval None
 */
static void MX_USB_PCD_Init(void)
{

	/* USER CODE BEGIN USB_Init 0 */

	/* USER CODE END USB_Init 0 */

	/* USER CODE BEGIN USB_Init 1 */

	/* USER CODE END USB_Init 1 */
	hpcd_USB_FS.Instance = USB;
	hpcd_USB_FS.Init.dev_endpoints = 8;
	hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
	hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
	hpcd_USB_FS.Init.low_power_enable = DISABLE;
	hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
	if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USB_Init 2 */

	/* USER CODE END USB_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
			|LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
			|LD6_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT1_Pin
                           MEMS_INT2_Pin */
	GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT1_Pin
			|MEMS_INT2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
	GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
			|LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
			|LD6_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void USART1_IRQHandler(){

	char single_inp;
	int test = 0;

	test = SerialReceiveChar(&USART1_PORT, &single_inp);
	if (test == 1){
		input_buffer[input_counter] = single_inp;
		input_counter ++;
		//set_led(0b0101);
		//SerialOutputChar(single_inp, &USART1_PORT);
	}
	if (single_inp == 33){ //check for end of string character
		if (strncmp(input_buffer, "AA55", 4) == 0){ //check for sentinal char
			if (input_buffer[4] == 49){ //if msgtype is 1
				char *data = malloc(4);
				strcpy(data, input_buffer + 5);
				data[2] = 0;

				//SerialOutputString(data, &USART1_PORT);
				empty_buffer(input_buffer, BUFFER);
				input_counter = 0;
				test = 0;
				single_inp = 0;

				state_handler(data, &current_game_state);

				return;
			}
		}
		else{
			//set_led(0b100010); //error
		}
		//SerialOutputString(input_buffer, &USART1_PORT);


		empty_buffer(input_buffer, BUFFER);
		input_counter = 0;
		test = 0;
		single_inp = 0;


	}

}

void EXTI0_IRQHandler(void){
	//set_led(0b11111111);

	uint8_t *str_to_send = "AA5521!";
	SerialOutputString(str_to_send, &USART1_PORT);


	// reset the interrupt (so it doesn't keep firing until the next trigger)
	EXTI->PR |= EXTI_PR_PR0;
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
