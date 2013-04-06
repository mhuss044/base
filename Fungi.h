/*
 * Fungi.h
 *
 *  Created on: Dec 19, 2012
 *      Author: EvorateAwMaDemCriAnR
 */

#ifndef FUNGI_H_
#define FUNGI_H_


class fungiNode
{
public:
	Vert3xf nodePos;
	fungiNode *nextFungiNode;

	fungiNode(int xPos, int zPos);
	~fungiNode(void);
};

fungiNode::fungiNode(int xPos, int zPos)
{
	nodePos.x = xPos;
	nodePos.z = zPos;
	nextFungiNode = NULL;
}

fungiNode::~fungiNode(void)
{
}

class CFungi
{
public:
	fungiNode *fungiNodeHead;

	CFungi(void);
	~CFungi(void);
	void drawFungi(void);
	void newNode(int xPos, int zPos);
	void Grow(int x, int z);
};

CFungi::CFungi(void)
{
	fungiNodeHead = NULL;
}

CFungi::~CFungi(void)
{
	if(fungiNodeHead != NULL)
		for(fungiNode *tempNode = fungiNodeHead; tempNode != NULL; tempNode = fungiNodeHead)
		{
			fungiNodeHead = tempNode->nextFungiNode;
			if(tempNode != NULL)
				delete tempNode;
		}
}

void CFungi::drawFungi(void)
{
	for(fungiNode *tempNode = fungiNodeHead; tempNode != NULL; tempNode = tempNode->nextFungiNode)
	{
		glPushMatrix();
			glTranslatef(tempNode->nodePos.x, tempNode->nodePos.y, tempNode->nodePos.z);
			DrawCubeNoBottom();
		glPopMatrix();
	}
}

void CFungi::newNode(int xPos, int zPos)
{
	if(fungiNodeHead == NULL)
		fungiNodeHead = new fungiNode(xPos, zPos);
	else
	{
		fungiNode *tempNode = fungiNodeHead;
		for(;tempNode != NULL; tempNode = tempNode->nextFungiNode);
		tempNode = new fungiNode(xPos, zPos);
	}
}

void CFungi::Grow(int x, int z)
{
	newNode(x, z);
}


#endif /* FUNGI_H_ */
