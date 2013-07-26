/*
 * SpaceSelector.h
 *
 *  Created on: Nov 9, 2012
 *      Author: EvorateAwMaDemCriAnR
 *
 *
 *      k heres what can be done
 *      	colour code objs and when mouse click, read in x,y mouse colour = gets obj identity
 *      		user doesnt have to be aware of obj colour, bcs can draw to back buffer
 *
 *      	selection mode; draw selectables with name, pickMatrix sets up cursor volume, gets names intersected
 *      	bounding circle of obj on eye coords; so know obj pos, unproject, det mouse intersect
 *
 *      	selection mode seems to be easiest to scale up
 *      		get list of selectables, draw em, selection mode, det which selected
 *
 *		ok the problems;
 *			selection mode; never gives hits, no idea why,
 *			gluProject; i get -ve x,y vals and sometimes huge -ve vals; need window coordinates right, so 0-Winheight/WINwidth wtf
 *
 *
 */
/*
 * gluProject;
 * upon mouse click
 * run through selectables
 * project their vertices, gets bounding rectangle in window coords
 * det mouse and box col
 *
 * disAdv;
 * 	requires selectable list, with vertice data avail
 * 	must proj every vertice
 * 	must construct box
 */

/*
 * Readpixels;
 * 	upon mouse click
 * 	Run through selectables
 * 	draw on 2nd buffer; apparently this is possible
 * 	set each selectable to unique colour
 * 	draw each selectable
 * 	ReadPixels to det selected
 *
 * 	note;
 * 		invalid color vals clamped; use 1.2, 0.1, 1.0, read in 1, not 1.2
 *
 *
 * 		get some sort of rounding; use glColor3f to specify 0.3, 0.5, 0.9; read in 0.29, 0.50, 0.89; seems to always give highest round? so need to round up func to 1dec
 * 								   use glColor3f to specify 0.2, 0.4, 0.8; read in 0.2, 0.4, 0.8 so...
 * 								   use " " 0.25,0.66,0.88 read in: 0.25098 , 0.658824 , 0.878431 so 0.0X needs round correct
 * 			-> mayb different on diff systems? platforms?, ??
 *
 * 		use 1dec place; 0.1 - 1.0 = 1000 selectables; probably enough
 *
 *		after swap back for front buffer; back copied to front
 *			detect mouse click;
 *				write to back buffer colour coded selectables, read from back buffer, det which one picked, clear buffer(colour buffer bit), draw normally
 *
 *		multiple CColourCodedSelectables class for diff selection bits, colours wont conflict bcs each class processes on a clean back buffer
 *			get around the 1000 item limit? yes?
 *			other uses?
 *
 * 	adv;
 * 		since read pix of unique col, cant evver misclick, no additional depth comparison
 * 	disadv;
 * 		draw operations slow? if inc copy/write rectangle; costs more
 * 		will be able to select selectables that are normally behind non selectable items; this is bcs dont completely redraw entire scene, depth test pass
 *
 *
 *
 *		success;
 *			give primitive draw func to CCCSelection, detects mouse selection
 *		future;
 *			menu select
 *			works with 3d world pick on the fly
 *			incoporate VBO ptr efficcient draw
 *			Dont need to store colour vals; instead know how many selectables in list, and gen colour as draw the fly, increment by set amt so when det know which obj
 *				->this results in greater compute time; realistically, wont ever hit limit of colours available
 */


#ifndef SPACESELECTOR_H_
#define SPACESELECTOR_H_

#include <map>						// Needed to store colourXcallback data
#include <CommonTypes.h>			// Needed for RGB
#include <CGUI.h>
#include <gl/gl.h>					// Needed for gl funcs; glColor3f, glReadPixels, etc

using namespace std;				// bcs utilize std::map

void square500(void)
{
	glBegin(GL_QUADS);
		glVertex3i( 0, 0, 1);											// Bottom left
		glVertex3i( 500, 0, 1);											// Bottom right
		glVertex3i( 500, 500, 1);											// Top right
		glVertex3i( 0, 500, 1);											// Top left
	glEnd();
}

void square700(void)
{
	glBegin(GL_QUADS);
		glVertex3i( 700, 700, 0);											// Bottom left
		glVertex3i( 1000, 700, 0);											// Bottom right
		glVertex3i( 1000, 1000, 0);											// Top right
		glVertex3i( 700, 1000, 0);											// Top left
	glEnd();
}

