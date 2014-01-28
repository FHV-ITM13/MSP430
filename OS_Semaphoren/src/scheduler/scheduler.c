/*
 * scheduler.c
 *
 *  Created on: 17.01.2014
 *  	Author: Stefan
 */
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include "scheduler.h"
#include <msp430.h>

#define MAX_THREADS		10
#define THREAD_SIZE		32

/*
 * Scheduler methods
 */
void scheduler_killThread();
int getFreeThreadSlot();
int getNextThreadID();
void ATOMIC_START(); // down
void ATOMIC_END(); // up

/**
 * THREAD STUFF
 */
typedef struct {
	threadID id;
	unsigned int state;
	threadFunc func;
	jmp_buf context;
} thread;

typedef unsigned int intstate_t;

enum ThreadState {
	INVALID = 0, // init value of thread
	READY,
	RUNNING,
	BLOCKED
};

static threadID INVALID_ID = -1;
static threadID runningThread = -1;
static thread threads[MAX_THREADS];

threadID scheduler_startThread(threadFunc func) {
	ATOMIC_START();

	threadID newThreadSlot = getFreeThreadSlot();

	if (newThreadSlot == INVALID_ID) {
		ATOMIC_END();
		return INVALID_ID;
	}

	// new thread
	threads[newThreadSlot].id = newThreadSlot;
	threads[newThreadSlot].state = READY;
	threads[newThreadSlot].func = func;

	if (setjmp(threads[newThreadSlot].context) == 0) {
		// current context saved
		ATOMIC_END();
		return newThreadSlot;
	} else {
		//coming from long jump
		// running here as active Thread
		__set_SP_register(0x3FF - (runningThread * THREAD_SIZE));

		ATOMIC_END();

		threads[runningThread].func();

		scheduler_killThread();

		return INVALID_ID;
	}
}

void scheduler_runNextThread() {
	ATOMIC_START();
	threadID nextThread = getNextThreadID();

	if (nextThread != INVALID_ID) {
		switch (threads[nextThread].state) {
		case READY:
			if (setjmp(threads[runningThread].context) == 0) {
				// saved current execution state
				if (threads[runningThread].state == RUNNING) {
					threads[runningThread].state = READY;
				}

				//switch threads
				runningThread = nextThread;
				threads[runningThread].state = RUNNING;
				longjmp(threads[runningThread].context, 1);
			}
			break;
		default:
			break; // no thread to run -> sleep no atomic
		}
	}

	ATOMIC_END();
}

void scheduler_killThread() {
	threads[runningThread].state = INVALID;
	threads[runningThread].func = 0;
}

int getFreeThreadSlot() {
	int i;

	for (i = 0; i < MAX_THREADS; i++) {
		if (threads[i].state == INVALID) {
			return i;
		}
	}

	return INVALID_ID;
}

int getNextThreadID() {
	threadID nextThread = runningThread + 1;

	while(nextThread < MAX_THREADS && nextThread != runningThread) {
		if ( READY == threads[ nextThread ].state )
		{
			return nextThread;
		}

		nextThread = ++nextThread % MAX_THREADS;
	}

	return INVALID_ID;
}

void ATOMIC_START() {
	_disable_interrupts();
}

void ATOMIC_END() {
	_enable_interrupts();
}

/*
 * Stemaphore - nur fuer die synchronisierung
 * benoetigt
 */
//void semaphor_P() {
//	ATOMIC_START();
//	dec counter;
//	if (counter < 0) {
//		queue < -current
//		threadID
//		scheduler_suspend(current threadID);
//		scheduler_runNextThread();
//	}
//	ATOMIC_END();
//}
//void semaphor_V() {
//	ATOMIC_START();
//	inc counter;
//	if (counter <= 0) {
//		scheduler_continueThread(queue_dequeue());
//	}
//	ATOMIC_END();
//}
