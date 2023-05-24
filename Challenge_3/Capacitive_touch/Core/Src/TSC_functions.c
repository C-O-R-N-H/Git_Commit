/*#include "main.h"
#include "serial.h"
#include "data_handler.h"
#include "TSC_functions.h"
#include "led.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define NUM_NOTES 9				// Number of notes to be played
#define NUM_GROUPS 6			// Number of TSC groups active
#define SENSE_VALUE_BUFFER 250	// Set number of readings used to compute average
#define ROLL_AVERAGE_BUFFER 100 // Set number of averages used to detect touch
#define BUFFER_SIZE 500			// Size of string to be transmitted
#define CHANNEL_BUFFER 60		// Set number of channel outputs used to detect change
#define BUFFER 100				// Size of string to be sent over serial

extern TSC_HandleTypeDef htsc;

uint8_t string_to_send[BUFFER_SIZE];	// String to be transmitted
uint8_t *completed_str = "AA5521!";		// String to be sent on completion

uint16_t channel_out_arr[NUM_GROUPS];	// Output channel status for each group
uint16_t transmit_cnt_arr[NUM_GROUPS];	// Transmit count for each group
uint16_t note_array[NUM_NOTES];			// Array of played notes
uint16_t correct_order[NUM_NOTES] = {1, 2, 1, 2, 1, 3, 4, 5, 6};	// Correct order of notes
uint16_t group_roll_average[NUM_GROUPS];	// Rolling average of each group
uint16_t channel_out_str[BUFFER_SIZE];	// Output channel number
uint16_t group_sense_values[NUM_GROUPS];	// Sensor values of each group

//uint16_t group_roll_av_buffer[NUM_GROUPS][ROLL_AVERAGE_BUFFER];		// Buffer of rolling averages for each group
uint16_t channel_out_buff_arr[NUM_GROUPS][CHANNEL_BUFFER];			// Buffer of channel outputs for each group
uint16_t group_sense_val_buffer[NUM_GROUPS][SENSE_VALUE_BUFFER];	// Buffer of sensor values for each group

uint32_t group_sum[NUM_GROUPS];		// Rolling sum of each group

uint16_t sense_index = 0;			// Rolling sum index
uint16_t roll_index = 0;			// Rolling average index
uint16_t channel_index = 0;			// Output channel buffer index
uint16_t check_zero = 0;			// Counts if output status is zero
uint16_t note_index = 0;			// Array index of current note array
uint16_t is_zero = 0;				// Counts how many times a channel output is zero

//memset(group_sense_values, 0 ,sizeof(group_sense_values));
//memset(group_sum, 0 ,sizeof(group_sum));
//memset(group_sense_val_buffer, 0 ,sizeof(group_sense_val_buffer));

// Initialize all arrays to 0

void update_roll_average(uint16_t (*group_roll_av_buffer)[NUM_GROUPS][ROLL_AVERAGE_BUFFER])
{
	for(int i = 0; i < NUM_GROUPS; i++)
	{
		group_sense_values[i] = (uint16_t)(HAL_TSC_GroupGetValue(&htsc, i));
		group_sum[i] = group_sum[i] - group_sense_val_buffer[i][sense_index] + group_sense_values[i];
		group_sense_val_buffer[i][sense_index] = group_sense_values[i];
		*group_roll_av_buffer[i][roll_index] = (uint16_t)(round(group_sum[i]/SENSE_VALUE_BUFFER));
	}

	// Update rolling buffer indexes
	channel_index = (channel_index + 1)%CHANNEL_BUFFER;
	sense_index = (sense_index + 1) % SENSE_VALUE_BUFFER;
	roll_index = (roll_index + 1) % ROLL_AVERAGE_BUFFER;

	set_led(0b11111111);

}*/
