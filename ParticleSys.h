//TODO: render funcs utilize display lists
/*
 * CParticleSys.h
 *
 *  Created on: Oct 15, 2012
 *      Author: EvorateAwMaDemCriAnR
 *
 *      Increase frames: spawn thread; run calculations for next particle position, check if particle dead, change shape of particle
 *      				main thread; render particle
 *      particleMotionRoutine family of functions; each particle gets assigned a motion func(func ptr), pass particle pos, updates pos, scale, etc
 */

#ifndef PARTICLESYS_H_
#define PARTICLESYS_H_

/*
 * ***********************************************
 * ***********************************************
 * PARTICLE MOTION ROUTINES (PMR)
 * ***********************************************
 * ***********************************************
 * 		Manipulate particle position, foraward vector
 */

enum PMR_TYPE
{
	FORWARD_INCREMENT = 0,
	FORWARD_INCREMENT_RAND,
	FORWARD_CURVE_LEFT,
	FORWARD_CURVE_RIGHT,
	ORBIT_AT_HALF_MAXDISP,
	ORBIT_AT_MAXDISP
};

void PMRForwardIncrement(Vert3xf *pos, Vec3d *forwardVec, Bounds *particleBounds)
{
	pos->x += 1*forwardVec->e1;
	pos->y += 1*forwardVec->e2;
	pos->z += 1*forwardVec->e3;
}

void PMRForwardIncrementRand(Vert3xf *pos, Vec3d *forwardVec, Bounds *particleBounds)
{
	if(rand()%2)	// returns 0, 1
		pos->x += 10*forwardVec->e1;
	if(rand()%2)
		pos->y += 10*forwardVec->e2;
	if(rand()%2)
		pos->z += 10*forwardVec->e3;
}

/*
 * ***********************************************
 * ***********************************************
 * PARTICLE RENDER DEFINITION (PRD)
 * ***********************************************
 * ***********************************************
 * 		Defines what particle looks like; draws particle, operations on particle Size,Colour
 */

enum PRD_TYPE
{
	TRIANGLE = 0,
	SQUARE,
	TORUS,
	TEXTURED_TRIANGLE,
	TEXTURED_SQUARE,
	BOTTOMLESS_CUBE
};

void PRDTorus(Vert3xf *pos, Vec3d *forwardVec, Bounds *particleBounds)
{
	glPushMatrix();
		glTranslatef(pos->x, pos->y, pos->z);
		//glScalef
		glutSolidTorus(0.2,0.8,10,10);
	glPopMatrix();
}

void PRDBottomlessCube(Vert3xf *pos, Vec3d *forwardVec, Bounds *particleBounds)
{
	// Front face.
	glBegin(GL_QUADS);
		glVertex3f( pos->x, pos->y, pos->z);							// Bottom left
		glVertex3f( pos->x + 10, pos->y, pos->z);						// Bottom right
		glVertex3f( pos->x + 10, pos->y + 10, pos->z);					// Top right
		glVertex3f( pos->x, pos->y + 10, pos->z);						// Top left
	glEnd();
	// Right face.
	glBegin(GL_QUADS);
		glVertex3f( pos->x + 10, pos->y, pos->z);						// Bottom left
		glVertex3f( pos->x + 10, pos->y, pos->z - 10);					// Bottom right
		glVertex3f( pos->x + 10, pos->y + 10, pos->z - 10);				// Top right
		glVertex3f( pos->x + 10, pos->y + 10, pos->z);					// Top left
	glEnd();
	// Back face.
	glBegin(GL_QUADS);
		glVertex3f( pos->x , pos->y, pos->z);							// Bottom left
		glVertex3f( pos->x + 10, pos->y, pos->z);						// Bottom right
		glVertex3f( pos->x + 10, pos->y, pos->z - 10);					// Top right
		glVertex3f( pos->x, pos->y, pos->z - 10);						// Top left
	glEnd();
	// Left face.
	glBegin(GL_QUADS);
		glVertex3f( pos->x, pos->y, pos->z - 10);						// Bottom left
		glVertex3f( pos->x, pos->y, pos->z);							// Bottom right
		glVertex3f( pos->x, pos->y + 10, pos->z);						// Top right
		glVertex3f( pos->x, pos->y + 10, pos->z - 10);					// Top left
	glEnd();
	// Top face.
	glBegin(GL_QUADS);
		glVertex3f( pos->x, pos->y + 10, pos->z);						// Bottom left
		glVertex3f( pos->x + 10, pos->y + 10, pos->z);					// Bottom right
		glVertex3f( pos->x + 10, pos->y + 10, pos->z - 10);				// Top right
		glVertex3f( pos->x, pos->y + 10, pos->z - 10);					// Top left
	glEnd();
}

