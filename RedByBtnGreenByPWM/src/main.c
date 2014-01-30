/*
 * main.c
 *
 *  Created on: 29.01.2014
 *      Author: Stefan
 *
 *  -Use Button to turn RED LED on/off
 *  -Use PWM to fade GREEN LED in/out
 */

#include <msp430.h>

#define LED1	0x01 //BIT0
#define LED2	0x40 //BIT6
#define BUTTON	0x08 //BIT3


int pwmDirection = 1;

int main() {
	WDTCTL = WDTPW + WDTHOLD; //stop watchdog

	//configure LEDs
	P1DIR |= LED1 + LED2;	//led1 led2 output direction
	P1OUT &= ~(LED1 + LED2); 	//turn both leds off
    P1SEL |= LED2;          // Green LED Pulse width modulation

	//configure button
	P1DIR &= ~BUTTON;	//button input direction
	P1REN |= BUTTON;	//enable pull-up resistor
	P1OUT |= BUTTON; 	//Pullup on
	P1IE |= BUTTON;		//enable button interrupt

	//configure timer A
	TA0CCR0 = 1000;// PWM period
	TA0CCTL0 = 0x10; //up mode
	TA0CCR1 = 100;   // PWM duty cycle, time cycle on vs. off, on 1/1000 initially
	TA0CCTL1 = OUTMOD_7;    // TA0CCR1 reset/set -- high voltage below count and low voltage when past
	TA0CTL = TASSEL_1 + MC_1; // Timer A control set to ACLK clock and count up mode MC_1

	_BIS_SR(LPM0_bits + GIE);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER(void) {
	TA0CCR1 += pwmDirection * 20; // Increase/decrease duty cycle, on vs. off time

	if (TA0CCR1 > 980 || TA0CCR1 < 20) // Pulse brighter (increasing TA0CCR1) or dimmer
		pwmDirection = -pwmDirection;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1INTERUPT(void) {
	P1OUT ^= LED1;
	P1IFG &= ~BUTTON; // P1.3 Interrupt Flag cleared
}
