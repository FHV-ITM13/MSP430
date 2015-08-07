#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
	
	WDTCTL =   WDTPW   +   WDTHOLD;    //  Stop    watchdog    timer
	P1SEL  &=  (~BIT0);
	P1OUT  &=  (~BIT0);
	P1DIR  |=  (BIT0);

	int blink_count;
	for(blink_count = 0;  blink_count < 50; blink_count++)
	{
		P1OUT ^=  (BIT0); //  Toggle  LED
		int i;
		for( i =   0;  i   <   20000;  i++);
	}

	/* Check   if  8MHz    Calibration is  present */
	if (CALBC1_8MHZ    !=  0xFF)
	{
		DCOCTL    =   0;  //  Select  lowest  DCOx    and MODx
		BCSCTL1   =   CALBC1_8MHZ;    //  Set range
		DCOCTL    =   CALDCO_8MHZ;    //  Set DCO step    +   modulation
	}

	for(blink_count    =   0;  blink_count <   50; blink_count++)
	{
		P1OUT ^=  (BIT0); //  Toggle  LED
		int i;
		for( i =   0;  i   <   20000;  i++);
	}
	return 0;
}
