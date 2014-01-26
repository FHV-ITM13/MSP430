/*
 * LED_BLINK.c
 *
 *  Created on: 25.01.2014
 *      Author: johannes
 */

#include <msp430.h>
#include "LED1_BLINK.h"
#include "main.h"

void LED1_BLINK_init(app_t* app) {
	P1DIR |= LED1;
}

void LED1_BLINK_run(app_t* app) {
	while(1){
		P1OUT ^= LED1;
		nextApp();
	}
}
