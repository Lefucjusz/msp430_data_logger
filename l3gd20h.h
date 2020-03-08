/*
 * l3gd20h.h
 *
 *  Created on: 8 mar 2020
 *      Author: Samsung
 */

#ifndef L3GD20H_H_
#define L3GD20H_H_

#include <stdint.h>

#define L3GD20H_ADDRESS_HIGH 0b1101011
#define L3GD20H_ADDRESS_LOW 0b1101010

#define L3GD20H_WHO_AM_I_VALUE 0b11010111

#define L3GD20H_WHO_AM_I 0x0F
#define L3GD20H_CTRL1 0x20
#define L3GD20H_CTRL2 0x21
#define L3GD20H_CTRL3 0x22
#define L3GD20H_CTRL4 0x23
#define L3GD20H_CTRL5 0x24
#define L3GD20H_REFERENCE 0x25
#define L3GD20H_OUT_TEMP 0x26
#define L3GD20H_STATUS 0x27
#define L3GD20H_OUT_X_L 0x28
#define L3GD20H_OUT_X_H 0x29
#define L3GD20H_OUT_Y_L 0x2A
#define L3GD20H_OUT_Y_H 0x2B
#define L3GD20H_OUT_Z_L 0x2C
#define L3GD20H_OUT_Z_H 0x2D
#define L3GD20H_FIFO_CTRL 0x2E
#define L3GD20H_FIFO_SRC 0x2F
#define L3GD20H_IG_CFG 0x30
#define L3GD20H_IG_SRC 0x31
#define L3GD20H_IG_THS_XH 0x32
#define L3GD20H_IG_THS_XL 0x33
#define L3GD20H_IG_THS_YH 0x34
#define L3GD20H_IG_THS_YL 0x35
#define L3GD20H_IG_THS_ZH 0x36
#define L3GD20H_IG_THS_ZL 0x37
#define L3GD20H_IG_DURATION 0x38
#define L3GD20H_LOW_ODR 0x39

#define L3GD20H_AUTOINCREMENT (1<<7)

typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
} l3gd20h_t;

void l3gd20h_init(void);
l3gd20h_t l3gd20h_get_measurements(void);

#endif /* L3GD20H_H_ */
