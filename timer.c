/*
 * timer.c
 *
 *  Created on: 6 mar 2020
 *      Author: Lefucjusz
 *      Configures and turns on 1Hz basic timer, which fires BTIFG interrupt every second
 */

#include <msp430.h>
#include <stddef.h>

static void (*timer_callback)(void) = NULL;

void timer_init(void (*callback)(void))
{
	/* Divide timer 32768Hz clocking by 256*128, what gives 1Hz interrupt
	 * Stop f_lcd timer - it is not needed
	 */
	BTCTL = BTDIV | BTIP2 | BTIP1;
	IE2 |= BTIE; //Enable basic timer interrupt
	timer_callback = callback; //Assign passed callback function to timer callback
}

#pragma vector=BASICTIMER_VECTOR
__interrupt void timer_isr(void)
{
	if(timer_callback != NULL)
	{
		timer_callback();
	}
}


