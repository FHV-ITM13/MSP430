#include <msp430.h> 

/*
 * main.c
 */
int main(void) {

	unsigned int i;
	WDTCTL = WDTPW + WDTHOLD; // StopWDT
	TACTL = TASSEL0 + TACLR + TAIE; // ACLK, clear TAR, interrupt enabled
	CCTL0 = OUTMOD_4 + CCIE; // CCR0 toggle, interrupt enabled
	CCTL1 = OUTMOD_4 + CCIE; // CCR1 toggle, interrupt enabled
	CCTL2 = OUTMOD_4 + CCIE; // CCR2 toggle, interrupt enabled
	CCR0 = 0x7FFF;
	CCR1 = 0x3FFF;
	CCR2 = 0x1FFF;
	P1SEL |= 0x01; // P1.0
	P1DIR |= 0x01; // P1.0 Output
	TACTL |= MC1; // Start Timer_A in continuous mode
	_EINT(); // Enable interrupts
	for (;;) {
		_BIS_SR(CPUOFF);
		// CPU off

		_NOP(); // Required only for C-spy
	}
}
// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
	CCR0 += 0x7FFF; // Add Offset to CCR0
}
// Timer_A3 Interrupt Vector (TAIV) handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt  void Timer_A1(void)
{
	switch( TAIV )
	{
		case 2: CCR1 += 0x3FFF; // Add Offset to CCR1
		break;
		case 4: CCR2 += 0x1FFF;// Add Offset to CCR2
		break;
		case 10: P1OUT ^= 0x01;// Timer_A3 overflow
		break;
	}
}

