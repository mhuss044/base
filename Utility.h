/*
 * Utility.h
 *
 *  Created on: May 27, 2013
 *      Author: EvorateAwMaDemCriAnR
 *
 *      Hold non specific functions
 *
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <CommonTypes.h>		// Require types for below funcs;

//  UTILITY FUNCS UTILITY FUNCS UTILITY FUNCS UTILITY FUNCS UTILITY FUNCS UTILITY FUNCS:
//  UTILITY FUNCS UTILITY FUNCS UTILITY FUNCS UTILITY FUNCS UTILITY FUNCS UTILITY FUNCS:

void enterOrthographic(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax, int viewPortX, int viewPortY, int viewPortWidth, int viewPortHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(viewPortX, viewPortY, viewPortWidth, viewPortHeight);
	glOrtho(xMin, xMax, yMin, yMax, zMin, zMax);// specify coord sys; xmax, xmin

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Now do ortho drawing.
}

void enterPerspective(int viewPortX, int viewPortY, int viewPortWidth, int viewPortHeight, int viewDistance)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();																// This is so that previous transformations do not accum
	glViewport(viewPortX, viewPortY, viewPortWidth, viewPortHeight);
	if(viewPortWidth == 0 || viewPortHeight == 0)					                //Prevent division by zero, cant make a window of 0 width..
	{
		viewPortWidth = 50;
		viewPortHeight = 50;
	}
	gluPerspective(45,(float)viewPortWidth/viewPortHeight,1,viewDistance);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Now do perspective drawing.
}

void DegToRad(float *deg)
{
    *deg = float(((*deg)*PI)/180);
}

void RadToDeg(float *rad)
{
    *rad = float(((*rad)*180)/PI);
}

float Round(const float &number, const int num_digits)  //Got this from: http://www.codeproject.com/cpp/ffile:///C:/Users/EvorateAwMaDemCriAnR/Documents/Programming%20Work/Modern%20GL%20Tutorial%200.3.7/Tutorial%200.3.7/html/Positioning/AngleAxisRotationMatrix.svgloatutils.asp
{
    float doComplete5i, doComplete5(number * powf(10.0f, (float) (num_digits + 1)));

    if(number < 0.0f)
        doComplete5 -= 5.0f;
    else
        doComplete5 += 5.0f;

    doComplete5 /= 10.0f;
    modff(doComplete5, &doComplete5i);

    return doComplete5i / powf(10.0f, (float) num_digits);
}

float highest01(float num)		// highest 0.1, so 0.29 -> 0.3
{
	float x10 = num*10.0;		// need 10.0 here or else float*int = int
								// same with below, even tho float typecast; int()/int = int, but dont need cast when return int()/float = float

	//x10 - int(x10); // 2 - 2, mayb have to typecast to flaot
	if((x10 - int(x10)) >= 0.5)
		return (int(x10)/10.0)+0.1;	// 0.2 + 0.1
	else
		return (int(x10)/10.0);
}

float Distance2f(float x1, float y1, float x2, float y2)
{
	float dist;
	dist = sqrt( pow( (x1 - x2), 2) + pow( (y1 - y2), 2) );

	Round(dist, 1);

	return dist;
}

float Distance2V(Point2x *p1, Point2x *p2)
{
	float dist;
	dist = sqrt( pow( (p1->x - p2->x), 2) + pow( (p1->y - p2->y), 2) );

	Round(dist, 1);

	return dist;
}

float Distance2V(Vert2x *p1, Vert2x *p2)
{
	float dist;
	dist = sqrt( pow( (p1->x - p2->x), 2) + pow( (p1->y - p2->y), 2) );

	Round(dist, 1);

	return dist;
}

float Distance2V(Vert2xf *p1, Vert2xf *p2)
{
	float dist;
	dist = sqrt( pow( (p1->x - p2->x), 2) + pow( (p1->y - p2->y), 2) );

	Round(dist, 1);

	return dist;
}

float Distance2V(Vert2xd *p1, Vert2xd *p2)
{
	float dist;
	dist = sqrt( pow( ( float(p1->x) - float(p2->x) ), 2) + pow( ( float(p1->y) - float(p2->y) ), 2) );

	Round(dist, 1);

	return dist;
}

float Distance3f(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float dist;
	dist = sqrt( pow( (x1 - x2), 2) + pow( (y1 - y2), 2) + pow( (z1 - z2), 2) );

	Round(dist, 1);

	return dist;
}

float Distance2V(Vert3xf *p1, Vert3xf *p2)
{
	float dist;
	dist = sqrt( pow( ( p1->x - p2->x ), 2) + pow( ( p1->y - p2->y ), 2) + pow( ( p1->z - p2->z ), 2) );

	Round(dist, 1);

	return dist;
}

float Distance2V(Vert3xd *p1, Vert3xd *p2)
{
	float dist;
	dist = sqrt( pow( ( float(p1->x) - float(p2->x) ), 2) + pow( ( float(p1->y) - float(p2->y) ), 2) + pow( ( float(p1->z) - float(p2->z) ), 2) );

	Round(dist, 1);

	return dist;
}

float dotProduct(Vert3xf vec1, Vert3xf vec2)		// Two vectors, returns real number
{
	return vec1.x*vec2.x+vec1.y*vec2.y+vec1.z*vec2.z;
}

void radiansAngleV1V2(Vert3xf v1, Vert3xf v2)
{
//	return cosInverse(dotProduct(v1,v2)/((dotProduct(v1,v1)/dotProduct(v1,v1))*(dotProduct(v1,v1)/dotProduct(v1,v1))))
}

void crossProduct3V(Vert3xf v1, Vert3xf v2, Vert3xf v3, Vert3xf *crossProduct)
{
	crossProduct->x = (v2.y-v1.y)*(v3.z-v1.z) - (v2.z-v1.z)*(v3.y-v1.y);
	crossProduct->y = (v2.z-v1.z)*(v3.x-v1.x) - (v2.x-v1.x)*(v3.z-v1.z);
	crossProduct->z = (v2.x-v1.x)*(v3.y-v1.y) - (v2.y-v1.y)*(v3.x-v1.x);
}

void lengthProjectionV1onV2()
{

}

void normalize(Vert2xf *vec)
{
	vec->x = vec->x/(abs(vec->x));
	vec->y = vec->y/(abs(vec->y));
}

void normalize(Vert3xf *vec)
{
	vec->x = vec->x/(abs(vec->x));
	vec->y = vec->y/(abs(vec->y));
	vec->z = vec->z/(abs(vec->z));
}

void normalize(float *vec)
{
	vec[0] = vec[0]/(abs(vec[0]));
	vec[1] = vec[1]/(abs(vec[1]));
	vec[2] = vec[2]/(abs(vec[2]));
}

void ZeroVard(int &var, int condition)	                        //'Vard': variable, integer
{																//This is made for rotation variables,
	if(var >= condition)										//so they dont get too big..
		var = 0;
}

/*************** MATRICIES ******************
********************************************/

