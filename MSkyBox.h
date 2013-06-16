// Render a skybox:

#include <fstream>
#include <ENDIAN.h>
#include <TGA Loader.h>

#define gameLevelSky "E:\\Users\\CoM\\Documents\\Programming Work\\Eclipse Workspace\\Mohammed FunProj\\Debug\\Game Files\\Sky Config.txt"

// Will load after everything..
// Draw this after drawn terrain.

class GameSkyBox
{
public:
	GLuint skyBox_up, skyBox_down, skyBox_west, skyBox_east, skyBox_north, skyBox_south;// skybox texture ids
	char textureFilename[90];
	int skyboxFaceSize, skyBoxScaleFactor;

	GameSkyBox();
	~GameSkyBox();

	void deleteSkyBox(void);
	void loadSkyBox(void);
	void editSkyboxScaleFac(int editScaleFac);	// value to scale skybox, allows adjustable viewdist
	void renderSkyBox(float camPosX, float camPosY, float camPosZ);
}MainGameSkyBox;

GameSkyBox::GameSkyBox(void)					// setup? loaded skybox images
{
	// Signify to not delete textures, for first run.
	//skyBox_up = NOVAL;

	// GOD DAMNED COMPILER WARNINGS
	skyBox_up = 0;
	skyBox_down = 0;
	skyBox_west = 0;
	skyBox_east = 0;
	skyBox_north = 0;
	skyBox_south = 0;
	skyboxFaceSize = 0;

	// Set default val
	skyBoxScaleFactor = 2;
}

GameSkyBox::~GameSkyBox(void)					// Delete loaded skybox images
{
	// Delete loaded textures:
	deleteSkyBox();
}

void GameSkyBox::deleteSkyBox(void)				// think this recycles tex ids????
{
	glDeleteTextures(1, &skyBox_up);
	glDeleteTextures(1, &skyBox_down);
	glDeleteTextures(1, &skyBox_west);
	glDeleteTextures(1, &skyBox_east);
	glDeleteTextures(1, &skyBox_north);
	glDeleteTextures(1, &skyBox_south);
}

void GameSkyBox::editSkyboxScaleFac(int editScaleFac)	// value to scale skybox, allows adjustable viewdist
{														// so dont get map clipped by skybox
	// Update scale factor;
	skyBoxScaleFactor = editScaleFac;
}

void GameSkyBox::loadSkyBox(void)											// Load images for skybox
{
	char data[90], *cPtr;

	// First run?
	/*if(skyBox_up != NOVAL)
		deleteSkyBox();													// Delete previous skybox.
    */

    // clear ids ???
    deleteSkyBox();													// Delete previous skybox.

	// Open file
	ifstream GameSkyBoxIN(gameLevelSky, ios::in);

	if(GameSkyBoxIN.fail())
	{
		cout << EXC << "(GameSkyBoxIN)Failed to open: " << gameLevelSky;
		return;
	}


    // Loop to find start of skybox definition
	do
	{
		GameSkyBoxIN.getline( data, 90, '\n');
		cPtr = &data[0];


		if(data[0] == 'L')
		{
			cPtr++;
			skyboxFaceSize = (int)strtod(++cPtr, &cPtr);
			cout << INS << "skyboxFaceSize: " << skyboxFaceSize;

            // Get filename 1(up):
            GameSkyBoxIN.getline( data, 90, '\n');
            cPtr = &data[0];
            cPtr++; // Now at the space beside 'S'

            // copy
            strncpy(textureFilename, ++cPtr, (strlen(data) - 1) );       // Move pointer to first char of file name, copy
            textureFilename[(strlen(data) - 1)] = '\0';

            // Report:
			cout << INS << INS << "Found: SkyBox Filename: " << textureFilename;
            skyBox_up = TGA_Texture(textureFilename);


            // Get filename 2(down):
            GameSkyBoxIN.getline( data, 90, '\n');
            cPtr = &data[0];
            cPtr++;

            strncpy(textureFilename, ++cPtr, (strlen(data) - 1) );
                    textureFilename[(strlen(data) - 1)] = '\0';

            // Report:
				cout << INS << INS << "Found: SkyBox Filename: " << textureFilename;
                        skyBox_down = TGA_Texture(textureFilename);


            // Get filename 3(west):
            GameSkyBoxIN.getline( data, 90, '\n');
            cPtr = &data[0];
            cPtr++;

            strncpy(textureFilename, ++cPtr, (strlen(data) - 1) );
                    textureFilename[(strlen(data) - 1)] = '\0';

            // Report:
				cout << INS << INS << "Found: SkyBox Filename: " << textureFilename;
                        skyBox_west = TGA_Texture(textureFilename);


            // Get filename 4(east):
            GameSkyBoxIN.getline( data, 90, '\n');
            cPtr = &data[0];
            cPtr++;

            strncpy(textureFilename, ++cPtr, (strlen(data) - 1) );
                    textureFilename[(strlen(data) - 1)] = '\0';

            // Report:
				cout << INS << INS << "Found: SkyBox Filename: " << textureFilename;
                        skyBox_east = TGA_Texture(textureFilename);


            // Get filename 5(north):
            GameSkyBoxIN.getline( data, 90, '\n');
            cPtr = &data[0];
            cPtr++;

            strncpy(textureFilename, ++cPtr, (strlen(data) - 1) );
                    textureFilename[(strlen(data) - 1)] = '\0';

            // Report:
				cout << INS << INS << "Found: SkyBox Filename: " << textureFilename;
                        skyBox_north = TGA_Texture(textureFilename);


            // Get filename 6(south):
            GameSkyBoxIN.getline( data, 90, '\n');
            cPtr = &data[0];
            cPtr++;

            strncpy(textureFilename, ++cPtr, (strlen(data) - 1) );
                    textureFilename[(strlen(data) - 1)] = '\0';

            // Report:
				cout << INS << INS << "Found: SkyBox Filename:_" << textureFilename << "_";
                        skyBox_south = TGA_Texture(textureFilename);

            // Done, break out of loop.
            break;
		}
	}while(!GameSkyBoxIN.eof());

	GameSkyBoxIN.close();
	GameSkyBoxIN.clear();
}

