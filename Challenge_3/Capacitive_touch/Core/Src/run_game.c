#include "run_game.h"
#include "serial.h"
#include "board.h"
#include "main.h"
#include "led.h"
#include "data_handler.h"

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
#define BUFFER 100

extern TSC_HandleTypeDef htsc;
extern int current_game_state;
extern char input_buffer[BUFFER];
extern int input_counter;
extern int completed;

uint16_t correct_order[NUM_NOTES] = {1, 2, 1, 2, 1, 3, 4, 5, 6};	// Correct order of notes
uint8_t *completed_str = "AA5521!";		// String to be sent on completion
uint8_t string_to_send[BUFFER_SIZE];	// String to be transmitted

uint16_t channel_out_arr[NUM_GROUPS];	// Output channel status for each group
uint16_t transmit_cnt_arr[NUM_GROUPS];	// Transmit count for each group
uint16_t note_array[NUM_NOTES];			// Array of played notes
uint16_t group_sense_values[NUM_GROUPS];	// Sensor values of each group
uint8_t channel_out_str[BUFFER_SIZE];	// Output channel number

uint32_t group_sum[NUM_GROUPS];			// Rolling sum of each group

uint16_t group_sense_val_buffer[NUM_GROUPS][SENSE_VALUE_BUFFER];	// Buffer of sensor values for each group
uint16_t group_roll_av_buffer[NUM_GROUPS][ROLL_AVERAGE_BUFFER];		// Buffer of rolling averages for each group
uint16_t channel_out_buff_arr[NUM_GROUPS][CHANNEL_BUFFER];			// Buffer of channel outputs for each group

uint16_t sense_index = 0;			// Rolling sum index
uint16_t roll_index = 0;			// Rolling average index
uint16_t channel_index = 0;			// Output channel buffer index
uint16_t check_zero = 0;			// Counts if output status is zero
uint16_t note_index = 0;			// Array index of current note array
uint16_t is_zero = 0;				// Counts how many times a channel output is zero

void enter_challenge_3()
{
	// Start code AA55103!
	set_led(0b111);

	memset(channel_out_arr, 0 ,sizeof(channel_out_arr));
	memset(transmit_cnt_arr, 0 ,sizeof(transmit_cnt_arr));
	memset(note_array, 0 ,sizeof(note_array));
	memset(group_sense_values, 0 ,sizeof(group_sense_values));
	memset(group_sum, 0 ,sizeof(group_sum));
	memset(channel_out_str, 0 ,sizeof(channel_out_str));
	memset(group_sense_val_buffer, 0 ,sizeof(group_sense_val_buffer));
	memset(group_roll_av_buffer, 0 ,sizeof(group_roll_av_buffer));
	memset(channel_out_buff_arr, 0 ,sizeof(channel_out_buff_arr));

	while (1)
	{
		// If not in current challenge, send EOS character and wait
		if(current_game_state != 3)
		{
			SerialOutputString("!",&USART1_PORT);
			enter_waiting();
		}

		// Reset buffers and acquisition status
		check_zero = 0;
		memset(channel_out_arr, 0, sizeof(channel_out_arr));
		memset(string_to_send, 0, sizeof(string_to_send));

		// All transmissions begin with sentinel bits
		strcat(string_to_send, "AA553");

		// Reset sampling capacitors
		HAL_TSC_IODischarge(&htsc, ENABLE);

		// Begin TSC acquisition and wait for completion
		HAL_TSC_Start_IT(&htsc);

		// Waits until acquisition of each group is complete
		HAL_TSC_PollForAcquisition(&htsc);

		//get_output_channel();

		if(completed == 1)
		{
			return;
		}

		//set_led(0b11111111);
	}
}

void handle_error()
{
	// If max count error occurs, flash all LEDs for 2 seconds, then return to main game loop
	set_led(0b11111111);
	// Upon returning to main game loop, everything gets reset
	enter_challenge_3();
}

void enter_waiting()
{
	set_led(0b11001100);
	while(1)
	{
		check_state(current_game_state);
	}
}

void get_output_channel()
{
	// Update rolling average for each group
	for(int i = 0; i < NUM_GROUPS; i++)
	{
	  group_sense_values[i] = (uint16_t)(HAL_TSC_GroupGetValue(&htsc, i));
	  group_sum[i] = group_sum[i] - group_sense_val_buffer[i][sense_index] + group_sense_values[i];
	  group_sense_val_buffer[i][sense_index] = group_sense_values[i];
	  group_roll_av_buffer[i][roll_index] = (uint16_t)(round(group_sum[i]/SENSE_VALUE_BUFFER));
	}

	// Checks if average has dropped by 3% and updates channel output buffer
	for(int i = 0; i < NUM_GROUPS; i++)
	{
	  if (group_roll_av_buffer[i][roll_index] < 0.97*(group_roll_av_buffer[i][0]))
	  {
		  channel_out_arr[i] = i+1;
	  }

	  channel_out_buff_arr[i][channel_index] = channel_out_arr[i];

	}

	// Checks if group output is stable
	for(int i = 0; i < NUM_GROUPS; i++)
	{
		is_zero = 0;
		for(int k = 0; k < CHANNEL_BUFFER; k++)
		{
			if(channel_out_buff_arr[i][k] == 0)
			{
				is_zero++;
			}
		}

		if(is_zero == CHANNEL_BUFFER)
		{
			transmit_cnt_arr[i] = 0;
		}

	}

	// Checks if output group is ready to transmit
	for(int i = 0; i < NUM_GROUPS; i++)
	{
	  if(channel_out_buff_arr[i][channel_index] != 0 && transmit_cnt_arr[i] == 0)
	  {
		  sprintf(channel_out_str, "%hu", channel_out_buff_arr[i][channel_index]);

		  // Add current note to array and increment transmit counter
		  note_array[note_index] = channel_out_buff_arr[i][channel_index];
		  transmit_cnt_arr[i]++;

		  // If the note is incorrect, reset note array and index
		  if(note_array[note_index] != correct_order[note_index])
		  {
			  set_led(0b00100010);
			  memset(note_array, 0, sizeof(note_array));
			  note_index = 0;
		  }
		  // If note is correct, continue through array
		  else
		  {
			  set_led(0b10001000);
			  note_index = (note_index + 1)%NUM_NOTES;
		  }
	  }

	  // If not ready to transmit, send 0
	  else
	  {
		  sprintf(channel_out_str, "%hu", 0);
		  check_zero++;
	  }

	  strcat(string_to_send, channel_out_str);
	}

	// Update rolling buffer indexes
	channel_index = (channel_index + 1)%CHANNEL_BUFFER;
	sense_index = (sense_index + 1) % SENSE_VALUE_BUFFER;
	roll_index = (roll_index + 1) % ROLL_AVERAGE_BUFFER;

	// Add EOS character
	strcat(string_to_send,"!");

	// If there is a value to send, transmit
	if(check_zero != NUM_GROUPS)
	{
		SerialOutputString(string_to_send, &USART1_PORT);
	}

	// Send completion string if note array is correct
	if(note_array[NUM_NOTES - 1] == correct_order[NUM_NOTES - 1])
	{
		SerialOutputString(completed_str, &USART1_PORT);
		completed = 1;
	}
}
