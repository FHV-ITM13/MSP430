#include <msp430.h> 

/*
 * main.c
 */

#define button BIT3

int main(void) {

	WDTCTL = WDTPW + WDTHOLD;

	 P1DIR|=BIT0;
	 P1DIR|=BIT6;
	 P1OUT^=(1<<0);

	P1REN |= button; // Enable Port P1.3 (push button) pull-up resistor
	P1OUT |= button; //Pullup on


	for (;;) {
		while ((P1IN & button)==0) //while pusing S1
		P1OUT ^= 0X01; //LED OFF
	}
}