void wireSphere(void)
{
  	glPushMatrix();
		glTranslated(400, 0,  400);
		glScalef(100, 100, 100);
		glutWireSphere(1,10,10);										// 10: slices, 10: stacks
	glPopMatrix();
}

void solidTorus(void)
{
    glPushMatrix();
        glTranslated(500, 0, 0);
		glScalef(100, 100, 100);
		glutSolidTorus(0.2,0.8,10,10);
    glPopMatrix();
}

void wireCone(void)
{
    glPushMatrix();
        glTranslated(1000, 0, 0);
		glScalef(100, 100, 100);
		glutWireCone(1,1,10,10);
    glPopMatrix();
}

enum SELECT_MOVE_DIR
{
	SELECTION_FORWARD = 0,
	SELECTION_BACKWARD,
	SELECTION_LEFT,
	SELECTION_RIGHT
};

// Cant get ptr to member func to work, so mayb just store state to change to (int), GUIsys makes the change instead
// use references?? &GUISys::??
// bool (CGUISystem::*stateSetFunc)(int) = &CGUISystem::setCurrentGUIState(stateToSet); ???????
class selectableData		// rename this to selectableDataGUISys
{
private:
	Vert3xf colourCode;
	void (*callBack)(void);
	int stateToSet;
	int respondsToKey;
public:
	selectableData(float *x, float *y, float *z, int key, void (*callBackToSet)(void), int _stateToSet);
	~selectableData();
	void setCallBack(void (*callBackToSet)(void));
	void (*getCallBack(void))(void);
	int getStateToSet(void);
	char getResponseKey(void);
	Vert3xf *getColourCode(void);
};

selectableData::selectableData(float *x, float *y, float *z, int key, void (*callBackToSet)(void), int _stateToSet)		// No default vals, might result in ambiguos callback/key/stateSet vals??
{
	callBack = callBackToSet;
	stateToSet = _stateToSet;
	respondsToKey = key;

	// Get a new colour code
	*x < 1.0 ? *x += 0.1 : (*y < 1.0 ? *y += 0.1 : (*z < 1.0 ? *z += 0.1 : 1));

	colourCode.x = *x;
	colourCode.y = *y;
	colourCode.z = *z;
}

selectableData::~selectableData()
{
}

void selectableData::setCallBack(void (*callBackToSet)(void) = NULL)
{
	callBack = callBackToSet;
}

void (*selectableData::getCallBack(void))(void)
{
	return callBack;
}

int selectableData::getStateToSet(void)
{
	return stateToSet;
}

char selectableData::getResponseKey(void)
{
	return respondsToKey;
}

Vert3xf *selectableData::getColourCode(void)
{
	return &colourCode;
}

typedef struct _selectableMapData
{
	void (*drawFunc)(void), (*callBack)(void);
	_selectableMapData(void)
	{
		drawFunc = NULL;
		callBack = NULL;
	}
}selectableMapData;

void getPixelFromGLBuffer(float *pixel, int mousePosX, int mousePosY, int windowWidth, int windowHeight, int bufferToRead)// Read frame buffer at mouseX/Y
{
	// Read frame buffer
	float frameBuffVal[1][1][3];
	// Specify source buffer to read from;
	glReadBuffer(bufferToRead);
	// Read pixel data
	glReadPixels(mousePosX, (windowHeight-mousePosY), 1, 1, GL_RGB, GL_FLOAT, frameBuffVal);		// lower left of buffer, size, format, type, array

	pixel[0] = highest01(frameBuffVal[0][0][0]);
	pixel[1] = highest01(frameBuffVal[0][0][1]);
	pixel[2] = highest01(frameBuffVal[0][0][2]);
}

// UNTESTED UNTESTEDUNTESTED UNTESTEDUNTESTED UNTESTEDUNTESTED UNTESTEDUNTESTED UNTESTED
// WAaaaaaay less code
// thing is, map<keyType, valueType> myMap;
//		then myMap[myKey] = myValue;
// 			whats happenning; keyType key = myKey; so cant pass classes, or structs here, only addresses of em
// the question becomes, when map is deleted, does it delete the key*?
//		TEST^^^^^^^^^^^^; MADE A DESTRUCTOR TO CHECK IF !=NULL; DELETE; seems to successfully delete... memory is set to EEEEEEEE... needs further research
class CColourCodedSelection_map									// Colour Coded Selection using <map>
{
private:
	map<Vert3xf*, selectableMapData> selectablesMap;					// Make a map with key; RGB, value; selectableMapData
public:
	Vert3xf lastColour;
	int componentToIncrement;									// either 0, 1, 2 corresponding to x, y, z in lastColour

