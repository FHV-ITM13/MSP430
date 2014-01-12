#include <msp430.h> 

#define redLED		0x01 //BIT0
#define pushButton 	0x08 //BIT3
#define greenLED	0x40 //BIT6
#define bothLED		0x41

long i = 0;
enum LEDState {
	BOTHON, REDON, GREENON, BOTHOFF
};

enum LEDState ledState;

void main(void) {
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

	P1DIR |= bothLED; 		// Set P1.0 and P1.6 to output direction
	P1DIR &= ~pushButton;	// Port 1 P1.3 (push button) as input, 0 is input

	P1REN |= pushButton; // Enable Port P1.3 (push button) pull-up resistor

	P1OUT = bothLED; // Set both LED on
	P1OUT |= pushButton; //Pullup on
	ledState = BOTHOFF; //both led on

	P1IE |= pushButton; // Port 1 Interrupt Enable P1.3 (push button)

	P1IES |= pushButton; // P1.3 Hi/lo edge

	P1IFG &= ~pushButton; // Clear interrupt flag

	_BIS_SR(GIE);// Enable interrupts

	while (1)
		i++; // Execute some useful computation
}
// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {

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

	__delay_cycles(300000);

	P1IFG &= ~pushButton; // P1.3 Interrupt Flag cleared
}
