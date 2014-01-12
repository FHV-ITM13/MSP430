#include <msp430.h>

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
	P1DIR = 0;
	P1DIR |= 0x41;                      // Set P1.0 and P1.6 to output direction
	P1OUT = 0x01;				//Set P1.0 to high;

	P1REN |= (0x08); // Enable internal pull up register
	P1OUT |= (0x08); // Pull-Up on

	for (;;) {
		volatile unsigned int i = 8000;

		volatile unsigned int u = P1IN;
		while (i > 0) {
			--i;
		}

		if (!(P1IN & 0x08)) {
			P1OUT ^= 0x40;
			P1OUT ^= 0x01;
		}
	}
}

