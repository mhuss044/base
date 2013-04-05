//============================================================================
// Name        : BattleShip.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Space ship collecting debris for conv to weaps to fight pirates/space aliens
//============================================================================

#include <windows.h>
#include <GL/gl.h>
#include <gl/glu.h>
//#include <gl/glut.h>
#include <gl/freeglut.h>
//#include <glaux.h>            // This isn't needed. Apparently its outdated and now unsupported, if need bmp load, use nehe glaux replcaement
#include <iostream>
#include <time.h>				// Need this so dont get clock_t does not name a type
#include <stdio.h>
#include <math.h>

using namespace std;
/*****PRIMITIVE VARIABLES********************************************
*************-*-*-*-*VARIABLES*-*-*-*-*********************
***********************************************************/
#define PI 3.14159265358979
#define EXC "\n(!!)"                                             //EXCELMATION
#define GDD "\n(:D)"											 //GOOD
#define INS "\n(->)"											 //insert?
#define GAME_MAP_SIZE_X 100
#define GAME_MAP_SIZE_Z 100
#define VIEW_DISTANCE 200000										// how far is clipping plane


bool fullscreen = false, perspective = true, mousePos[4] = {0};
float ptSize = 0, lineWth = 0;
float xrot = 0.0, yrot = 0, xfa = 0.1, yfa = 0.1;  //Rotation values, and increment factor. (to be increase/decreased)
float lightAmbient[] = { 0.2f, 0.3f, 0.6f, 1.0f };//Light colour values.
float lightDiffuse[] = { 0.2f, 0.3f, 0.6f, 1.0f };
int WINWidth = 1000, WINHeight = 700, SHADE = 0, gameFoodLocH, gameFoodLocV;
int cursorX = 0, cursorY = 0, mousePosX = 0, mousePosY = 0;


int polygonMode[2] = {0, 0};
char a;

typedef struct cube1
{
    int xp, yp, zp;
}cube;

cube newCube;

int map[2][2];

/*****USR**************************************************
*************-*-*-*-*USR*-*-*-*-***************************
***********************************************************/
#include "MFunctions.h"
#include "MFont.h"
#include "CGUI.h"
#include "MInput.h"
#include "CCamera.h"            // Game camera containing class
#include "CShip.h"
#include "CPlayerShip.h"
#include "ParticleSys.h"
#include "SpaceSelector.h"
#include "Builder.h"

/*****HIGH LEVEL VARIABLES*********************************
*************-*-*-*-*VARIABLES*-*-*-*-*********************
***********************************************************/
CKeyboard playerKeyboard;
CPlayerShip player;
CCamera MainGameCamera;
CParticleSys partSys(10000, 0, 10, 0, 1000);
// CGUI MainGameGUI;	// turns out a lot of the vars, funcs need to display info reside in main, some headers, wtf do, function pointer?
/*****CLASSES***********************************************
*************-*-*-*-*CLASSES*****-*-*-*-********************
***********************************************************/
class GUIdrawer
{
public:
	GUIdrawer(void);
	void drawMainHUD(void);
}MainGameGUI;

GUIdrawer::GUIdrawer(void)
{

}