void InitMat(Mat2x mat)
{
  mat[0][0]=1.0; mat[0][1]=0.0;
  mat[1][0]=0.0; mat[1][1]=1.0;
}

void InitMat(Mat3x mat)
{
  mat[0][0]=1.0; mat[0][1]=0.0; mat[0][2]=0.0;
  mat[1][0]=0.0; mat[1][1]=1.0; mat[1][2]=0.0;
  mat[2][0]=0.0; mat[2][1]=0.0; mat[2][2]=1.0;
}

void InitMat3x(Mat3x mat)
{
  mat[0][0]=1.0; mat[0][1]=0.0; mat[0][2]=0.0;
  mat[1][0]=0.0; mat[1][1]=1.0; mat[1][2]=0.0;
  mat[2][0]=0.0; mat[2][1]=0.0; mat[2][2]=1.0;
}

void InitMat(Mat3x4 mat)
{
  mat[0][0]=1.0; mat[0][1]=0.0; mat[0][2]=0.0; mat[0][3]=0.0;
  mat[1][0]=0.0; mat[1][1]=1.0; mat[1][2]=0.0; mat[0][3]=0.0;
  mat[2][0]=0.0; mat[2][1]=0.0; mat[2][2]=1.0; mat[2][3]=0.0;
}

void CopyMat(Mat2x MatTofill, Mat2x Source)
{
  for (int i=0; i<2; ++i)
   for (int j=0; j<2; ++j)
     MatTofill[i][j] = Source[i][j];
}

void CopyMat(Mat3x MatTofill, Mat3x Source)
{
  for (int i=0; i<3; ++i)
   for (int j=0; j<3; ++j)
     MatTofill[i][j] = Source[i][j];
}

void CopyMat(Mat3x4 MatTofill, Mat3x4 Source)
{
  for (int i=0; i<3; ++i)
   for (int j=0; j<4; ++j)
     MatTofill[i][j] = Source[i][j];
}

