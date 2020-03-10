/*
 * uart.c
 *
 *  Created on: 6 mar 2020
 *      Author: Lefucjusz
 */

#include "uart.h"
#include <msp430.h>
#include "globals.h"

void usart1_init_9600(void)
{
	P4SEL |= 0x03;                            // P4.0,1 = USART1 TXD, RXD
	ME2 |= UTXE1 | URXE1;                     // Enable USART1 TXD
	U1CTL |= CHAR;                            // 8-bit character
	U1TCTL |= SSEL0;                          // UCLK = ACLK
	U1BR0 = 0x03;                             // 32k/9600 - 3.41
	U1BR1 = 0x00;                             //
	U1MCTL = 0x4A;                            // Modulation
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

char usart1_getc(void)
{
	while(!(IFG2 & URXIFG1));
	return RXBUF1;
}

void usart1_gets(char* buffer)
{
	uint8_t index = 0;
	do
	{
		while(!(IFG2 & URXIFG1)); //Wait for the character to come
		buffer[index] = RXBUF1; //Store character
	} while(buffer[index++] != '\r'); //Do it to first CR character

	buffer[index-1] = '\0'; //Null-terminate on CR position
	__delay_cycles(5*DELAY_1MS); //Delay 5ms for discarded CRLFs
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
