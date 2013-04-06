/*
 * Membrane.h
 *
 *  Created on: Dec 31, 2012
 *      Author: EvorateAwMaDemCriAnR
 */

#ifndef MEMBRANE_H_
#define MEMBRANE_H_

#define MEMBRANE_NODE_SEPARATION 20

class CMembraneNode
{
public:
	Vert3xf nodePos;
	CMembraneNode *nextNode;

	CMembraneNode(float, float, float);
	CMembraneNode(Vert3xf*);
	~CMembraneNode(void);
};

CMembraneNode::CMembraneNode(float x, float y, float z)
{
	nextNode = NULL;

	nodePos.x = x;
	nodePos.y = y;
	nodePos.z = z;
}

CMembraneNode::CMembraneNode(Vert3xf *pos)
{
	nextNode = NULL;

	nodePos.x = pos->x;
	nodePos.y = pos->y;
	nodePos.z = pos->z;
}

CMembraneNode::~CMembraneNode(void)
{
}

class CMembrane
{
public:
	int numNodes;
	Vert3xf startPos;
	CMembraneNode *membraneNodeHead;

	CMembrane(int initialNumNodes, float, float, float);
	~CMembrane(void);
	void addMembraneNode(void);
	void createMembrane(int, float, float, float);
	void destroyMembrane(void);								// Delete all the membrane nodes
	void drawMembrane(void);
};

CMembrane::CMembrane(int initialNumNodes, float initX = 0, float initY = 0, float initZ = 0)
{
	numNodes = initialNumNodes;
	membraneNodeHead = NULL;

	startPos.x = initX;
	startPos.y = initY;
	startPos.z = initZ;

	createMembrane(numNodes, startPos.x, startPos.y, startPos.z);
}

CMembrane::~CMembrane(void)
{
	destroyMembrane();
}

void CMembrane::createMembrane(int newNumNodes, float newX = 0, float newY = 0, float newZ = 0)
{
	CMembraneNode *tempMembraneNode = NULL;

	if(membraneNodeHead != NULL)
		destroyMembrane();

	// Set the new total nodes
	numNodes = newNumNodes;

	startPos.x = newX;
	startPos.y = newY;
	startPos.z = newZ;

	if(membraneNodeHead == NULL)
		membraneNodeHead = new CMembraneNode(startPos.x, startPos.y, startPos.z);
	else
	{
		cout << EXC << "error creating membrane???";
		return;
	}

	tempMembraneNode = membraneNodeHead;

	for(int x = 1; x < numNodes; x++)
	{
		tempMembraneNode->nextNode = new CMembraneNode(startPos.x+MEMBRANE_NODE_SEPARATION*x, startPos.y, startPos.z);
		tempMembraneNode = tempMembraneNode->nextNode;
	}
}

void CMembrane::drawMembrane(void)
{
	if(membraneNodeHead == NULL)
	{
		cout << EXC << "Error drawing membrane: Cant draw a null head membrane..";
		cout << INS << "This memb has " << startPos.x << ", start position";
		return;
	}

	glBegin(GL_LINE_STRIP);
		for(CMembraneNode *tempMembraneNode = membraneNodeHead; tempMembraneNode != NULL; tempMembraneNode = tempMembraneNode->nextNode)
		{
			glVertex3f(tempMembraneNode->nodePos.x, tempMembraneNode->nodePos.y+10, tempMembraneNode->nodePos.z);
			glPushMatrix();
				glTranslatef(tempMembraneNode->nodePos.x, tempMembraneNode->nodePos.y, tempMembraneNode->nodePos.z);
				DrawCubeNoBottom();
			glPopMatrix();
		}
	glEnd();
}

void CMembrane::destroyMembrane(void)
{
	if(membraneNodeHead == NULL)
		return;

	// 1. Temp == head, 2. head = head->nextNode, 3. delete temp, 1. temp = Head, temp != NULL.
	for(CMembraneNode *tempMembraneNode = membraneNodeHead; tempMembraneNode != NULL; tempMembraneNode = membraneNodeHead)
	{
		membraneNodeHead = membraneNodeHead->nextNode;
		delete tempMembraneNode;
	}
}


#endif /* MEMBRANE_H_ */
