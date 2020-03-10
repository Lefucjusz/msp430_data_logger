/*
 * bmp280.h
 *
 *  Created on: 9 mar 2020
 *      Author: Lefucjusz
 */

#ifndef BMP280_H_
#define BMP280_H_

#include <stdint.h>

#define BMP280_ID 0x58
#define BMP280_ADDR_LOW 0b1110110  //Chosen by the SDO pin state
#define BMP280_ADDR_HIGH 0b1110111 //
#define BMP280_CAL_DATA_SIZE 26 //26 calibration registers

#define BMP280_CAL0_REG 0x88
#define BMP280_CAL1_REG 0x89
#define BMP280_CAL2_REG 0x8A
#define BMP280_CAL3_REG 0x8B
#define BMP280_CAL4_REG 0x8C
#define BMP280_CAL5_REG 0x8D
#define BMP280_CAL6_REG 0x8E
#define BMP280_CAL7_REG 0x8F
#define BMP280_CAL8_REG 0x90
#define BMP280_CAL9_REG 0x91
#define BMP280_CAL10_REG 0x92
#define BMP280_CAL11_REG 0x93
#define BMP280_CAL12_REG 0x94
#define BMP280_CAL13_REG 0x95
#define BMP280_CAL14_REG 0x96
#define BMP280_CAL15_REG 0x97
#define BMP280_CAL16_REG 0x98
#define BMP280_CAL17_REG 0x99
#define BMP280_CAL18_REG 0x9A
#define BMP280_CAL19_REG 0x9B
#define BMP280_CAL20_REG 0x9C
#define BMP280_CAL21_REG 0x9D
#define BMP280_CAL22_REG 0x9E
#define BMP280_CAL23_REG 0x9F
#define BMP280_CAL24_REG 0xA0
#define BMP280_CAL25_REG 0xA1

#define BMP280_ID_REG 0xD0
#define BMP280_RESET_REG 0xE0
#define BMP280_STATUS_REG 0xF3
#define BMP280_CTRL_MEAS_REG 0xF4
#define BMP280_CONFIG_REG 0xF5
#define BMP280_PRESS_MSB_REG 0xF7
#define BMP280_PRESS_LSB_REG 0xF8
#define BMP280_PRESS_XLSB_REG 0xF9
#define BMP280_TEMP_MSB_REG 0xFA
#define BMP280_TEMP_LSB_REG 0xFB
#define BMP280_TEMP_XLSB_REG 0xFC

typedef struct
{
	uint32_t pressure;
	int32_t temperature;
} bmp280_t;

void bmp280_init(void);
bmp280_t bmp280_get_measurements(void);

#endif /* BMP280_H_ */
