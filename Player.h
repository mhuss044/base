/*
 * Player.h
 *
 *  Created on: Nov 10, 2012
 *      Author: EvorateAwMaDemCriAnR
 *
 *      Holds properties of player;
 *     		pos, health,
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#define PLAYER_MOVE_SPEED 1		//  Units to translate per move request

enum PLAYER_MOVE_REQUEST
{
	PLAYER_MOVE_REQUEST_FORWARD = 0,
	PLAYER_MOVE_REQUEST_BACKWARD,
	PLAYER_MOVE_REQUEST_LEFT,
	PLAYER_MOVE_REQUEST_RIGHT
};

class CPlayer
{
public:
	Vert3xf playerPos;
	int health;

	CPlayer(float xPos, float yPos);
	~CPlayer();

	void setPlayerPos(float xPos, float zPos);
	void translatePlayer(int x, int z);
	void requestMove(PLAYER_MOVE_REQUEST DIR);
	void drawPlayer(void);
};

CPlayer::CPlayer(float xSpawnPos, float zSpawnPos)
{
	playerPos.x = xSpawnPos;
	playerPos.z = zSpawnPos;
	health = 100;
}

CPlayer::~CPlayer()
{

}

void CPlayer::translatePlayer(int x, int z)
{
	playerPos.x += x;
	playerPos.z += z;
}

void CPlayer::requestMove(PLAYER_MOVE_REQUEST DIR)
{
	// Make sure ship is in bounds:
	/*shipPos.e1 > GAME_MAP_SIZE_X ? shipPos.e1 = GAME_MAP_SIZE_X - 10 : (shipPos.e1 < 0 ? shipPos.e1 = 10 : NULL);
	shipPos.e2 > GAME_MAP_SIZE_Z ? shipPos.e2 = GAME_MAP_SIZE_Z - 10 : (shipPos.e2 < 0 ? shipPos.e2 = 10 : NULL);
	 */
	switch(DIR)
	{
		case PLAYER_MOVE_REQUEST_FORWARD:	//	Z++
			translatePlayer(0, 1);
			break;
		case PLAYER_MOVE_REQUEST_BACKWARD:	// Z--
			translatePlayer(0, -1);
			break;
		case PLAYER_MOVE_REQUEST_RIGHT:	// X++
			translatePlayer(1, 0);
			break;
		case PLAYER_MOVE_REQUEST_LEFT:	//	X--
			translatePlayer(-1, 0);
			break;
	}
}

void CPlayer::drawPlayer(void)
{
	//DrawStencilRect(playerPos.x, playerPos.z, 10, 10);
	glPushMatrix();
		glTranslatef(playerPos.x, 0, playerPos.z);
		DrawCubeNoBottom();
	glPopMatrix();
}

#endif /* PLAYER_H_ */
