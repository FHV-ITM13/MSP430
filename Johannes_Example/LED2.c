/*
 * LED_BLINK.c
 *
 *  Created on: 25.01.2014
 *      Author: johannes
 */

#include <msp430.h>
#include "LED2.h"
#include "main.h"

void LED2_ON_init(app_t* app) {
	P1DIR |= LED2;
}

void LED2_ON_run(app_t* app) {
	while (1) {
		if (!(P1OUT & LED2)) {
			P1OUT ^= LED2;
		}
	}
}
void LED2_OFF_init(app_t* app) {
	P1DIR |= LED2;
}

void LED2_OFF_run(app_t* app) {
	while (1) {
		if (P1OUT & LED2) {
			P1OUT ^= LED2;
		}
	}
}
