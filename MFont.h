/*
 * MFont.h
 *
 *  Created on: Aug 20, 2012
 *      Author: hp
 */

#ifndef MFONT_H_
#define MFONT_H_

// Everything Font!

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

void Fps(void)
{
	static bool firsttime = true;
//	static time_t start, end;
	static clock_t sec;
	static int fps = 1, FPS = 0;
	static char sFps[10];

	glColor3f(1.0, 1.0, 1.0);  //Colour Of String
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

void DrawStrings(void)										// Changes to ortho, writes fps, back to perspective, place in game loop
{
	glDisable(GL_TEXTURE_2D);                                 // Must disable textures during text setup;
                                                              // Without, colours will be obscured.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, WINWidth, WINHeight);                    //-+-+-+_+_+_+_+_+_+_+_+_
	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);     // CHANGE TO 2D

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*-*-*----*-*-*-***-***-*-*-*-*-**-*-*-*-**/
	Fps();
	/*-*-*----*-*-*-***-***-*-*-*-*-**-*-*-*-**/
	glMatrixMode(GL_PROJECTION);                             /*-+-+-+_+_+_+_+_+_+_+_+_*/
	glLoadIdentity();									     /* CHANGE BACK TO 3D   */

	glViewport(0, 0, WINWidth, WINHeight);
	gluPerspective(45,(float)WINWidth/WINHeight,1,VIEW_DISTANCE);
	glMatrixMode(GL_MODELVIEW);
   // glLoadIdentity();

	glEnable(GL_TEXTURE_2D);                                  // Re-enable textures.
}




#endif /* MFONT_H_ */
