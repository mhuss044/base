/*
 * Animation.h
 *
 *  Created on: Nov 10, 2012
 *      Author: EvorateAwMaDemCriAnR
 *
 *      class to calc new pos of obj after certain time in sec
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

class CAnimInfo
{
public:
	Vert3xf *posToAnimate;

	CAnimInfo(void);
	~CAnimInfo();

	void animationCalculations();		// per frame
};

CAnimInfo::CAnimInfo(void)
{
	posToAnimate = NULL;
}

#endif /* ANIMATION_H_ */
