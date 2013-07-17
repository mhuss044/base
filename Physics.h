/*
 * Physics.h
 *
 *  Created on: Jun 28, 2013
 *      Author: CoM
 *
 *	allows physics simulations
 *	start with animating a ball falling with gravity
 *
 *	
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

class CPhysicsEngine
{
private:
	Vert3xf gravity;
public:
	void setGravity(Vert3xf dir, float magnitude);
	void updatePhysics(void);							// Per unit time; update verticies?
};


#endif /* PHYSICS_H_ */
