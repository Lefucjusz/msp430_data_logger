/*
 * l3gd20h.c
 *
 *  Created on: 8 mar 2020
 *      Author: Lefucjusz
 */

#include "l3gd20h.h"
#include "i2c.h"

void l3gd20h_init(void)
{
	i2c_init(L3GD20H_ADDRESS_HIGH);
	i2c_start(WRITE);
	i2c_write_byte(L3GD20H_CTRL1 | L3GD20H_AUTOINCREMENT);
	i2c_write_byte(0b00011111); //ODR=100Hz, LPF_cutoff=25Hz, normal mode, enable all axis
	i2c_write_byte(0b00000011); //HPF_cutoff=1Hz
	i2c_start(WRITE);
	i2c_write_byte(L3GD20H_CTRL4 | L3GD20H_AUTOINCREMENT);
	i2c_write_byte(0b00100000); //Enable 2000dps scale
	i2c_write_byte(0b00010010); //HPF enable
	i2c_stop();
}

l3gd20h_t l3gd20h_get_measurements(void)
{
	l3gd20h_t measurements;
	int16_t temp;

	i2c_init(L3GD20H_ADDRESS_HIGH);
	i2c_start(WRITE);
	i2c_write_byte(L3GD20H_OUT_X_L | L3GD20H_AUTOINCREMENT);
	i2c_start(READ);

	measurements.x = i2c_read_byte(ACK); //x axis low byte
	temp = i2c_read_byte(ACK); //x axis high byte
	measurements.x |= (temp << 8); //merging

	measurements.y = i2c_read_byte(ACK); //y axis low byte
	temp = i2c_read_byte(ACK); //y axis high byte
	measurements.y |= (temp << 8); //merging

	measurements.z = i2c_read_byte(ACK); //y axis low byte
	temp = i2c_read_byte(NACK); //y axis high byte
	measurements.z |= (temp << 8); //merging

	i2c_stop();

    /*Page 10 of datasheet states that for
     *2000dps measurement range
     *2000dps  correction factor is 70mdps/LSB
     */
	measurements.x = 7*(int32_t)measurements.x/100;
	measurements.y = 7*(int32_t)measurements.y/100;
	measurements.z = 7*(int32_t)measurements.z/100;

	return measurements;
}