void PrintMat(Mat2x mat)
{
    for(int x= 0; x < 2; x++)
    {
        cout << endl;
        for(int y = 0; y < 2; y++)
            cout << mat[x][y] << ",";
    }
}

void PrintMat(Mat3x mat)
{
    for(int x= 0; x < 3; x++)
    {
        cout << endl;
        for(int y = 0; y < 3; y++)
            cout << mat[x][y] << ",";
    }
}

void PrintMat(Mat3x4 mat)
{
    for(int x= 0; x < 3; x++)
    {
        cout << endl;
        for(int y = 0; y < 4; y++)
            cout << mat[x][y] << ",";
    }
}

void MakeArbitraryAxisRotMat3x4(Mat3x4 mat, float Degrees, float axisX, float axisY, float axisZ)// Make a rotation matrix using an specific axis
{
  float radians = Degrees * (PI / 180), Xsq = pow(axisX,2), Ysq = pow(axisY,2), Zsq = pow(axisZ,2);
  float aC = cos(radians), aS = sin(radians), iC = 1 - cos(radians);

  mat[0][0] = Xsq + (1 - Xsq)*aC;			mat[0][1] = iC*axisX*axisY - axisZ*aS;	mat[0][2] = iC*axisX*axisZ + axisY*aS;	mat[0][3] = 0.0;
  mat[1][0] = iC*axisX*axisY + axisZ*aS;	mat[1][1] = Ysq + (1 - Ysq)*aC; 		mat[1][2] = iC*axisY*axisZ - axisX*aS;	mat[1][3] = 0.0;
  mat[2][0] = iC*axisX*axisZ - axisY*aS;	mat[2][1] = iC*axisY*axisZ + axisX*aS; 	mat[2][2] = Zsq + (1 - Zsq)*aC;			mat[2][3] = 0.0;
}

void MakeRotMat3x4(Mat3x4 mat, float Degrees, int x, int y, int z)      // ints to specify axis to rotate around.
{
  float radians;
  radians = Degrees * (PI / 180);

  if(x == 1)                                                            // Rotate around x-axis
  {
    mat[0][0] = 1.0;	mat[0][1] = 0.0;			mat[0][2] = 0.0;			mat[0][3] = 0.0;
    mat[1][0] = 0.0;	mat[1][1] = cos(radians); 	mat[1][2] = -sin(radians);	mat[1][3] = 0.0;
    mat[2][0] = 0.0;	mat[2][1] = sin(radians); 	mat[2][2] = cos(radians);	mat[2][3] = 0.0;
  }

  if(y == 1)
  {
    mat[0][0] = cos(radians);	mat[0][1] = 0.0;	mat[0][2] = sin(radians);	mat[0][3] = 0.0;
    mat[1][0] = 0.0; 			mat[1][1] = 1.0;	mat[1][2] = 0.0;			mat[1][3] = 0.0;
    mat[2][0] = -sin(radians);	mat[2][1] = 0.0;	mat[2][2] = cos(radians);	mat[2][3] = 0.0;
  }

  if(z == 1)
  {
    mat[0][0] = cos(radians);	mat[0][1] = -sin(radians);	mat[0][2] = 0.0;	mat[0][3] = 0.0;
    mat[1][0] = sin(radians);	mat[1][1] = cos(radians);	mat[1][2] = 0.0;	mat[1][3] = 0.0;
    mat[2][0] = 0.0;			mat[2][1] = 0.0;			mat[2][2] = 1.0;	mat[2][3] = 0.0;
  }
}

void MakeRotMat(Mat2x mat, float Rot)
{
  float radians;
  radians = Rot * (PI / 180);

  mat[0][0] = cos(radians); mat[0][1]=  -sin(radians);
  mat[1][0]= sin(radians); mat[1][1]= cos(radians);
}

void MakeRotMat(Mat3x mat, float Rot)
{
  float radians;
  radians = Rot * (PI / 180);

  mat[0][0] = cos(radians); mat[0][1]=  -sin(radians);
  mat[1][0]= sin(radians); mat[1][1]= cos(radians);
  mat[2][0]=0.0; mat[2][1]=0.0; mat[2][2]=1.0;
}

void MultMat3x4xV(Mat3x4 mat, Vert3xf *vert)
{
    float tempX, tempY, tempZ;

    tempX = (vert->x * mat[0][0]) + (vert->y * mat[0][1]) + (vert->z * mat[0][2]) + mat[0][3];
    tempY = (vert->x * mat[1][0]) + (vert->y * mat[1][1]) + (vert->z * mat[1][2]) + mat[1][3];
    tempZ = (vert->x * mat[2][0]) + (vert->y * mat[2][1]) + (vert->z * mat[2][2]) + mat[2][3];

    vert->x = tempX;
    vert->y = tempY;
    vert->z = tempZ;
}

