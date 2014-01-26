#include <msp430.h> 
#include "main.h"
#include "LED1_BLINK.h"
#include "LED2.h"

app_t apps[MAX_APPS];

int currentApp = -1;
int lastApp = -1;

int createApp(char* name, initFunc init, runFunc run, buttonFunc button) {
	int x = lastApp + 1;

	// create App
	apps[x].init = init;
	apps[x].name = name;
	apps[x].id = lastApp;
	apps[x].run = run;
	apps[x].state = 1;
	if (button != NULL) {
		apps[x].button = button;
	} else {
		apps[x].button = &defaultButton;
	}

	lastApp += 1;

	return x;
}

int addApp(char* name, initFunc init, runFunc run, buttonFunc button) {
	ATOMIC_START;
	int x = createApp(name, init, run, button);

	if (0 == setjmp(apps[x].context)) {
		ATOMIC_END;
		return x;
	} else {
		_set_SP_register(START_ADDRESS - x * STACK_SIZE_PER_THREAD);
		apps[x].init(&apps[x]);
		ATOMIC_END;

		apps[x].run(&apps[x]);

		return apps[x].id;
	}
}

AppId findNextApp(void) {
	AppId x = currentApp;
	while (apps[++x].state != 1) {
		if (x >= MAX_APPS) {
			x = -1;
		}
	}
	return x;
}

void switchToApp(AppId next) {
	currentApp = next;

	apps[currentApp].init(&apps[next]);
	longjmp(apps[next].context, 1);
}

void nextApp() {
	ATOMIC_START;
	AppId next = findNextApp();

	if (-1 != currentApp) {
		if (0 == setjmp(apps[currentApp].context)) {
			switchToApp(next);
		}
	} else {
		switchToApp(next);
	}
	ATOMIC_END;
}

bool defaultButton(void) {
	nextApp();

	return TRUE;
}

void initButtonInterupt(void) {
	// set button input
	P1DIR &= ~BUTTON;
	// set button high
	P1OUT |= BUTTON;
	// enable button pull up
	P1REN |= BUTTON;
	// enable button interrupt
	P1IE |= BUTTON;
}

void initTimerA() {
	TACCTL0 = CCIE;
	TA0CTL = TASSEL_1 + MC_1;
	TA0CCR0 = 12000;
}

void main_init() {
	// stop watchdog timer
	WDTCTL = WDTPW + WDTHOLD;

	initButtonInterupt();

	initTimerA();
	_enable_interrupts();
}

/*
 * main.c
 */
int main(void) {
	main_init();

	addApp("RED", LED1_BLINK_init, LED1_BLINK_run, NULL);
	addApp("GREEN ON", LED2_ON_init, LED2_ON_run, NULL);
	addApp("GREEN OFF", LED2_OFF_init, LED2_OFF_run, NULL);
	nextApp();

	_BIS_SR(LPM0_bits + GIE);
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
	P1IFG &= ~BUTTON;
	apps[currentApp].button();
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
	nextApp();
}
