#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer. Important Restet
	  P1DIR |= 0x01; 							// P1DIR = <PIN7><PIN6><PIN5><PIN4><PIN3><PIN2><PIN1><PIN0>
												//Set Pin0 to output

	  P1DIR |= 0x40;

	  for (;;)
	  {
	    P1OUT ^= 0x01; // Toggle P1.0
	    int i=0;
	    for(i=0; i< 20000; i++);                // Delay between LED toggles. This for-loop will run until the condition is met.
	    P1OUT ^= 0x40; // Toggle P1.6

	  }
}
