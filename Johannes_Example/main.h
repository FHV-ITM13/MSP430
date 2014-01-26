/*
 * main.h
 *
 *  Created on: 25.01.2014
 *      Author: johannes
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MAX_APPS 5
#define STACK_SIZE_PER_THREAD 32
#define START_ADDRESS 0x3FF

#define TRUE    1
#define FALSE   0
#define NULL    (void*)0

#define LED1    BIT0
#define LED2    BIT6
#define BUTTON  BIT3

#define ATOMIC_START _DINT()
#define ATOMIC_END   _EINT()

#include <setjmp.h>

typedef struct App app_t;

typedef int bool;
typedef int AppId;

typedef void (*initFunc)(app_t* t);
typedef void (*runFunc)(app_t* t);
typedef bool (*buttonFunc)(void);

typedef struct App {
	AppId id;
	char* name;
	int state;
	initFunc init;
	runFunc run;
	buttonFunc button;

	jmp_buf context;
} app_t;

int addApp(char* name, initFunc init, runFunc run, buttonFunc button);
void nextApp();
bool defaultButton(void);

#endif /* MAIN_H_ */
