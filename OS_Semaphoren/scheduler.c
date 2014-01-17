/*
 * scheduler.c
 *
 *  Created on: 17.01.2014
 *      Author: Stefan
 */

//threadID scheduler_startThread(threadFunc tFunc) {
//	ATOMIC_START();
//	threadID newThread = THREAD_ID_INVALID;
//	newThread = free
//	thread slot
//	gThreads[newThread].state = THREAD_READY;
//	gThreads[newThread].threadFunc = tFunc;
//	if (setjmp(gThreads[newThread].context) == 0) {
//// current context saved
//		ATOMIC_END();
//		return newThread;
//	} else {
//// running here as active Thread
//		SP = allocate
//		Stack (Pointer)
//		ATOMIC_END();
//		gThread[gRunningThread].tFunc();
//		scheduler_killThread();
//		return THREAD_ID_INVALID;
//	}
//}
//
//void scheduler_runNextThread() {
//	ATOMIC_START();
//	threadID nextThread = determine
//	next Thread(round robin)
//	switch (gThreads[nextThread].state) {
//	case THREAD_RUNNING:
//		break; // already running
//	case THREAD_READY:
//		if (setjmp(gThreads[gRunningThread].context == 0) {
//// saved current execution state
//					if (gThreads[gRunningThread].state == THREAD_RUNNING)
//					gThreads[gRunningThread].state = THREAD_READY;
//					gRunningThread = nextThread;
//					gThreads[gRunningThread].state = THREAD_RUNNING;
//					longjmp(gThreads[gRunningThread].context, 1);
//				}
//				break;
//				default:
//				break; // no thread to run -> sleep no atomic
//			}
//			ATOMIC_END();
//		}
