/*
 * CGUI.h
 *
 *  Created on: Sep 29, 2012
 *      Author: EvorateAwMaDemCriAnR
 *
 *
 *		NEEDS TO BE CHECKED OVER;
 *			- delete list funcs
 *			- crash scen
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
 *		callback, listener
 *		how to dynamically change menu items?
 *		how dynamically change menus in a menu state?
 *			scope issue here, only main.cpp has access to GUIstate
 *			Global data structure holding modifyable fields,
 *				have a class; 'menu modifyer', reads thsi struc, changes menus accordingly
 *
 *		TRY AND RUN WITH; CLEAR+SWAP+SWAP, AND WITH CLEAR+CLEAR+SWAP
 *
 *		Future;
 *			get GUIsys to read from .gui file and load GUI
 */

#ifndef CGUI_H_
#define CGUI_H_

// Used to define the 2D plane on which the GUI is drawn
// Will have to change these values when have strange window shapes, or strange resolutions?
#define GUI_ORTHO_WIDTH 200			// Make this an even number
#define GUI_ORTHO_HEIGHT 200
#define GUI_MENU_INSET_LENGTH 10	// Length of the inset to use in the default menu drawing funcs
#define GUI_MENU_ITEM_PADDING 10	// Length of distance from left most boundary of a menu
#define GUI_MENU_ITEM_BETWEEN_PADDING 10// Length of distance between items in a menu

#include <MFont.h>					// Need render_string()

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
	MENU_STYLE_SQUARE_MOUSE			// Try and orient this menus pos relative to mouse pos; context menu
};

// Used to position the a menu on the screen via translating the default menu position
typedef struct _menuTransform
{
	Vert3xf translation;
}menuTransform;