void MultMat3x4xVArr(Mat3x4 mat, Vert3xf vertArr[], int numVerts)
{
    float tempX, tempY, tempZ;

    for(int x = 0; x < numVerts; x++)
    {
        tempX = (vertArr[x].x * mat[0][0]) + (vertArr[x].y * mat[0][1]) + (vertArr[x].z * mat[0][2]) + mat[0][3];
        tempY = (vertArr[x].x * mat[1][0]) + (vertArr[x].y * mat[1][1]) + (vertArr[x].z * mat[1][2]) + mat[1][3];
        tempZ = (vertArr[x].x * mat[2][0]) + (vertArr[x].y * mat[2][1]) + (vertArr[x].z * mat[2][2]) + mat[2][3];

        vertArr[x].x = tempX;
        vertArr[x].y = tempY;
        vertArr[x].z = tempZ;
    }
}

void MultMat3x(Mat3x product, Mat3x matrix1, Mat3x matrix2)
{
  for (int x=0; x<3; ++x)
    for (int y=0; y<3; ++y)
    {
      float RowAdd = 0;
      for (int z=0; z<3; ++z)
        RowAdd += matrix1[x][z] * matrix2[z][y];
      product[x][y] = RowAdd;
    }
}

void MultMat2x(Mat2x product, Mat2x matrix1, Mat2x matrix2)
{
  for (int x=0; x<2; ++x)
    for (int y=0; y<2; ++y)
    {
      float RowAdd = 0;
      for (int z=0; z<2; ++z)
        RowAdd += matrix1[x][z] * matrix2[z][y];
      product[x][y] = RowAdd;
    }
}

void MultMat2x1(Vert2x& product,Mat2x matrix, Vert2x& vertex)
{
    float temp;
    temp = vertex.x;

    product.x = (vertex.x * matrix[0][0]) + (vertex.y * matrix[0][1]);
    product.y = (temp * matrix[1][0]) + (vertex.y * matrix[1][1]);
}


/*************** TRANSFORM ******************
********************************************/

void TranslateV1x(float xTrans, float yTrans, Vert2x& vertex)
{
    vertex.x = vertex.x + xTrans;
    vertex.y = vertex.y + yTrans;
}

void TranslatePolyV2x(float xTrans, float yTrans, int NumVerts, Vert2x Verts[])
{
    for(int x = 0; x < NumVerts; x++)
    {
        Verts[x].x = Verts[x].x + xTrans;
        Verts[x].y = Verts[x].y + yTrans;
    }
}

void SubtractPtPoly(float Px, float Py, int NumVerts, Vert2x Verts[])
{
    for(int x = 0; x < NumVerts; x++)
    {
        Verts[x].x -= Px;
        Verts[x].y -= Py;
    }
}

void AddPtPoly(float Px, float Py, int NumVerts, Vert2x Verts[])
{
    for(int x = 0; x < NumVerts; x++)
    {
        Verts[x].x += Px;
        Verts[x].y += Py;
    }
}

void RotatePoly2x(float Degrees, int NumVerts, Vert2x Verts[])
{
	float radians,temp;
    radians = Degrees * (PI / 180);

	for(int x = 0; x < NumVerts; x++)
	{
	    temp = Verts[x].x; //Avoid error when x coord transformed.

	    Verts[x].x = (cos(radians) * Verts[x].x) - (sin(radians) * Verts[x].y);
	    Verts[x].y = (sin(radians) * temp) + (cos(radians) * Verts[x].y);
	}

}

void RotatePolyPt(float Px, float Py, float Degrees, int NumVerts, Vert2x Verts[])
{
	float radians,temp;
    radians = Degrees * (PI / 180);

	for(int x = 0; x < NumVerts; x++)
	{
	    temp = (Verts[x].x -= Px); //Avoid error when x coord transformed.
        //Verts[x].x -= Px;
        Verts[x].y -= Py;

        Verts[x].x = (cos(radians) * Verts[x].x) - (sin(radians) * Verts[x].y);
	    Verts[x].y = (sin(radians) * temp) + (cos(radians) * Verts[x].y);

	    Verts[x].x += Px;
	    Verts[x].y += Py;
	}
}

