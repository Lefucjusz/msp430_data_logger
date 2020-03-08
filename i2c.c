/*
 * i2c.c
 *
 *  Created on: 8 mar 2020
 *      Author: Lefucjusz
 *      Hardware I2C implementation, synchronization using simple loop polling
 *      Interrupt-driven library would be better, but who cares? For this purpose it's more than enough...
 */

#include "i2c.h"
#include <msp430xG46x.h>

void i2c_init(uint8_t slave_addr) //Slave address is 7-bit!
{
	P3SEL |= (1<<1) | (1<<2);                 // Assign I2C pins to USCI_B0
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 | UCSWRST;            // Use SMCLK (~8MHz), keep SW reset
	UCB0BR0 = 81;							  // fSCL = SMCLK/81 = 98.7kHz
	UCB0BR1 = 0;							  //
	UCB0I2CSA = slave_addr;                   // Set slave address
	UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
}

void i2c_start(BOOL read_write)
{
	if(read_write == READ) //Turn on receiver or transmitter
	{
		UCB0CTL1 &= ~UCTR;
	}
	else
	{
		UCB0CTL1 |= UCTR;
	}
	UCB0CTL1 |= UCTXSTT; //Send start
	while(!(IFG2 & UCB0TXIFG)); //Wait for transmission to end
}

void i2c_stop(void)
{
	UCB0CTL1 |= UCTXSTP; //Send stop
	while(!(IFG2 & UCB0TXIFG)); //Wait for transmission to end
}

void i2c_write_byte(uint8_t byte)
{
	UCB0TXBUF = byte; //Load byte to Tx register, what starts transmission
	while(!(IFG2 & UCB0TXIFG)); //Wait for transmission to end
}

uint8_t i2c_read_byte(BOOL ack)
{
	uint8_t byte;

	while(!(IFG2 & UCB0RXIFG)); //Wait for reception to end
	byte = UCB0RXBUF; //Store Rx buffer value
	if(ack == NACK) //Send NACK
	{
		UCB0CTL1 |= UCTXNACK;
	}
	return byte;
}
