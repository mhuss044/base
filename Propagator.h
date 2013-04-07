/*
 * Propagator.h
 *
 *  Created on: Dec 31, 2012
 *      Author: EvorateAwMaDemCriAnR
 */

#ifndef PROPAGATOR_H_
#define PROPAGATOR_H_

#define PROPAGATOR_NODE_SEPARATION 20

class CPropagatorNode
{
public:
	Vert3xf nodePos;
	CPropagatorNode *nextNode;

	CPropagatorNode(float, float, float);
	CPropagatorNode(Vert3xf*);
	~CPropagatorNode(void);
};

CPropagatorNode::CPropagatorNode(float x, float y, float z)
{
	nextNode = NULL;

	nodePos.x = x;
	nodePos.y = y;
	nodePos.z = z;
}

CPropagatorNode::CPropagatorNode(Vert3xf *pos)
{
	nextNode = NULL;

	nodePos.x = pos->x;
	nodePos.y = pos->y;
	nodePos.z = pos->z;
}

CPropagatorNode::~CPropagatorNode(void)
{
}

class CPropagator
{
public:
	int propDistance;								// how far to propagate
	Vert3xf initPos;
	CPropagatorNode *propogatorNodeHead, *propogatorNodeTail;

	CPropagator(int propDistance, float, float, float);
	~CPropagator(void);
	void initiateProp(int, float, float, float);
	void propagate(void);							// add a node
	void destroy(void);								// Delete all the nodes
	void draw(void);
};

CPropagator::CPropagator(int length, float initX = 0, float initY = 0, float initZ = 0)
{
	propDistance = length;
	propogatorNodeHead = NULL;
	propogatorNodeTail = NULL;

	initPos.x = initX;
	initPos.y = initY;
	initPos.z = initZ;

	initiateProp(length, initPos.x, initPos.y, initPos.z);
}

CPropagator::~CPropagator(void)
{
	destroy();
}

void CPropagator::initiateProp(int length, float newX = 0, float newY = 0, float newZ = 0)
{
	// Dont initiate an already initiated;
	if(propogatorNodeHead != NULL)
		destroy();

	propDistance = length;

	initPos.x = newX;
	initPos.y = newY;
	initPos.z = newZ;

	if(propogatorNodeHead == NULL)
		propogatorNodeHead = new CPropagatorNode(initPos.x, initPos.y, initPos.z);
	else
	{
		cout << EXC << "error creating propagator???";
		return;
	}

	// Set tail;
	propogatorNodeTail = propogatorNodeHead;
}

void CPropagator::propagate(void)
{
	if(propogatorNodeHead == NULL)
	{
		cout << EXC << "Prop error: can't propagate a null head..";
		return;
	}
	else if(propogatorNodeTail->nextNode != NULL)
	{
		cout << EXC << "Propagate error: node next to tail cant be non null?";
		return;
	}
	else if(propogatorNodeTail->nodePos.x > (propDistance + initPos.x))
	{
		destroy();
		initiateProp(propDistance, initPos.x, initPos.y, initPos.z);
	}

	// Add onto the tail, the new pos being the tail pos + node separation
	propogatorNodeTail->nextNode = new CPropagatorNode(propogatorNodeTail->nodePos.x+PROPAGATOR_NODE_SEPARATION, initPos.y, initPos.z);
	// New tail:
	propogatorNodeTail = propogatorNodeTail->nextNode;
}

void CPropagator::draw(void)
{
	if(propogatorNodeHead == NULL)
	{
		cout << EXC << "Error drawing propagator: Cant draw a null head prop..";
		cout << INS << "This prop has " << initPos.x << ", start position";
		return;
	}

	glBegin(GL_LINE_STRIP);
		for(CPropagatorNode *tempPropagatorNode = propogatorNodeHead; tempPropagatorNode != NULL; tempPropagatorNode = tempPropagatorNode->nextNode)
		{
			glVertex3f(tempPropagatorNode->nodePos.x, tempPropagatorNode->nodePos.y, tempPropagatorNode->nodePos.z);
			glPushMatrix();
				glTranslatef(tempPropagatorNode->nodePos.x, tempPropagatorNode->nodePos.y, tempPropagatorNode->nodePos.z);
				DrawCubeNoBottom();
			glPopMatrix();
		}
	glEnd();
}

void CPropagator::destroy(void)
{
	if(propogatorNodeHead == NULL)
		return;

	// 1. Temp == head, 2. head = head->nextNode, 3. delete temp, 1. temp = Head, temp != NULL.
	for(CPropagatorNode *tempPropagatorNode = propogatorNodeHead; tempPropagatorNode != NULL; tempPropagatorNode = propogatorNodeHead)
	{
		propogatorNodeHead = propogatorNodeHead->nextNode;
		delete tempPropagatorNode;
	}
}




#endif /* PROPAGATOR_H_ */