	CColourCodedSelection_map(void);
	~CColourCodedSelection_map();

	void deleteSelectableID(void *(drawFunc)(void));			// Find this drawFunc, and delete selection for it
	void drawColourCodedSelectables(void);
	void addToSelectablesList(void (*drawFunc)(void), void (*callBack)(void));
	void evaluateNextColour(void);
	void detectSelection2D(int mouseX, int mouseY, int OrthoWidth, int OrthoHeight);		// Upon mouse click; detect which selected, call callback
};

CColourCodedSelection_map::CColourCodedSelection_map(void)
{
	lastColour.x = 0.0;
	lastColour.y = 0.0;
	lastColour.z = 0.0;

	componentToIncrement = 0;
}

CColourCodedSelection_map::~CColourCodedSelection_map(void)
{
	// Run through selectables, draw
	for(map<Vert3xf*, selectableMapData>::iterator iter = selectablesMap.begin(); iter != selectablesMap.end(); ++iter)
	{
		if(iter->first != NULL)
			delete iter->first;
	}
}

void CColourCodedSelection_map::drawColourCodedSelectables(void)
{
	// Run through selectables, draw
	for(map<Vert3xf*, selectableMapData>::iterator iter = selectablesMap.begin(); iter != selectablesMap.end(); ++iter)
	{
		glColor3f(iter->first->x, iter->first->y, iter->first->z);								// Specify colour
		iter->second.drawFunc();															// draw selectable
	}
}

void CColourCodedSelection_map::evaluateNextColour(void)											// change lastColour to next available colour
{
	if(componentToIncrement > 2)
		componentToIncrement = 0;
	switch(componentToIncrement)
	{
	case 0:
		lastColour.x += 0.1;
		break;
	case 1:
		lastColour.y += 0.1;
		break;
	case 2:
		lastColour.z += 0.1;
		break;
	}
	componentToIncrement++;
}

void CColourCodedSelection_map::addToSelectablesList(void (*drawFunc)(void), void (*callBack)(void))// Add node to list
{
	Vert3xf *selectableColour = new Vert3xf;
	selectableMapData data;

	data.callBack = callBack;
	data.drawFunc = drawFunc;

	evaluateNextColour();																			// Get next colour
	selectableColour->x = lastColour.x;
	selectableColour->y = lastColour.y;
	selectableColour->z = lastColour.z;

	selectablesMap[selectableColour] = data;														// Insert selectable
}

void CColourCodedSelection_map::detectSelection2D(int mouseX, int mouseY, int OrthoWidth, int OrthoHeight)
{
	// Draw Selectables; drawing to back buffer; user doesnt see any of this
	// MUST BE IDENTICAL DRAWING SETUP AS IN MAIN LOOP; camera, transforms,
	// must be perfect transforms as where they normally are:
	enterOrthographic(-(OrthoWidth/2), (OrthoWidth/2), -(OrthoHeight/2), (OrthoHeight/2), 0, 0, 0, 0, resizeWINWidth, resizeWINHeight);

	// BLank slate; clear pixel colour info, and depth component of pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					//Clear window to previously defined colour ^^^.
																		//Also specifies what to clear.(COLOR BUFFER)
																		// Have to clear buffer bit bcs dont want previous drawn items to be atop the following drawn items?
	// Draw selectables
	drawColourCodedSelectables();

	// Read frame buffer
	float pixelData[1][1][3];
	// Specify source buffer to read from;
	glReadBuffer(GL_BACK);
	// Read pixel data
	glReadPixels(mouseX, (WINHeight-mouseY), 1, 1, GL_RGB, GL_FLOAT, pixelData);		// lower left of buffer, size, format, type, array
	cout << INS << highest01(pixelData[0][0][0]) << ", " << highest01(pixelData[0][0][1]) << ", " << highest01(pixelData[0][0][2]);

	// Clear back buffer as to allow real scene to be drawn without the previous obstructing it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// detect selected;
	Vert3xf *pixelColour = new Vert3xf;
	pixelColour->x = highest01(pixelData[0][0][0]);
	pixelColour->y = highest01(pixelData[0][0][1]);
	pixelColour->z = highest01(pixelData[0][0][2]);

	// Get selected;
	for(map<Vert3xf*, selectableMapData>::iterator iter = selectablesMap.begin(); iter != selectablesMap.end(); ++iter)
	{
		if(iter->first->x == pixelColour->x)
			if(iter->first->y == pixelColour->y)
				if(iter->first->z == pixelColour->z)
					iter->second.callBack();
	}
}

