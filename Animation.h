/*
 * Animation.h
 *
 *  Created on: Nov 10, 2012
 *      Author: EvorateAwMaDemCriAnR
 *
 *      class to calc new pos of obj after certain time in sec
 *		use gluttimerfunc
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

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
