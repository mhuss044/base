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

enum EKeyState
{
	Key_UP = false,
	Key_DOWN = true		// Key is pushed
};

class CKeyboard
{
public:
	EKeyState normalKey[256], specialKey[256];

	CKeyboard(void);
	void resetKeys(void);
	void updateNormalKey(int keyNum, EKeyState status);
	void updateSpecialKey(int keyNum, EKeyState status);
	EKeyState queryNormalKey(int keyNum);
	EKeyState querySpecialKey(int keyNum);
};

CKeyboard::CKeyboard(void)
{
	for(int x = 0; x < 256; x++)		// init keys
	{
		normalKey[x] = Key_UP;
		specialKey[x] = Key_UP;
	}
}

void CKeyboard::resetKeys(void)			// GLUT api provides a keyUP callback func to update each key, not needed
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
}

void CKeyboard::updateSpecialKey(int keyNum, EKeyState status)
{
	specialKey[keyNum] = status;
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