// Background appearance of menu
// These are basic, user can define their own, add via func ptr@CMenu constructor
void defaultRowMenuBackdrop(void)
{
	glColor3f(1.0,1.0,1.0);
	glColor3f(1, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge(-(GUI_ORTHO_WIDTH/4), (GUI_ORTHO_WIDTH/4), -((GUI_ORTHO_HEIGHT/2)-10), ((GUI_ORTHO_HEIGHT/2)-10));
	glColor3f(1.0,1.0,1.0);
	glColor3f(0, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge(-(GUI_ORTHO_WIDTH/4) + GUI_MENU_INSET_LENGTH , (GUI_ORTHO_WIDTH/4) - GUI_MENU_INSET_LENGTH , -((GUI_ORTHO_HEIGHT/2)-10) + GUI_MENU_INSET_LENGTH , ((GUI_ORTHO_HEIGHT/2)-10) - GUI_MENU_INSET_LENGTH );
}

void defaultColumnMenuBackdrop(void)
{
	glColor3f(1.0,1.0,1.0);
	glColor3f(1, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge(-((GUI_ORTHO_WIDTH/2)-10), ((GUI_ORTHO_WIDTH/2)-10), (GUI_ORTHO_HEIGHT/2) - 20, (GUI_ORTHO_HEIGHT/2) - 10);
	glColor3f(1.0,1.0,1.0);
	glColor3f(0, 0.0, 0.0);									// using glColor3i results in black, always ???
	DrawRectanlge(-((GUI_ORTHO_WIDTH/2)-10) + GUI_MENU_INSET_LENGTH , ((GUI_ORTHO_WIDTH/2)-10) - GUI_MENU_INSET_LENGTH , ((GUI_ORTHO_HEIGHT/2) - 30) + GUI_MENU_INSET_LENGTH , ((GUI_ORTHO_HEIGHT/2) - 10) - GUI_MENU_INSET_LENGTH );
}

void defaultSquareMenuBackdrop(void)
{

}

class CMenuItemNode
{
public:
	const char* menuItemText;
	MENU_ITEM_STYLE style;
	void (*callBack)(void);

	CMenuItemNode *nextMenuItem;

	CMenuItemNode(MENU_ITEM_STYLE itemStyle, const char* itemText, void (*callBackFunc)(void));
};

CMenuItemNode::CMenuItemNode(MENU_ITEM_STYLE itemStyle, const char* itemText, void (*callBackFunc)(void))
{
	nextMenuItem = NULL;
	menuItemText = itemText;
	callBack = callBackFunc;
	style = itemStyle;
}

class CMenu								// Menu which lists items in decending rows
{
private:
	MENU_STYLE menuStyle;
	CMenuItemNode *menuItemsHead;
public:
	const char *menuHeadingText;
	CMenu(MENU_STYLE _menuStyle, const char *headingText);
	~CMenu();
	MENU_STYLE getMenuStyle(void);
	CMenuItemNode *getMenuItemHead(void);
	void addMenuItem(MENU_ITEM_STYLE style, const char* text, void (*callback)(void));
	void deleteMenuItemsList(void);			// Deletes all menu items in list
};

CMenu::CMenu(MENU_STYLE _menuStyle, const char *headingText)
{
	menuItemsHead = NULL;
	menuStyle = _menuStyle;
	menuHeadingText = headingText;
}

CMenu::~CMenu(void)
{
	deleteMenuItemsList();
}

MENU_STYLE CMenu::getMenuStyle(void)
{
	return menuStyle;
}

CMenuItemNode *CMenu::getMenuItemHead(void)
{
	return menuItemsHead;
}

void CMenu::addMenuItem(MENU_ITEM_STYLE style, const char* text, void (*callback)(void))
{
	// Run through list, delete menu items
	if(menuItemsHead == NULL)
		menuItemsHead = new CMenuItemNode(style, text, callback);
	else
	{
		CMenuItemNode *tempNode = menuItemsHead;
		// find last menu item;
		for(; tempNode->nextMenuItem != NULL; tempNode = tempNode->nextMenuItem);
		// Add menu item;
		tempNode->nextMenuItem = new CMenuItemNode(style, text, callback);
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
	GUIStateNodeID = ID;
}

CGUIStateNode::CGUIStateNode(CGUIState *GUIStateToAttach, int ID)
{
	GUIState = GUIStateToAttach;
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
//		Danger; if define a gui state, pass it to GUIsys to display, but dont addGUIState first; get hanging handle, wont be deleted
//			Put in setCurrentGUIState a check to see if in list?
class CGUISystem
{
private:
	CGUIStateNode *guiStateNodeHead, *currentGUIStateNode;
	int currentID, GUI_OrthoWidth, GUI_OrthoHeight;
public:
	CGUISystem(CGUIStateNode *GUIStateHead);		// Accept list of gui states
	CGUISystem(void);
	~CGUISystem();
	void deleteGUIStateNodes(void);
	//void addGUIStateNode(CGUIStateNode *GUIStateNode);
	int addGUIState(CGUIState *GUIState);
	CGUIStateNode *getCurrentGUIStateNode(void);
	int getCurrentGUIStateNodeID(void);
	bool setCurrentGUIState(CGUIStateNode *currGUINode);
	bool setCurrentGUIState(int ID);
	void render(void);								// Draws scene and GUI
};

CGUISystem::CGUISystem(CGUIStateNode *GUIStateHead)
{
	guiStateNodeHead = GUIStateHead;
	currentGUIstate = guiStateNodeHead;
	currentID = 0;
}

CGUISystem::CGUISystem(void)
{
	guiStateNodeHead = NULL;
	currentGUIstate = guiStateNodeHead;
	currentID = 0;
}

CGUISystem::~CGUISystem()
{
	deleteGUIStateNodes(void);
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
	CGUIStateNode *tempNode = NULL;

	if(guiStateNodeHead != NULL)
	{
		// Find tail;
		for(tempNode = guiStateNodeHead; tempNode->nextGUIStateNode != NULL; tempNode = tempNode->nextGUIStateNode);
		tempNode->nextGUIStateNode = new CGUIStateNode(GUIState, currentID);
		currentID++;
	}
	else
	{
		guiStateNodeHead = new CGUIStateNode(GUIState, currentID);
		currentGUIStateNode = guiStateNodeHead;		// Set head to currNode
		currentID++;
	}

	return currentID - 1;		// Provide handle to this GUI state
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
	for(CGUIStateNode *tempNode = guiStateNodeHead; tempNode->nextGUIStateNode != NULL; tempNode = tempNode->nextGUIStateNode)
	{
		if(ID == tempNode->getGUIStateNodeID())
		{
			currentGUIStateNode = tempNode;
			return true;
		}
	}
	return false;		// Cant find ID
}

CGUISystem::render(void)
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
		enterOrthographic(-100, 100, -100, 100, -100, 100, 0, 0, resizeWINWidth, resizeWINHeight);

		enterPerspective(0, 0, resizeWINWidth, resizeWINHeight, VIEW_DISTANCE);
	}
	else
	{
		// Draw error GUI
		// Error, current GUI state is not defined
	}

	// Done drawing; put onto screen;
//	glFlush();                               		/*Ensures all commands processed; single buffer  */
	glutSwapBuffers();								// this is required in double buffer mode, or else see nothing; require addition of GLUT_DOUBLE in glutInitDisplayMode at init
	glutPostRedisplay();							// fix flickering? nop
}

// handles gui drawing/input handling
class CGUI
{
private:
	//CColourCodedSelection GUIselection;

	GUI_State currentGUI;
	void (*HUD_ESCMenuFuncPtr)(void);
	void (*HUD_RUNTIMEDIALOGUE)(void);
	void (*HUD_RUNTIME)(void);
public:
	CGUI(void);
	void setESCMenu(void (*ESCMENUFUNC)(void));
	void setRUNTIMEMenu(void (*RUNTIMEMENUFUNC)(void));
	void setDIALOGUEMenu(void (*DIALOGUEMENUFUNC)(void));

	void creatESCMenu();
	void setGUI(GUI_State guistate);		// Upon user input, call this, set appropriate gui to draw
	void drawGUI(void);						// draw the gui, and handle any selection
};

CGUI::CGUI(void)
{
	currentGUI = STATE_ESC_MENU;

	HUD_ESCMenuFuncPtr = NULL;
	HUD_RUNTIMEDIALOGUE = NULL;
	HUD_RUNTIME = NULL;
}

void CGUI::setESCMenu(void (*ESCMENUFUNC)(void))
{
	//HUD_ESCMenuFuncPtr = ESCMENUFUNC;
}

void CGUI::setRUNTIMEMenu(void (*RUNTIMEMENUFUNC)(void))
{
	HUD_RUNTIMEDIALOGUE = RUNTIMEMENUFUNC;
}

void CGUI::setDIALOGUEMenu(void (*DIALOGUEMENUFUNC)(void))
{
	HUD_RUNTIME = DIALOGUEMENUFUNC;
}

void CGUI::setGUI(GUI_State guistate)
{
	currentGUI = guistate;
}

void CGUI::drawGUI(void)
{
	switch(currentGUI)
	{
	case STATE_ESC_MENU:
		// NEW GAME, QUIT
		if(HUD_ESCMenuFuncPtr != NULL)
			(*HUD_ESCMenuFuncPtr)();
		break;
	case STATE_RUNTIME:
		// THIS IS A HUD, DISLPAY WHERE YOU AT IN THE MAP
		if(HUD_RUNTIMEDIALOGUE != NULL)
			(*HUD_RUNTIMEDIALOGUE)();
		break;
	case DIALOG_OUT:
		if(HUD_RUNTIME != NULL)
			(*HUD_RUNTIME)();
		break;
	}
}

class CFixedGUI					// Activated by keypress, selection via mouse
{
private:
	CMenu *escMenu, *headerMenu, *footerMenu, *leftMenu, *rightMenu, *squareMenu;
	//CMenuNode *escMenuHead, *runtimeMenuHead, *optionMenuHead;			// hold a list of menues to display under a certain GUI state
	GUI_State currentGUI;
	int GUI_OrthoWidth, GUI_OrthoHeight;
public:
	CFixedGUI(void);
	~CFixedGUI();
	void setGUI(GUI_State setGUI);
	GUI_State getGUIstate(void);
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

	currentGUI = STATE_ESC_MENU;
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

void CFixedGUI::setGUI(GUI_State setGUI)
{
	currentGUI = setGUI;
}

GUI_State CFixedGUI::getGUIstate(void)
{
	return currentGUI;
}

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
}

void CFixedGUI::setStateEscMenu(void)
{
	currentGUI = STATE_ESC_MENU;

}

void CFixedGUI::setStateRuntime(void)
{
	currentGUI = STATE_RUNTIME;

}

void CFixedGUI::processGUIselection(void)
{

}

class CHUD						// Selection via mouse
{
private:
public:
};


#endif /* CGUI_H_ */
