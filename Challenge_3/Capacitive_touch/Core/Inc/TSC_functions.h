/*
 * TSC_functions.h
 *
 *  Created on: May 23, 2023
 *      Author: Quinn
 */

#ifndef INC_TSC_FUNCTIONS_H_
#define INC_TSC_FUNCTIONS_H_

#define NUM_NOTES 9				// Number of notes to be played
#define NUM_GROUPS 6			// Number of TSC groups active
#define SENSE_VALUE_BUFFER 250	// Set number of readings used to compute average
#define ROLL_AVERAGE_BUFFER 100 // Set number of averages used to detect touch
#define BUFFER_SIZE 500			// Size of string to be transmitted
#define CHANNEL_BUFFER 60		// Set number of channel outputs used to detect change
#define BUFFER 100				// Size of string to be sent over serial

void update_roll_average(uint16_t (*group_roll_av_buffer)[NUM_GROUPS][ROLL_AVERAGE_BUFFER]);


#endif /* INC_TSC_FUNCTIONS_H_ */
