/*
 * CSnake.h
 *
 *  Created on: Feb 3, 2013
 *      Author: EvorateAwMaDemCriAnR
 */

#ifndef CSNAKE_H_
#define CSNAKE_H_

#define SNAKE_MOVE_SPEED 10

enum PICKUP
{
	FOOD = 0,
	POISON,
	UPGRADE
};

class CSnakeNode
{
	Vert3xf pos;
	CSnakeNode *nextSnakeNode;
	CSnakeNode(void);
	~CSnakeNode(void);
};

CSnakeNode::CSnakeNode(void)
{
	nextSnakeNode = NULL;
}

class CSnake
{
public:
	CSnakeNode *snakeHead;

	CSnake(void);
	~CSnake(void);

	void newSnake(void);
	void killSnake(void);
	bool hasPickup(void);		// calls processPickup
	void processPickup(PICKUP type);
	void propagate(void);
	void grow(void);
	void decay(void);
};

CSnake::CSnake(void)
{
	snakeHead = NULL;
}

void CSnake::propagate(void)
{
	// Tail takes dir of head
	for(snakeNode *tempSNode = snakeTail; tempSNode != NULL; tempSNode = tempSNode->prevSNode)
	{
		tempSNode->forwardVec.x = tempSNode->prevSNode->forwardVec.x;
		tempSNode->forwardVec.z = tempSNode->prevSNode->forwardVec.z;
	}

	// Move snake
	for(snakeNode *tempSNode = snakeHead; tempSNode != NULL; tempSNode = tempSNode->nextSNode)
	{
		tempSNode->pos.x += tempSNode->forwardVec.x*;
		tempSNode->pos.z += tempSNode->forwardVec.z;
	}
}

void CSnake::processPickup(PICKUP type)
{
	switch(type)
	{
	case POISON:
		// -- node @ tail
		break;
	case FOOD:
		//++ node to tail
		break;
	case UPGRADE:
		// ++Snake speed
		break;
	}
}

#endif /* CSNAKE_H_ */
