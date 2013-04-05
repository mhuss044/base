/*
 * ArtifactGenerator.h
 *
 *  Created on: Nov 11, 2012
 *      Author: EvorateAwMaDemCriAnR
 *
 *      test tes
 */

#ifndef ARTIFACTGENERATOR_H_
#define ARTIFACTGENERATOR_H_
/*
void oscillatingSponge(void)
{
	static int dir = rand()%4;

	if(rand()%2)
	{
		if(rand()%2)
			rand()%2 ? pos.x += 0.1 : pos.x -= 0.1;
		if(rand()%2)
			rand()%2 ? pos.y += 0.1 : pos.y -= 0.1;
		if(rand()%2)
			rand()%2 ? pos.z += 0.1 : pos.z -= 0.1;
	}
	else
	{
		switch(dir)
		{
		case 0:
			if(rand()%2)
				pos.x += 0.001*pos.x;
			else
				pos.z += 0.0001*pos.z;
			break;
		case 1:
			if(rand()%2)
				pos.x -= 0.001*pos.x;
			else
				pos.z -= 0.0001*pos.z;
			break;
		case 2:
			if(rand()%2)
				pos.x += 0.001*pos.x;
			else
				pos.z -= 0.0001*pos.z;
			break;
		case 3:
			if(rand()%2)
				pos.x -= 0.001*pos.x;
			else
				pos.z += 0.0001*pos.z;
			break;
		}
	}

	if(pos.x > 1000 || pos.x < 0)
	{
		pos.x = rand()%1000;
		dir = rand()%5;
	}
	if(pos.z > 1000 || pos.z < 0)
	{
		pos.z = rand()%1000;
		dir = rand()%5;
	}
}
*/
/*
void spongeMotionInUnison(void)
{
	static int dir = rand()%4;

	if(rand()%2)
	{
		if(rand()%2)
			rand()%2 ? pos.x += 0.1 : pos.x -= 0.1;
		if(rand()%2)
			rand()%2 ? pos.y += 0.1 : pos.y -= 0.1;
		if(rand()%2)
			rand()%2 ? pos.z += 0.1 : pos.z -= 0.1;
	}
	else
	{
		switch(dir)
		{
		case 0:
			if(rand()%2)
				pos.x += 0.001*pos.x;
			else
				pos.z += 0.001*pos.z;
			break;
		case 1:
			if(rand()%2)
				pos.x -= 0.001*pos.x;
			else
				pos.z -= 0.001*pos.z;
			break;
		case 2:
			if(rand()%2)
				pos.x += 0.001*pos.x;
			else
				pos.z -= 0.001*pos.z;
			break;
		case 3:
			if(rand()%2)
				pos.x -= 0.001*pos.x;
			else
				pos.z += 0.001*pos.z;
			break;
		}
	}

	if(pos.x > 1000 || pos.x < 0)
	{
		pos.x = rand()%1000;
		dir = rand()%5;
	}
	if(pos.z > 1000 || pos.z < 0)
	{
		pos.z = rand()%1000;
		dir = rand()%5;
	}
}
*/
class CArtifact
{
public:
	Vert3xf pos, colour;

	CArtifact(float xPos, float zPos);
	~CArtifact();
	void render(void);
	void tickleColour(void);
	void move(void);
};

CArtifact::CArtifact(float xPos, float zPos)
{
	pos.x = xPos;
	pos.z = zPos;

	// Set colour to green;
	colour.x = 0;
	colour.y = 1;
	colour.z = 0;
}

CArtifact::~CArtifact()
{

}

void CArtifact::render(void)
{
	glPushMatrix();
		glTranslatef(pos.x, 0, pos.z);
		DrawCubeNoBottom();
	glPopMatrix();
}

void CArtifact::tickleColour(void)// change colour var randomly
{
	if(rand()%2)
		rand()%2 ? colour.x += 0.1 : colour.x -= 0.1;
	if(rand()%2)
		rand()%2 ? colour.y += 0.1 : colour.y -= 0.1;
	if(rand()%2)
		rand()%2 ? colour.z += 0.1 : colour.z -= 0.1;
}

