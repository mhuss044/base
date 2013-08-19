/*
 * CRender.h
 *
 *  Created on: Aug 16, 2013
 *      Author: EvorateAwMaDemCriAnR
 *
 *
 *      Handle maintenance of vertex data
 *      Handle render of that data
 *      Want an a vert list to be rendered? add it to this class
 *
 */

#ifndef CRENDER_H_
#define CRENDER_H_

#include <vector>

class CVertexDataDB
{
private:
	vector<int> objHandles;		// list of IDs
	vector<float*> vertexDatabase;		// list of float* so store vertex arrs for each obj
public:
	// float* getVertexList(int objHandle);
	// void makeBufferObjects(void);
};

class CRender
{
private:
	CVertexDataDB vDatabase;
	vector<int> sceneHandles;		// Holds defined scenes; Specify which scene want to be displayed
	vector<int*> sceneContents;		// Holds which objIDs to display for each particular scene

public:
	CRender(void);
	~CRender();
	void init(void);
	// void setScene(int sceneHandle);
	void renderScene(void);
};

CRender::CRender(void)
{

//	glvatt = glutGetProcAddress("glGenBuffers");
}

CRender::~CRender(void)
{

}

void CRender::init(void)
{
	/*
	glvatt(1, &objectHandle);		// Create buffer obj (memory opengl can read), store in handle

    glBindBuffer(GL_ARRAY_BUFFER, objectHandle);		// Specify buffer to work with
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
*/
}

void CRender::renderScene(void)
{
	//glUseProgram(theProgram);
/*
	glBindBuffer(GL_ARRAY_BUFFER, positionDataArray);		// use this data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);		// Draw

	glDisableVertexAttribArray(0);
	//glUseProgram(0);
*/
}

#endif /* CRENDER_H_ */
