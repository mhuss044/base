/*
 * CNet.h
 *
 *  Created on: May 25, 2013
 *      Author: EvorateAwMaDemCriAnR
 *
 *      class which holds a net definition
 *			def;
 *				vertex arr of net pts
 *				fixed net; bends only to maximum flexiblity
 *				dynamic net; add new nodes when pressed
 *
 *		Cloth simulation
 */

#ifndef CNET_H_
#define CNET_H_

class CNet
{
public:
	Vert3xf *vertArr;

	CNet(void);
	~CNet();
};

CNet::CNet(void)
{
	vertArr = NULL;
}

#endif /* CNET_H_ */
