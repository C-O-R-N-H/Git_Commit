/*
 * LSM303DLHC_definitions.h
 *
 *  Created on: May 7, 2023
 *      Author: richardgan
 */

#ifndef INC_LSM303DLHC_DEFINITIONS_H_
#define INC_LSM303DLHC_DEFINITIONS_H_


#define accel_wr 0x32
#define accel_rd 0x33

#define LSM303DLHC_CTRL_REG1_A 0x20
#define LSM303DLHC_CTRL_REG2_A 0x21
#define LSM303DLHC_CTRL_REG3_A 0x22
#define LSM303DLHC_CTRL_REG4_A 0x23
#define LSM303DLHC_CTRL_REG5_A 0x24
#define LSM303DLHC_CTRL_REG6_A 0x25

#define STATUS_REG_A 0x27

#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29

#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B

#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D

#define CTRL_REG1_A_DATA_RATE_Pos				4U
#define CTRL_REG1_A_POWER_DOWN_MODE 			0b0000 << CTRL_REG1_A_DATA_RATE_Pos
#define CTRL_REG1_A_LOW_POWER_MODE_1_HZ			0b0001 << CTRL_REG1_A_DATA_RATE_Pos
#define CTRL_REG1_A_LOW_POWER_MODE_10_HZ		0b0010 << CTRL_REG1_A_DATA_RATE_Pos
#define CTRL_REG1_A_LOW_POWER_MODE_25_HZ		0b0011 << CTRL_REG1_A_DATA_RATE_Pos
#define CTRL_REG1_A_LOW_POWER_MODE_50_HZ		0b0100 << CTRL_REG1_A_DATA_RATE_Pos
#define CTRL_REG1_A_LOW_POWER_MODE_100_HZ		0b0101 << CTRL_REG1_A_DATA_RATE_Pos
#define CTRL_REG1_A_LOW_POWER_MODE_200_HZ		0b0110 << CTRL_REG1_A_DATA_RATE_Pos
#define CTRL_REG1_A_LOW_POWER_MODE_400_HZ		0b0111 << CTRL_REG1_A_DATA_RATE_Pos
#define CTRL_REG1_A_LOW_POWER_MODE_1620_HZ		0b1000 << CTRL_REG1_A_DATA_RATE_Pos

#define CTRL_REG1_A_Zen							1U << 2U			// Z-axis enable
#define CTRL_REG1_A_Yen							1U << 1U			// Y-axis enable
#define CTRL_REG1_A_Xen							1U << 0U			// X_axis enable

#define CTRL_REG4_A_BDU							1U << 7U			// Block data update
#define CTRL_REG4_A_HR							1U << 3U			// High-resolution output mode



#endif /* INC_LSM303DLHC_DEFINITIONS_H_ */
