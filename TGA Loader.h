// Following TGA loader from: Apron Tutorials: http://www.morrowland.com/apron/tut_gl.php
// Some variable names, and data types were changed to suit my purposes.

/* Most significant change is the original loader used the type 'byte'
** I searched to the definition of 'byte', but I could not find any such.
** I changed the loader to use unsigned char instead of 'byte', they both
** are the same size via sizeof(). It compiled, and tests showed no inhibited
** functionality of the loader.
*/

/**
*** Note: Endian converter is from John Adler.
*** Note: This TGA Loader supports compressed tga's.
*** Note: this TGA Loader supports Alpha channel containing TGA's
**/

#define TGA_RGB		2
#define TGA_A		3
#define TGA_RLE		10

struct tImageTGA
{
	int channels;
	int size_x;
	int size_y;
	unsigned char *data;
};

tImageTGA *Load_TGA(const char *strfilename)
{
	tImageTGA *pImgData	= NULL;
	FILE *pFile			= NULL;
	short width			= 0;
	short height			= 0;
	unsigned char length     = 0;
	unsigned char imgType	 = 0;
	unsigned char bits		 = 0;
	int channels		= 0;
	int stride			= 0;
	int i				= 0;


	if((pFile = fopen(strfilename, "rb")) == NULL)
	{
		cout << endl << EXC << "Failed to open: " << strfilename;
		return NULL;
	}

	pImgData = (tImageTGA*)malloc(sizeof(tImageTGA));

	fread(&length, sizeof(char), 1, pFile);

	fseek(pFile,1,SEEK_CUR);

	fread(&imgType, sizeof(char), 1, pFile);

	fseek(pFile, 9, SEEK_CUR);

	fread(&width,  sizeof(short), 1, pFile);
/*Mac	if(OPSYS == 0)
		width = endianConverter.convertLittleToBig(width);
*/

	fread(&height, sizeof(short), 1, pFile);
/*Mac	if(OPSYS == 0)
		height = endianConverter.convertLittleToBig(height);
*/
	// Chars are apparrently always the same.
	fread(&bits,   sizeof(char), 1, pFile);
	fseek(pFile, length + 1, SEEK_CUR);

	if(imgType != TGA_RLE)
	{
		// Check for 24 or 32 Bit
		if(bits == 24 || bits == 32)
		{

			channels = bits / 8;
			stride = channels * width;
			pImgData->data = new unsigned char[stride * height];

			for(int y = 0; y < height; y++)
			{
				unsigned char *pLine = &(pImgData->data[stride * y]);

				fread(pLine, stride, 1, pFile);

				for(i = 0; i < stride; i += channels)
				{
					int temp     = pLine[i];
					pLine[i]     = pLine[i + 2];
					pLine[i + 2] = temp;
				}
			}
		}

		// Check for 16 Bit
		else if(bits == 16)
		{
			unsigned short pixels = 0;
			int r=0, g=0, b=0;

			channels = 3;
			stride = channels * width;
			pImgData->data = new unsigned char[stride * height];

			for(int i = 0; i < width*height; i++)
			{
				fread(&pixels, sizeof(unsigned short), 1, pFile);
				pixels = endianConverter.convertLittleToBig(pixels);

				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;

				pImgData->data[i * 3 + 0] = r;
				pImgData->data[i * 3 + 1] = g;
				pImgData->data[i * 3 + 2] = b;
			}
		}

		else
			return NULL;
	}

	else
	{
		unsigned char rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		pImgData->data = new unsigned char[stride * height];
		unsigned char *pColors = new unsigned char[channels];

		while(i < width*height)
		{

			fread(&rleID, sizeof(unsigned char), 1, pFile);


			if(rleID < 128)
			{
				rleID++;

				while(rleID)
				{
					fread(pColors, sizeof(char) * channels, 1, pFile);

					pImgData->data[colorsRead + 0] = pColors[2];
					pImgData->data[colorsRead + 1] = pColors[1];
					pImgData->data[colorsRead + 2] = pColors[0];

					if(bits == 32)	pImgData->data[colorsRead + 3] = pColors[3];

					i++;
					rleID--;
					colorsRead += channels;
				}
			}

			else
			{
				rleID -= 127;

				fread(pColors, sizeof(char) * channels, 1, pFile);

				while(rleID)
				{
					pImgData->data[colorsRead + 0] = pColors[2];
					pImgData->data[colorsRead + 1] = pColors[1];
					pImgData->data[colorsRead + 2] = pColors[0];

					if(bits == 32)	pImgData->data[colorsRead + 3] = pColors[3];

					i++;
					rleID--;
					colorsRead += channels;
				}
			}
		}
		delete[] pColors;
	}

	fclose(pFile);


	pImgData->channels  = channels;
	pImgData->size_x    = width;
	pImgData->size_y    = height;

	return pImgData;
}

GLuint TGA_Texture(char *strFileName)
{
	GLuint texId;
	int textureType;

	tImageTGA *pBitMap = Load_TGA(strFileName);

	if(pBitMap == NULL)
	{
        cout << EXC << "Error load TGA...:" << strFileName;
        return 0;
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	textureType = GL_RGB;
	if(pBitMap->channels == 4)
		textureType = GL_RGBA;

	gluBuild2DMipmaps(GL_TEXTURE_2D, pBitMap->channels, pBitMap->size_x, pBitMap->size_y, textureType , GL_UNSIGNED_BYTE, pBitMap->data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	if (pBitMap)
	{
		if (pBitMap->data)
		{
			free(pBitMap->data);
		}
		free(pBitMap);
	}

	return texId;
}