void CArtifact::move(void)// translate randomly
{
	if(rand()%2)
		rand()%2 ? pos.x += 25 : pos.x -= 25;
	if(rand()%2)
		rand()%2 ? pos.y += 25 : pos.y -= 25;
	if(rand()%2)
		rand()%2 ? pos.z += 25 : pos.z -= 25;

}

class CArtifactNode
{
public:
	CArtifact *artifactPtr;
	CArtifactNode *nextArtiNode;
	CArtifactNode();
	~CArtifactNode();
};

CArtifactNode::CArtifactNode()
{
	artifactPtr = NULL;
	nextArtiNode = NULL;
}

CArtifactNode::~CArtifactNode()
{
	if(artifactPtr != NULL)
		delete artifactPtr;
}

class CArtifactSys
{
public:
	int numArtifacts, spawnPosX, spawnPosZ;
	CArtifactNode *headOfArtifactList;

	CArtifactSys(int numArtis, int spawnX, int spawnZ);
	~CArtifactSys();

	void artifactMotionRoutines(void);
	void artifactRenderRoutines(void);
};

CArtifactSys::CArtifactSys(int numArtis, int spawnX, int spawnZ)
{
	numArtifacts = numArtis;
	spawnPosX = spawnX;
	spawnPosZ = spawnZ;

	headOfArtifactList = NULL;

	CArtifactNode *tempArtiNode;
	headOfArtifactList = new CArtifactNode;


	for(int x = 0; x < numArtifacts; x++)
	{

	}
}

CArtifactSys::~CArtifactSys()
{

	for(CArtifactNode *tempArtiNode = headOfArtifactList; tempArtiNode != NULL; )
	{
		headOfArtifactList = tempArtiNode;
		tempArtiNode = tempArtiNode->nextArtiNode;
		if(headOfArtifactList != NULL)
			delete headOfArtifactList;
	}
}





class CSelectableBox
{
public:
	Vert3xf pos, colour;

	CSelectableBox(void);
	~CSelectableBox();
	void render(void);
	void tickleColour(void);
	void move(void);
};

CSelectableBox::CSelectableBox()
{
	pos.x = rand()%1000;
	pos.z = rand()%1000;

	// Set colour to green;
	colour.x = 0;
	colour.y = 1;
	colour.z = 0;
}

CSelectableBox::~CSelectableBox()
{

}

void CSelectableBox::render(void)
{
	glPushMatrix();
		glTranslatef(pos.x, 0, pos.z);
		DrawCubeNoBottom();
	glPopMatrix();
}

void CSelectableBox::tickleColour(void)// change colour var randomly
{
	if(rand()%2)
		rand()%2 ? colour.x += 0.1 : colour.x -= 0.1;
	if(rand()%2)
		rand()%2 ? colour.y += 0.1 : colour.y -= 0.1;
	if(rand()%2)
		rand()%2 ? colour.z += 0.1 : colour.z -= 0.1;
}

void CSelectableBox::move(void)// translate randomly
{
	static int dir = rand()%4;

	if(rand()%2)
	{
		if(rand()%2)
			rand()%2 ? pos.x += 0.1 : pos.x -= 0.1;
		if(rand()%2)
			rand()%2 ? pos.y += 0.1 : pos.y -= 0.1;
		if(rand()%2)
			rand()%2 ? pos.z += 0.1 : pos.z -= 0.1;
	}
	else
	{
		switch(dir)
		{
		case 0:
			if(rand()%2)
				pos.x += 0.001*pos.x;
			else
				pos.z += 0.001*pos.z;
			break;
		case 1:
			if(rand()%2)
				pos.x -= 0.001*pos.x;
			else
				pos.z -= 0.001*pos.z;
			break;
		case 2:
			if(rand()%2)
				pos.x += 0.001*pos.x;
			else
				pos.z -= 0.001*pos.z;
			break;
		case 3:
			if(rand()%2)
				pos.x -= 0.001*pos.x;
			else
				pos.z += 0.001*pos.z;
			break;
		}
	}

	if(pos.x > 1000 || pos.x < 0)
	{
		pos.x = rand()%1000;
		dir = rand()%5;
	}
	if(pos.z > 1000 || pos.z < 0)
	{
		pos.z = rand()%1000;
		dir = rand()%5;
	}
}

CSelectableBox boxArray[10000];
#endif /* ARTIFACTGENERATOR_H_ */
