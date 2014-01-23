/*
 * scheduler.h
 *
 *  Created on: 17.01.2014
 *      Author: Stefan
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/*
 * attr, const,...
 */
const int INVALID_ID = -1;

/*
 * struct
 */
typedef struct {
	int threadID;
} threadID;

/*
 * methods
 */
void 		scheduler_runNextThread	();
threadID 	scheduler_startThread	(void (*threadFunc)());
void		scheduler_stop			();

#endif /* SCHEDULER_H_ */
