#include <msp430.h> 


void main(void){

    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer

    P1DIR |= BIT6;                        // Green LED

    P1SEL |= BIT6;                        // Green LED selected for Pulse Width Modulation

    TA0CCR0 = 12000;                      // PWM period, 12000 ACLK ticks or 1/second

    TA0CCR1 = 1200;                       // PWM duty cycle, time cycle on vs. off, on 10% initially

    TA0CCTL1 = OUTMOD_7;                  // TA0CCR1 reset/set -- high voltage below TA0CCR1 count
                                          // and low voltage when past

    TA0CTL = TASSEL_1 + MC_1;             // Timer A control set to submain clock TASSEL_1 ACLK
                                          // and count up to TA0CCR0 mode MC_1

    _BIS_SR(LPM0_bits);                   // Enter Low power mode 0
}
