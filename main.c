#include <msp430.h>
#include <intrinsics.h>
#include <stdint.h>
#include "globals.h"
#include "dht11.h"


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

    while(1)
    {
    	values = dht11_get_measurements();

    	__delay_cycles(DELAY_1S);
    }
}
