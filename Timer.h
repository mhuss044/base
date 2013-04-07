/*
 * Timer.h
 *
 *  Created on: Jan 12, 2013
 *      Author: EvorateAwMaDemCriAnR
 */

#ifndef TIMER_H_
#define TIMER_H_

class CTimer
{
public:
	CTimer(void (*oneSec)(void) = NULL, void (*thirtiethSec)(void) = NULL, void (*tirtySec)(void) = NULL);
	~CTimer(void);

	void (*callbackOneSecond)(void);			// 1s
	void (*callbackThirtySeconds)(void);		// 30s
	void (*callbackThirtiethOfASecond)(void);	// 1/30 second
	void keepTime(void);						// per frame
};


#endif /* TIMER_H_ */
