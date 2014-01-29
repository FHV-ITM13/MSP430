#include <msp430.h> 

/*
 * main.c
 */

int i=0;


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR|=BIT0; //Ausgang LED 1
    P1DIR|=BIT6; //Ausgang LED 2

    P1OUT^=BIT0; //LED Einschalten
    P1OUT &=~BIT6; //LED 2 ausschalten

    P1OUT|=BIT3; //Pullup button aktivieren

    P1REN|=BIT3; // Enable Port P1.3 (push button) pull-up resistor


    // interrupt stuff
	P1IE |= BIT3; // Port 1 Interrupt Enable P1.3 (push button)
	P1IES |= BIT3; // P1.3 Hi/lo edge
	P1IFG &= ~BIT3; // Clear interrupt flag

	_BIS_SR(GIE);// Enable interrupts

	while (1)
			i++; // Execute some useful computation



	return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1 (void) {
	if(P1OUT & BIT0)
	{
		 P1OUT&=~BIT0; //LED ausschalten
		 P1OUT^=BIT6; //LED 2 Einschalten

	}
	else
	{
		P1OUT^=BIT0; //LED Einschalten
		P1OUT&=~BIT6; //LED 2 ausschalten

	}

	   __delay_cycles(300000);
	   P1IFG &= ~BIT3; // P1.3 Interrupt Flag cleared
}

