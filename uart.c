/*
 * uart.c
 *
 *  Created on: 6 mar 2020
 *      Author: Lefucjusz
 */

#include "uart.h"
#include <msp430fg4618.h>
#include "globals.h"

void usart1_init_38400(void)
{
	P4SEL |= (1<<0) | (1<<1);                 // P4.0,1 = USART1 TXD, RXD
	ME2 |= UTXE1 | URXE1;                     // Enable USART1 TXD, RXD
	U1CTL |= CHAR;                            // 8-bit character
	U1TCTL |= SSEL1;                          // UCLK = SMCLK
	U1BR0 = 0xD0;                             // 2*(121+1)*32768/38400 = 208.21
	U1BR1 = 0x00;                             //
	U1MCTL = 0x84;                            // Modulation
	U1CTL &= ~SWRST;                          // Initialize USART state machine
}

void usart1_putc(char byte)
{
	TXBUF1 = byte; //Load byte to be sent to buffer
	while (!(IFG2 & UTXIFG1)); //Wait for transmission to complete
}

void usart1_puts(const char* str)
{
	while(*str)
	{
		usart1_putc(*str++);
	}
}

void usart1_put_s32(int32_t num) //Send signed 16-bit integer through UART
{
	if(num < 0) //Check if negative
	{
		usart1_putc('-'); //Send minus sign
		num = -num; //Treat value as positive
	}
	usart1_putc((num/1000000000l) + 0x30); //Split num to digits and send as ASCII
	usart1_putc(((num/100000000l) % 10) + 0x30);
	usart1_putc(((num/10000000l) % 10) + 0x30);
	usart1_putc(((num/1000000l) % 10) + 0x30);
	usart1_putc(((num/100000l) % 10) + 0x30);
	usart1_putc(((num/10000) % 10) + 0x30);
	usart1_putc(((num/1000) % 10) + 0x30);
	usart1_putc(((num/100) % 10) + 0x30);
	usart1_putc(((num/10) % 10) + 0x30);
	usart1_putc((num % 10) + 0x30);
}