class CSelectableNode
{
public:
	Vert3xf colour;
	const char *name;
	void (*drawingFunc)(void);		// Func ptr to drawing func of selectable; FUNC MUST; not have colour operations, completely transform vertices to world coords
	CSelectableNode *nextSelectable;
	CSelectableNode(void (*drawFunc)(void), const char *chId, float x, float y, float z);
	~CSelectableNode();
};

CSelectableNode::CSelectableNode(void (*drawFunc)(void), const char *chId, float x, float y, float z)
{
	colour.x = x;
	colour.y = y;
	colour.z = z;

	name = chId;
	drawingFunc = drawFunc;
	nextSelectable = NULL;
}

CSelectableNode::~CSelectableNode()
{

}

// *********************************************************
// **********Process Selection via colour coding selectables
// *********************************************************
/* What happens if delete selectable; need to recycle colour
 * what happens if greater than 1000 selectables: 0.1 colour def
 * selectable callback response?
 *
 */

class CColourCodedSelection
{
public:
	CSelectableNode *selectablesHead;
	Vert3xf lastColour;
	int componentToIncrement;									// either 0, 1, 2 corresponding to x, y, z in lastColour

	CColourCodedSelection(void);
	~CColourCodedSelection();

	void deleteSelectablesList(void);
	void deleteSelectableID(char id);
	void addToRecycleList(void);								// Want a selectable to be removed from the selectable list, have the colour available so can reuse/not fill list
	void callbackForSelectable(char id);
	void drawColourCodedSelectables(void);
	bool addToSelectablesList(void (*drawFunc)(void), const char *name);
	void evaluateNextColour(void);
	void detectSelection3D(int mouseX, int mouseY, Vert3xf *camPos, Vert3xf *viewPt);		// Upon mouse click; detect which selected, call callback
	void detectSelection2D(int mouseX, int mouseY, int OrthoWidth, int OrthoHeight, int windowWidth, int windowHeight);// Upon mouse click; detect which selected, call callback
};

CColourCodedSelection::CColourCodedSelection(void)
{
	selectablesHead = NULL;

	lastColour.x = 0.0;
	lastColour.y = 0.0;
	lastColour.z = 0.0;

	componentToIncrement = 0;
}

CColourCodedSelection::~CColourCodedSelection(void)				// Run through, delete all nodes
{
	for(CSelectableNode *tempNode = selectablesHead; tempNode != NULL; selectablesHead = tempNode)
	{
		tempNode = tempNode->nextSelectable;
		delete selectablesHead;
	}

	selectablesHead = NULL;
}

void CColourCodedSelection::deleteSelectablesList(void)			// Run through, delete all nodes
{
	for(CSelectableNode *tempNode = selectablesHead; tempNode != NULL; selectablesHead = tempNode)
	{
		tempNode = tempNode->nextSelectable;
		delete selectablesHead;
	}

	// list empty
	selectablesHead = NULL;
	// start at lastColour.x
	componentToIncrement = 0;
	// All colours available again
	lastColour.x = 0.0;
	lastColour.y = 0.0;
	lastColour.z = 0.0;
}

void CColourCodedSelection::drawColourCodedSelectables(void)	// Add node to list
{
	// if list is empty;
	if(selectablesHead == NULL)
		return;
	else
	{
		CSelectableNode *tempNode = NULL;
		// Run through, draw
		for(tempNode = selectablesHead; tempNode != NULL;  tempNode = tempNode->nextSelectable)
		{
			glColor3f(0.0,0.0,0.0);		// ensures the following color command goes thru pure?
			glColor3f(tempNode->colour.x, tempNode->colour.y, tempNode->colour.z);
			tempNode->drawingFunc();
		}
	}
}

void CColourCodedSelection::evaluateNextColour(void)											// change lastColour to next available colour
{
	if(componentToIncrement > 2)
		componentToIncrement = 0;
	switch(componentToIncrement)
	{
	case 0:
		lastColour.x += 0.1;
		break;
	case 1:
		lastColour.y += 0.1;
		break;
	case 2:
		lastColour.z += 0.1;
		break;
	}
	componentToIncrement++;
}

