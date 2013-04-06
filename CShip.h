/*
 * CShip.h
 *
 *  Created on: Sep 15, 2012
 *      Author: EvorateAwMaDemCriAnR
 */

#ifndef CSHIP_H_
#define CSHIP_H_

enum EShipMoveDirection
{
	MOVE_UP = 0,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};

class CShip
{
	Vec2d shipPos;
public:
	CShip(int spawnPosX, int spawnPosZ);
	void translateShip(int x, int z);
	void requestMove(EShipMoveDirection DIR);
	void drawShip(void);
};

CShip::CShip(int spawnPosX = GAME_MAP_SIZE_X/2, int spawnPosZ = GAME_MAP_SIZE_Z/2)
{
	shipPos.e1 = spawnPosX;
	shipPos.e1 = spawnPosZ;
}

void CShip::translateShip(int x, int z)
{
	shipPos.e1 += x;
	shipPos.e2 += z;
}

void CShip::requestMove(EShipMoveDirection DIR)
{
	// Make sure ship is in bounds:
	shipPos.e1 > GAME_MAP_SIZE_X ? shipPos.e1 = GAME_MAP_SIZE_X - 10 : (shipPos.e1 < 0 ? shipPos.e1 = 10 : NULL);
	shipPos.e2 > GAME_MAP_SIZE_Z ? shipPos.e2 = GAME_MAP_SIZE_Z - 10 : (shipPos.e2 < 0 ? shipPos.e2 = 10 : NULL);

	switch(DIR)
	{
		case MOVE_UP:	//	Z++
			if((shipPos.e2 + 1) < GAME_MAP_SIZE_Z)
				translateShip(0, 1);
			break;
		case MOVE_DOWN:	// Z--
			if((shipPos.e2 - 1) > 0)
				translateShip(0, -1);
			break;
		case MOVE_RIGHT:	// X++
			if((shipPos.e1 + 1) < GAME_MAP_SIZE_X)
				translateShip(1, 0);
			break;
		case MOVE_LEFT:	//	X--
			if((shipPos.e1 - 1) > 0)
				translateShip(-1, 0);
			break;
	}
}




#endif /* CSHIP_H_ */