void GUIdrawer::drawMainHUD(void)
{
	// Set up ortho: GO TO 2D; not really 2d.. just going to ortho; and only drawing in x,y plane
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WINWidth, WINHeight);
	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);// specify coord sys; xmax, xmin

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//	Draw HUD;
	glColor3f(0,1,0);									// using glColor3i results in black, always ???
    glPushMatrix();
        glTranslatef(90.0f, -90.0f, 0.0f);
        glScalef(20, 20, 20);
        	DrawTriangle();
    glPopMatrix();

	glColor3f(1,0,0);									// using glColor3i results in black, always ???
    render_string(60,-90,0, GLUT_BITMAP_HELVETICA_18, "PLACE HUD here pls");

    // Set up perspective: GO TO 3D
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WINWidth, WINHeight);
	if(WINWidth == 0 || WINHeight == 0)					                //Prevent division by zero, cant make a window of 0 width..
	{
		WINWidth = 50;
		WINHeight = 50;
	}

   	gluPerspective(45,(float)WINWidth/WINHeight,1,VIEW_DISTANCE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
/*****DEFINITIONS*******************************************
*************-*-*-*-*DEFINITIONS*-*-*-*-********************
***********************************************************/
void drawGround(void)													// Costs approx 50 fps
{
	// Draw grid
    // Make sure colour ..
    glColor3f(0.0, 0.0, 0.0);
	glColor3f(0.1, 0.0, 0.0);
	DrawGrid(GAME_MAP_SIZE_Z, GAME_MAP_SIZE_X, 50);
}
void particleRoutines(void)
{
	partSys.particleMotionRoutines();
	partSys.renderParticles();
	partSys.particleRespawner();

}
void placeFood(void)													// Picks random food location
{
	// Get num from 1 - 10:
	gameFoodLocH = rand()%10 + 1;
	gameFoodLocV = rand()%10 + 1;

	// get in range of -100 - 100, add neg/pos
	if((rand()%2 + 1) == 2)
		gameFoodLocH *= -10;
    else
        gameFoodLocH *= 10;
	if((rand()%2 + 1) == 2)
		gameFoodLocV *= -10;
    else
        gameFoodLocV *= 10;

	cout << INS << "Food Pos: H: " << gameFoodLocH << " V: " << gameFoodLocV;
}
void drawFood(void)														// Costs approx 170 fps
{
	glColor3f(0.5, 0.8, 0.3);

	glPushMatrix();
		glTranslated(gameFoodLocH, 0, gameFoodLocV);
		glutSolidSphere(1,10,10);										// 10: slices, 10: stacks
	glPopMatrix();

	glPushMatrix();
		glTranslated(gameFoodLocH - 2, 0, gameFoodLocV);
		glutSolidCone(1,1,10,10);
	glPopMatrix();

	glPushMatrix();
		glTranslated(gameFoodLocH + 2, 0, gameFoodLocV);
		glutSolidTorus(0.2,0.8,10,10);
	glPopMatrix();

	glPushMatrix();
		glTranslated(gameFoodLocH, 0, gameFoodLocV - 2);
		glutWireSphere(1,10,10);
	glPopMatrix();

	glPushMatrix();
		glTranslated(gameFoodLocH, 0, gameFoodLocV + 2);
		glutWireCone(1,1,10,10);
	glPopMatrix();

	glPushMatrix();
		glTranslated(gameFoodLocH + 1, 0, gameFoodLocV + 1);
		glutWireTorus(0.2,0.8,10,10);
	glPopMatrix();
}
void inputDistributor(void)
{
	// Player movement:
	if(playerKeyboard.queryNormalKey(119))	// w
    {
    	MainGameCamera.Move(FORWARD);
		player.requestMove(MOVE_UP);
    }
	if(playerKeyboard.queryNormalKey(115))	// s
    {
		player.requestMove(MOVE_DOWN);
		MainGameCamera.Move(BACKWARD);
    }
	if(playerKeyboard.queryNormalKey(100))	// d
    {
    	player.requestMove(MOVE_RIGHT);
    	MainGameCamera.Move(RIGHT);
    }
	if(playerKeyboard.queryNormalKey(97))	// a
	{
		player.requestMove(MOVE_LEFT);
		MainGameCamera.Move(LEFT);
	}

	// Settings:
	if(playerKeyboard.queryNormalKey(99))	// c
	{
		if(SHADE == 0)
			SHADE = 1;
		else
			SHADE = 0;
	}
	if(playerKeyboard.queryNormalKey(112))	// p
	{
        if(perspective == true)
            perspective = false;
        else
            perspective = true;
	}
	if(playerKeyboard.queryNormalKey(113))	// q
	{
		if (glIsEnabled(GL_DEPTH_TEST))
			glDisable(GL_DEPTH_TEST);
		else
			glEnable(GL_DEPTH_TEST);
	    glutPostRedisplay();
	}
	if(playerKeyboard.queryNormalKey(122))	// z
	{
		partSys.resetAllParticles();
	}



	if(playerKeyboard.querySpecialKey(GLUT_KEY_RIGHT))
	{
		MainGameCamera.Yaw(0.1);
	}
	if(playerKeyboard.querySpecialKey(GLUT_KEY_LEFT))
	{
		MainGameCamera.Yaw(-0.1);
	}
	if(playerKeyboard.querySpecialKey(GLUT_KEY_UP))
	{
		MainGameCamera.Pitch(0.1);
	}
	if(playerKeyboard.querySpecialKey(GLUT_KEY_DOWN))
	{
		MainGameCamera.Pitch(-0.1);
	}

	// Autorotate:
	MainGameCamera.processCamera(mousePosX, mousePosY);
}

/*****INITIALIZE*******************************************************************************************
**************************-*-*-*-*INITIALIZE*-*-*-*-***************************************************
*******************************************************************************************************/

bool init()
{                                            // Sets the clearing colour, whenever window is cleared
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // it will be this color
    glColor3f(1.0f, 0.0f, 0.0f);             //this will set the drawing colour


    glGetFloatv(GL_POINT_SIZE, &ptSize);     /*Call to glGetFloatv, and pass point size attrib.**Need this value later */
                                             /*to retrive the point size and store in ptSize.  **when we restore it back to origonal.*/

    glGetFloatv(GL_LINE_WIDTH, &lineWth);    //Gets current line width.

    glGetIntegerv(GL_POLYGON_MODE, polygonMode); //Gets polygon mode with that attrib, stores in a two
                                                 //dimensional array. For later when we restore it back.
/****DEPTH***************************************************************************************************
**********DEPTH************************************************************************************/
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

//	glutSetCursor(GLUT_CURSOR_NONE);			// set cursor image

	// Seed random # generator.
	srand( (unsigned int) clock());

    newCube.xp = 50;
    newCube.zp = 50;

  /*  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


    //glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
*/

	// Pick food location:
	//placeFood();

    return true;                             //Tell program things went well.
}

void Shutdown()
{
    glPointSize(ptSize);                     //Restore to default size.
    glLineWidth(lineWth);                    //Restore to default width.
    glPolygonMode(GL_FRONT, polygonMode[0]); //Restore this part of mode back to origonal.
	glPolygonMode(GL_BACK, polygonMode[1]);  //And the second.
    //Do other stuff

    exit(0);
}

void display()                               //Main function that will draw.
{
	// ok, so no idea why when set gluPerspective in INIT and RESIZE, somehow gets reset??? makes zFar = 500..
	// so pasted it in the main loop.. works now??
	// YEAH OK, TURNS OUT HAD GLUPERSPECTIVE IN DRAWSTRINGS LATER DOWN THAR, HAD OLD VIEW DIST CODE, ALL THIS TIME, FML
/*	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(float)WINWidth/WINHeight,1,VIEW_DISTANCE);
	glMatrixMode(GL_MODELVIEW);
*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);            //Clear window to previously defined colour ^^^.
    glLoadIdentity();                        //Also specifies what to clear.(COLOR BUFFER)

    // CAMERA
    gluLookAt(
		MainGameCamera.camPos.x, MainGameCamera.camPos.y, MainGameCamera.camPos.z,
		MainGameCamera.viewPt.x, MainGameCamera.viewPt.y, MainGameCamera.viewPt.z,
		0.0f, 1.0f, 0.0f);

	// Draw grid:
	drawGround();

  	glPushMatrix();
		glTranslated(newCube.xp, newCube.yp+5,  newCube.zp);
		glScalef(10, 10, 10);
		glutWireSphere(1,10,10);										// 10: slices, 10: stacks
	glPopMatrix();
/*    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -5.0f);
            DrawTriangle();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(5.0f, 0.0f, 0.0f);
        glRotatef(-15.0, 0.0,0.0,1.0);
            DrawTriangle();
    glPopMatrix();


*/
/*
*/
	// Particles:
  	glPushMatrix();
		glScalef(10, 10, 10);
		particleRoutines();
	glPopMatrix();

	// Draw strings.
	DrawStrings();

	// Handle input;
	inputDistributor();



	MainGameGUI.drawMainHUD();

	glFlush();                               		/*Ensures all commands processed  */
	glutSwapBuffers();
	glutPostRedisplay();							// fix flickering?
}

void passiveMouse(int x, int y)
{
	// Set current cursor pos
	mousePosX = x;
	mousePosY = y;

	//cout << INS << "passive mouse: " << x << " " << y;
	// Auto rotate:
	for(int t = 0; t < 4; t++)
		mousePos[t] = false;

	// Set mouse pos
	if(y < 50)															// Extreme top edge
		mousePos[0] = true;
    if(x > (WINWidth - 50))												// Extreme right edge
    	mousePos[1] = true;
	if(y > (WINHeight - 50))											// Extreme bottom edge
		mousePos[2] = true;
	if(x < 50)															// Extreme left edge
		mousePos[3] = true;
}

void mouse(int button, int state, int x, int y)
{
    int mod;

/*
    if(SELECT == 1)
    {
        if(button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
            return;

        // Set:
        cursorX = x;
        cursorY = y;

        MainGameMenu.pickStartMenu();                                   // Trigger picking.
    }
*/

    if (button == GLUT_RIGHT_BUTTON)
	{
		mod = glutGetModifiers();

		if (state == GLUT_DOWN)
            {
                cout << "Right button pressed" << endl;

                if(mod == GLUT_ACTIVE_CTRL)
                	cout << INS << "mouse click and ctrl";
                else
                	cout << INS << "mouse click and no ctrl";
            }
            else
            {
                cout << "Right button lifted "
                << "at (" << x << "," << y
                << ")" << endl;
            }
	}
	else if (button == GLUT_LEFT_BUTTON)
	{
		mod = glutGetModifiers();
		if (state == GLUT_DOWN)
			{
			    cout << "Left button pressed"
                << endl;

                if(mod == GLUT_ACTIVE_CTRL)
                	cout << INS << "mouse click and ctrl";
                else
                	cout << INS << "mouse click and no ctrl";
			}
		else
			{
			    cout << "Left button lifted "
                << "at (" << x << "," << y
                << ")" << endl;
			}
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
	    mod = glutGetModifiers();
	    if(mod == GLUT_ACTIVE_CTRL)
        	cout << INS << "mouse click and ctrl";
        else
        	cout << INS << "mouse click and no ctrl";
	}

	cout << "{{X:}}" << x << endl;
    cout << "{{Y:}}" << y << endl;
}

void normalKeyDown(unsigned char key, int x, int y)      	//	key pressed, and mouse pos when pressed.
{
	switch(key)
	{
    case 27:															// (ESC)
        Shutdown();                        								// Exit.
        break;
	case 45:    // -                                                    // Zoom in.
	    MainGameCamera.scalePos(-2);// cant put this in main loop input distributor; results in seemingly uncontrolled scaling, cant fix too lazy
		playerKeyboard.updateNormalKey(45, Key_DOWN);
        break;
    case 61:    // +                                                    // Zoom out.
        MainGameCamera.scalePos(2);
    	playerKeyboard.updateNormalKey(61, Key_DOWN);
        break;
	case 97://a
        playerKeyboard.updateNormalKey(97, Key_DOWN);
        break;
    case 99://c
    	playerKeyboard.updateNormalKey(99, Key_DOWN);
        break;
    case 100://d
        playerKeyboard.updateNormalKey(100, Key_DOWN);
		break;
    case 101://e
    	playerKeyboard.updateNormalKey(101, Key_DOWN);
        break;
	case 112://p
        playerKeyboard.updateNormalKey(112, Key_DOWN);
        break;
    case 113: //q
        playerKeyboard.updateNormalKey(113, Key_DOWN);
        break;
	case 115://s
        playerKeyboard.updateNormalKey(115, Key_DOWN);
        break;
    case 119://w
        playerKeyboard.updateNormalKey(119, Key_DOWN);
        break;
	case 120://x
		playerKeyboard.updateNormalKey(120, Key_DOWN);
        break;
	case 122://	z
		playerKeyboard.updateNormalKey(122, Key_DOWN);
		break;
	}
}

void specialKeyDown(int key, int x, int y)   //Used for special keys like F1.
{
	if (key == GLUT_KEY_F1)                   //Check if F1 pressed.
	{
		fullscreen = !fullscreen;             //If F1 pressed, toggle boolean value.

		if (fullscreen)                       //If true, call glut's fullscreen func.
			glutFullScreen();
		else                                  //If not true, make window to default size, and pos.
		{
			glutReshapeWindow(500, 500);      //Resizes.
			glutPositionWindow(50, 50);       //Repositions.
		}

		playerKeyboard.updateSpecialKey(GLUT_KEY_F1, Key_DOWN);
	}
	else if(key == GLUT_KEY_RIGHT)            // Detect keyboard arrow keys and rotate about y or x axis
	{
        MainGameCamera.Yaw(5);
        newCube.xp+= 100;

        map[0][0] = 0;
        map[0][1] = 1;

        playerKeyboard.updateSpecialKey(GLUT_KEY_RIGHT, Key_DOWN);
	}
    else if(key == GLUT_KEY_LEFT)
	{
	    MainGameCamera.Yaw(-5);
	    newCube.xp-= 100;

	    map[0][1] = 0;
	    map[0][0] = 1;

        playerKeyboard.updateSpecialKey(GLUT_KEY_LEFT, Key_DOWN);
	}
    else if(key == GLUT_KEY_UP)
	{
	    MainGameCamera.Pitch(5);
	    newCube.zp+= 100;

	    map[1][1] = 0;
	    map[1][0] = 1;

	    playerKeyboard.updateSpecialKey(GLUT_KEY_UP, Key_DOWN);
	}
    else if(key == GLUT_KEY_DOWN)
	{
        MainGameCamera.Pitch(-5);
        newCube.zp-= 100;

	    map[1][0] = 0;
	    map[1][1] = 1;

	    playerKeyboard.updateSpecialKey(GLUT_KEY_DOWN, Key_DOWN);
	}
}

void normalKeyUp(unsigned char key, int x, int y)
{
	playerKeyboard.updateNormalKey(key, Key_UP);
}

void specialKeyUp(int key, int x, int y)
{
	playerKeyboard.updateSpecialKey(key, Key_UP);
}

void resize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	if(w == 0 || h == 0)					                //Prevent division by zero, cant make a window of 0 width..
	{
		w = 50;
		h = 50;
	}

    if(perspective == true)
    	gluPerspective(45,(float)w/h,1,VIEW_DISTANCE);
    else
        glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle()
{
	xrot =+ 0.20f;
	glutPostRedisplay();// dont need this here?
}
int main(int argc, char *argv[])
{
 	glutInit(&argc, argv);                           //Initialize glut.

	glutInitWindowPosition(50, 10);                  //Specifies window pos.
	glutInitWindowSize(WINWidth, WINHeight);                    //Specifies window size

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);     //Type of colour, and buffer descript ?

	glutCreateWindow("BATTLE SHIP!");	 	         //Name of window.

	glutDisplayFunc(display);						// main loop
	glutKeyboardFunc(normalKeyDown);                       /*func to call when key is pushed				 */
	glutSpecialFunc(specialKeyDown);                /*into the main loop                            */
	glutKeyboardUpFunc(normalKeyUp);
	glutSpecialUpFunc(specialKeyUp);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(passiveMouse);
    glutIdleFunc(idle);
	if (!init())                                     //Make sure everything went well in initialization.
		return 1;

	glutMainLoop();                                  //The main loop, displays, checks for input.

	return 0;
}
