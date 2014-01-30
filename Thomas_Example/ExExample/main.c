#include <msp430.h> 
#include <setjmp.h>
#include <stdbool.h>


/*
 * main.c
 */

#define throw(EX) longjmp(gEnv,EX)
#define try int x=setjmp(gEnv);\
	if(0==x)
#define catch(EX) else if(x==EX)
#define globalcatch else

#define finally

typedef enum {
	EX_OK = 0, NotImplementedException = 10, AnotherNiceException = 20,GlobalException = 30
} ExceptionType;

ExceptionType gException = EX_OK;
jmp_buf gEnv;
bool gExSet = false;

bool run=true;

#define button BIT3

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
     	 P1DIR=BIT0+BIT6;
     	 P1OUT&=~(BIT0+BIT6);

     	 P1OUT|=button;
     	 P1REN|=button;

     	 P1IE=button;
     	 P1IFG &= ~button; // Clear interrupt flag

     	 _BIS_SR(GIE);// Enable interrupts

     	 int i=0;
    while(run)
    {
    	try
    	{
    		if(i%2)
    		{
    		throw(GlobalException);
    		}
    		P1OUT|=BIT6;

    	}
    	catch(NotImplementedException)
    	{

    		P1OUT|=BIT0;
    	}
    	catch(AnotherNiceException)
    	{

    		P1OUT|=BIT0;
    	}
    	globalcatch
    	{

    	  P1OUT|=BIT0;
    	}
    	finally
    	{

    		__delay_cycles(640000);
    		__delay_cycles(640000);
    		__delay_cycles(640000);
    		__delay_cycles(640000);
    		P1OUT&=~(BIT0+BIT6);
    		__delay_cycles(640000);
    		__delay_cycles(640000);
    		__delay_cycles(640000);
    		__delay_cycles(640000);

    	}

    	++i;
    }

    P1OUT|=BIT0+BIT6;


	return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void button3(void)
{
	run=false;
	P1IFG &= ~button;
}
