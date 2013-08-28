/*
 * Physics.h
 *
 *  Created on: Jun 28, 2013
 *      Author: CoM
 *
 *	allows physics simulations
 *	start with animating a ball falling with gravity
 *
 *	beam idea;
		beam motion as ray, propagate, not at speed of light tho
		beam1 pointing at beam2 = get deflection at collision pt; get two beams perp to beam1&2
		beams moving in parallel/occupying same space = amplified
		
		beam gun;
			3 beams pointed at common intersection pt
				= get 2 beams w/ vec perp to plane of the 3src beams
			4th beam placed so that produced beam perp to plane of 3src beams
				'pushes one dir of 2 beams gen by 3src beams'
			result is 1 amplified beam propagating in 1 dir out of gun
		compute?
			get intersection pts; produce a new beam
			
		advanced;
			deathstar beam;
				180deg = 2 beams produced (dragonballz); (mag each beam = (b1+b2)/2)
				<180 = 1 amplified beam (mag = b1+b2)
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
