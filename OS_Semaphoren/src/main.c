#include <msp430.h> 
#include "scheduler/scheduler.h"

void turnBothLedOn();
void turnBothLedOff();
void turnRedLedOn();
void turnRedLedOff();
void turnGreenLedOn();
void turnGreenLedOff();

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1DIR |= 0x41;				// Set P1.0  and P1.6 to output direction

	TA0CTL = TASSEL_1 | MC_1 | ID_0;
	TACCR0 = 16000;
	TACCTL0 = CCIE;;

	scheduler_startThread(turnBothLedOn);
	scheduler_startThread(turnBothLedOff);
	scheduler_startThread(turnRedLedOn);
	scheduler_startThread(turnRedLedOff);
	scheduler_startThread(turnGreenLedOn);
	scheduler_startThread(turnGreenLedOff);

	_enable_interrupts();

	// idle thread
	while (1) {
	}
}

// Timer0 A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void) {
	scheduler_runNextThread(); 	//change threads each time cycle
}

void turnBothLedOn() {
	while (1) {
		P1OUT ^= 0x41;				// turn both leds on
	}
}

void turnBothLedOff() {
	while (1) {
		P1OUT = 0x00;				// turn both leds off
	}
}

void turnRedLedOn() {
	while (1) {
		P1OUT ^= 0x01;				// turn red led on
	}
}

void turnRedLedOff() {
	while (1) {
		P1OUT &= ~0x01;				// turn red led off
	}
}

void turnGreenLedOn() {
	while (1) {
		P1OUT ^= 0x40;				// turn green led on
	}
}

void turnGreenLedOff() {
	while (1) {
		P1OUT &= ~0x40;				// turn green led off
	}
}
