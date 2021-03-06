/*
 * CGUI.h
 *
 *  Created on: Sep 29, 2012
 *      Author: EvorateAwMaDemCriAnR
 *
 *
 *		NEEDS TO BE CHECKED OVER;
 *			- delete list funcs
 *			-
 *			- this doesnt execute on last node;
 *				for(CGUIStateNode *tempNode = guiStateNodeHead; tempNode->nextGUIStateNode != NULL; tempNode = tempNode->nextGUIStateNode)
 *
 *		Gets;
 *			- fully customizable menus; add textures, onHover, collapsable(onMenuSelected)
 *			- transition animations
 *
 *      k want;
 *      	this draws GUI depending on what user wants
 *      	have func to set GUIs; allowing GUI class to cater to all projects
 *      	have func to set gui ptr, so draw
 *
 *		got a esc menu to show up upon key press and disp on keypress, no mouse select yet
 *
 *		shiiiiiiiiiiiittt
 *		this is really runnning away from me here, thought up a way to dynamically add menu states,
 *		and differentiate b/w em
 *
 *		this is going to be so fucking epic when its done, robust as fuck, extensive as fuck
 *		-allow menu states to hold drawing funcs,,,, ie entire screens .....
 *
 *
 *		how to dynamically change menu items?
 *		how dynamically change menus in a menu state?
 *			scope issue here, only main.cpp has access to GUIstate
 *			Global data structure holding modifyable fields,
 *				have a class; 'menu modifyer', reads thsi struc, changes menus accordingly
 *
 *		TRY AND RUN WITH; CLEAR+SWAP+SWAP, AND WITH CLEAR+CLEAR+SWAP
 *
 *		successful
 *			generating menus, add em to GUI states
 *			switching b/w guiStates
 *			menu pos/size dynamic to screen size
 *
 *		Future;
 *			Responds to key checks all menu items for the key reqested; instead State vector of chars, check if the state has that key, yes; go find menu item
 *			finish rest of GUI menu types; column_center, column_header/footer
 *			finish allow set of menu item colours, menu colour
 *			test with std display sizes; 1024x768, 1280x768, 1366x768, 1600x1200,
 *				make sure respond with best looking gui @ each res
 *			get GUIsys to read from .gui file and load GUI
 *			pass bitmap pixel array to CMenu() to use as backdrop
 *			pass bitmap pixel array to CMenuItem to use as backdrop
 *			pass transform values to CMenu to customize default width/height of menu
 *			pass transform values to CMenuItem to customize width/height of menuItems
 *			give CMenu a callback, do selection on Menus; when click menu, callback
 *			get menu items ability to change GUIstates; click 'Options Menu', change state to option menu
 *			give option to have menu backdrop drawn only wide/long enough to fit all menu items in without waste of space
 *
 *			incorporate GUI_MENU_ROW_HEIGHT_FACTOR into menuBackDropFunc, render
 *
 *			"scrollable containers, buttons, text labels, inputs, links"
 *
 *			custom vert menus;
 *				pass VBO, texCoordArr to CMenu; get custom shape menu
 *				pass ", " to CMenuItem; get custom shape menu item
 */

// GUI sys intructions;
/*
 *  Create a GUIsys, create menu state handles
 *  Create GUIstates, add them to GUIsys
 *  Create menus, fill them with menu items; pick their responseKey, their void func callback or state change call back (using GUIstate handles just created)
 *  Add created menus to appropriate GUIstate
 *  -> done making the GUI sys
 *  Input handling;
 *  Go to inputDistributor; add GUISys.queryResponseKey(int key) member under each utilized key
 *  Mouse selection; place processMouseSelection(x,y) at each mouse click
 *  State render Func; place GUIsys.render(); per frame
 *  Changes to window; place .setGUIproperties where ever change window size; resizing/fullscreen
 */

#ifndef CGUI_H_
#define CGUI_H_

#include <list>
#include <MFont.h>					// Need render_string()
#include <Utility.h>				// Needed for highest01()
#include <SpaceSelector.h>			// Needed for Menu mouse selection; getPixelFromGLBuffer

// Used to define the 2D plane on which the GUI is drawn
// Will have to change these values when have strange window shapes, or strange resolutions?
#define GUI_ORTHO_WIDTH 200			// Make this an even number DELETE THIS DELETE THIS DELETE THIS
#define GUI_ORTHO_HEIGHT 200		// DELETE THIS DELETE THIS DELETE THIS

#define GUI_MENU_INSET_LENGTH 10		// Length of the inset to use in the default menu drawing funcs
#define GUI_EDGE_PADDING 10				// Length of distance from boundary; menu boundary/menu item, screen edge/menu
#define GUI_MENU_ITEM_PADDING 30		// Length of distance between items in a menu

#define GUI_MENU_ITEM_DEFAULT_COLOUR_X 0.3
#define GUI_MENU_ITEM_DEFAULT_COLOUR_Y 0.3
#define GUI_MENU_ITEM_DEFAULT_COLOUR_Z 0.3

// Define default positions of menu types;
#define GUI_MENU_ROW_WIDTH_FACTOR 0.1															// 0.16 = 1/6, 0.2 = 1/5 of screen width taken by row menu
#define GUI_MENU_ROW_HEIGHT_FACTOR 0.75
#define GUI_MENU_COLUMN_WIDTH_FACTOR 0.75
#define GUI_MENU_COLUMN_HEIGHT_FACTOR 0.15			 											// 0.1 = Column menu height is 1/10 of max height

#define GUI_MENU_ROW_CENTER_X -(GUI_ORTHO_WIDTH/4)												// Orthogonal plane coordinates, not window coords
#define GUI_MENU_ROW_CENTER_Y (GUI_ORTHO_HEIGHT/4)
#define GUI_MENU_ROW_LEFT_X -(GUI_ORTHO_WIDTH/2) + GUI_EDGE_PADDING								// So just beside edge of screen
#define GUI_MENU_ROW_LEFT_Y (GUI_ORTHO_HEIGHT/2) - GUI_MENU_COLUMN_HEIGHT_FACTOR - GUI_EDGE_PADDING	// bcs need room for header menu
#define GUI_MENU_ROW_RIGHT_X (GUI_ORTHO_WIDTH/2) - GUI_EDGE_PADDING - GUI_MENU_ROW_WIDTH_FACTOR		// So just beside edge of screen
#define GUI_MENU_ROW_RIGHT_Y (GUI_ORTHO_HEIGHT/2) - GUI_MENU_COLUMN_HEIGHT_FACTOR - GUI_EDGE_PADDING	// bcs need room for header menu
#define GUI_MENU_COLUMN_HEADER_X -(GUI_ORTHO_WIDTH/2) + GUI_EDGE_PADDING
#define GUI_MENU_COLUMN_HEADER_Y (GUI_ORTHO_HEIGHT/2) - GUI_EDGE_PADDING
#define GUI_MENU_COLUMN_FOOTER_X -(GUI_ORTHO_WIDTH/2) + GUI_EDGE_PADDING
#define GUI_MENU_COLUMN_FOOTER_Y -(GUI_ORTHO_HEIGHT/2) + GUI_EDGE_PADDING
#define GUI_MENU_SQUARE_X (GUI_ORTHO_WIDTH/4)
#define GUI_MENU_SQUARE_Y (GUI_ORTHO_HEIGHT/4)


enum GUI_STATE_TYPE
{
	GUI_STATE_BLOCKING = 0,			// pauses runtime, scene never drawn, "new game, save game, quit
	GUI_STATE_NON_BLOCKING			// while runtime; aka ontop of scene, aka HUD
};

enum MENU_ITEM_STYLE				// Different styles to specify; menu item styles
{
	MENU_ITEM_RECTANGULAR = 0,		// RECTANGULAR
	MENU_ITEM_OVAL,
	MENU_ITEM_CIRCULAR
};

