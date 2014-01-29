/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

/*
 * ======== Grace related includes ========
 */
#include <ti/mcu/msp430/Grace.h>

/*
 *  ======== main ========
 */
int main(void)
{
    Grace_init();                   // Activate Grace-generated configuration
    
    WDTCTL=WDTPW+WDTHOLD;

    P1DIR|=BIT0+BIT6;
    P1OUT|=BIT6;


    TA0CCTL0|=0x10;
    TA0CCR0=12000;

    TA0CTL=TASSEL_1+MC_1;

    _BIS_SR(LPM0_bits);
    
    return (0);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 ( void )
{
  if(P1OUT & BIT0)
  {
	  TA0CCR0=40000;
  }
  else
  {
	  TA0CCR0=12000;
  }

  P1OUT ^=BIT0+BIT6;
}
