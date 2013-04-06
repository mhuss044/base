/*
 * Builder.h
 *
 *  Created on: Nov 10, 2012
 *      Author: EvorateAwMaDemCriAnR
 *
 *      Process selection data to det where place new material
 *      Consume material to produce an object consisting of components from material (cubes)
 */

#ifndef BUILDER_H_
#define BUILDER_H_

#define SHIP_BLOCK_WIDTH 10

// func which exports structure class to file
// func which imports structure class to StructureBuilder class to cont build

// Declare;
class CBuildingBlock;
class CSelectableBlock;

enum BLOCK_TYPE								// Block types; det block properties
{
	BLOCK_GENERIC = 0,
	BLOCK_SPECIAL
};

class CSelectableBlock
{
public:
	Vert3xf pos;
	CSelectableBlock *nextSelectableBlock;
	CBuildingBlock *hatchingBlock;
	CSelectableBlock(CBuildingBlock *hatching, float x, float y, float z);
	~CSelectableBlock();

	void drawSelectableBlock(void);
};

CSelectableBlock::CSelectableBlock(CBuildingBlock *hatching, float x, float y, float z)
{
	nextSelectableBlock = NULL;
	hatchingBlock = hatching;

	pos.x = x;
	pos.y = y;
	pos.z = z;
}

CSelectableBlock::~CSelectableBlock()
{

}

void CSelectableBlock::drawSelectableBlock(void)
{
	glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		DrawCube();
	glPopMatrix();
}


class CBuildingBlock		// bits that makeup a buildable ship
{
public:
	BLOCK_TYPE blockType;
	Vert3xf pos;
	CBuildingBlock *nextBuildingBlock;
	CBuildingBlock(BLOCK_TYPE type, float x, float y, float z);
	~CBuildingBlock();
	void drawBlock(void);
};

