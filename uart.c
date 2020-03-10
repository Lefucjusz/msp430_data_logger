/*
 * uart.c
 *
 *  Created on: 6 mar 2020
 *      Author: Lefucjusz
 */

#include "uart.h"
#include <msp430.h>

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
	//IE2 |= URXIE1;                            // Enable USART1 RX interrupt
}

void usart1_putchar(char byte)
{
	TXBUF1 = byte; //Load byte to be sent to buffer
	while (!(IFG2 & UTXIFG1)); //Wait for transmission to complete
}

void usart1_send_string(const char* str)
{
	while(*str)
	{
		usart1_putchar(*str++);
	}
}

char usart1_getchar(void)
{
	while(!(IFG2 & URXIFG1));
	return RXBUF1;
}

void usart1_get_string(char* buffer)
{
	uint8_t index = 0;
	do
	{
		while(!(IFG2 & URXIFG1)); //Wait for the character to come
		buffer[index] = RXBUF1; //Store character
	} while(buffer[index++] != '\r'); //Do it to first CR character

	buffer[index-1] = '\0'; //Null-terminate on CR position
}

//#pragma vector=USART1RX_VECTOR
//__interrupt void usart1rx_isr(void) //RX interrupt handler
//{
//	TXBUF1 = RXBUF1; //Redirect input to output
//	while (!(IFG2 & UTXIFG1));
//}
