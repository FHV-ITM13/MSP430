#include <msp430.h> 


void main(void) {

	WDTCTL = WDTPW + WDTHOLD;

	P1DIR |= BIT0 + BIT6; //output
	P1OUT &= ~(BIT0 + BIT6); //off

	TA0CTL = TASSEL_1 + MC_1;
	TA0CCR0 = 12000; //1 ms
	TA0CCTL0 |= CCIE; //interrupt enable

	_BIS_SR(GIE);

	while (1) {
	}
}

int counter = 0;

// Timer0 A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void) {
	if(counter == 0) {
		P1OUT ^= BIT0; //red on
	}
	//3
	if(counter == 3) {
		P1OUT ^= BIT6 + BIT0;  //green on, red off
	}

	if(counter == 10) {
		P1OUT ^= BIT6;
		counter = -1;
	}

	++counter;
}