void PRDSquare(Vert3xf *pos, Vec3d *forwardVec, Bounds *particleBounds)
{
    glBegin(GL_QUADS);
        glVertex3f( pos->x, pos->y, pos->z);
        glVertex3f( pos->x + 10, pos->y, pos->z);
        glVertex3f( pos->x + 10, pos->y + 10, pos->z);
        glVertex3f( pos->x, pos->y + 10, pos->z);
    glEnd();
}

void PRDTriangle(Vert3xf *pos, Vec3d *forwardVec, Bounds *particleBounds)
{
    glBegin(GL_QUADS);
        glVertex3f( pos->x, pos->y, pos->z);
        glVertex3f( pos->x + 5, pos->y + 10, pos->z);
        glVertex3f( pos->x + 10, pos->y, pos->z);
    glEnd();
}

class CParticle
{
public:
	Vert3xf pos;
	Vec3d forwardVec;		// max displacement in each of the coords
	Bounds particleBounds;	// max and min pos
	bool isAlive;

	CParticle(Vert3xf particleOrigin, Bounds particlePosLimits, Vec3d forwardDir);
	void (*PRDfuncPtr)(Vert3xf *pos, Vec3d *forwardVec, Bounds *particleBounds);// func ptr def to a func which draws the particle
	void (*PMRfuncPtr)(Vert3xf *pos, Vec3d *forwardVec, Bounds *particleBounds);// func ptr to func which manip pos/forward vec
	void setPRDfunc(PRD_TYPE selection);
	void setPMRfunc(PMR_TYPE selection);
	bool isDead(void);
	void updateLifeStatus(void);
};

CParticle::CParticle(Vert3xf particleOrigin, Bounds particlePosLimits, Vec3d forwardDir)
{
	particleBounds = particlePosLimits;

	pos.x = particleOrigin.x;
	pos.y = particleOrigin.y;
	pos.z = particleOrigin.z;

	forwardVec.e1 = forwardDir.e1;
	forwardVec.e2 = forwardDir.e2;
	forwardVec.e3 = forwardDir.e3;

	isAlive = true;

	PRDfuncPtr = NULL;
	PMRfuncPtr = NULL;
}

void CParticle::setPRDfunc(PRD_TYPE selection)
{
	switch(selection)
	{
	case TRIANGLE:
		PRDfuncPtr = &PRDTriangle;
		break;
	case SQUARE:
		PRDfuncPtr = &PRDSquare;
		break;
	case TORUS:
		PRDfuncPtr = &PRDTorus;
		break;
	case TEXTURED_TRIANGLE:
		break;
	case TEXTURED_SQUARE:
		break;
	case BOTTOMLESS_CUBE:
		PRDfuncPtr = &PRDBottomlessCube;
		break;
	}
}

