#include <msp430.h> 

enum LEDState {
	BOTHON, REDON, GREENON, BOTHOFF
};

enum LEDState ledState;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	P1DIR |= 0x41;			// Set P1.0 , P1.6 to output (=1), P1.3 input (=0)
	P1OUT = 0x41;				// set LED 1 and 2 on
	P1IE |= BIT3;				// P1.3 interrupt enabled
	P1IES |= BIT3; 				// P1.3 Hi/lo edge
	P1IFG &= ~BIT3;				// P1.3 interrupt flag cleared
	P1REN |= BIT3;    //Enable internal pull up register
	P1OUT |= BIT3; // Schaltet Pull-Up ein - alternativ &= ~(BITXY) fuer Pull-Down


	for (;;) {
		volatile unsigned int i = 5000;

		volatile unsigned int u = P1IN;
		while (i > 0) {
			--i;
		}

		if ((P1IN & 0x08)) {
			switch (ledState) {
				case BOTHON:	//both on -> red on, green off
					ledState = REDON;
					P1OUT ^= greenLED; //green LED off, red LED on
					break;
				case REDON: //red on -> red off, green on
					ledState = GREENON;
					P1OUT ^= redLED; //red off
					P1OUT |= greenLED; //green on
					break;
				case GREENON: //led 2 on; switch to 3
					ledState = BOTHOFF;
					P1OUT ^= greenLED; //turn of green led (red is off already)
					break;
				case BOTHOFF: //both off -> both on
					ledState = BOTHON;
					P1OUT |= bothLED; //turn both on
					break;
			}
		}
	}
}
