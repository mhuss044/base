/*
 * CScene.h
 *
 *  Created on: Aug 19, 2013
 *      Author: EvorateAwMaDemCriAnR
 *
 *
 * each scene has its own inputrouter, renderer
 * leave one scene, must close it, then open new scene
 *
 *      eliminate this... too abstract..
 */

#ifndef CSCENE_H_
#define CSCENE_H_

class CScene
{
private:
	void (*firstRun)(void), (*execLoop)(void), (*onExit)(void);
public:
};


#endif /* CSCENE_H_ */
