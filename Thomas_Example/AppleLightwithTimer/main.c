#include <msp430.h> 

/*
 * main.c
 */

int pwmDirection=1;

int main(void) {
	 WDTCTL = WDTPW+WDTHOLD;


	        P1DIR |= BIT6;                        // Green LED for output
	        P1SEL |= BIT6;                        // Green LED Pulse width modulation

	        TA0CCR0 = 1000;                       // PWM period
	        TA0CCR1 = 100;                          // PWM duty cycle, time cycle on vs. off, on 1/1000 initially

	        TA0CCTL0|=0x10;
	        TA0CCTL1 |= OUTMOD_7;                  // TA0CCR1 reset/set -- high voltage below count and
	                                              // low voltage when past
	        TA0CTL |= TASSEL_2+ID_3+TACLR+ MC_1;             // Timer A control set to SMCLK clock TASSEL_2, 1MHz
	                                              // and count up mode MC_1

	        _BIS_SR(LPM0_bits + GIE);             // Enter Low power mode 0
	    }



#pragma vector=TIMER0_A0_VECTOR
   __interrupt void Timer0_A0 (void) {		// Timer0 A0 interrupt service routine

	   TA0CCR1 += pwmDirection*20;           // Increase/decrease duty cycle, on vs. off time

		if( TA0CCR1 > 980 || TA0CCR1 < 20 )   // Pulse brighter (increasing TA0CCR1) or dimmer
		   pwmDirection = -pwmDirection;

}
