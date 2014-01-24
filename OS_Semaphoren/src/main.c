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

	// LPM0 (low power mode) with interrupts enabled

	scheduler_startThread(turnLedOn);
	scheduler_startThread(turnLedOff);

	_enable_interrupts();

	// idle thread
	while (1) {

	}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void) {		// Timer0 A0 interrupt service routine
	//change threads each time cycle
	scheduler_runNextThread();
}

void turnLedOn() {
	while (1) {
		if(P1OUT & 0x41) {
			P1OUT ^= 0x41;				// Toggle P1.0 and P1.6 using exclusive-OR
		}
	}

}

void turnLedOff() {
	while (1) {
		if(!(P1OUT & 0x41)) {
			P1OUT ^= 0x41;				// Toggle P1.0 and P1.6 using exclusive-OR
		}
	}

}
