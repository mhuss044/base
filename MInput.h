/*
 * MInput.h
 *
 *  Created on: Aug 22, 2012
 *      Author: hp
 *      //http://www.swiftless.com/tutorials/opengl/keyboard.html
 *
 */

#ifndef MINPUT_H_
#define MINPUT_H_

typedef struct _mouseEvent
{
	bool mouseLeftDown, mouseRightDown, mouseMidDown;
	int windowPosX, windowPosY;

	_mouseEvent(void)
	{
		mouseLeftDown = false;
		mouseRightDown = false;
		mouseMidDown = false;
		windowPosX = 0;
		windowPosY = 0;
	};

	void reset(void)
	{
		mouseLeftDown = false;
		mouseRightDown = false;
		mouseMidDown = false;
		windowPosX = 0;
		windowPosY = 0;
	}
}mouseEvent;

enum EKeyState
{
	Key_UP = false,
	Key_DOWN = true		// Key is pushed
};

class CKeyboard
{
private:
	EKeyState normalKey[256], specialKey[256];
	bool normalAlreadyDown[256], specialAlreadyDown[256];			// Seems this is necessary, GLUT key repeat ignore funcs cant get em to work..
public:
	CKeyboard(void);
	void resetKeys(void);
	void updateNormalKey(int keyNum, EKeyState status);
	void updateSpecialKey(int keyNum, EKeyState status);
	void normalSetKeyAlreadyDown(int keyNum);
	void specialSetKeyAlreadyDown(int keyNum);
	bool normalKeyAlreadyDown(int keyNum);
	bool specialKeyAlreadyDown(int keyNum);
	EKeyState queryNormalKey(int keyNum);
	EKeyState querySpecialKey(int keyNum);
};

CKeyboard::CKeyboard(void)
{
	for(int x = 0; x < 256; x++)		// init keys
	{
		normalKey[x] = Key_UP;
		specialKey[x] = Key_UP;

		specialAlreadyDown[x] = false;	// Key is not down
		normalAlreadyDown[x] = false;	// Key is not down
	}
}

void CKeyboard::resetKeys(void)								// GLUT api provides a keyUP callback func to update each key, not needed
{
	for(int x = 0; x < 256; x++)		// init keys
	{
		normalKey[x] = Key_UP;
		specialKey[x] = Key_UP;
	}
}

void CKeyboard::updateNormalKey(int keyNum, EKeyState status)
{
	normalKey[keyNum] = status;
	if(status == Key_UP)									// Set to not down
		normalAlreadyDown[keyNum] = false;
}

void CKeyboard::updateSpecialKey(int keyNum, EKeyState status)
{
	specialKey[keyNum] = status;
	if(status == Key_UP)
		specialAlreadyDown[keyNum] = false;
}

void CKeyboard::normalSetKeyAlreadyDown(int keyNum)			// Set down state
{
	normalAlreadyDown[keyNum] = true;
}

void CKeyboard::specialSetKeyAlreadyDown(int keyNum)
{
	specialAlreadyDown[keyNum] = true;
}

bool CKeyboard::normalKeyAlreadyDown(int keyNum)
{
	return normalAlreadyDown[keyNum];
}

bool CKeyboard::specialKeyAlreadyDown(int keyNum)
{
	return specialAlreadyDown[keyNum];
}

EKeyState CKeyboard::queryNormalKey(int keyNum)
{
	return normalKey[keyNum];
}

EKeyState CKeyboard::querySpecialKey(int keyNum)
{
	return specialKey[keyNum];
}

#endif /* MINPUT_H_ */
