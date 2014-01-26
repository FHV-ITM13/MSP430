#include <msp430.h> 

//http://homepages.ius.edu/RWISMAN/C335/HTML/msp430Timer.HTM

void main(void){

    WDTCTL = WDT_MDLY_32;                 // Watchdog timer ≈32ms

    IE1 |= WDTIE;                         // enable Watchdog timer interrupts

    P1DIR |= BIT6+BIT0;                   // Green and red LED

    P1SEL |= BIT6;                        // Green LED Pulse width modulation

    TA0CCR0 = 1000;                       // PWM period

    TA0CCR1 = 100;                        // PWM duty cycle, time cycle on vs. off, on 10% initially

    TA0CCTL1 = OUTMOD_7;                  // CCR1 reset/set -- high voltage below count
                                          // and low voltage when past

    TA0CTL = TASSEL_1 + MC_1;             // Timer A control set to submain clock TASSEL_1
                                          // and count up mode MC_1

    _BIS_SR(LPM0_bits + GIE);             // Enter Low power mode 0 with interrupts enabled
}

#pragma vector=WDT_VECTOR                 // Watchdog Timer interrupt service routine

  __interrupt void watchdog_timer(void) {

    P1OUT ^= BIT0;                        // Toggle red LED
}
