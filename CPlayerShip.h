/*
 * MFont.h
 *
 *  Created on: Aug 20, 2012
 *      Author: hp
 */

#ifndef PLAYERSHIP_H_
#define PLAYERSHIP_H_

#define PLAYER_MOVE_SPEED 1		//  Units to translate per move request

class CPlayerShip
{
	Vec2d shipPos;
public:
	CPlayerShip(int startPosX, int startPosZ);
	void translateShip(int x, int z);
	void requestMove(EShipMoveDirection DIR);
	void drawShip(void);
};

CPlayerShip::CPlayerShip(int startPosX = GAME_MAP_SIZE_X/2, int startPosZ = GAME_MAP_SIZE_Z/2)
{
	shipPos.e1 = startPosX;
	shipPos.e1 = startPosZ;
}

void CPlayerShip::translateShip(int x, int z)
{
	shipPos.e1 += x;
	shipPos.e2 += z;
}

void CPlayerShip::requestMove(EShipMoveDirection DIR)
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


#endif /* PLAYERSHIP_H_ */
