#include <msp430.h>
#include <intrinsics.h>
#include <stdint.h>
#include "bool.h"
#include "globals.h"
#include "dht11.h"
#include "uart.h"
#include "lcd.h"
#include "timer.h"
#include "bmp280.h"
#include "wifi.h"
/*
 *
 * TODO - WIFI, I2C timeouts, USART timeouts, LCD display all values
 *
 */

volatile BOOL timer_flag = FALSE;

void set_mclk_8mhz(void)
{
	__disable_interrupt(); //Disable interrupts
	FLL_CTL0 |= XCAP18PF; //Connect capacitors to LFXT1 crystal, 18pF
	FLL_CTL0 |= DCOPLUS; //Multiply DCO frequency by 2, f = 2*(N+1)*32768Hz
	SCFI0 |= FN_4; //DCO frequency in 2.8-26.6MHz range
	SCFQCTL = 121; //N = 121 for ~8MHz MCLK
	__enable_interrupt(); //Enable interrupts
}

void led_init(void)
{
	P5DIR |= 0x02;
}

void timer_callback(void)
{
	P5OUT ^= 0x02;
	timer_flag = TRUE;
}

void main(void)
{
	dht11_t dht_values;
	bmp280_t bmp_values;

	uint8_t seconds = 0;

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	set_mclk_8mhz();

	usart1_init_9600();
	lcd_init();
	led_init();
	bmp280_init();
	timer_init(&timer_callback);

    while(TRUE)
    {
    	if(timer_flag == TRUE)
    	{
    		timer_flag = FALSE;
    		seconds++;

    		if(seconds >= 5)
    		{
    			dht_values = dht11_get_measurements();
    			bmp_values = bmp280_get_measurements();

    			wifi_send_data_frame(dht_values, bmp_values);
    			seconds = 0;
    		}
    	}
    }
}
