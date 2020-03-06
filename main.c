#include <msp430.h>
#include <intrinsics.h>
#include <stdint.h>
#include "globals.h"
#include "dht11.h"
#include "uart.h"
#include "lcd.h"

void set_mclk_8mhz(void)
{
	__disable_interrupt(); //Disable interrupts
	FLL_CTL0 |= XCAP18PF; //Connect capacitors to LFXT1 crystal, 18pF
	FLL_CTL0 |= DCOPLUS; //Multiply DCO frequency by 2, f = 2*(N+1)*32768Hz
	SCFI0 |= FN_4; //DCO frequency in 2.8-26.6MHz range
	SCFQCTL = 121; //N = 121 for 8MHz MCLK
	__enable_interrupt(); //Enable interrupts
}

dht11_t values;

uint8_t i, j;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	set_mclk_8mhz();
	usart1_init_9600();

	lcd_init();

	lcd_putchar(5, 20);

    while(1)
    {
    	values = dht11_get_measurements();

    	usart1_putchar(values.temperature/10 + 0x30);
    	usart1_putchar(values.temperature%10 + 0x30);
    	usart1_string("°C");

    	usart1_string(" ");

    	usart1_putchar(values.humidity/10 + 0x30);
    	usart1_putchar(values.humidity%10 + 0x30);
    	//usart1_send_char('\n');
    	usart1_string("%\n");

    	lcd_putchar(3, values.temperature/10);
    	lcd_putchar(4, values.temperature%10);
    	lcd_putchar(6, 11); //C

    	__delay_cycles(DELAY_1S);

    	lcd_putchar(3, values.humidity/10);
    	lcd_putchar(4, values.humidity%10);
    	lcd_putchar(6, 15); //H

    	__delay_cycles(DELAY_1S);
    }
}