bool CColourCodedSelection::addToSelectablesList(void (*drawFunc)(void), const char *name)		// Add node to list
{
	// if list is empty;
	if(selectablesHead == NULL)
	{
		evaluateNextColour();
		selectablesHead = new CSelectableNode(drawFunc, name,lastColour.x, lastColour.y, lastColour.z);
	}
	else
	{
		CSelectableNode *tempNode = NULL;

		// Find the tail;
		for(tempNode = selectablesHead; tempNode->nextSelectable != NULL;  tempNode = tempNode->nextSelectable);

		// Get next colour available
		evaluateNextColour();
		if(tempNode->nextSelectable == NULL)
			tempNode->nextSelectable = new CSelectableNode(drawFunc, name, lastColour.x, lastColour.y, lastColour.z);
	}

	// if list full = false, no more memory? -> false
	return true;
}

void CColourCodedSelection::detectSelection3D(int mouseX, int mouseY, Vert3xf *camPos, Vert3xf *viewPt)
{
	// Draw Selectables; drawing to back buffer; user doesnt see any of this
	// MUST BE IDENTICAL DRAWING SETUP AS IN MAIN LOOP; camera, transforms,
	// must be perfect transforms as where they normally are:
	enterPerspective(0, 0, WINWidth, WINHeight, VIEW_DISTANCE);
	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();													// This is so that previous transformations do not accum
	gluPerspective(45,(float)WINWidth/WINHeight,1,VIEW_DISTANCE);
	glMatrixMode(GL_MODELVIEW);
*/

	// BLank slate; pixel colour info, and depth component of pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					//Clear window to previously defined colour ^^^.
																		//Also specifies what to clear.(COLOR BUFFER)
																		// Have to clear buffer bit bcs dont want previous drawn items to be atop the following drawn items?
	// CAMERA
	gluLookAt(
		camPos->x, camPos->y, camPos->z,
		viewPt->x, viewPt->y, viewPt->z,
		0.0f, 1.0f, 0.0f);

	// Draw selectables
	drawColourCodedSelectables();

	// Read frame buffer
	float pixelData[1][1][3];
	// Specify source buffer to read from;
	glReadBuffer(GL_BACK);
	// Read pixel data
	glReadPixels(mouseX, (WINHeight-mouseY), 1, 1, GL_RGB, GL_FLOAT, pixelData);		// lower left of buffer, size, format, type, array
	cout << INS << highest01(pixelData[0][0][0]) << ", " << highest01(pixelData[0][0][1]) << ", " << highest01(pixelData[0][0][2]);

	// Clear back buffer as to allow real scene to be drawn without the previous obstructing it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// detect selected;
	// if list is empty;
	if(selectablesHead == NULL)
	{
		return;
	}
	else
	{
		// Find the colour which matches;
		for(CSelectableNode *tempNode = selectablesHead; tempNode != NULL;  tempNode = tempNode->nextSelectable)
		{
			// colour will always be whole here, ie 0.1, 0.3, and pixel data is rounded to whole, so can be compared this way
			if(tempNode->colour.x == highest01(pixelData[0][0][0]))
				if(tempNode->colour.y == highest01(pixelData[0][0][1]))
					if(tempNode->colour.z == highest01(pixelData[0][0][2]))
					{
						cout << INS << tempNode->name << ", selected.";
						break;		// Found the correct selectable, so stop traversing the rest of the list
					}
		}
	}
}

void CColourCodedSelection::detectSelection2D(int mouseX, int mouseY, int OrthoWidth, int OrthoHeight, int windowWidth, int windowHeight)
{
	// Draw Selectables; drawing to back buffer; user doesnt see any of this
	// MUST BE IDENTICAL DRAWING SETUP AS IN MAIN LOOP; camera, transforms,
	// must be perfect transforms as where they normally are:
	enterOrthographic(-(OrthoWidth/2), (OrthoWidth/2), -(OrthoHeight/2), (OrthoHeight/2), 0, 0, 0, 0, windowWidth, windowHeight);

	// BLank slate; clear pixel colour info, and depth component of pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					//Clear window to previously defined colour ^^^.
																		//Also specifies what to clear.(COLOR BUFFER)
																		// Have to clear buffer bit bcs dont want previous drawn items to be atop the following drawn items?
	// Draw selectables
	drawColourCodedSelectables();

	// Read frame buffer
	float pixelData[1][1][3];
	// Specify source buffer to read from;
	glReadBuffer(GL_BACK);
	// Read pixel data
	glReadPixels(mouseX, (WINHeight-mouseY), 1, 1, GL_RGB, GL_FLOAT, pixelData);		// lower left of buffer, size, format, type, array
	cout << INS << highest01(pixelData[0][0][0]) << ", " << highest01(pixelData[0][0][1]) << ", " << highest01(pixelData[0][0][2]);

	// Clear back buffer as to allow real scene to be drawn without the previous obstructing it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// detect selected;
	// if list is empty;
	if(selectablesHead == NULL)
	{
		return;
	}
	else
	{
		// Find the colour which matches;
		for(CSelectableNode *tempNode = selectablesHead; tempNode != NULL;  tempNode = tempNode->nextSelectable)
		{
			// colour will always be whole here, ie 0.1, 0.3, and pixel data is rounded to whole, so can be compared this way
			if(tempNode->colour.x == highest01(pixelData[0][0][0]))
				if(tempNode->colour.y == highest01(pixelData[0][0][1]))
					if(tempNode->colour.z == highest01(pixelData[0][0][2]))
					{
						cout << INS << tempNode->name << ", selected.";
						break;		// Found the correct selectable, so stop traversing the rest of the list
					}
		}
	}
}

