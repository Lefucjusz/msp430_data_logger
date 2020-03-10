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
 * TODO - WIFI, I2C timeouts, USART timeouts, LCD display connection
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
	P5OUT &= ~0x02;
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
	uint8_t display_content = 0;

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	set_mclk_8mhz();

	__delay_cycles(DELAY_1S); //Wait for voltages to stabilize

	usart1_init_38400();
	lcd_init();
	led_init();
	bmp280_init();
	timer_init(&timer_callback);

	dht_values = dht11_get_measurements();
	bmp_values = bmp280_get_measurements();

    while(TRUE)
    {
    	if(timer_flag == TRUE)
    	{
    		timer_flag = FALSE;
    		seconds++;
    		display_content++;

    		if(seconds >= 59)
    		{
    			dht_values = dht11_get_measurements();
    		    bmp_values = bmp280_get_measurements();

    		    wifi_send_data_frame(dht_values, bmp_values);
    		    seconds = 0;
    		}

    		switch(display_content)
    		{
    			case 1:
    	       		lcd_cls();
    				lcd_putchar(1,21); //t
					lcd_putchar(2,12); //d
					lcd_special_char(LCD_COL3); //:
					lcd_putchar(4, dht_values.temperature/10);
					lcd_putchar(5, dht_values.temperature%10);
    				break;
    			case 3:
    	       		lcd_cls();
    				lcd_putchar(1,21); //t
    				lcd_putchar(2,22); //b
    				lcd_special_char(LCD_COL3); //:
    				lcd_putchar(4, (bmp_values.temperature/100)/10);
    				lcd_putchar(5, (bmp_values.temperature/100)%10);
    				break;
    			case 5:
    	       		lcd_cls();
    				lcd_putchar(2,15); //H
    				lcd_special_char(LCD_COL3); //:
    				lcd_putchar(4, dht_values.humidity/10);
    				lcd_putchar(5, dht_values.humidity%10);
    				break;
    			case 7:
    	       		lcd_cls();
    				lcd_putchar(2,18); //P
    				lcd_special_char(LCD_COL3 | LCD_DP7); //: and .
    				lcd_putchar(3, bmp_values.pressure/100000);
    				lcd_putchar(4, (bmp_values.pressure/10000)%10);
    				lcd_putchar(5, (bmp_values.pressure/1000)%10);
    			    lcd_putchar(6, (bmp_values.pressure/100)%10);
    			    lcd_putchar(7, (bmp_values.pressure/10)%10);
    			    break;
    			case 8:
    			    display_content = 0;
    				break;
    		}
    	}
    }
}
