/*
 * CGUI.h
 *
 *  Created on: Sep 29, 2012
 *      Author: EvorateAwMaDemCriAnR
 */

#ifndef CGUI_H_
#define CGUI_H_

enum GUI_State
{
	START_MENU = 0,					// pauses runtime, "new game, save game, quit
	HUD,
	DIALOG_OUT,						// doesnt pause run time, display dialogue overlay
};

// handles gui drawing/input handling
class CGUI
{
private:
	GUI_State currentGUI;
public:
	CGUI(void);
	void drawGUI(void);
	void guiEvents(void);
};

CGUI::CGUI(void)
{
	currentGUI = START_MENU;
}

void CGUI::drawGUI(void)
{
	switch(currentGUI)
	{
	case START_MENU:
		// NEW GAME, QUIT
		break;
	case HUD:
		// THIS IS A HUD, DISLPAY WHERE YOU AT IN THE MAP
		break;
	case DIALOG_OUT:
		// TEXT TOP LEFT
		break;
	}
}

#endif /* CGUI_H_ */
