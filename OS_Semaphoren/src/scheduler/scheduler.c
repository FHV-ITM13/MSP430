/*
 * scheduler.c
 *
 *  Created on: 17.01.2014
 *  	Author: Stefan
 */
#include <stdio.h>
#include <setjmp.h>
#include "scheduler.h"
#define INVALID_THREAD_ID -1

void scheduler_killThread();
int getFreeThreadSlot();
int getFreeThreadSlot();

typedef struct {
	threadID id;;
	int state = 0;

	void (*threadFunc)(void);
	jmp_buf context;
} thread;

enum ThreadState {
	INVALID = 0, // init value of thread
	READY,
	RUNNING,
	BLOCKED,
	SUSPENDED
};

static threadID runningThread = INVALID_THREAD_ID;
static thread threads[];

threadID scheduler_startThread(void (*threadFunc)(void)) {
	ATOMIC_START();

	threadID newThreadSlot;
	newThreadSlot.threadID = getFreeThreadSlot();

	if (newThreadSlot.threadID == INVALID_THREAD_ID) {
		return INVALID_THREAD_ID;
	}

	// new thread
	threads[newThreadSlot].id = newThreadSlot;
	threads[newThreadSlot].state = READY;
	threads[newThreadSlot].threadFunc = threadFunc;

	if (setjmp(threads[newThreadSlot].context) == 0) {
		// current context saved
		ATOMIC_END();
		return newThreadSlot;
	} else {
		// running here as active Thread
		void* SP = malloc(200);
		_set_SP_register(SP);

		ATOMIC_END();

		threads[runningThread].threadFunc();

		scheduler_killThread();

		return INVALID_THREAD_ID;
	}
}

void scheduler_runNextThread() {
	ATOMIC_START();
	threadID nextThread; //determine next Thread(round robin)

	switch (threads[nextThread.threadID].state) {
	case RUNNING:
		break; // already running
	case READY:
		if (setjmp(threads[runningThread.threadID].context == 0)) {
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

	ATOMIC_END();
}

void scheduler_stop() {

}

void scheduler_killThread() {

}

int getFreeThreadSlot() {
	int i;
	int length = sizeof threads / sizeof(threads[0]);

	for (i = 0; i < length; i++) {
		if (threads[i] == NULL) {
			return i;
		}
	}

	return INVALID_THREAD_ID;
}

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
//
//void semaphor_V() {
//	ATOMIC_START();
//	inc counter;
//	if (counter <= 0) {
//		scheduler_continueThread(queue_dequeue());
//	}
//	ATOMIC_END();
//}
