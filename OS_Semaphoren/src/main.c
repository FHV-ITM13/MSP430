#include <msp430.h> 
#include "scheduler/scheduler.h"

void turnLedOn();
void turnLedOff();

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1DIR |= 0x41;				// Set P1.0  and P1.6 to output direction

	TA0CCR0 = 12000;			// Count limit (16 bit)
	TA0CCTL0 = 0x10;			// Enable counter interrupts, bit 4=1
	TA0CTL = TASSEL_1 + MC_1; 	// Timer A 0 with ACLK @ 12KHz, count UP
	_BIS_SR(LPM0_bits + GIE); 	// LPM0 (low power mode) with interrupts enabled

	scheduler_init();

	scheduler_startThread((*turnLedOn));
	scheduler_startThread((*turnLedOff));

	while(1) {

	}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void) {		// Timer0 A0 interrupt service routine
	//change threads each time cycle
	scheduler_runNextThread();
}

void turnLedOn() {
	P1OUT ^= 0x41;				// Toggle P1.0 and P1.6 using exclusive-OR

}

void turnLedOff() {
	P1OUT ^= 0x41;				// Toggle P1.0 and P1.6 using exclusive-OR

}