void GameSkyBox::renderSkyBox(float camPosX, float camPosY, float camPosZ)					// Draw the sky box
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	// Correct?
	glPushMatrix();
    glTranslatef(camPosX, camPosY, camPosZ);	// Translate skybox
    glScalef(skyBoxScaleFactor,skyBoxScaleFactor,skyBoxScaleFactor);

	//glColor3f(1.0f, 1.0f, 1.0f);

	// Up
	 glBindTexture(GL_TEXTURE_2D, skyBox_up);
	  glBegin(GL_QUADS);		// Draw The Cube Using quads
	 //   glColor3f(0.0f,1.0f,0.0f);	// Color Blue
		glTexCoord2f(0,0);
		glVertex3f( skyboxFaceSize, skyboxFaceSize,-skyboxFaceSize);	// Top Right Of The Quad (Top)
		glTexCoord2f(1,0);
		glVertex3f(-skyboxFaceSize, skyboxFaceSize,-skyboxFaceSize);	// Top Left Of The Quad (Top)
		glTexCoord2f(1,1);
		glVertex3f(-skyboxFaceSize, skyboxFaceSize, skyboxFaceSize);	// Bottom Left Of The Quad (Top)
		glTexCoord2f(0,1);
		glVertex3f( skyboxFaceSize, skyboxFaceSize, skyboxFaceSize);	// Bottom Right Of The Quad (Top)
	  glEnd();			// End Drawing The Cube

	// Down
	glBindTexture(GL_TEXTURE_2D, skyBox_down);
	  glBegin(GL_QUADS);		// Draw The Cube Using quads
	 //   glColor3f(1.0f,0.5f,0.0f);	// Color Orange
		glTexCoord2f(0,0);
		glVertex3f( skyboxFaceSize,-skyboxFaceSize, skyboxFaceSize);	// Top Right Of The Quad (Bottom)
		glTexCoord2f(1,0);
		glVertex3f(-skyboxFaceSize,-skyboxFaceSize, skyboxFaceSize);	// Top Left Of The Quad (Bottom)
		glTexCoord2f(1,1);
		glVertex3f(-skyboxFaceSize,-skyboxFaceSize,-skyboxFaceSize);	// Bottom Left Of The Quad (Bottom)
		glTexCoord2f(0,1);
		glVertex3f( skyboxFaceSize,-skyboxFaceSize,-skyboxFaceSize);	// Bottom Right Of The Quad (Bottom)
	  glEnd();			// End Drawing The Cube

	// West
	glBindTexture(GL_TEXTURE_2D, skyBox_west);
	  glBegin(GL_QUADS);		// Draw The Cube Using quads
	   // glColor3f(0.0f,0.0f,1.0f);	// Color Blue
		glTexCoord2f(1,1);
		glVertex3f(-skyboxFaceSize, skyboxFaceSize, skyboxFaceSize);	// Top Right Of The Quad (Left)
		glTexCoord2f(0,1);
		glVertex3f(-skyboxFaceSize, skyboxFaceSize,-skyboxFaceSize);	// Top Left Of The Quad (Left)
		glTexCoord2f(0,0);
		glVertex3f(-skyboxFaceSize,-skyboxFaceSize,-skyboxFaceSize);	// Bottom Left Of The Quad (Left)
		glTexCoord2f(1,0);
		glVertex3f(-skyboxFaceSize,-skyboxFaceSize, skyboxFaceSize);	// Bottom Right Of The Quad (Left)
	  glEnd();			// End Drawing The Cube

	// East
	glBindTexture(GL_TEXTURE_2D, skyBox_east);
	  glBegin(GL_QUADS);		// Draw The Cube Using quads
	  //  glColor3f(1.0f,0.0f,1.0f);	// Color Violet
		glTexCoord2f(1,1);
		glVertex3f( skyboxFaceSize, skyboxFaceSize,-skyboxFaceSize);	// Top Right Of The Quad (Right)
		glTexCoord2f(0,1);
		glVertex3f( skyboxFaceSize, skyboxFaceSize, skyboxFaceSize);	// Top Left Of The Quad (Right)
		glTexCoord2f(0,0);
		glVertex3f( skyboxFaceSize,-skyboxFaceSize, skyboxFaceSize);	// Bottom Left Of The Quad (Right)
		glTexCoord2f(1,0);
		glVertex3f( skyboxFaceSize,-skyboxFaceSize,-skyboxFaceSize);	// Bottom Right Of The Quad (Right)
	  glEnd();			// End Drawing The Cube

	// North
	glBindTexture(GL_TEXTURE_2D, skyBox_north);
	  glBegin(GL_QUADS);		// Draw The Cube Using quads
	  //  glColor3f(1.0f,0.0f,0.0f);	// Color Red
		glTexCoord2f(1,1);
		glVertex3f( skyboxFaceSize, skyboxFaceSize, skyboxFaceSize);	// Top Right Of The Quad (Front)
		glTexCoord2f(0,1);
		glVertex3f(-skyboxFaceSize, skyboxFaceSize, skyboxFaceSize);	// Top Left Of The Quad (Front)
		glTexCoord2f(0,0);
		glVertex3f(-skyboxFaceSize,-skyboxFaceSize, skyboxFaceSize);	// Bottom Left Of The Quad (Front)
		glTexCoord2f(1,0);
		glVertex3f( skyboxFaceSize,-skyboxFaceSize, skyboxFaceSize);	// Bottom Right Of The Quad (Front)
	  glEnd();			// End Drawing The Cube

	// South
	glBindTexture(GL_TEXTURE_2D, skyBox_south);
	  glBegin(GL_QUADS);		// Draw The Cube Using quads
	  //  glColor3f(1.0f,1.0f,0.0f);	// Color Yellow
		glTexCoord2f(0,0);
		glVertex3f( skyboxFaceSize,-skyboxFaceSize,-skyboxFaceSize);	// Top Right Of The Quad (Back)
		glTexCoord2f(1,0);
		glVertex3f(-skyboxFaceSize,-skyboxFaceSize,-skyboxFaceSize);	// Top Left Of The Quad (Back)
		glTexCoord2f(1,1);
		glVertex3f(-skyboxFaceSize, skyboxFaceSize,-skyboxFaceSize);	// Bottom Left Of The Quad (Back)
		glTexCoord2f(0,1);
		glVertex3f( skyboxFaceSize, skyboxFaceSize,-skyboxFaceSize);	// Bottom Right Of The Quad (Back)
	  glEnd();			// End Drawing The Cube

	// Correct?
	glPopMatrix();

//	glEnable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}

// For extern:
void loadGameSkyBox(void)
{
	// Load game skybox:
	MainGameSkyBox.loadSkyBox();
}

