#include <msp430.h> 

/*
 * main.c
 */

#define button BIT3

int i=0;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR|=BIT0;
    P1DIR|=BIT6;
    P1OUT^=(1<<0);
    P1OUT|=button; //Pullup on

    P1REN|=button; // Enable Port P1.3 (push button) pull-up resistor

    // interrupt stuff
    P1IE |= button; // Port 1 Interrupt Enable P1.3 (push button)
    P1IES |= button; // P1.3 Hi/lo edge
    P1IFG &= ~button; // Clear interrupt flag
    _BIS_SR(GIE);// Enable interrupts

    while (1)
            i++; // Execute some useful computation


}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
        P1OUT ^= BIT0;         // Toggle LED state

        __delay_cycles(300000);

        P1IFG &= ~button; // P1.3 Interrupt Flag cleared
}