void CParticle::setPMRfunc(PMR_TYPE selection)
{
	switch(selection)
	{
	case FORWARD_INCREMENT:
		PMRfuncPtr = &PMRForwardIncrement;
		break;
	case FORWARD_INCREMENT_RAND:
		PMRfuncPtr = &PMRForwardIncrementRand;
		break;
	case FORWARD_CURVE_LEFT:
		break;
	case FORWARD_CURVE_RIGHT:
		break;
	case ORBIT_AT_HALF_MAXDISP:
		break;
	case ORBIT_AT_MAXDISP:
		break;
	}
}

bool CParticle::isDead(void)
{
	if(pos.x > particleBounds.maxX || pos.x < particleBounds.minX)
		isAlive = false;
	if(pos.y > particleBounds.maxY || pos.y < particleBounds.minY)
		isAlive = false;
	if(pos.z > particleBounds.maxZ || pos.z < particleBounds.minZ)
		isAlive = false;

	if(isAlive)
		return false;
	else
		return true;
}

class particleNode
{
public:
	CParticle *particle;
	particleNode *nextParticleNode;

	particleNode(void);
	~particleNode(void);
};

particleNode::particleNode(void)
{
	particle = NULL;
	nextParticleNode = NULL;
}

particleNode::~particleNode(void)
{
	if(particle != NULL)
		delete particle;
}

class CParticleSys
{
public:
	int numParticles;
	Vert3xf systemOrigin;				// Specific range of possible values for the origin/end of a new particle
	Bounds particleSysBounds;
	particleNode *particleHead;

	CParticleSys(int numP, float originX, float originY, float originZ, int maxDisplacement);
	~CParticleSys();
	inline void initializeParticleNode(particleNode *nodeToInit);		// Give particle its properties from this particle system
	void particleRespawner(void);										// Check if particle is dead, recycle it
	void recycleParticle(particleNode *nodeToRecycle);			//	Translate particle, reset, dont delete particle data
	void destroyParticle(void);			//	delete

	void originModifier(void);			// Provide a state, modifier acts on origin accordingly
	void destinationModifier(void);		// Provide state, modifier acts on endpt accordingly
	void particleMotionRoutines(void);	// Run pos and forwad vec operations
	void renderParticles(void);			// RUn render routines
	void resetAllParticles(void);		// reset em all
};

CParticleSys::CParticleSys(int numP, float originX, float originY, float originZ, int maxDisplacement)
{
	numParticles = numP;
	particleHead = NULL;

	systemOrigin.x = originX;
	systemOrigin.y = originY;
	systemOrigin.z = originZ;

	particleSysBounds.minX = systemOrigin.x - maxDisplacement;
	particleSysBounds.maxX = systemOrigin.x + maxDisplacement;
	particleSysBounds.minY = systemOrigin.y - maxDisplacement;
	particleSysBounds.maxY = systemOrigin.y + maxDisplacement;
	particleSysBounds.minZ = systemOrigin.z - maxDisplacement;
	particleSysBounds.maxZ = systemOrigin.z + maxDisplacement;

	particleHead = new particleNode;
	initializeParticleNode(particleHead);

	particleNode *tempParticleNode = particleHead;

	for(int x = 1; x < numParticles; x++)
	{
		tempParticleNode->nextParticleNode = new particleNode;
		tempParticleNode = tempParticleNode->nextParticleNode;
		initializeParticleNode(tempParticleNode);
	}
}

CParticleSys::~CParticleSys(void)
{
	if(particleHead != NULL)
		for(particleNode *tempParticleNode = particleHead; tempParticleNode != NULL; tempParticleNode = particleHead)
		{
			particleHead = tempParticleNode->nextParticleNode;
			if(tempParticleNode != NULL)
				delete tempParticleNode;
		}
}