class CSelection
{
public:
	Vert3xf selectionPos;
	unsigned int *mousePickable;
	GLuint selectionBuffer[1024];
	GLuint numHits, nameSelected;

	CSelection(void);
	~CSelection();
	void moveSelection(SELECT_MOVE_DIR moveDir);
	void renderSelectionBox(void);
	void startPicking(int mousePosX, int mousePosY);
	void stopPicking(void);
	void fillSelectionBuffer(void);
	void getHitName(void); 		// parses selection buffer, returning name selected
	int processMouseSelection(void);
	int processMouseSelScene(int mousePosX, int mousePosY);
};
CSelection::CSelection(void)
{
	selectionPos.x = 0;
	selectionPos.y = 0;
	selectionPos.z = 0;

	mousePickable = NULL;

	numHits = 0;
	nameSelected = 0;
}
CSelection::~CSelection(void)
{
}
void CSelection::moveSelection(SELECT_MOVE_DIR moveDir)
{
	switch(moveDir)
	{
	case SELECTION_FORWARD:
		selectionPos.x += 10;
		break;
	case SELECTION_BACKWARD:
		selectionPos.x -= 10;
		break;
	case SELECTION_LEFT:
		selectionPos.z += 10;
		break;
	case SELECTION_RIGHT:
		selectionPos.z -= 10;
		break;
	}
}
void CSelection::renderSelectionBox(void)
{
    glBegin(GL_LINE_STRIP);
    // Bottom rect:
    	glVertex3f( selectionPos.x, 0, selectionPos.z);                        // Bottom left.
        glVertex3f( selectionPos.x + 10, 0, selectionPos.z);                   // Bottom Right.
        glVertex3f( selectionPos.x + 10, 0, selectionPos.z + 10);	           // Top Right.
        glVertex3f( selectionPos.x, 0, selectionPos.z + 10);                   // Top Left.
        //Connect to first point;
        glVertex3f( selectionPos.x, 0, selectionPos.z);                        // Bottom left.
    // top rect:
        glVertex3f( selectionPos.x, 10, selectionPos.z);                        // Bottom left.
	    glVertex3f( selectionPos.x + 10, 10, selectionPos.z);                   // Bottom Right.
	    glVertex3f( selectionPos.x + 10, 10, selectionPos.z + 10);	           // Top Right.
	    glVertex3f( selectionPos.x, 10, selectionPos.z + 10);                   // Top Left.
	   //Connect to first point;
	   	glVertex3f( selectionPos.x, 10, selectionPos.z);                        // Bottom left.
    glEnd();
}
void CSelection::startPicking(int mousePosX, int mousePosY)
{
	GLint viewport[4];
	float ratio;

	glSelectBuffer(1024,selectionBuffer);
	glRenderMode(GL_SELECT);
	//glInitNames();	// creates an empty name stack, required prior to pushing names
	//glLoadName(0);	// make atleast 1 name on stack

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();	// save current matrix before mult by below pick matrix
	glLoadIdentity();	// init the new mat?

	// define the viewing volume so that rednering is done to onlya area around cursor
	// viewport[3] - to get dist from bottom, bcs origin of viewport is bottom left for OpenGL
	glGetIntegerv(GL_VIEWPORT,viewport);	// gives viewport with origin at top left
	gluPickMatrix(mousePosX,viewport[3]-mousePosY,5,5,viewport);	// x,y, width, height, viewport[4]; x,y; center of picking area, width/heigh; too big = too many hits
	ratio = (viewport[2]+0.0) / viewport[3];
	gluPerspective(45,ratio,1,VIEW_DISTANCE);
	glMatrixMode(GL_MODELVIEW);

	glInitNames();	// creates an empty name stack, required prior to pushing names
	glLoadIdentity();

	numHits = 0;
	nameSelected = 0;
}
void CSelection::stopPicking(void)
{
	glMatrixMode(GL_PROJECTION);	// restore original projection matrix
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	numHits = glRenderMode(GL_RENDER);	// get number of hits, return to normal rendering mode
}
/*
void CSelection::getHitName(void)
{
	if(numHits == 0)
	{
		cout << INS << "Zero hits. return";
		return;
	}

	cout << INS << "Parse selection buffer.";
	cout << INS << "numHits: " << numHits << INS;

	GLuint *bufferPtr = selectionBuffer;
	GLuint name, numNamesInRecord, smallestZ = 0xffffffff, *ptrToClosestNames;

	for(GLuint j = 0; j < numHits; j++)
	{
		numNamesInRecord = *bufferPtr;
		cout << INS << "Names in record: " << numNamesInRecord;
		cout << INS << "Closest Z in this rec: " << *(++bufferPtr);
		cout << INS << "Furthest Z in this rec: " << *(++bufferPtr);

		// Move to first name of the name stack for thsi rec
		while(numNamesInRecord > 0)
		{
			cout << INS << "Name in this record: " << *(++bufferPtr);
			numNamesInRecord--;
		}
		// Pt to next record
		bufferPtr++;
	}

	// reset numHits
	numHits = 0;
}
*/
/*
int CSelection::processMouseSelScene(int mousePosX, int mousePosY)
{
	startPicking(mousePosX, mousePosY);

	// Push name, draw object, popname.
	// If the object just drawn intersects the viewing volume, gets hit record
	// hit rec will then contain this name, plus depth vals

	glPushName(5);
		glBegin(GL_QUADS);
			glVertex3i( 0, 0, 0);											// Bottom left
			glVertex3i( 500, 0, 0);											// Bottom right
			glVertex3i( 500, 500, 0);											// Top right
			glVertex3i( 0, 500, 0);											// Top left
		glEnd();
	glPopName();


    stopPicking();

    getHitName();
}
*/
/*
void selection(void)
{
	GLuint selecBuf[1024], hitsCnt = 0;
	GLint viewport[4];
	float ratio;

	glSelectBuffer(1024,selecBuf);
	glRenderMode(GL_SELECT);
	glInitNames();	// creates an empty name stack, required prior to pushing names
	glLoadName(0);	// make atleast 1 name on stack

	glPushMatrix ();
	// Setup View;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	// init the new mat?

	// define the viewing volume so that rednering is done to onlya area around cursor
	// viewport[3] - to get dist from bottom, bcs origin of viewport is bottom left for OpenGL
	glGetIntegerv(GL_VIEWPORT,viewport);	// gives viewport with origin at top left
	gluPickMatrix((GLdouble)mousePosX,(GLdouble)(viewport[3]-mousePosY),5,5,viewport);	// x,y, width, height, viewport[4]; x,y; center of picking area, width/heigh; too big = too many hits
	ratio = (viewport[2]+0.0) / viewport[3];
	gluPerspective(45,ratio,1,VIEW_DISTANCE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLoadName(5);
		glBegin(GL_QUADS);
			glVertex3i( 0, 0, 0);											// Bottom left
			glVertex3i( 500, 0, 0);											// Bottom right
			glVertex3i( 500, 500, 0);											// Top right
			glVertex3i( 0, 500, 0);											// Top left
		glEnd();

	glLoadName(2);
		glBegin(GL_QUADS);
			glVertex3i( 700, 700, 0);											// Bottom left
			glVertex3i( 1000, 700, 0);											// Bottom right
			glVertex3i( 1000, 1000, 0);											// Top right
			glVertex3i( 700, 1000, 0);											// Top left
		glEnd();

	glFlush(); //flush the pipeline
	hitsCnt = glRenderMode(GL_RENDER); //switch back to RENDER mode

	if(hitsCnt == 0)
	{
		cout << INS << "Zero hits. return";
	}
	else
	{
		cout << INS << "Hit!";
		cin.get();

		cout << INS << "Parse selection buffer.";
		cout << INS << "numHits: " << hitsCnt << INS;

		GLuint *bufferPtr = selecBuf;
		GLuint name, numNamesInRecord, smallestZ = 0xffffffff, *ptrToClosestNames;

		for(GLuint j = 0; j < hitsCnt; j++)
		{
			numNamesInRecord = *bufferPtr;
			cout << INS << "Names in record: " << numNamesInRecord;
			cout << INS << "Closest Z in this rec: " << *(++bufferPtr);
			cout << INS << "Furthest Z in this rec: " << *(++bufferPtr);

			// Move to first name of the name stack for thsi rec
			while(numNamesInRecord > 0)
			{
				cout << INS << "Name in this record: " << *(++bufferPtr);
				numNamesInRecord--;
			}
			// Pt to next record
			bufferPtr++;
		}
	}




//	glMatrixMode(GL_PROJECTION);	// restore original projection matrix
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, WINWidth, WINHeight);
    if(perspective == true)
    	gluPerspective(45,(float)WINWidth/WINHeight,1,VIEW_DISTANCE);
    else
        glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
*/
void det( GLdouble *winVerts)
{
	GLdouble modelViewMat[16], projectionMat[16];
	/*
	GLDouble winVerts[12];*/
	GLdouble objVerts[12] = {
							0.1,0.1,0.1,
							500,0.1,0.1,
							500,500,0.1,
							0.1, 500, 0.1
							};

	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelViewMat);
	//GL_TRANSPOSE_MODELVIEW_MATRIX
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMat);
	glGetIntegerv(GL_VIEWPORT,viewport);

	for(int x = 0; x < 4; x++)
	{
		if(!gluProject(	objVerts[x*3], objVerts[(x*3)+1], objVerts[(x*3)+2], modelViewMat, projectionMat, viewport, &winVerts[(x*3)], &winVerts[(x*3)+1],&winVerts[(x*3)+2]))
			cout << EXC << "gluProject failure";
		else
			cout << EXC << "gluProject success";
/*
		if(gluProject(	10.0,0.0,1.0, modelViewMat, projectionMat, viewport, winVerts, winVerts+1, winVerts+2) == GLU_FALSE )
			cout << EXC << "gluProject failure";
		else
			cout << EXC << "gluProject success";
*/
		//cout << INS << "Object: ";
		//cout << INS <<
	}
}
void detectColour(void)		// read from frontbuf into float arr, draw float arr into backbuf at raster pos
{							// flicker gets if draw directly to front buffer
	static float pixelData[5][5][3];
	// Specify source buffer to read from;
	glReadBuffer(GL_FRONT);
	// Read pixel data
	//		-2 bcs want mouse pointer point to be center of detection area
	//		total area; 5x5
	glReadPixels(mousePosX-2, (WINHeight-mousePosY)-2, 5, 5, GL_RGB, GL_FLOAT, pixelData);		// lower left of buffer, size, format, type, array
	for(int x = 0; x < 5; x++)
	{
		for(int y = 0; y < 5; y++)
		{
			cout << INS;
			for(int z = 0; z < 3; z++)
				cout << " , " << pixelData[x][y][z] << "(" << highest01(pixelData[x][y][z]) << ")";
		}
	}

	// Specify destination;
	glDrawBuffer(GL_BACK);
	// Specify valid insertion pt
	glRasterPos3f(90,90,0);
	// Wont draw if raster pos invalid
	glDrawPixels(5, 5, GL_RGB, GL_FLOAT, pixelData);		// width, height of pixel rect to draw. lower left or rect is at raster pos
}
void readFromFrontDrawToBack(void)		// read from frontbuf into float arr, draw float arr into backbuf at raster pos
{										// flicker gets if draw directly to front buffer
	static float pixelData[500][500][3];
	// Specify source buffer to read from;
	glReadBuffer(GL_FRONT);
	// Read pixel data
	glReadPixels(0, 0, 500, 500, GL_RGB, GL_FLOAT, pixelData);		// lower left of buffer, size, format, type, array
	// Specify destination;
	glDrawBuffer(GL_BACK);
	// Specify valid insertion pt
	glRasterPos3f(0,0,0);
	// Wont draw if raster pos invalid
	glDrawPixels(500, 500, GL_RGB, GL_FLOAT, pixelData);
}
void copyFromFrontWriteToFront(void)		// go to front buffer, bottom left; 0,0; read 500x500, write to 0,0 when go ortho
{
	glRasterPos3f(0,0,0);

	glReadBuffer(GL_FRONT);
	glCopyPixels(0, 0, 500, 500, GL_COLOR);		// Copy from 0-500, paste to raster pos on same frame buffer
}

#endif /* SPACESELECTOR_H_ */
