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
	IE2 |= URXIE1;                            // Enable USART1 RX interrupt
}

void usart1_putchar(uint8_t byte)
{
	TXBUF1 = byte; //Load byte to be sent to buffer
	while (!(IFG2 & UTXIFG1)); //Wait for transmission to complete
}

void usart1_string(const char* str)
{
	while(*str)
	{
		usart1_putchar(*str++);
	}
}

#pragma vector=USART1RX_VECTOR
__interrupt void usart1rx_isr(void) //RX interrupt handler
{
	TXBUF1 = RXBUF1; //Redirect input to output
	while (!(IFG2 & UTXIFG1));
}