inline void CParticleSys::initializeParticleNode(particleNode *nodeToInit)
{
	Vert3xf particleOrigin = systemOrigin;
	Vec3d forwardDir;
	Bounds particleBounds = particleSysBounds;

	//particleOrigin = systemOrigin;
/*	particleOrigin.y = systemOrigin.y;
	particleOrigin.z = systemOrigin.z;

	maxDisp.e1 = displacementMax.e1;
	maxDisp.e2 = displacementMax.e2;
	maxDisp.e3 = displacementMax.e3;
*/
	forwardDir.e1 = rand()%3 - 1;		// rand()%3 returns 0,1,2: so -1 = -1, 0, 1
	forwardDir.e2 = rand()%3 - 1;
	forwardDir.e3 = rand()%3 - 1;

	// Make sure atleast have a forward vec;
	while(forwardDir.e1 == 0 && forwardDir.e2 == 0 && forwardDir.e3 == 0)
	{
		forwardDir.e1 = rand()%3 - 1;		// rand()%3 returns 0,1,2: so -1 = -1, 0, 1
		forwardDir.e2 = rand()%3 - 1;
		forwardDir.e3 = rand()%3 - 1;
	}

	// New particle:
	nodeToInit->particle = new CParticle(particleOrigin, particleBounds, forwardDir);

	//switch(PARTICLE_SYS_TYPE)
	nodeToInit->particle->setPRDfunc(BOTTOMLESS_CUBE);//BOTTOMLESS_CUBE, SQUARE
	nodeToInit->particle->setPMRfunc(FORWARD_INCREMENT_RAND);
}

void CParticleSys::recycleParticle(particleNode *nodeToRecycle)
{
	nodeToRecycle->particle->pos = systemOrigin;
/*
	nodeToRecycle->particle->pos.x = originMin.x;
	nodeToRecycle->particle->pos.x = originMin.y;
	nodeToRecycle->particle->pos.x = originMin.z;
*/
	nodeToRecycle->particle->isAlive = true;
}

void CParticleSys::particleRespawner(void)
{
	particleNode *tempParticleHead = particleHead;
	if(tempParticleHead != NULL)
	{
		// Run motion routine:
		for(particleNode *tempParticleNode = tempParticleHead; tempParticleNode != NULL; tempParticleNode = tempParticleHead)
		{
			tempParticleHead = tempParticleNode->nextParticleNode;
			// Check if need reset
			if(tempParticleNode->particle->isDead())
				recycleParticle(tempParticleNode);
		}
	}
}

void CParticleSys::particleMotionRoutines(void)
{
	particleNode *tempParticleHead = particleHead;
	if(tempParticleHead != NULL)
	{
		// Run motion routine:
		for(particleNode *tempParticleNode = tempParticleHead; tempParticleNode != NULL; tempParticleNode = tempParticleHead)
		{
			tempParticleHead = tempParticleNode->nextParticleNode;
			// Make sure particle isnt dead:
			if(!tempParticleNode->particle->isDead())
				tempParticleNode->particle->PMRfuncPtr(&tempParticleNode->particle->pos, &tempParticleNode->particle->forwardVec, &tempParticleNode->particle->particleBounds);
		}
	}
}

void CParticleSys::renderParticles(void)
{
	particleNode *tempParticleHead = particleHead;
	if(tempParticleHead != NULL)
	{
		// Run render routine:
		for(particleNode *tempParticleNode = tempParticleHead; tempParticleNode != NULL; tempParticleNode = tempParticleHead)
		{
			tempParticleHead = tempParticleNode->nextParticleNode;
			tempParticleNode->particle->PRDfuncPtr(&tempParticleNode->particle->pos, &tempParticleNode->particle->forwardVec, &tempParticleNode->particle->particleBounds);
		}
	}
}

void CParticleSys::resetAllParticles(void)
{
	particleNode *tempParticleHead = particleHead;
	if(tempParticleHead != NULL)
	{
		// Run motion routine:
		for(particleNode *tempParticleNode = tempParticleHead; tempParticleNode != NULL; tempParticleNode = tempParticleHead)
		{
			tempParticleHead = tempParticleNode->nextParticleNode;
			recycleParticle(tempParticleNode);
		}
	}
}

#endif /* PARTICLESYS_H_ */
