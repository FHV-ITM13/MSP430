/*
 * scheduler.h
 *
 *  Created on: 17.01.2014
 *      Author: Stefan
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/*
 * Typedef
 */
typedef int threadID;
typedef void (*threadFunc)();

/*
 * methods
 */
void 		scheduler_runNextThread	();
threadID 	scheduler_startThread	(threadFunc func);

#endif /* SCHEDULER_H_ */