/** DRAWING FUNCS ***********
*****************************/
void DrawCubeNoBottom(void)												// Draw cube without bottom face, performance++
{
    glShadeModel(SHADE ? GL_FLAT : GL_SMOOTH); 							//	if true -> flat, false -> smooth

	// Front face.
    glBegin(GL_QUADS);
        glVertex3i( 0, 0, 0);											// Bottom left
        glVertex3i( 10, 0, 0);											// Bottom right
        glVertex3i( 10, 10, 0);											// Top right
        glVertex3i( 0, 10, 0);											// Top left
    glEnd();

	// Right face.
    glBegin(GL_QUADS);
        glVertex3i( 10, 0, 0);											// Bottom left
        glVertex3i( 10, 0, -10);										// Bottom right
        glVertex3i( 10, 10, -10);										// Top right
        glVertex3i( 10, 10, 0);											// Top left
    glEnd();

	// Back face.
    glBegin(GL_QUADS);
        glVertex3i( 0, 0, 0);											// Bottom left
        glVertex3i( 10, 0, 0);											// Bottom right
        glVertex3i( 10, 0, -10);										// Top right
        glVertex3i( 0, 0, -10);											// Top left
    glEnd();

	// Left face.
    glBegin(GL_QUADS);
        glVertex3i( 0, 0, -10);											// Bottom left
        glVertex3i( 0, 0, 0);											// Bottom right
        glVertex3i( 0, 10, 0);											// Top right
        glVertex3i( 0, 10, -10);										// Top left
    glEnd();

	// Top face.
    glBegin(GL_QUADS);
        glVertex3i( 0, 10, 0);											// Bottom left
        glVertex3i( 10, 10, 0);											// Bottom right
        glVertex3i( 10, 10, -10);											// Top right
        glVertex3i( 0, 10, -10);										// Top left
    glEnd();
}

void DrawCube(void)														// Draw cube
{
    //glShadeModel(SHADE ? GL_FLAT : GL_SMOOTH); 							//	if true -> flat, false -> smooth

	// Front face.
    glBegin(GL_QUADS);
        glVertex3i( 0, 0, 0);											// Bottom left
        glVertex3i( 10, 0, 0);											// Bottom right
        glVertex3i( 10, 10, 0);											// Top right
        glVertex3i( 0, 10, 0);											// Top left
    glEnd();

	// Right face.
    glBegin(GL_QUADS);
        glVertex3i( 10, 0, 0);											// Bottom left
        glVertex3i( 10, 0, -10);										// Bottom right
        glVertex3i( 10, 10, -10);										// Top right
        glVertex3i( 10, 10, 0);											// Top left
    glEnd();

	// Back face.
    glBegin(GL_QUADS);
        glVertex3i( 0, 0, 0);											// Bottom left
        glVertex3i( 10, 0, 0);											// Bottom right
        glVertex3i( 10, 0, -10);										// Top right
        glVertex3i( 0, 0, -10);											// Top left
    glEnd();

	// Left face.
    glBegin(GL_QUADS);
        glVertex3i( 0, 0, -10);											// Bottom left
        glVertex3i( 0, 0, 0);											// Bottom right
        glVertex3i( 0, 10, 0);											// Top right
        glVertex3i( 0, 10, -10);										// Top left
    glEnd();

	// Top face.
    glBegin(GL_QUADS);
        glVertex3i( 0, 10, 0);											// Bottom left
        glVertex3i( 10, 10, 0);											// Bottom right
        glVertex3i( 10, 10, -10);											// Top right
        glVertex3i( 0, 10, -10);										// Top left
    glEnd();

	// Bottom face.
    glBegin(GL_QUADS);
        glVertex3i( 0, 0, 0);											// Bottom left
        glVertex3i( 10, 0, 0);											// Bottom right
        glVertex3i( 10, 0, -10);										// Top right
        glVertex3i( 0, 0, -10);										// Top left
    glEnd();
}

void DrawRecPtDim(int xBottomLeft, int yBottomLeft, int width, int height)     // Point and dimensions.
{
    glBegin(GL_QUADS);
        glVertex3f( xBottomLeft, yBottomLeft, 0.0f);                            // Bottom left.
        glVertex3f( xBottomLeft + width, yBottomLeft, 0.0f);                   // Bottom Right.
        glVertex3f( xBottomLeft + width, yBottomLeft + height, 0.0f);          // Top Right.
        glVertex3f( xBottomLeft, yBottomLeft + height, 0.0f);                   // Top Left.
    glEnd();
}