enum MENU_STYLE						// Menu styles; ie types of menu layouts
{
	MENU_STYLE_ROW_CENTER = 0,		// Center screen; Displays items along y-axis
	MENU_STYLE_ROW_LEFT,			// Left hand side; "
	MENU_STYLE_ROW_RIGHT,			// Right hand side; "
	MENU_STYLE_COLUMN_CENTER,		// Center screen; Displays items along x-axis
	MENU_STYLE_COLUMN_HEADER,		// Header area; "
	MENU_STYLE_COLUMN_FOOTER,		// Footer area; "
	MENU_STYLE_SQUARE_CENTER,		// Center screen; Display items in a square orientation
	MENU_STYLE_SQUARE_MOUSE,		// Try and orient this menus pos relative to mouse pos; context menu
	MENU_STYLE_CUSTOM				// Shape, position, is defined via vbo's
};

// Used to position the a menu on the screen via translating the default menu position
typedef struct _menuTransform
{
	Vert3xf translation;
}menuTransform;

// Background appearance of menu
// These are basic, user can define their own, add via func ptr@CMenu constructor
void defaultRowCenterMenuBackdrop(int screenWidth, int screenHeight)
{
	glColor3f(1.0,1.0,1.0);
	glColor3f(1, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge(-(screenWidth/4), (screenWidth/4), -(screenHeight/2) + GUI_EDGE_PADDING, ((screenHeight/2) - GUI_EDGE_PADDING));
	glColor3f(1.0,1.0,1.0);
	glColor3f(0, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge(-(screenWidth/4) + GUI_MENU_INSET_LENGTH , (screenWidth/4) - GUI_MENU_INSET_LENGTH , -(screenHeight/2) + GUI_EDGE_PADDING + GUI_MENU_INSET_LENGTH, ((screenHeight/2) - GUI_EDGE_PADDING) - GUI_MENU_INSET_LENGTH);
}

void defaultRowLeftMenuBackdrop(int screenWidth, int screenHeight)
{
	glColor3f(1.0,1.0,1.0);
	glColor3f(1, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge(-(screenWidth/2) + GUI_EDGE_PADDING, -(screenWidth/2) + (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR), -(screenHeight/2) + GUI_EDGE_PADDING, ((screenHeight/2) - GUI_EDGE_PADDING));
	glColor3f(1.0,1.0,1.0);
	glColor3f(0, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge(-(screenWidth/2) + GUI_MENU_INSET_LENGTH + GUI_EDGE_PADDING, -(screenWidth/2) + (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR) - GUI_MENU_INSET_LENGTH , -(screenHeight/2) + GUI_EDGE_PADDING + GUI_MENU_INSET_LENGTH, ((screenHeight/2) - GUI_EDGE_PADDING) - GUI_MENU_INSET_LENGTH);
}

void defaultRowRightMenuBackdrop(int screenWidth, int screenHeight)
{
	glColor3f(1.0,1.0,1.0);
	glColor3f(1, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge((screenWidth/2) - (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR) - GUI_EDGE_PADDING, (screenWidth/2) - GUI_EDGE_PADDING, -(screenHeight/2) + GUI_EDGE_PADDING, ((screenHeight/2) - GUI_EDGE_PADDING));
	glColor3f(1.0,1.0,1.0);
	glColor3f(0, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge((screenWidth/2) - (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR) - GUI_EDGE_PADDING + GUI_MENU_INSET_LENGTH, (screenWidth/2) - GUI_MENU_INSET_LENGTH - GUI_EDGE_PADDING, -(screenHeight/2) + GUI_EDGE_PADDING + GUI_MENU_INSET_LENGTH, ((screenHeight/2) - GUI_EDGE_PADDING) - GUI_MENU_INSET_LENGTH);
}

void defaultColumnCenterMenuBackdrop(int screenWidth, int screenHeight)
{
	glColor3f(1.0,1.0,1.0);
	glColor3f(1, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge(((-screenWidth/2) + GUI_EDGE_PADDING*2 + (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR)), ((screenWidth/2) - GUI_EDGE_PADDING*2 - (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR)), (screenHeight/2) - (screenHeight/2)*GUI_MENU_COLUMN_HEIGHT_FACTOR,(screenHeight/2) - GUI_EDGE_PADDING);
	glColor3f(1.0,1.0,1.0);
	glColor3f(0, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge(((-screenWidth/2) + GUI_EDGE_PADDING*2 + (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR)) + GUI_MENU_INSET_LENGTH, ((screenWidth/2) - GUI_EDGE_PADDING*2 - (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR)) - GUI_MENU_INSET_LENGTH, (screenHeight/2) - (screenHeight/2)*GUI_MENU_COLUMN_HEIGHT_FACTOR + GUI_MENU_INSET_LENGTH, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH );
}

void defaultSquareMenuBackdrop(void)
{

}

class CMenuItemNode
{
public:
	const char* menuItemText;
	MENU_ITEM_STYLE style;
	Vert3xf menuItemColour;
	RGB *itemTextureRGB;					// Array of float[3] holding texture to use for menu item
	RGBA *itemTextureRGBA;					// Array of float[4] holding texture to use for menu item

	CMenuItemNode *nextMenuItem;

	CMenuItemNode(MENU_ITEM_STYLE itemStyle, const char* itemText, RGB *rgbPixelArr, RGBA *rgbaPixelArr, float itemColourX, float itemColourY, float itemColourZ);
	~CMenuItemNode();
};

CMenuItemNode::CMenuItemNode(MENU_ITEM_STYLE itemStyle = MENU_ITEM_RECTANGULAR, const char* itemText = NULL, RGB *rgbPixelArr = NULL, RGBA *rgbaPixelArr = NULL, float itemColourX = GUI_MENU_ITEM_DEFAULT_COLOUR_X, float itemColourY = GUI_MENU_ITEM_DEFAULT_COLOUR_Y, float itemColourZ = GUI_MENU_ITEM_DEFAULT_COLOUR_Z)
{
	nextMenuItem = NULL;
	menuItemText = itemText;
	style = itemStyle;

	itemTextureRGB = rgbPixelArr;
	itemTextureRGBA = rgbaPixelArr;
}

CMenuItemNode::~CMenuItemNode()
{
	if(itemTextureRGB != NULL)
		delete itemTextureRGB;
	if(itemTextureRGBA != NULL)
		delete itemTextureRGBA;
}

class CMenu																// Menu which lists items in decending rows
{
private:
	MENU_STYLE menuStyle;
	CMenuItemNode *menuItemsHead;
	int numMenuItems;
public:
	list<selectableData> selectionInfoList;							// list holding menu item colour code and callback
	const char *menuHeadingText;
	RGB *menuTextureRGB;												// Array of float[3] holding texture to use as backdrop for menu
	RGBA *menuTextureRGBA;												// Array of float[4] holding texture to use as backdrop for menu
	CMenu(MENU_STYLE _menuStyle, const char *headingText, RGB *rgbPixelArr, RGBA *rgbaPixelArr);
	~CMenu();
	MENU_STYLE getMenuStyle(void);
	int getNumMenuItems(void);
	CMenuItemNode *getMenuItemHead(void);
	void addMenuItem(MENU_ITEM_STYLE style, const char* text, char responseKey, void (*callback)(void), int stateToSet);
	void deleteMenuItemsList(void);			// Deletes all menu items in list
};

CMenu::CMenu(MENU_STYLE _menuStyle = MENU_STYLE_ROW_CENTER, const char *headingText = NULL, RGB *rgbPixelArr = NULL, RGBA *rgbaPixelArr = NULL)
{
	menuItemsHead = NULL;
	menuStyle = _menuStyle;
	numMenuItems = 0;

	menuHeadingText = headingText;
	menuTextureRGB = rgbPixelArr;
	menuTextureRGBA = rgbaPixelArr;
}

CMenu::~CMenu(void)
{
	deleteMenuItemsList();

	if(menuTextureRGB != NULL)		// texture for menu is defined
		delete menuTextureRGB;
	if(menuTextureRGBA != NULL)		// texture for menu is defined
		delete menuTextureRGBA;
}

MENU_STYLE CMenu::getMenuStyle(void)
{
	return menuStyle;
}

int CMenu::getNumMenuItems(void)
{
	return numMenuItems;
}

CMenuItemNode *CMenu::getMenuItemHead(void)
{
	return menuItemsHead;
}

void CMenu::addMenuItem(MENU_ITEM_STYLE style, const char* text, char responseKey, void (*callback)(void), int stateToSet)		// No default vals for callback/stateToSet bcs make sure user implements taking both paths into consideration
{
	static float lastColour[3] = {0.0, 0.0, 0.0};
	selectableData *newSelectable = new selectableData((lastColour + 0), (lastColour + 1), (lastColour + 2), responseKey, callback, stateToSet);

	// Run through list, delete menu items
	if(menuItemsHead == NULL)
	{
		menuItemsHead = new CMenuItemNode(style, text);
		numMenuItems++;

		// Add created selectable to list:
		selectionInfoList.push_back(*newSelectable);
	}
	else
	{
		CMenuItemNode *tempNode = menuItemsHead;
		// find last menu item;
		for(; tempNode->nextMenuItem != NULL; tempNode = tempNode->nextMenuItem);
		// Add menu item;
		tempNode->nextMenuItem = new CMenuItemNode(style, text);
		numMenuItems++;

		// Add created selectable to list:
		selectionInfoList.push_back(*newSelectable);
	}
}

void CMenu::deleteMenuItemsList(void)
{
	// Run through list, delete menu items
	for(CMenuItemNode *tempNode = menuItemsHead; tempNode != NULL; tempNode = menuItemsHead)
	{
		menuItemsHead = menuItemsHead->nextMenuItem;
		delete tempNode;
	}
}

class CMenuNode
{
public:
	CMenu *menu;
	CMenuNode *nextMenuNode;

	CMenuNode(MENU_STYLE menuStyle, const char* headingText);		// Make a new menu, providing params;CANT USE? BCS ITS A NODE.. SO CANT SPECIFY MENU ITEMS?
	CMenuNode(CMenu *menuToAttach);									// Add existing menu
	~CMenuNode();
};

CMenuNode::CMenuNode(MENU_STYLE menuStyle, const char* headingText)
{
	menu = new CMenu(menuStyle, headingText);
	nextMenuNode = NULL;
}

CMenuNode::CMenuNode(CMenu *menuToAttach)
{
	menu = menuToAttach;
	nextMenuNode = NULL;
}

CMenuNode::~CMenuNode(void)
{
	// Delete menu;
	if(menu != NULL)
		delete menu;
}

class CGUIState								// Defines a screenful of menues
{
private:
//	GUI_STATE_TYPE guiStateType;
	CMenuNode *menuListHead;				// List of menues to display
	void (*scenePtr)(void);					// dont need blocking/non, just check for null. If NON_BLOCKING type, Func holds draw calls that the HUD is to be overlaid upon
public:
	CGUIState(void (*sPtr)(void));
	CGUIState(void);
	~CGUIState();
	void drawScene(void);					// Calls scenePtr; points to draw calls for scene
	CMenuNode *getMenuListHead(void);
	void addMenu(CMenu *menuToAdd);
	void deleteMenuNodeList(void);			// Deletes all menues in list
};

CGUIState::CGUIState(void (*sPtr)(void))
{
	menuListHead = NULL;
	scenePtr = sPtr;
}

CGUIState::CGUIState(void)
{
	menuListHead = NULL;
	scenePtr = NULL;
}

CGUIState::~CGUIState()
{
	deleteMenuNodeList();
}

void CGUIState::drawScene(void)
{
	if(scenePtr != NULL)
		scenePtr();								// Call draw funcs
}

CMenuNode *CGUIState::getMenuListHead(void)
{
	return menuListHead;
}

void CGUIState::addMenu(CMenu *menuToAdd)
{
	CMenuNode *tempNode = NULL;

	if(menuListHead != NULL)
	{
		// Find tail;
		for(tempNode = menuListHead; tempNode->nextMenuNode != NULL; tempNode = tempNode->nextMenuNode);
		tempNode->nextMenuNode = new CMenuNode(menuToAdd);
	}
	else
	{
		menuListHead = new CMenuNode(menuToAdd);
	}
}

void CGUIState::deleteMenuNodeList(void)
{
	if(menuListHead != NULL)
	{
		for(CMenuNode *tempNode = menuListHead; tempNode != NULL; tempNode = menuListHead)
		{
			menuListHead = menuListHead->nextMenuNode;
			delete tempNode;
		}
	}
}

class CGUIStateNode
{
private:
	int GUIStateNodeID;
public:
	CGUIState *GUIState;
	CGUIStateNode *nextGUIStateNode;

	CGUIStateNode(void (*sPtr)(void), int ID);											// Make a new gui state, providing params
	CGUIStateNode(CGUIState *GUIStateToAttach, int ID);									// Add existing gui state
	~CGUIStateNode();
	int getGUIStateNodeID(void);
};

CGUIStateNode::CGUIStateNode(void (*sPtr)(void), int ID)
{
	GUIState = new CGUIState(sPtr);
	nextGUIStateNode = NULL;
	GUIStateNodeID = ID;
}

CGUIStateNode::CGUIStateNode(CGUIState *GUIStateToAttach, int ID)
{
	GUIState = GUIStateToAttach;
	nextGUIStateNode = NULL;
	GUIStateNodeID = ID;
}

CGUIStateNode::~CGUIStateNode(void)
{
	// Delete menu;
	if(GUIState != NULL)
		delete GUIState;
}

int CGUIStateNode::getGUIStateNodeID(void)
{
	return GUIStateNodeID;
}

// GUI System class
//		Swaps gui states to be displayed on the screen
//		Have global, application specific handles to GUI screens(GUI states), define the keypress that setsGUIState(state); app specific
//			This allows applications the responsibility of tracking their GUI states
//			Easy cleanup; since handles are global, CGUISystem add em to its list of GUIs, onClose = delete list of GUIStates = delete asociated menus+menuItems
//		Danger;
//			if define a gui state, pass it to GUIsys to display, but dont addGUIState first; get hanging handle, wont be deleted
//			Put in setCurrentGUIState a check to see if in list?
//
//		Remember;
//			Place call to setGUIProperties(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)); when resize/fullscreen
//
class CGUISystem
{
private:
	CGUIStateNode *guiStateNodeHead, *currentGUIStateNode;
	int currentID, screenWidth, screenHeight, menuScaleFactor, menuItemScaleFactor;
public:
	CGUISystem(int screenWidth, int screenHeight, CGUIStateNode *GUIStateHead);		// Accept list of gui states
	~CGUISystem();
	void setGUIProperties(int winWidth, int winHeight, int _menuScaleFactor, int _menuItemScaleFactor);
	void deleteGUIStateNodes(void);
	//void addGUIStateNode(CGUIStateNode *GUIStateNode);
	int addGUIState(CGUIState *GUIState);
	CGUIStateNode *getCurrentGUIStateNode(void);
	int getCurrentGUIStateNodeID(void);
	bool setCurrentGUIState(CGUIStateNode *currGUINode);
	bool setCurrentGUIState(int ID);
	bool queryResponseKey(int key);												// Find menu item with this key, call callback, return false if key not found
	void callBackFromPixel(float *pixel, CMenuNode *tempMenuNode);
	void render(void);																// Draws scene and GUI
	void processMenuMouseSelection(int mousePosX, int mousePosY);
};

CGUISystem::CGUISystem(int _screenWidth = GUI_ORTHO_WIDTH, int _screenHeight = GUI_ORTHO_HEIGHT, CGUIStateNode *GUIStateHead = NULL)// Screen size should be even numbers
{
	guiStateNodeHead = GUIStateHead;
	currentGUIStateNode = guiStateNodeHead;
	currentID = 0;

	screenWidth = _screenWidth;
	screenHeight = _screenHeight;

	menuScaleFactor = 1;
	menuItemScaleFactor = 1;
}

CGUISystem::~CGUISystem()
{
	deleteGUIStateNodes();
}

void CGUISystem::setGUIProperties(int winWidth, int winHeight, int _menuScaleFactor = 1, int _menuItemScaleFactor = 1)
{
	screenWidth = winWidth;
	screenHeight = winHeight;
	menuScaleFactor = _menuScaleFactor;
	menuItemScaleFactor = _menuItemScaleFactor;
}

void CGUISystem::deleteGUIStateNodes(void)
{
	if(guiStateNodeHead != NULL)
		{
			for(CGUIStateNode *tempNode = guiStateNodeHead; tempNode != NULL; tempNode = guiStateNodeHead)
			{
				guiStateNodeHead = guiStateNodeHead->nextGUIStateNode;
				delete tempNode;
			}
		}
}

int CGUISystem::addGUIState(CGUIState *GUIState)
{
	if(guiStateNodeHead != NULL)											// Add to head
	{
		CGUIStateNode *tempNode = new CGUIStateNode(GUIState, currentID);
		tempNode->nextGUIStateNode = guiStateNodeHead;
		guiStateNodeHead = tempNode;
	}
	else
	{
		guiStateNodeHead = new CGUIStateNode(GUIState, currentID);
		currentGUIStateNode = guiStateNodeHead;								// Set head to currNode
	}

	currentID++;
	return currentID - 1;													// Provide handle to this GUI state
}

CGUIStateNode *CGUISystem::getCurrentGUIStateNode(void)
{
	return currentGUIStateNode;
}

int CGUISystem::getCurrentGUIStateNodeID(void)
{
	return currentGUIStateNode->getGUIStateNodeID();
}

bool CGUISystem::setCurrentGUIState(CGUIStateNode *currGUINode)			// Probably a bad idea..
{
	currentGUIStateNode = currGUINode;
	return true;
}

bool CGUISystem::setCurrentGUIState(int ID)
{
	for(CGUIStateNode *tempNode = guiStateNodeHead; tempNode != NULL; tempNode = tempNode->nextGUIStateNode)
	{
		if(ID == tempNode->getGUIStateNodeID())
		{
			currentGUIStateNode = tempNode;
			return true;
		}
	}
	return false;		// Cant find ID
}

bool CGUISystem::queryResponseKey(int key)
{
	void (*menuItemCallBackPtr)(void) = NULL;

	// Run through all menus of current GUI state: Current GUI state because dont want key selection on hidden states
	for(CMenuNode *tempMenuNode = currentGUIStateNode->GUIState->getMenuListHead(); tempMenuNode != NULL; tempMenuNode = tempMenuNode->nextMenuNode)
	{
		// Find the menu item to which responds to this key: run thru menus selectableInfoList, check keys
		for(list<selectableData>::iterator iter = tempMenuNode->menu->selectionInfoList.begin(); iter != tempMenuNode->menu->selectionInfoList.end(); ++iter)
		{
			// Check key with response key for this menu item;
			if(iter->getResponseKey() == key)
			{
				menuItemCallBackPtr = iter->getCallBack();
				if(menuItemCallBackPtr != NULL)		// Check if void callback is set
					menuItemCallBackPtr();			// Make the call
				else
					setCurrentGUIState(iter->getStateToSet());		// No void callback, its state change callback
			}
		}
	}

	// Didnt find key;
	return false;
}

void CGUISystem::callBackFromPixel(float *pixel, CMenuNode *tempMenuNode)// Check colour codes and callback
{
	Vert3xf *menuItemColourCode = NULL;
	void (*menuItemCallBackPtr)(void) = NULL;

	// Find the call back which was selected: run thru menus selectableInfoList, check colour codes
	for(list<selectableData>::iterator iter = tempMenuNode->menu->selectionInfoList.begin(); iter != tempMenuNode->menu->selectionInfoList.end(); ++iter)
	{
		menuItemColourCode = iter->getColourCode();
		cout << INS << "Found pixel: " << menuItemColourCode->x << ", " << menuItemColourCode->y << ", " << menuItemColourCode->z;
		if(highest01(menuItemColourCode->x) == highest01(pixel[0]))
			if(highest01(menuItemColourCode->y) == highest01(pixel[1]))
				if(highest01(menuItemColourCode->z) == highest01(pixel[2]))
				{
					cout << INS << "Callback found, calling..";
					menuItemCallBackPtr = iter->getCallBack();
					if(menuItemCallBackPtr != NULL)		// Check if void callback is set
						menuItemCallBackPtr();			// Make the call
					else
						setCurrentGUIState(iter->getStateToSet());		// No void callback, maybe its
				}
	}
}

void CGUISystem::render(void)
{
	// BLank slate; pixel colour info, and depth component of pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					//Clear window to previously defined colour ^^^.
																		//Also specifies what to clear.(COLOR BUFFER)
																		// Have to clear buffer bit bcs dont want previous drawn items to be atop the following drawn items?
    // Blank matrix;
	glLoadIdentity();		// Important to have a blank modelview matrix so that previous manipulations do not skew the following manipulations

	if(currentGUIStateNode != NULL)				// Draw GUI
	{
		// Draw Scene: this is first so that GUI is overlaid on top; also bcs probably contains glClear
		currentGUIStateNode->GUIState->drawScene();

		// Draw GUI
		enterOrthographic(-(screenWidth/2), (screenWidth/2), -(screenHeight/2), (screenHeight/2), -(screenHeight/2), (screenHeight/2), 0, 0, screenWidth, screenHeight);

			// Run through list of menus for this GUI state
			for(CMenuNode *tempMenuNode = currentGUIStateNode->GUIState->getMenuListHead(); tempMenuNode != NULL; tempMenuNode = tempMenuNode->nextMenuNode)
			{
				//int spacing = 0, menuItemDynamicSpacing = GUI_MENU_ITEM_PADDING + ((screenHeight - GUI_EDGE_PADDING*2 - GUI_MENU_INSET_LENGTH*2)/tempMenuNode->menu->getNumMenuItems());
				int spacing = 0;
				switch(tempMenuNode->menu->getMenuStyle())			// Get type of menu to display
				{
					case MENU_STYLE_ROW_CENTER:						// Center screen; Displays items along y-axis

						spacing = (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS - GUI_MENU_ITEM_PADDING;// Position the row menu underneath the header menu

						// Check if this menu has a texture defined;
						if(tempMenuNode->menu->menuTextureRGB != NULL)
						{
							// Draw texture onto rectangle menu space;
							// Check if texture coord array != NULL, draw text
							//		else use generic rectangle coord
						}
						else if(tempMenuNode->menu->menuTextureRGBA != NULL)
						{
							// Draw texture onto rectangle menu space;
						}
						else// Default menu back drop; row
						{
							defaultRowCenterMenuBackdrop(screenWidth, screenHeight);				// Dont need to apply transforms, default is center
							// Draw menu Heading;
							glColor3f(0,1,0);										// using glColor3i results in black, always ???
							render_string(-(screenWidth/4) + GUI_EDGE_PADDING + GUI_MENU_INSET_LENGTH, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS, 0, GLUT_BITMAP_HELVETICA_18, tempMenuNode->menu->menuHeadingText);
						}

						// Draw Menu items on top of menu back drop;
						for(CMenuItemNode *tempItem = tempMenuNode->menu->getMenuItemHead(); tempItem != NULL; tempItem = tempItem->nextMenuItem)
						{
							if(tempItem->itemTextureRGB != NULL)		// Item texture defined; draw textured menu item
							{

							}
							else if(tempItem->itemTextureRGBA != NULL)	// Item texture defined; draw textured menu item
							{

							}
							else										// Item texture not defined; draw default menu item
							{
								switch(tempItem->style)
									{
									case MENU_ITEM_RECTANGULAR:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.3, 0.3, 0.3);									// using glColor3i results in black, always ???
										// xleft; get inside insetRectangle, xRight; inside inset + space for text + padding*3; space from edge of inset, space from edge of xleft, space from edge of xRight
										DrawRectanlge(-(screenWidth/4) + GUI_EDGE_PADDING + GUI_MENU_INSET_LENGTH, -(screenWidth/4) + GUI_EDGE_PADDING*3 + GUI_MENU_INSET_LENGTH + getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempItem->menuItemText), spacing - 5, spacing + FONT_HEIGHT_IN_PIXELS);

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(-(screenWidth/4) + GUI_EDGE_PADDING*2 + GUI_MENU_INSET_LENGTH, spacing, 0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									case MENU_ITEM_CIRCULAR:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing,0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									case MENU_ITEM_OVAL:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing,0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									}
							}

							// Make room for next item;
							spacing -= GUI_MENU_ITEM_PADDING;
						}
					break;
					case MENU_STYLE_ROW_LEFT:				// Left hand side; "

						spacing = (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS - GUI_MENU_ITEM_PADDING;// Position the row menu underneath the header menu

						// Check if this menu has a texture defined;
						if(tempMenuNode->menu->menuTextureRGB != NULL)
						{
							// Draw texture onto rectangle menu space;
							// Check if texture coord array != NULL, draw text
							//		else use generic rectangle coord
						}
						else if(tempMenuNode->menu->menuTextureRGBA != NULL)
						{
							// Draw texture onto rectangle menu space;
						}
						else// Default menu back drop; row
						{
							defaultRowLeftMenuBackdrop(screenWidth, screenHeight);				// Dont need to apply transforms, default is center
							// Draw menu Heading;
							glColor3f(0,1,0);										// using glColor3i results in black, always ???
							render_string(-(screenWidth/2) + GUI_EDGE_PADDING*2 + GUI_MENU_INSET_LENGTH, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS, 0, GLUT_BITMAP_HELVETICA_18, tempMenuNode->menu->menuHeadingText);
						}

						// Draw Menu items on top of menu back drop;
						for(CMenuItemNode *tempItem = tempMenuNode->menu->getMenuItemHead(); tempItem != NULL; tempItem = tempItem->nextMenuItem)
						{
							if(tempItem->itemTextureRGB != NULL)		// Item texture defined; draw textured menu item
							{

							}
							else if(tempItem->itemTextureRGBA != NULL)	// Item texture defined; draw textured menu item
							{

							}
							else										// Item texture not defined; draw default menu item
							{
								switch(tempItem->style)
									{
									case MENU_ITEM_RECTANGULAR:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.3, 0.3, 0.3);									// using glColor3i results in black, always ???
										DrawRectanlge(-(screenWidth/2) + GUI_EDGE_PADDING*2 + GUI_MENU_INSET_LENGTH, -(screenWidth/2) + GUI_EDGE_PADDING*4 + GUI_MENU_INSET_LENGTH + getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempItem->menuItemText), spacing - 5, spacing + FONT_HEIGHT_IN_PIXELS);

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(-(screenWidth/2) + GUI_EDGE_PADDING*3 + GUI_MENU_INSET_LENGTH, spacing, 0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									case MENU_ITEM_CIRCULAR:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing,0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									case MENU_ITEM_OVAL:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing,0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									}
							}

							// Make room for next item;
							spacing -= GUI_MENU_ITEM_PADDING;
						}
					break;
					case MENU_STYLE_ROW_RIGHT:				// Right hand side; "

						spacing = (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS - GUI_MENU_ITEM_PADDING;// Position the row menu underneath the header menu

						// Check if this menu has a texture defined;
						if(tempMenuNode->menu->menuTextureRGB != NULL)
						{
							// Draw texture onto rectangle menu space;
							// Check if texture coord array != NULL, draw text
							//		else use generic rectangle coord
						}
						else if(tempMenuNode->menu->menuTextureRGBA != NULL)
						{
							// Draw texture onto rectangle menu space;
						}
						else// Default menu back drop; row
						{
							defaultRowRightMenuBackdrop(screenWidth, screenHeight);				// Dont need to apply transforms, default is center
							// Draw menu Heading;
							glColor3f(0,1,0);										// using glColor3i results in black, always ???
							render_string((screenWidth/2) + GUI_MENU_INSET_LENGTH - (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR), (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS, 0, GLUT_BITMAP_HELVETICA_18, tempMenuNode->menu->menuHeadingText);
						}

						// Draw Menu items on top of menu back drop;
						for(CMenuItemNode *tempItem = tempMenuNode->menu->getMenuItemHead(); tempItem != NULL; tempItem = tempItem->nextMenuItem)
						{
							if(tempItem->itemTextureRGB != NULL)		// Item texture defined; draw textured menu item
							{

							}
							else if(tempItem->itemTextureRGBA != NULL)	// Item texture defined; draw textured menu item
							{

							}
							else										// Item texture not defined; draw default menu item
							{
								switch(tempItem->style)
									{
									case MENU_ITEM_RECTANGULAR:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.3, 0.3, 0.3);									// using glColor3i results in black, always ???
										DrawRectanlge((screenWidth/2) - (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR) - GUI_EDGE_PADDING*2 + GUI_MENU_INSET_LENGTH*2 + GUI_EDGE_PADDING, (screenWidth/2) - (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR) + GUI_MENU_INSET_LENGTH*2 + GUI_EDGE_PADDING*2 + getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempItem->menuItemText), spacing - 5, spacing + FONT_HEIGHT_IN_PIXELS);

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string((screenWidth/2) - (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR) - GUI_EDGE_PADDING*2 + GUI_MENU_INSET_LENGTH*2 + GUI_EDGE_PADDING*2, spacing, 0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									case MENU_ITEM_CIRCULAR:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing,0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									case MENU_ITEM_OVAL:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing,0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									}
							}

							// Make room for next item;
							spacing -= GUI_MENU_ITEM_PADDING;
						}
					break;
					case MENU_STYLE_COLUMN_CENTER:			// Center screen; Displays items along x-axis
					break;
					case MENU_STYLE_COLUMN_HEADER:			// Header area; "

						// For column menus, spacing increments along the x axis
						spacing = ((-screenWidth/2) + GUI_EDGE_PADDING + (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR)) + GUI_EDGE_PADDING + GUI_MENU_INSET_LENGTH + GUI_EDGE_PADDING;// Position the column menu's left x to be beside the left row menu

						// Check if this menu has a texture defined;
						if(tempMenuNode->menu->menuTextureRGB != NULL)
						{
							// Draw texture onto rectangle menu space;
							// Check if texture coord array != NULL, draw text
							//		else use generic rectangle coord
						}
						else if(tempMenuNode->menu->menuTextureRGBA != NULL)
						{
							// Draw texture onto rectangle menu space;
						}
						else// Default menu back drop; row
						{
							defaultColumnCenterMenuBackdrop(screenWidth, screenHeight);				// Dont need to apply transforms, default is center
							// Draw menu Heading;
							glColor3f(0.0,1.0,0.0);													// using glColor3i results in black, always ???
							render_string(spacing, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS, 0, GLUT_BITMAP_HELVETICA_18, tempMenuNode->menu->menuHeadingText);

							// Add the pixels used to draw the heading text, acquiring xleft of button beside menu heading text
							spacing += getStringLength(GLUT_BITMAP_HELVETICA_18, (unsigned char*)tempMenuNode->menu->menuHeadingText) + GUI_EDGE_PADDING;
						}

						// Draw Menu items on top of menu back drop;
						for(CMenuItemNode *tempItem = tempMenuNode->menu->getMenuItemHead(); tempItem != NULL; tempItem = tempItem->nextMenuItem)
						{
							if(tempItem->itemTextureRGB != NULL)		// Item texture defined; draw textured menu item
							{

							}
							else if(tempItem->itemTextureRGBA != NULL)	// Item texture defined; draw textured menu item
							{

							}
							else										// Item texture not defined; draw default menu item
							{
								switch(tempItem->style)
									{
									case MENU_ITEM_RECTANGULAR:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.3, 0.3, 0.3);									// using glColor3i results in black, always ???
										DrawRectanlge(spacing, spacing + getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempItem->menuItemText) + GUI_EDGE_PADDING*2, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - GUI_EDGE_PADDING - FONT_HEIGHT_IN_PIXELS, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - GUI_EDGE_PADDING);

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(spacing + GUI_EDGE_PADDING, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS, 0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									case MENU_ITEM_CIRCULAR:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.3, 0.3, 0.3);									// using glColor3i results in black, always ???
										DrawRectanlge(spacing, spacing + getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempItem->menuItemText) + GUI_EDGE_PADDING*2, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - GUI_EDGE_PADDING - FONT_HEIGHT_IN_PIXELS, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - GUI_EDGE_PADDING);

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(spacing + GUI_EDGE_PADDING, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS, 0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									case MENU_ITEM_OVAL:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.3, 0.3, 0.3);									// using glColor3i results in black, always ???
										DrawRectanlge(spacing, spacing + getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempItem->menuItemText) + GUI_EDGE_PADDING*2, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - GUI_EDGE_PADDING - FONT_HEIGHT_IN_PIXELS, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - GUI_EDGE_PADDING);

										// Draw menu item text;
										glColor3f(0,1,0);										// using glColor3i results in black, always ???
										render_string(spacing + GUI_EDGE_PADDING, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS, 0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
										break;
									}
							}

							// Make room for next item;
							spacing += getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempMenuNode->menu->menuHeadingText) + GUI_EDGE_PADDING + GUI_MENU_ITEM_PADDING;
						}
					break;
					case MENU_STYLE_COLUMN_FOOTER:			// Footer area; "
					break;
					case MENU_STYLE_SQUARE_CENTER:			// Center screen; Display items in a square orientation
					break;
					case MENU_STYLE_SQUARE_MOUSE:			// Try and orient this menus pos relative to mouse pos; context menu
					break;
					case MENU_STYLE_CUSTOM:
					break;
				}
			}
		enterPerspective(0, 0, resizeWINWidth, resizeWINHeight, VIEW_DISTANCE);
	}
	else
	{
		// Draw error GUI
		// Error, current GUI state is not defined
	}

	// Done drawing; put onto screen;
//	glFlush();                               		/*Ensures all commands processed; single buffer  */
	glutSwapBuffers();								// replaces glFlush, this is required in double buffer mode, or else see nothing; require addition of GLUT_DOUBLE in glutInitDisplayMode at init; also results in the back buffer becoming undefined
	glutPostRedisplay();							// Tells glut to call display func next cycle..?; fix flickering? nop
}

void CGUISystem::processMenuMouseSelection(int mousePosX, int mousePosY)
{
	float pixel[3] = {0.0, 0.0, 0.0};
	Vert3xf *menuItemColourCode = NULL;

	// BLank slate; pixel colour info, and depth component of pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);					// Clear window to previously defined colour ^^^.
																		// Also specifies what to clear.(COLOR BUFFER)
																		// Have to clear buffer bit bcs dont want previous drawn items to be atop the following drawn items?
    // Blank matrix;
	glLoadIdentity();		// Important to have a blank modelview matrix so that previous manipulations do not skew the following manipulations

	if(currentGUIStateNode != NULL)				// Draw GUI
	{
		// Draw GUI
		enterOrthographic(-(screenWidth/2), (screenWidth/2), -(screenHeight/2), (screenHeight/2), -(screenHeight/2), (screenHeight/2), 0, 0, screenWidth, screenHeight);

			// Run through list of menus for this GUI state
			for(CMenuNode *tempMenuNode = currentGUIStateNode->GUIState->getMenuListHead(); tempMenuNode != NULL; tempMenuNode = tempMenuNode->nextMenuNode)
			{
				// Get iterator to first menu item's selection info:
				list<selectableData>::iterator iter = tempMenuNode->menu->selectionInfoList.begin();

				//int spacing = 0, menuItemDynamicSpacing = GUI_MENU_ITEM_PADDING + ((screenHeight - GUI_EDGE_PADDING*2 - GUI_MENU_INSET_LENGTH*2)/tempMenuNode->menu->getNumMenuItems());
				int spacing = 0;
				switch(tempMenuNode->menu->getMenuStyle())			// Get type of menu to display
				{
					case MENU_STYLE_ROW_CENTER:						// Center screen; Displays items along y-axis

						spacing = (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS - GUI_MENU_ITEM_PADDING;// Position the row menu underneath the header menu

						// Check if this menu has a texture defined;
						if(tempMenuNode->menu->menuTextureRGB != NULL)
						{
							// Draw texture onto rectangle menu space;
							// Check if texture coord array != NULL, draw text
							//		else use generic rectangle coord
						}
						else if(tempMenuNode->menu->menuTextureRGBA != NULL)
						{
							// Draw texture onto rectangle menu space;
						}
						else// Default menu back drop; row
						{
							// Insert menu backdrop selection here
							//defaultRowCenterMenuBackdrop(screenWidth, screenHeight);				// Dont need to apply transforms, default is center
							// Draw menu Heading;
						}

						// Draw Menu items on top of menu back drop;
						for(CMenuItemNode *tempItem = tempMenuNode->menu->getMenuItemHead(); tempItem != NULL; tempItem = tempItem->nextMenuItem)
						{
							menuItemColourCode = iter->getColourCode();

							// Get colour code;
							if(tempItem->itemTextureRGB != NULL)		// Item texture defined; draw textured menu item
							{

							}
							else if(tempItem->itemTextureRGBA != NULL)	// Item texture defined; draw textured menu item
							{

							}
							else										// Item texture not defined; draw default menu item
							{
								switch(tempItem->style)
									{
									case MENU_ITEM_RECTANGULAR:
										glColor3f(menuItemColourCode->x, menuItemColourCode->y, menuItemColourCode->z);
										// xleft; get inside insetRectangle, xRight; inside inset + space for text + padding*3; space from edge of inset, space from edge of xleft, space from edge of xRight
										DrawRectanlge(-(screenWidth/4) + GUI_EDGE_PADDING + GUI_MENU_INSET_LENGTH, -(screenWidth/4) + GUI_EDGE_PADDING*3 + GUI_MENU_INSET_LENGTH + getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempItem->menuItemText), spacing - 5, spacing + FONT_HEIGHT_IN_PIXELS);
										break;
									case MENU_ITEM_CIRCULAR:
										glColor3f(menuItemColourCode->x, menuItemColourCode->y, menuItemColourCode->z);
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();
										break;
									case MENU_ITEM_OVAL:
										glColor3f(menuItemColourCode->x, menuItemColourCode->y, menuItemColourCode->z);
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();
										break;
									}
							}

							// Make room for next item;
							spacing -= GUI_MENU_ITEM_PADDING;
							iter++;
						}

						// Process Selection:
						getPixelFromGLBuffer(pixel, mousePosX, mousePosY, screenWidth, screenHeight, GL_BACK);

						// Check colour codes, callback:
						callBackFromPixel(pixel, tempMenuNode);
					break;
					case MENU_STYLE_ROW_LEFT:						// Left hand side; "

						spacing = (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS - GUI_MENU_ITEM_PADDING;// Position the row menu underneath the header menu

						// Check if this menu has a texture defined;
						if(tempMenuNode->menu->menuTextureRGB != NULL)
						{
							// Draw texture onto rectangle menu space;
							// Check if texture coord array != NULL, draw text
							//		else use generic rectangle coord
						}
						else if(tempMenuNode->menu->menuTextureRGBA != NULL)
						{
							// Draw texture onto rectangle menu space;
						}
						else// Default menu back drop; row
						{
							defaultRowLeftMenuBackdrop(screenWidth, screenHeight);				// Dont need to apply transforms, default is center
						}

						// Draw Menu items on top of menu back drop;
						for(CMenuItemNode *tempItem = tempMenuNode->menu->getMenuItemHead(); tempItem != NULL; tempItem = tempItem->nextMenuItem)
						{
							// Get colour code;
							menuItemColourCode = iter->getColourCode();

							if(tempItem->itemTextureRGB != NULL)		// Item texture defined; draw textured menu item
							{

							}
							else if(tempItem->itemTextureRGBA != NULL)	// Item texture defined; draw textured menu item
							{

							}
							else										// Item texture not defined; draw default menu item
							{
								switch(tempItem->style)
									{
									case MENU_ITEM_RECTANGULAR:
										glColor3f(menuItemColourCode->x, menuItemColourCode->y, menuItemColourCode->z);
										DrawRectanlge(-(screenWidth/2) + GUI_EDGE_PADDING*2 + GUI_MENU_INSET_LENGTH, -(screenWidth/2) + GUI_EDGE_PADDING*4 + GUI_MENU_INSET_LENGTH + getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempItem->menuItemText), spacing - 5, spacing + FONT_HEIGHT_IN_PIXELS);
										break;
									case MENU_ITEM_CIRCULAR:
										glColor3f(menuItemColourCode->x, menuItemColourCode->y, menuItemColourCode->z);
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();
										break;
									case MENU_ITEM_OVAL:
										glColor3f(menuItemColourCode->x, menuItemColourCode->y, menuItemColourCode->z);
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();
										break;
									}
							}
							// Make room for next item;
							spacing -= GUI_MENU_ITEM_PADDING;
							// Next selection info;
							iter++;
						}

						// Process Selection:
						getPixelFromGLBuffer(pixel, mousePosX, mousePosY, screenWidth, screenHeight, GL_BACK);

						// Check colour codes, callback:
						callBackFromPixel(pixel, tempMenuNode);
					break;
					case MENU_STYLE_ROW_RIGHT:				// Right hand side; "

						spacing = (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - FONT_HEIGHT_IN_PIXELS - GUI_MENU_ITEM_PADDING;// Position the row menu underneath the header menu

						// Check if this menu has a texture defined;
						if(tempMenuNode->menu->menuTextureRGB != NULL)
						{
							// Draw texture onto rectangle menu space;
							// Check if texture coord array != NULL, draw text
							//		else use generic rectangle coord
						}
						else if(tempMenuNode->menu->menuTextureRGBA != NULL)
						{
							// Draw texture onto rectangle menu space;
						}
						else// Default menu back drop; row
						{
							defaultRowRightMenuBackdrop(screenWidth, screenHeight);				// Dont need to apply transforms, default is center
						}

						// Draw Menu items on top of menu back drop;
						for(CMenuItemNode *tempItem = tempMenuNode->menu->getMenuItemHead(); tempItem != NULL; tempItem = tempItem->nextMenuItem)
						{
							// Get colour code;
							menuItemColourCode = iter->getColourCode();

							if(tempItem->itemTextureRGB != NULL)		// Item texture defined; draw textured menu item
							{

							}
							else if(tempItem->itemTextureRGBA != NULL)	// Item texture defined; draw textured menu item
							{

							}
							else										// Item texture not defined; draw default menu item
							{
								switch(tempItem->style)
									{
									case MENU_ITEM_RECTANGULAR:
										glColor3f(menuItemColourCode->x, menuItemColourCode->y, menuItemColourCode->z);
										DrawRectanlge((screenWidth/2) - (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR) - GUI_EDGE_PADDING + GUI_MENU_INSET_LENGTH + GUI_EDGE_PADDING*2, (screenWidth/2) - (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR) + GUI_MENU_INSET_LENGTH*2 + GUI_EDGE_PADDING*2 + getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempItem->menuItemText), spacing - 5, spacing + FONT_HEIGHT_IN_PIXELS);
										break;
									case MENU_ITEM_CIRCULAR:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();
										break;
									case MENU_ITEM_OVAL:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();
										break;
									}
							}

							// Make room for next item;
							spacing -= GUI_MENU_ITEM_PADDING;

							// Next Menu item;
							iter++;
						}
						// Process Selection:
						getPixelFromGLBuffer(pixel, mousePosX, mousePosY, screenWidth, screenHeight, GL_BACK);

						// Check colour codes, callback:
						callBackFromPixel(pixel, tempMenuNode);
					break;
					case MENU_STYLE_COLUMN_CENTER:			// Center screen; Displays items along x-axis
					break;
					case MENU_STYLE_COLUMN_HEADER:			// Header area; "

						// For column menus, spacing increments along the x axis
						spacing = ((-screenWidth/2) + GUI_EDGE_PADDING + (screenWidth*GUI_MENU_ROW_WIDTH_FACTOR)) + GUI_EDGE_PADDING + GUI_MENU_INSET_LENGTH + GUI_EDGE_PADDING;// Position the column menu's left x to be beside the left row menu

						// Add the pixels used to draw the heading text, acquiring xleft of button beside menu heading text
						spacing += getStringLength(GLUT_BITMAP_HELVETICA_18, (unsigned char*)tempMenuNode->menu->menuHeadingText) + GUI_EDGE_PADDING;

						// Check if this menu has a texture defined;
						if(tempMenuNode->menu->menuTextureRGB != NULL)
						{
							// Draw texture onto rectangle menu space;
							// Check if texture coord array != NULL, draw text
							//		else use generic rectangle coord
						}
						else if(tempMenuNode->menu->menuTextureRGBA != NULL)
						{
							// Draw texture onto rectangle menu space;
						}
						else// Default menu back drop; row
						{
							defaultColumnCenterMenuBackdrop(screenWidth, screenHeight);				// Dont need to apply transforms, default is center
						}

						// Draw Menu items on top of menu back drop;
						for(CMenuItemNode *tempItem = tempMenuNode->menu->getMenuItemHead(); tempItem != NULL; tempItem = tempItem->nextMenuItem)
						{
							// Get colour code;
							menuItemColourCode = iter->getColourCode();

							if(tempItem->itemTextureRGB != NULL)		// Item texture defined; draw textured menu item
							{

							}
							else if(tempItem->itemTextureRGBA != NULL)	// Item texture defined; draw textured menu item
							{

							}
							else										// Item texture not defined; draw default menu item
							{
								switch(tempItem->style)
									{
									case MENU_ITEM_RECTANGULAR:
										glColor3f(menuItemColourCode->x, menuItemColourCode->y, menuItemColourCode->z);
										DrawRectanlge(spacing, spacing + getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempItem->menuItemText) + GUI_EDGE_PADDING*2, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - GUI_EDGE_PADDING - FONT_HEIGHT_IN_PIXELS, (screenHeight/2) - GUI_EDGE_PADDING - GUI_MENU_INSET_LENGTH - GUI_EDGE_PADDING);
										break;
									case MENU_ITEM_CIRCULAR:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();
										break;
									case MENU_ITEM_OVAL:
										glColor3f(1.0,1.0,1.0);
										glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
										glPushMatrix();
											glTranslatef(GUI_MENU_ROW_CENTER_X + GUI_EDGE_PADDING, spacing, 0.0);// Move rectangle down w.r.t last rect
											DrawRectanlge(0, 50, 80, 90);
										glPopMatrix();
										break;
									}
							}

							// Make room for next item;
							spacing += getStringLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*)tempMenuNode->menu->menuHeadingText) + GUI_EDGE_PADDING + GUI_MENU_ITEM_PADDING;

							// Next Menu item;
							iter++;
						}
						// Process Selection:
						getPixelFromGLBuffer(pixel, mousePosX, mousePosY, screenWidth, screenHeight, GL_BACK);

						// Check colour codes, callback:
						callBackFromPixel(pixel, tempMenuNode);
					break;
					case MENU_STYLE_COLUMN_FOOTER:			// Footer area; "
					break;
					case MENU_STYLE_SQUARE_CENTER:			// Center screen; Display items in a square orientation
					break;
					case MENU_STYLE_SQUARE_MOUSE:			// Try and orient this menus pos relative to mouse pos; context menu
					break;
					case MENU_STYLE_CUSTOM:					//
					break;
					}
			}
	}
	else
	{
		// Draw error GUI
		// Error, current GUI state is not defined
	}

	// Clear back buffer as to allow real scene to be drawn without the previous obstructing it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

class CFixedGUI					// Activated by keypress, selection via mouse
{
private:
	CMenu *escMenu, *headerMenu, *footerMenu, *leftMenu, *rightMenu, *squareMenu;
	//CMenuNode *escMenuHead, *runtimeMenuHead, *optionMenuHead;			// hold a list of menues to display under a certain GUI state
	//GUI_State currentGUI;
	int GUI_OrthoWidth, GUI_OrthoHeight;
public:
	CFixedGUI(void);
	~CFixedGUI();
	//void setGUI(GUI_State setGUI);
	//GUI_State getGUIstate(void);
	void attachEscMenu(CMenu *escMenu);
	void attachHeaderMenu(CMenu *headerMenu);
	void attachFooterMenu(CMenu *footerMenu);
	void attachLeftMenu(CMenu *leftMenu);
	void attachRightMenu(CMenu *rigtMenu);
	void attachSquareMenu(CMenu *squareMenu);

	void addMenu(void);	// gui state, CMenu
	// change Diolog_out to DYNAMIC? state where menu displayed is dynamically made on the fly based on input
	// press gui, need the rest of the loop to update keyboard/mouse
	// therefore, must change gui state to appropriate one

	// two categories; STATE_BLOCKING, STATE_RUNTIME
	// substates; BLOCKING; "ESC", "SB", "OPT" RUNTIME: "MAP",
	// setState(), setSubState();

	void setStateEscMenu(void);		// These two exist so that instead of setGUI(state), can func ptr these @ menu Items and then can do mouse sel
	void setStateRuntime(void);

	void drawGUI(void);
	void processGUImouseSelection(void);		// determines what state it is, what gui is drawn, which selection to call
};

CFixedGUI::CFixedGUI(void)
{
	// Dimensions of ortho plane to draw on
	GUI_OrthoWidth = GUI_ORTHO_WIDTH;
	GUI_OrthoHeight = GUI_ORTHO_HEIGHT;

	escMenu = NULL;
	headerMenu = NULL;
	footerMenu = NULL;
	leftMenu = NULL;
	rightMenu = NULL;
	squareMenu = NULL;

	//currentGUI = STATE_ESC_MENU;
}

CFixedGUI::~CFixedGUI(void)
{
	// Delete menu items in each menu;
	if(escMenu != NULL)
		escMenu->deleteMenuItemsList();
	if(headerMenu != NULL)
		headerMenu->deleteMenuItemsList();
	if(footerMenu != NULL)
		footerMenu->deleteMenuItemsList();
	if(leftMenu != NULL)
		leftMenu->deleteMenuItemsList();
	if(rightMenu != NULL)
		rightMenu->deleteMenuItemsList();
	if(squareMenu != NULL)
		squareMenu->deleteMenuItemsList();

	// Delete menus
	if(escMenu != NULL)
		delete escMenu;
	if(headerMenu != NULL)
		delete headerMenu;
	if(footerMenu != NULL)
		delete footerMenu;
	if(leftMenu != NULL)
		delete leftMenu;
	if(rightMenu != NULL)
		delete rightMenu;
	if(squareMenu != NULL)
		delete squareMenu;
}
/*
void CFixedGUI::setGUI(GUI_State setGUI)
{
	currentGUI = setGUI;
}

GUI_State CFixedGUI::getGUIstate(void)
{
	return currentGUI;
}
*/
void CFixedGUI::attachEscMenu(CMenu *attachMenu)
{
	if(escMenu != NULL)
		delete escMenu;
	// Assign menu;
	escMenu = attachMenu;
}

void CFixedGUI::attachHeaderMenu(CMenu *attachMenu)
{
	if(headerMenu != NULL)
		delete headerMenu;
	// Assign menu;
	headerMenu = attachMenu;
}

void CFixedGUI::attachFooterMenu(CMenu *attachMenu)
{
	if(footerMenu != NULL)
		delete footerMenu;
	// Assign menu;
	footerMenu = attachMenu;
}

void CFixedGUI::attachLeftMenu(CMenu *attachMenu)
{
	if(leftMenu != NULL)
		delete leftMenu;
	// Assign menu;
	leftMenu = attachMenu;
}

void CFixedGUI::attachRightMenu(CMenu *attachMenu)
{
	if(rightMenu != NULL)
		delete rightMenu;
	// Assign menu;
	rightMenu = attachMenu;
}

void CFixedGUI::attachSquareMenu(CMenu *attachMenu)
{
	if(squareMenu!= NULL)
		delete squareMenu;
	// Assign menu;
	squareMenu = attachMenu;
}

void CFixedGUI::drawGUI(void)			// Return the current GUI state so app knows what is appropriate to draw afterwards
{
	/*
	int spacing = 0;
	switch(currentGUI)
	{
	case STATE_ESC_MENU:
		if(escMenu != NULL)
		{
			// Go 2D:
			enterOrthographic(-(GUI_OrthoWidth/2), (GUI_OrthoWidth/2), -(GUI_OrthoHeight/2), (GUI_OrthoHeight/2), -100.0, 100.0, 0, 0, resizeWINWidth, resizeWINHeight);

			// Determine menu style to draw;
			switch(escMenu->getMenuStyle())
			{
			case MENU_ROW_LIKE:

				// Menu backdrop: row
				defaultRowMenuBackdrop();
				// Draw menu Heading;
				glColor3f(0,1,0);										// using glColor3i results in black, always ???
				render_string(-10,80,0, GLUT_BITMAP_HELVETICA_18, escMenu->menuHeadingText);

				for(CMenuItemNode *tempItem = escMenu->getMenuItemHead(); tempItem != NULL; tempItem = tempItem->nextMenuItem)
				{
					spacing -= 20;
					switch(tempItem->style)
					{
					case MENU_ITEM_RECTANGULAR:
						glColor3f(1.0,1.0,1.0);
						glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
						glPushMatrix();
							glTranslatef(-20.0, spacing, 0.0);						// Move rectangle down w.r.t last rect
							DrawRectanlge(0, 50, 80, 90);
						glPopMatrix();

						// Draw menu item text;
						glColor3f(0,1,0);										// using glColor3i results in black, always ???
						render_string(-10,85+spacing,0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
						break;
					case MENU_ITEM_CIRCULAR:
						glColor3f(1.0,1.0,1.0);
						glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
						glPushMatrix();
							glTranslatef(-20.0, spacing, 0.0);						// Move rectangle down w.r.t last rect
							DrawRectanlge(0, 50, 80, 90);
						glPopMatrix();

						// Draw menu item text;
						glColor3f(0,1,0);										// using glColor3i results in black, always ???
						render_string(-10,85+spacing,0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
						break;
					case MENU_ITEM_OVAL:
						glColor3f(1.0,1.0,1.0);
						glColor3f(0.0, 0.0, 1.0);									// using glColor3i results in black, always ???
						glPushMatrix();
							glTranslatef(-20.0, spacing, 0.0);						// Move rectangle down w.r.t last rect
							DrawRectanlge(0, 50, 80, 90);
						glPopMatrix();

						// Draw menu item text;
						glColor3f(0,1,0);											// using glColor3i results in black, always ???
						render_string(-10,85+spacing,0, GLUT_BITMAP_HELVETICA_12, tempItem->menuItemText);
						break;
					}
				}
				break;
			case MENU_COLUMN_LIKE:
				break;
			case MENU_SQUARE:
				break;
			}

			// Go 3D
			enterPerspective(0, 0, resizeWINWidth, resizeWINHeight, VIEW_DISTANCE);
		}
		break;
	case STATE_RUNTIME:																// aka HUD
		// Go 2D:
		enterOrthographic(-(GUI_OrthoWidth/2), (GUI_OrthoWidth/2), -(GUI_OrthoHeight/2), (GUI_OrthoHeight/2), -100.0, 100.0, 0, 0, resizeWINWidth, resizeWINHeight);
		// Draw fps
		Fps();
		// Go 3D
		enterPerspective(0, 0, resizeWINWidth, resizeWINHeight, VIEW_DISTANCE);
		break;
	}
*/
}

class CHUD						// Selection via mouse
{
private:
public:
};


#endif /* CGUI_H_ */
