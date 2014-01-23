/*
 * scheduler.h
 *
 *  Created on: 17.01.2014
 *      Author: Stefan
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

typedef struct {
	int threadID;
} threadID;

void 	scheduler_runNextThread	();
int 	scheduler_startThread	(void (*threadFunc)(void));
void	scheduler_stop			();

#endif /* SCHEDULER_H_ */
