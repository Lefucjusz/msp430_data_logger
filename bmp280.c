/*
 * bmp280.c
 *
 *  Created on: 9 mar 2020
 *      Author: Lefucjusz
 *
 *      MSP430 library for interfacing BMP280 sensor
 *
 *      Idea:
 *      -Library designed for weather station - low accuracy
 *      -BMP280 configured in both temperature and pressure 16-bit mode
 *      -IIR filter turned off
 *      -Readout in normal mode, 4s standby time
 *      -32-bit compensation procedures - limiting pressure precision to about 1Pa
 *      -BMP280 connected using I2C with SDO pulled down (lower address)
 */
#include "bmp280.h"
#include "i2c.h"
#include "globals.h"

typedef union
{
	uint8_t regs[BMP280_CAL_DATA_SIZE];
	struct
	{
		uint16_t T1;
		int16_t T2;
		int16_t T3;

		uint16_t P1;
		int16_t P2;
		int16_t P3;
		int16_t P4;
		int16_t P5;
		int16_t P6;
		int16_t P7;
		int16_t P8;
		int16_t P9;
	};
} bmp280_cal_t;

static bmp280_cal_t cal;
static bmp280_t meas;

static void bmp280_configure(void) //With such settings conversion time is approx. 6.4ms
{
	i2c_init(BMP280_ADDR_LOW); //Init I2C with slave address
	i2c_start(WRITE); //Start I2C in Tx mode
	i2c_write_byte(BMP280_CTRL_MEAS_REG); //Write value of register to write to
	i2c_write_byte(0b00100111); //Both temp and pres 16-bit accuracy, normal mode
	/* Turn off IIR filter, turn off 3-wire SPI, standby time 4s */
	i2c_write_byte(0b11100000);
	i2c_stop(); //End transmission
}

static void bmp280_read_calibration(void)
{
	uint8_t i;

	i2c_init(BMP280_ADDR_LOW); //Init I2C with slave address
	i2c_start(WRITE); //Start I2C in Tx mode
	i2c_write_byte(BMP280_CAL0_REG); //Write value of register to read from
	i2c_start(READ); //Restart I2C in Rx mode

	for(i = 0; i < BMP280_CAL_DATA_SIZE - 1; i++) //Read all registers but last with ACK
	{
		cal.regs[i] = i2c_read_byte(ACK);
	}
	cal.regs[i] = i2c_read_byte(NACK); //Read last with NACK

	i2c_stop(); //End transmission
}

static void bmp280_read_registers(void)
{
	uint8_t lsb, xlsb;

	i2c_init(BMP280_ADDR_LOW); //Init I2C with slave address
	i2c_start(WRITE); //Start I2C in Tx mode
	i2c_write_byte(BMP280_PRESS_MSB_REG); //Write value of register to read from
	i2c_start(READ); //Restart I2C in Rx mode

	meas.pressure = i2c_read_byte(ACK); //Read MSB
	lsb = i2c_read_byte(ACK); //Read LSB
	xlsb = i2c_read_byte(ACK); //Read XLSB
	meas.pressure <<= 12; //Merge into 20-bit value
	meas.pressure |= (lsb << 4) | (xlsb >> 4);

	meas.temperature = i2c_read_byte(ACK);
	lsb = i2c_read_byte(ACK);
	xlsb = i2c_read_byte(NACK); //NACK on last byte read
	meas.temperature <<= 12;
	meas.temperature |= (lsb << 4) | (xlsb >> 4);

	i2c_stop(); //End transmission
}

static void bmp280_compute_compensation(void) //Don't ask, I just copied it from datasheet...
{
	int32_t var1, var2, t_fine;
	int32_t pres_signed = (int32_t)meas.pressure;

	/* Compute t_fine and temperature */
	var1 = ((((meas.temperature>>3)-((int32_t)cal.T1<<1)))*((int32_t)cal.T2))>>11;
	var2 = (((((meas.temperature>>4)-((int32_t)cal.T1))*((meas.temperature>>4)-((int32_t)cal.T1)))>>12)*((int32_t)cal.T3))>>14;
	t_fine = var1+var2;
	meas.temperature = (t_fine*5+128)>>8;

	/* Compute pressure */
	var1 = (((int32_t)t_fine)>>1)-(int32_t)64000;
	var2 = (((var1>>2)*(var1>>2))>>11)*((int32_t)cal.P6);
	var2 = var2+((var1*((int32_t)cal.P5))<<1);
	var2 = (var2>>2)+(((int32_t)cal.P4)<<16);
	var1 = (((cal.P3*(((var1<<2)*(var1>>2))>>13))>>3)+((((int32_t)cal.P2)*var1)>>1))>>18;
	var1 = ((((32768+var1))*((int32_t)cal.P1))>>15);
	if(var1 == 0)
	{
		meas.pressure = 0;
		return;
	}
	meas.pressure = (((uint32_t)(((int32_t)1048576)-pres_signed)-(var2>>12)))*3125;
	if(meas.pressure < 0x80000000)
	{
		meas.pressure = (meas.pressure<<1)/((uint32_t)var1);
	}
	else
	{
		meas.pressure = (meas.pressure/(uint32_t)var1)>>1;
	}
	var1 = (((int32_t)cal.P9)*((int32_t)(((meas.pressure>>3)*(meas.pressure>>3))>>13)))>>12;
	var2 = (((int32_t)(meas.pressure>>2))*((int32_t)cal.P8))>>13;
	meas.pressure = (uint32_t)((int32_t)meas.pressure+((var1+var2+cal.P7)>>4));
}

void bmp280_init(void)
{
	bmp280_configure();
	bmp280_read_calibration();
}

bmp280_t bmp280_get_measurements(void)
{
	bmp280_read_registers();
	bmp280_compute_compensation();
	return meas;
}
