#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer

    P1DIR|=BIT6+BIT0;
    P1SEL|=BIT6+BIT0;

    TA0CCR0=12000;
    TA0CCR1=1200;

    TA0CCTL1|=OUTMOD_7;

    TA0CTL|=TASSEL_1+MC_1;

    _BIC_SR(LPM0_bits);

}
