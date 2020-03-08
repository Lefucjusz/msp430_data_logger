#include <msp430.h>
#include <intrinsics.h>
#include <stdint.h>
#include "globals.h"
#include "dht11.h"
#include "uart.h"
#include "lcd.h"
#include "timer.h"
#include "i2c.h"
#include "l3gd20h.h"

/*
 *
 * TODO - real time clock for datagrams timestamps
 * TODO - IMU library, I2C library
 *
 */

volatile uint8_t timer_flag = 0;

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
	timer_flag = 1;
}

uint8_t whoami;
uint16_t checksum;
l3gd20h_t measurements;

void main(void)
{
	dht11_t values;
	uint8_t state = 0;

    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	set_mclk_8mhz();

	usart1_init_9600();
	lcd_init();
	led_init();
	timer_init(&timer_callback);

	lcd_putchar(5, 20); //°

//	i2c_init(0b1101011);
//	i2c_start(WRITE);
//	i2c_write_byte(0x0F);
//	i2c_start(READ);
//	whoami = i2c_read_byte(NACK);
//	i2c_stop();
//
//	checksum = ((whoami == 0xD4) || (whoami == 0xD7));

	l3gd20h_init();

	while(1)
	{
		measurements = l3gd20h_get_measurements();

		//usart1_string("\nx: ");
		if(measurements.x < 0)
		{
			usart1_putchar('-');
			measurements.x = -measurements.x;
		}
		usart1_putchar((measurements.x / 10000) + 0x30);
		usart1_putchar(((measurements.x / 1000) % 10) + 0x30);
		usart1_putchar(((measurements.x / 100) % 10) + 0x30);
		usart1_putchar(((measurements.x / 10) % 10) + 0x30);
		usart1_putchar((measurements.x  % 10) + 0x30);

		usart1_putchar(' ');

		//usart1_string(" y: ");
		if(measurements.y < 0)
		{
			usart1_putchar('-');
			measurements.y = -measurements.y;
		}
		usart1_putchar((measurements.y / 10000) + 0x30);
		usart1_putchar(((measurements.y / 1000) % 10) + 0x30);
		usart1_putchar(((measurements.y / 100) % 10) + 0x30);
		usart1_putchar(((measurements.y / 10) % 10) + 0x30);
		usart1_putchar((measurements.y  % 10) + 0x30);

		usart1_putchar(' ');

		//usart1_string(" z: ");
		if(measurements.z < 0)
		{
			usart1_putchar('-');
			measurements.z = -measurements.z;
		}
		usart1_putchar((measurements.z / 10000) + 0x30);
		usart1_putchar(((measurements.z / 1000) % 10) + 0x30);
		usart1_putchar(((measurements.z / 100) % 10) + 0x30);
		usart1_putchar(((measurements.z / 10) % 10) + 0x30);
		usart1_putchar((measurements.z % 10) + 0x30);

		usart1_putchar('\n');

		__delay_cycles(DELAY_1MS*100);
	}

    while(1)
    {
    	if(timer_flag)
    	{
    		timer_flag = 0;
    		values = dht11_get_measurements();

			usart1_putchar(values.temperature/10 + 0x30);
			usart1_putchar(values.temperature%10 + 0x30);
			usart1_string("°C");

			usart1_string(" ");

			usart1_putchar(values.humidity/10 + 0x30);
			usart1_putchar(values.humidity%10 + 0x30);
			//usart1_send_char('\n');
			usart1_string("%\n");

			if(state == 0)
			{
				lcd_putchar(3, values.temperature/10);
				lcd_putchar(4, values.temperature%10);
				lcd_putchar(6, 11); //C
			}
			else if(state == 2)
			{
				lcd_putchar(3, values.humidity/10);
				lcd_putchar(4, values.humidity%10);
				lcd_putchar(6, 15); //H
			}

			state++;
			if(state >= 4)
			{
				state = 0;
			}
    	}
    }
}