CBuildingBlock::CBuildingBlock(BLOCK_TYPE type, float x, float y, float z)
{
	nextBuildingBlock = NULL;
	blockType = type;
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

CBuildingBlock::~CBuildingBlock(void)
{

}

void CBuildingBlock::drawBlock(void)
{
	switch(blockType)
	{
	case BLOCK_GENERIC:
		glPushMatrix();
			glTranslatef(pos.x, pos.y, pos.z);
			DrawCube();
		glPopMatrix();
		break;
	case BLOCK_SPECIAL:
		break;
	}
}


class CStructureBuilder
{
	// store selection region
	// store material type to lay
public:
	// pointer to structure class to fill
	// func which fills selection region with material

};

class CShipBuilder						// Builds a ship, providing a LL of ship blocks
{										// BuildingBlock = BBlock, SelectableBlock = SBlock
public:
	CBuildingBlock *shipBlocksHead;
	CSelectableBlock *selectableBlocksHead;

	CShipBuilder();
	~CShipBuilder();

	// Operations;
	void deleteShip(void);				// Delete ship LL from memory, all is lost; BBlock/SBlock LLs
	void initNewBuildSession(void);		// Sets initial building params, allowing for start building
	void selectedCallback(CSelectableBlock *selectedBlock);// Process SBlock, ++BBlock, ++new SBlocks
	void addNewBlock(float x, float y, float z);// ++BBlock to LL, also add new SBlocks considering this new add
	void addToSelectableLL(CBuildingBlock *hatching, float x, float y, float z);// Add SBlock to LL @x,y,z; add to head
	void addToSelectableLL(CBuildingBlock *hatching, float hDirX, float hDirY, float hDirz);

	// Render BBlock/SBlock;
	void renderShip(void);				// 'render' instead of draw; render includes colour ops
	void renderSelectableBlocks(void);	//

	// Save ship session;
	void exportShiptoFile(void);		// Generates a .txt w/ ship data, can load it later
	void exportShiptoImage(void);		// visual representation of ship data, can be loaded later
};

CShipBuilder::CShipBuilder(void)
{
	selectableBlocksHead = NULL;
	shipBlocksHead = NULL;

	// Set up initial BBlock/SBlock LLs;
	initNewBuildSession();
}

CShipBuilder::~CShipBuilder(void)
{
	deleteShip();		// All is deleted, BBlock list, SBlock list
}

void CShipBuilder::deleteShip(void)		// All is deleted, BBlock list, SBlock list
{
	if(shipBlocksHead != NULL)
	{
		// Release resources;
		for(CBuildingBlock *tempBlock = shipBlocksHead; tempBlock != NULL; shipBlocksHead = tempBlock)
		{
			// Move up before delete so dont del path;
			tempBlock = tempBlock->nextBuildingBlock;
			// delete block
			delete shipBlocksHead;
		}
		shipBlocksHead = NULL;
	}

	if(selectableBlocksHead != NULL)
	{
		// Release resources;
		for(CSelectableBlock *tempSBlock = selectableBlocksHead; tempSBlock != NULL; selectableBlocksHead = tempSBlock)
		{
			// Move up before delete so dont del path;
			tempSBlock = tempSBlock->nextSelectableBlock;
			// delete block
			delete selectableBlocksHead;
		}
		selectableBlocksHead = NULL;
	}
}

void CShipBuilder::initNewBuildSession(void)
{
	// Ensure starting with blank ship, in order to correctly set start for new
	deleteShip();

	// Initial starting block;
	shipBlocksHead = new CBuildingBlock(BLOCK_GENERIC, 0,0,0);

	// generate corresponding initial selectables;
	addToSelectableLL(shipBlocksHead, shipBlocksHead->pos.x+10, shipBlocksHead->pos.y, shipBlocksHead->pos.z);
	addToSelectableLL(shipBlocksHead, shipBlocksHead->pos.x-10, shipBlocksHead->pos.y, shipBlocksHead->pos.z);
	addToSelectableLL(shipBlocksHead, shipBlocksHead->pos.x, shipBlocksHead->pos.y+10, shipBlocksHead->pos.z);
	addToSelectableLL(shipBlocksHead, shipBlocksHead->pos.x, shipBlocksHead->pos.y-10, shipBlocksHead->pos.z);
	addToSelectableLL(shipBlocksHead, shipBlocksHead->pos.x, shipBlocksHead->pos.y, shipBlocksHead->pos.z+10);
	addToSelectableLL(shipBlocksHead, shipBlocksHead->pos.x, shipBlocksHead->pos.y, shipBlocksHead->pos.z-10);
	// Instead;
	//addToSelectableLL(shipBlocksHead, 0, 0, 0);
}

void CShipBuilder::selectedCallback(CSelectableBlock *selectedBlock)
{
	// Generate selectable list, give it to space selector
	// upon selection add new bblock, new ssblocks according to which selected and
	//		ensure that do not have duplicate selectables in LL

	Vert3xf hatchingDir;
	CBuildingBlock *newBBlock;
	// Selected block has been picked. Process its addition into the BBlock LL;

	// New BBlock gets;
	newBBlock = addNewBlock(selectedBlock->pos.x, selectedBlock->pos.y, selectedBlock->pos.z);
	// Add new selectable blocks;
	hatchingDir.x = selectedBlock->pos.x - selectedBlock->hatchingBlock->pos.x;
	hatchingDir.y = selectedBlock->pos.y - selectedBlock->hatchingBlock->pos.y;
	hatchingDir.z = selectedBlock->pos.z - selectedBlock->hatchingBlock->pos.z;
	addToSelectableLL(newBBlock, hatchingDir.x, hatchingDir.y, hatchingDir.z);
	// Delete selected SBlock from SBlock LL;

	// New BBlock;
	CBuildingBlock *tempBBlock = new CBuildingBlock(BLOCK_GENERIC, x,y,z);
	// Stack atop head;
	tempBBlock->nextBuildingBlock = shipBlocksHead;
	// Assign head to top;
	selectableBlocksHead = tempBBlock;
}

void CShipBuilder::addNewBlock(float x, float y, float z)
{
	// New BBlock;
	CBuildingBlock *tempBBlock = new CBuildingBlock(BLOCK_GENERIC, x,y,z);
	// Stack atop head;
	tempBBlock->nextBuildingBlock = shipBlocksHead;
	// Assign head to top;
	selectableBlocksHead = tempBBlock;
}

void CShipBuilder::addToSelectableLL(CBuildingBlock *hatching, float x, float y, float z)
{
	// New Selectable;
	CSelectableBlock *tempSBlock = new CSelectableBlock(hatching, x, y, z);
	// Stack it atop head;
	tempSBlock->nextSelectableBlock = selectableBlocksHead;
	// Assign head to top;
	selectableBlocksHead = tempSBlock;
}

void CShipBuilder::renderShip(void)
{
	if(shipBlocksHead != NULL)
	{
		// run through, draw;
		for(CBuildingBlock *tempBlock = shipBlocksHead; tempBlock != NULL; tempBlock = tempBlock->nextBuildingBlock)
		{
			glColor3f(0.0,0.0,0.0);
			glColor3f(0.0,1.0,0.0);
			tempBlock->drawBlock();
		}
	}
}

void CShipBuilder::renderSelectableBlocks(void)
{
	if(selectableBlocksHead != NULL)
	{
		// run through, draw;
		for(CSelectableBlock *tempSBlock = selectableBlocksHead; tempSBlock != NULL; tempSBlock = tempSBlock->nextSelectableBlock)
		{
			glColor3f(0.0,0.0,0.0);
			glColor3f(1.0,0.0,0.0);
			tempSBlock->drawSelectableBlock();
		}
	}
}

#endif /* BUILDER_H_ */
