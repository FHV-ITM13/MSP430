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

/*
 * Semaphor: counter, queue
 */

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

	void (*threadFunc)();
	jmp_buf context;
} thread;

enum ThreadState {
	INVALID = 0, // init value of thread
	READY,
	RUNNING,
	BLOCKED,
	SUSPENDED
};

static threadID THREAD_ID_INVALID;

static threadID runningThread;
static thread threads[MAX_THREADS];

void scheduler_init() {
	THREAD_ID_INVALID.threadID = INVALID_ID;

	runningThread = THREAD_ID_INVALID;
}

threadID scheduler_startThread(void (*threadFunc)()) {
	ATOMIC_START();

	threadID newThreadSlot;
	newThreadSlot.threadID = getFreeThreadSlot();

	if (newThreadSlot.threadID != INVALID_ID) {
		// new thread
		threads[newThreadSlot.threadID].id = newThreadSlot;
		threads[newThreadSlot.threadID].state = READY;
		threads[newThreadSlot.threadID].threadFunc = threadFunc;

		if (setjmp(threads[newThreadSlot.threadID].context) == 0) {
			// current context saved
			ATOMIC_END();
		} else {
			// running here as active Thread
			void* SP = malloc(200);
			_set_SP_register(SP);
			ATOMIC_END();

			threads[runningThread.threadID].threadFunc();

			scheduler_killThread();
		}
	}

	return newThreadSlot;
}

void scheduler_runNextThread() {
	ATOMIC_START();
	threadID nextThread;

	if ((nextThread.threadID = getNextThreadID()) != INVALID_ID) {
		switch (threads[nextThread.threadID].state) {
		case RUNNING:
			break; // already running
		case READY:
			if (setjmp(threads[runningThread.threadID].context) == 0) {
				// saved current execution state
				if (threads[runningThread.threadID].state == RUNNING) {
					threads[runningThread.threadID].state = READY;
				}

				//switch threads
				runningThread = nextThread;
				threads[runningThread.threadID].state = RUNNING;
				longjmp(threads[runningThread.threadID].context, 1);
			}
			break;
		default:
			break; // no thread to run -> sleep no atomic
		}
	}

	ATOMIC_END();
}

void scheduler_stop() {

}

void scheduler_killThread() {
	threads[runningThread.threadID].state = READY;
}

int getFreeThreadSlot() {
	int i;

	for (i = 0; i < MAX_THREADS; i++) {
		if (threads[i].threadFunc == NULL) {
			return i;
		}
	}

	return INVALID_ID;
}

int getNextThreadID() {
	int i;

	//search from running thread to max thread for next possible thread
	for (i = runningThread.threadID + 1; i < MAX_THREADS; i++) {
		if (threads[i].threadFunc != NULL && threads[i].state == READY) {
			return i;
		}
	}

	//search part of array before running therad
	for (i = 0; i < runningThread.threadID; i++) {
		if (threads[i].threadFunc != NULL && threads[i].state == READY) {
			return i;
		}
	}

	return INVALID_ID;
}

void ATOMIC_START() {
	//_disable_interrupts()
	TA0CCTL0 &= 0x0; // Disable counter interrupts

}

void ATOMIC_END() {
	//_enable_interrupts();
	TA0CCTL0 = 0x10; // Enable counter interrupts, bit 4=1
}

/*
 * Stemaphore
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
