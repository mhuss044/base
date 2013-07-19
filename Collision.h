/*
 * Collision.h
 *
 *  Created on: Jul 18, 2013
 *      Author: CoM
 *
 *      routines which return;
 *      	collision has occured, collision will occur, vector to collision pt
 *      	T-S seconds to impact
 */

#ifndef COLLISION_H_
#define COLLISION_H_

bool collisionPtLine(float PX,float PY,float PZ, float lineSlope, float lineIntercept)
{
	// 1. take negative inverse of the slope; -1/3 -> 3 = slope of line perp to target line
	// 2. get intercept of line with above slope and contains PX,Y,Z
	// 3. set eqn of target line equal to perp line; get x where they intersect, then get y at that x
	// 4. get dist b/w P and pt at which perp and target intersect
	//		D^2 = (x2-x1)^2 + (y2-y1)^2
	return true;
	return false;
}

// take two vecs, two pts, m/s; can get time to collision

// pt and plane

// vector and plane

// sphere and sphere



#endif /* COLLISION_H_ */