void DrawStencilRect(int xBottomLeft, int yBottomLeft, int width, int height)    // Point and dimensions, rect not filled in
{
    glBegin(GL_LINE_STRIP);
        glVertex3f( xBottomLeft, yBottomLeft, 0.0f);                            // Bottom left.
        glVertex3f( xBottomLeft + width, yBottomLeft, 0.0f);                   // Bottom Right.
        glVertex3f( xBottomLeft + width, yBottomLeft + height, 0.0f);          // Top Right.
        glVertex3f( xBottomLeft, yBottomLeft + height, 0.0f);                   // Top Left.
        //Connec to first point
        glVertex3f( xBottomLeft, yBottomLeft, 0.0f);                            // Bottom left.
    glEnd();
}

void DrawRectanlge(float xLeft, float xRight, float yBottom, float yTop)
{
    glBegin(GL_QUADS);
        glVertex3f( xLeft, yBottom, 0.0f);
        glVertex3f( xRight, yBottom, 0.0f);
        glVertex3f( xRight, yTop, 0.0f);
        glVertex3f( xLeft, yTop, 0.0f);
    glEnd();
}

void DrawRectanlgeWithInset(float xLeft, float xRight, float yBottom, float yTop, float inset)
{
	// Outer rectangle;
	glBegin(GL_QUADS);
        glVertex3f( xLeft, yBottom, 0.0f);
        glVertex3f( xRight, yBottom, 0.0f);
        glVertex3f( xRight, yTop, 0.0f);
        glVertex3f( xLeft, yTop, 0.0f);
    glEnd();

    // Inner rectangle;
    glColor3f(1.0,1.0,1.0);
	glColor3f(0.0, 0.0, 1.0);									// Inset has to be different colour, so can see
    glBegin(GL_QUADS);
        glVertex3f( xLeft + inset, yBottom + inset, 0.0f);
        glVertex3f( xRight - inset, yBottom + inset, 0.0f);
        glVertex3f( xRight - inset, yTop - inset, 0.0f);
        glVertex3f( xLeft + inset, yTop - inset, 0.0f);
    glEnd();

}

