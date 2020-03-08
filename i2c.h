/*
 * i2c.h
 *
 *  Created on: 8 mar 2020
 *      Author: Lefucjusz
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include "bool.h"

#define ACK TRUE
#define NACK FALSE
#define READ TRUE
#define WRITE FALSE

void i2c_init(uint8_t);
void i2c_start(BOOL);
void i2c_stop(void);
void i2c_write_byte(uint8_t);
uint8_t i2c_read_byte(BOOL);

#endif /* I2C_H_ */
