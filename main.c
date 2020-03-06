#include <msp430.h>
#include <intrinsics.h>
#include <stdint.h>
#include "globals.h"
#include "dht11.h"

void usart1_init(void)
{
	P4SEL |= 0x01;                            // P4.0 = USART1 TXD
	ME2 |= UTXE1;                     		  // Enable USART1 TXD
	U1CTL |= CHAR;                            // 8-bit character
	U1TCTL |= SSEL0;                          // UCLK = ACLK
	U1BR0 = 0x03;                             // 32k/9600 - 3.41
	U1BR1 = 0x00;                             //
	U1MCTL = 0x4A;                            // Modulation
	U1CTL &= ~SWRST;                          // Initialize USART state machine
	//IE2 |= URXIE1;                            // Enable USART1 RX interrupt
}

void usart1_send_char(uint8_t byte)
{
	TXBUF1 = byte;
	while (!(IFG2 & UTXIFG1));
}

void usart1_send_string(const char* str)
{
	while(*str)
	{
		usart1_send_char(*str++);
	}
}


void set_mclk_8mhz(void)
{
	//__disable_interrupt(); //Disable interrupts
	FLL_CTL0 |= XCAP18PF; //Connect capacitors to LFXT1 crystal, 18pF
	FLL_CTL0 |= DCOPLUS; //Multiply DCO frequency by 2, f = 2*(N+1)*32768Hz
	SCFI0 |= FN_4; //DCO frequency in 2.8-26.6MHz range
	SCFQCTL = 121; //N = 121 for 8MHz MCLK
	//__enable_interrupt(); //Enable interrupts
}

dht11_t values;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	set_mclk_8mhz();
	usart1_init();

    while(1)
    {
    	values = dht11_get_measurements();

    	usart1_send_char(values.temperature/10 + 0x30);
    	usart1_send_char(values.temperature%10 + 0x30);
    	usart1_send_string("°C");

    	usart1_send_string("  ");

    	usart1_send_char(values.humidity/10 + 0x30);
    	usart1_send_char(values.humidity%10 + 0x30);
    	usart1_send_string("%\n");

    	__delay_cycles(DELAY_1S);
    }
}
