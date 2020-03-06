/*
 * dht11.c
 *
 *  Created on: 5 mar 2020
 *      Author: Lefucjusz
 *
 *      MSP430 library for interfacing DHT11 sensor
 *
 *      Idea:
 *      -Transfer is realized using bit-banging with proper delays obtained using __delay_cycles() routine
 *      -On every one bit received timings self-synchronize
 *      -40-bit data read from DHT are stored in one 64-bit variable and properly bit-shifted to retrieve parameter needed
 *      -Decimal values are discarded, because they are always zero in DHT11
 *      -Checksum is calculated with equation from datasheet
 *      -DHT11 is supposed to be hooked on P3.0 with external pull-up
 */
#include "dht11.h"
#include <msp430.h>
#include "globals.h"


#define DHT11_OUT_PORT P3OUT
#define DHT11_IN_PORT P3IN
#define DHT11_PIN_DIR P3DIR
#define DHT11_PIN (1<<0)

#define DHT11_FRAME_LENGTH 40 //bits

#define DHT11_SYNCHRO_LOOP_TIMEOUT 100

static uint8_t dht11_reset(void)
{
	uint8_t resp;
	DHT11_PIN_DIR |= DHT11_PIN; //Set pin to output
	DHT11_OUT_PORT &= ~DHT11_PIN; //Pull down
	__delay_cycles(20*DELAY_1MS); //Hold pin low for at least 18ms (here - 20ms)
	DHT11_OUT_PORT |= DHT11_PIN; //Release
	DHT11_PIN_DIR &= ~DHT11_PIN; //Set pin to input
	__delay_cycles(40*DELAY_1US); //Wait 40us for DHT11 response
	resp = (DHT11_IN_PORT & DHT11_PIN); //Check line state, low - ok, high - error, takes approx. 5us
	__delay_cycles(150*DELAY_1US); //Wait 150us for 160us init frame to end
	return resp; //takes approx. 5us
}

static void dht11_get_data_frame(uint64_t* frame)
{
	uint8_t i, timeout;

	/* Reset DHT11 */
	dht11_reset();

	/* Retrieve data */
	for(i = 0; i < DHT11_FRAME_LENGTH; i++) //Read all the bits
	{
		*frame <<= 1; //Move bits left one place
		__delay_cycles(75*DELAY_1US + 1); //Wait 75us (remaining 5us is lost in other instructions)
		if(DHT11_IN_PORT & DHT11_PIN) //Read line state, if high...
		{
			*frame |= 1; //...one is being sent, store the value
			timeout = DHT11_SYNCHRO_LOOP_TIMEOUT; //Set the synchronisation loop timeout, so that program won't get stuck in case of error
			while((DHT11_IN_PORT & DHT11_PIN) && timeout) //This is very clever idea, enabling timings self-correcting on every one bit transmitted
			{ //Loop waits for start of transmission signal, so that the next bit will be received perfectly on time
				timeout--;
			}
		}
	}
}

static uint8_t dht11_checksum(uint64_t frame) //Procedure is described in DHT11 datasheet
{
	uint8_t a, b, c, d, e;
	a = (frame >> 32) & 0xFF;
	b = (frame >> 24) & 0xFF;
	c = (frame >> 16) & 0xFF;
	d = (frame >> 8) & 0xFF;
	e = frame & 0xFF;

	return (((a+b+c+d) & 0xFF) == e);
}

dht11_t dht11_get_measurements(void)
{
	uint64_t frame;
	dht11_t measurements;
	dht11_get_data_frame(&frame);

	if(!dht11_checksum(frame)) //If checksum is incorrect - report error
	{
		measurements.humidity = DHT11_ERROR;
		measurements.temperature = DHT11_ERROR;
	}
	else
	{
		measurements.humidity = (frame >> 32) & 0xFF;
		measurements.temperature = (frame >> 16) & 0xFF;
	}

	return measurements;
}
