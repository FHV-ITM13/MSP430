#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	P1DIR |= 0x41;					// Set P1.0  and P1.6 to output direction
	P1OUT = 0x01;					//LED 1 on LED 2 off

	for (;;) {
		volatile unsigned int i;	// volatile to prevent optimization

		P1OUT ^= 0x40;				//activate/deactivate Led 2
		P1OUT ^= 0x01;				//activate/deactivate Led 1
		i = 50000;					// SW Delay
		do
			i--;
		while (i != 0);
	}
	
	
}
