/*
 * MFont.h
 *
 *  Created on: Aug 20, 2012
 *      Author: hp
 *
 *		Everything Font!
 *
 *      Freeglut supports the following bitmap fonts:

			GLUT_BITMAP_8_BY_13 - A variable-width font with every character fitting in a rectangle of 13 pixels high by at most 8 pixels wide.
			GLUT_BITMAP_9_BY_15 - A variable-width font with every character fitting in a rectangle of 15 pixels high by at most 9 pixels wide.
			GLUT_BITMAP_TIMES_ROMAN_10 - A 10-point variable-width Times Roman font.
			GLUT_BITMAP_TIMES_ROMAN_24 - A 24-point variable-width Times Roman font.
			GLUT_BITMAP_HELVETICA_10 - A 10-point variable-width Helvetica font.
			GLUT_BITMAP_HELVETICA_12 - A 12-point variable-width Helvetica font.
			GLUT_BITMAP_HELVETICA_18 - A 18-point variable-width Helvetica font.

			The glutBitmapHeight function returns the height in pixels of the specified font.
			The glutBitmapWidth function returns the width in pixels of a single bitmapped character in the specified font.

			GLUT stroke fonts to render stroke characters
 */

#ifndef MFONT_H_
#define MFONT_H_

#define FONT_HEIGHT_IN_PIXELS 20									// Height in pixels of the font

#include <gl/freeglut.h>											// Needed bcs use glutBitmapCharacter, glutBitmapString
#include <fstream>													// Needed for extractString

void render_string(float x, float y, float z, void* font, const char* string)//Got from lighthouse3d.com
{
	const char* p;
	p = &string[0];

	glRasterPos3f(x, y , z);
	while (*p != '\0')
	{
	    glutBitmapCharacter(font, *p++);         //Got from lighthouse3d.com
	}
	//while (*p != '\0') glutStrokeCharacter(font,*p++);
}

void render_string_String(float x, float y, float z, void *font, const unsigned char *string)		// const unsigned char... shouldnt matter eh?
{
	glRasterPos3f(x, y, z);
	glutBitmapString(font, string);
}

int getStringLength(void *font, const unsigned char *string)		// Gives length in pixels to render this string
{
	return glutBitmapLength(font, string);
}

void Fps(void)
{
	static bool firsttime = true;
//	static time_t start, end;
	static clock_t sec;
	static int fps = 1, FPS = 0;
	static char sFps[10];

	glColor3f(0.0, 0.0, 0.0);  //Colour Of String
	render_string(-100, 95, 0, GLUT_BITMAP_HELVETICA_18, "FPS:");       // Draw the string

	fps++;

	if(firsttime)
	{
		sec = clock() + 1 * CLOCKS_PER_SEC;
		firsttime = false;
	}

    glColor3f(1.0, 0.0, 0.0);  //Colour Of String
	sprintf(sFps, "%d", FPS);
    render_string(-92, 95, 0, GLUT_BITMAP_HELVETICA_18, sFps);      // Draw the string

	if(clock() > sec)
	{
		sec = clock () + 1 * CLOCKS_PER_SEC;			                // Reset time
		FPS = fps;
		fps = 0;
	}
}

void DrawStrings(int windowWidth, int windowHeight, int viewDist)	// Changes to ortho, writes fps, back to perspective, place in game loop
{
	glDisable(GL_TEXTURE_2D);                                 // Must disable textures during text setup;
                                                              // Without, colours will be obscured.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, windowWidth, windowHeight);                    //-+-+-+_+_+_+_+_+_+_+_+_
	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);     // CHANGE TO 2D

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*-*-*----*-*-*-***-***-*-*-*-*-**-*-*-*-**/
	Fps();
	/*-*-*----*-*-*-***-***-*-*-*-*-**-*-*-*-**/
	glMatrixMode(GL_PROJECTION);                             /*-+-+-+_+_+_+_+_+_+_+_+_*/
	glLoadIdentity();									     /* CHANGE BACK TO 3D   */

	glViewport(0, 0, windowWidth, windowHeight);
	gluPerspective(45,(float)windowWidth/windowHeight,1,viewDist);
	glMatrixMode(GL_MODELVIEW);
   // glLoadIdentity();

	glEnable(GL_TEXTURE_2D);                                  // Re-enable textures.
}

const char *extractString(int key, const char* filename)					// Gets string from a file of ints and their respective strings
{
	//fstream stringFile(filename);
	//get int, check,
	//	true; get string

	return filename;
}


#endif /* MFONT_H_ */
