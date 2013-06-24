/*
 * Animation.h
 *
 *  Created on: Nov 10, 2012
 *      Author: EvorateAwMaDemCriAnR
 *
 *      class to calc new pos of obj after certain time in sec
 *		use gluttimerfunc
 *
 *		store frames as vertList, 1frame per 1/5 sec? interpolate b/w em
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <CommonTypes.h>

class CBone
{
private:
	int *AttachedVerts;			// Ptr to arr of index vals of a vertex array; get which verticies are attached to this bone
	float *vertArr;				// Array of verts associated with this bone
public:
};

class CBoneNode
{
private:

public:
};

class CSkeleton
{
private:
	CBoneNode *boneListHead;
public:
	void addBone(CBoneNode *boneToAdd);
};

class CAnimation
{
public:
	vertexList *frameList;
	drawableObj *animatedObj;

	CAnimation(void);
	~CAnimation();

	void animationCalculations();		// per frame
};

CAnimation::CAnimation(void)
{
	frameList = NULL;
	animatedObj = NULL;
}

#endif /* ANIMATION_H_ */