void DrawUnitSquare(void)
{
    glBegin(GL_QUADS);
        glVertex3f( 0.0f, 0.0f, 0.0f);
        glVertex3f( 1.0f, 0.0f, 0.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
    glEnd();
}

void DrawSquareCentr(int CentrX, int CentrY, int SPAN, RGB Colour)		// Specify center of rectangle, span = 1/2 width, colour
{
    glColor3f(Colour[0],Colour[1],Colour[2]);
    glBegin(GL_QUADS);

        glVertex3f( float(CentrX - SPAN), float(CentrY + SPAN), 0.0f);
        glVertex3f(float(CentrX - SPAN),float(CentrY - SPAN), 0.0f);
        glVertex3f(float(CentrX + SPAN),float(CentrY - SPAN), 0.0f);
        glVertex3f( float(CentrX + SPAN), float(CentrY + SPAN), 0.0f);

    glEnd();
}

void DrawGrid(int HEIGHT, int WIDTH, int INCREMENT)		// Draws a grid
{
    // Draw horizontal lines(size of width, spanning height in INCREMENT increments):
	for(int z = 0; z <= HEIGHT; z+=INCREMENT)
    {
        glBegin(GL_LINES);		// Line from vertice 1 to vertice 2
            glVertex3d( 0, 0, z);
            glVertex3d( WIDTH, 0, z);
        glEnd();
    }

    // Draw vertical lines(size of height, spanning width in INCREMENT increments):
    for(int x = 0; x <= WIDTH; x+=INCREMENT)
    {
        glBegin(GL_LINES);
            glVertex3d( x, 0, 0);
            glVertex3d( x, 0, HEIGHT);
        glEnd();
    }
}

void DrawGridofSquares(int WIDTH, int DIVISIONS)		// Draw grid of expanding squares.., looks neato
{
	// Setting divisions high has a scale effect.., put to 1
	//DIVISIONS = 1;

	for(int i = 0; i < WIDTH; i++)
    {
        glBegin(GL_LINE_STRIP);
            glVertex3f((i*-DIVISIONS), 0.0f,i*-DIVISIONS);
            glVertex3f(i*-DIVISIONS, 0.0f, i*DIVISIONS);
            glVertex3f(i*DIVISIONS, 0.0f, i*DIVISIONS);
            glVertex3f(i*DIVISIONS, 0.0f, i*-DIVISIONS);
            glVertex3f(i*-DIVISIONS, 0.0f, i*-DIVISIONS);
        glEnd();
    }
}

void DrawPoly2x(int NumVerts, Vert2x Verts[])
{
    glBegin(GL_POLYGON);
        for(int x = 0; x < NumVerts; x++)
            glVertex3f(Verts[x].x,Verts[x].y,0.0);
    glEnd();
}

void DrawTriangle()
{
    glShadeModel(SHADE ? GL_FLAT : GL_SMOOTH); //if true -> flat, false -> smooth

	glBegin(GL_TRIANGLES);
		glVertex3f( -0.5f, 0.0f, 0.0f);
		glVertex3f( 0.5f, 0.0f, 0.0f);
		glVertex3f( 0.0f, 1.0f, 0.0f);
	glEnd();
}

void MidPointV(float* VArray1[3],float* VArray2[3], float* MidPt[3]) //Pass the array name only. wont work properly otherwise.
{
    //VArray[3] is the first coord of a line, VArray[3] is the second
    //MidPt[3] is the midpoint coord to be filled.
    //-------------------
    //Get midpoint coord.

    //MidPt coord 1(the x coord):

    *MidPt[0] = ((*VArray1[0] + *VArray2[0]) / 2);
    *MidPt[1] = ((*VArray1[1] + *VArray2[1]) / 2);
}

void MidPointf(float V1XCoord,float V1YCoord, float V2XCoord, float V2YCoord, float* MidPt[3]) //Pass vertexes, 2 pairs.
{
    *MidPt[0] = ((V1XCoord + V1YCoord) / 2);
    *MidPt[1] = ((V2XCoord + V2YCoord) / 2);
}




// Copy pastes of snippets
/*
 *
 *     glBegin(GL_LINES);
 *     // First line:
        glVertex3d(-100, 0.0f, -100);
        glVertex3d( -100, 0.0f, 100);
        // Second Line:
        glVertex3d( 100, 0.0f, 100);
        glVertex3d( 100, 0.0f, -100);
    glEnd();

    // v for vector, accepts pointer to a pointer to a vector (or array) of values,
    // rather than a series of individual arguments.

    GLfloat color_array[] = {1.0, 0.0, 0.0};
	glColor3fv(color_array);






 */


/*
typedef struct
{
	Vert3xf Verts[30];

	    //Define Object
    MyObj.Vert[0].x = (MyObj.gunSize * -1);  MyObj.Vert[0].y = 8.5;
    MyObj.Vert[1].x = (MyObj.gunSize * -1);  MyObj.Vert[1].y = -3.5;
    MyObj.Vert[2].x = -4.0;                  MyObj.Vert[2].y = -3.5;  //.x was origonally 5, but then there was a bit of a flicker
    MyObj.Vert[3].x = -4.0;                  MyObj.Vert[3].y = 8.5;

    MyObj.Vert[4].x = -5.0;                  MyObj.Vert[4].y = 1.0;
    MyObj.Vert[5].x = -5.0;                  MyObj.Vert[5].y = -6.0;
    MyObj.Vert[6].x = 5.0;                   MyObj.Vert[6].y = -6.0;
    MyObj.Vert[7].x = 5.0;                   MyObj.Vert[7].y = 1.0;

    MyObj.Vert[8].x = 4.0;                   MyObj.Vert[8].y = 8.5;    //.x was origonally 5, but then there was a bit of a flicker
    MyObj.Vert[9].x = 4.0;                   MyObj.Vert[9].y = -3.5;
    MyObj.Vert[10].x = MyObj.gunSize;        MyObj.Vert[10].y = -3.5;
    MyObj.Vert[11].x = MyObj.gunSize;        MyObj.Vert[11].y = 8.5;


}Enemy;
*/

/*                            OLD OBJECT DEFINITION
                              OLD OBJECT DEFINITION
                              OLD OBJECT DEFINITION


typedef struct objA4x
{
    Vert2x Vert[12];
    float transSpd, radius, Tangle, gunSize, bodySize;  //Translation speed. Radius: used to make a pivot kinda thing..
                                // Tangle: angle to rotate-translate

    bool trans, stop;           // Translate or not? stop: imediate stop of trans.
    float DirFacX,DirFacY;      // DirFac: determines the direction in which translation occurs
    int check, Cntrl;           // Used to make sure both X and Y require no more translation
    int Direction[4];           // Will hold the directions left,top,right,bottom
    float sizeCmp[2][2], Direc; // Used to slowly increase size of Gun[0], and Body [1]


    float TransDur[2][2];   // Vars where anim translations are recorded.
                            // First dimension is where current trans recorded, and second
                            // dimension is where max or increment to is recorded.

    void setVals(bool T = false, float TDur0x0 = 0.1,float TDur1x0 = 0.1, float tSpd = 0.02, bool S = false, float gSize = 8, float bSize = 5, int Ctrl = 0)
    {
        TransDur[0][0] = TDur0x0;
        TransDur[1][0] = TDur1x0;
        transSpd = tSpd;
        trans = T;
        stop = S;
        gunSize = gSize;
        bodySize = bSize;
        Cntrl = Ctrl;
    }

    void GetCentr(float *X, float *Y)
    {
        *X = Vert[6].x - 5;
        *Y = Vert[6].y + 3.5;
    }

    void updateShp(objA4x& MyObj )
    {
        MyObj.Vert[0].x += 0.6 * (MyObj.Direc * -1);//(MyObj.gunSize * -1);
        MyObj.Vert[1].x += 0.6  * (MyObj.Direc * -1); //(MyObj.gunSize * -1);

        MyObj.Vert[10].x += 0.6  * MyObj.Direc; //MyObj.gunSize;
        MyObj.Vert[11].x += 0.6  * MyObj.Direc; //MyObj.gunSize;
    }

    void Translate()
    {
        if(trans == 1)
        {
            //if(TransDur[0][1] == 0)


                                    //    The reason why theres two checks
                                    //    instead of both in one if is because:
                                    //    Had trouble when translating a coord of
                                    //    0, then check would fail, and no translation
                                    //    would occur.

            if(TransDur[0][0] >= TransDur[0][1] && !(DirFacX == 0))
            {
                TransDur[0][0] = 0;  //Set X current, and max to zero
                TransDur[0][1] = 0;

                check++;
            }
            else if(TransDur[1][0] >= TransDur[1][1] && !(DirFacY == 0))
            {
                TransDur[1][0] = 0;  // Set Y current, and max to zero
                TransDur[1][1] = 0;

                check++;

// TODO (Owner#1#): Maybe increment in the four different directions... and when incremented too far, stop?
// TODO (Owner#1#): Have a counter, assign it what ever you want the translation to go up by, translate with the counter, increment the counter, Check if counter is greater than or equal to the desired..

            }

            if(Cntrl == 1)
                if(check == 2)
                    trans = false;

            if(stop )
            {
                trans = false;
            }

            for(int x = 0; x < 12; x++)
            {
                Vert[x].x += (transSpd * DirFacX);
                Vert[x].y += (transSpd * DirFacY);
            }

            if(DirFacX != 0)
                TransDur[0][0] += (0.01 * DirFacX);
            if(DirFacY != 0)
                TransDur[1][0] += (0.01 * DirFacY);
        }
    }

    void RotateObjCentr(float Degrees)
    {
        float radians,temp, X, Y;
        radians = Degrees * (PI / 180);

        GetCentr(&X, &Y);

        for(int x = 0; x < 12; x++)
        {
            temp = (Vert[x].x -= X); //Avoid error when x coord transformed.
            //Verts[x].x -= Px;
            Vert[x].y -= Y;

            Vert[x].x = (cos(radians) * Vert[x].x) - (sin(radians) * Vert[x].y);
            Vert[x].y = (sin(radians) * temp) + (cos(radians) * Vert[x].y);

            Vert[x].x += X;
            Vert[x].y += Y;
        }
    }

}objA4x;       //Object Animation 4 verticies

                OLD OBJECT DEFINITION
                OLD OBJECT DEFINITION
                OLD OBJECT DEFINITION


void SetTransObjV4x(float xTrans, float yTrans, objA4x& MyObj)
{
        MyObj.trans = 1;
        MyObj.stop = false;
        MyObj.TransDur[0][1] += xTrans;     //Set max trans
        MyObj.TransDur[1][1] += yTrans;

        if(xTrans == 0)
            MyObj.DirFacX = 0;
        else if(xTrans < 0)
            MyObj.DirFacX = -1;
        else                            // Set certain direction based on specified
            MyObj.DirFacX = 1;          // coords

        if(yTrans == 0)
            MyObj.DirFacY = 0;
        else if(yTrans < 0)
            MyObj.DirFacY = -1;
        else
            MyObj.DirFacY = 1;

        MyObj.TransDur[0][0] = 0.1;     //Set current trans
        MyObj.TransDur[1][0] = 0.1;
}

void DrawObj4x(objA4x MyObj)
{
	 glBegin(GL_QUADS);
        for(int x = 0; x < 12; x++)
            glVertex3f(MyObj.Vert[x].x,MyObj.Vert[x].y,0.0);
    glEnd();
}

*/



#endif /* UTILITY_H_ */
