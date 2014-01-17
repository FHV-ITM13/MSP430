/*
 * semaphore.c
 *
 *  Created on: 17.01.2014
 *      Author: Stefan
 */

//semaphor_P() {
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
//semaphor_V() {
//	ATOMIC_START();
//	inc counter;
//	if (counter <= 0) {
//		scheduler_continueThread(queue_dequeue());
//	}
//	ATOMIC_END();
//}
