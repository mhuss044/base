/*
 * CommonTypes.h
 *
 *  Created on: May 27, 2013
 *      Author: EvorateAwMaDemCriAnR
 */

#ifndef COMMONTYPES_H_
#define COMMONTYPES_H_


//*********TYPEDEFS*********
typedef float RGB[3];
typedef float RGBA[4];

typedef float floatArray3[3];
typedef float floatArray4[4];

typedef float Mat3x[3][3];
typedef float Mat2x[2][2];
typedef float Mat3x4[3][4];                                     // To be compatible with translation matrices

typedef struct Vector3f
{
	float e1, e2, e3;		                                    //Elements 1, 2, 3
}Vec3f;

typedef struct Vector3d
{
	int e1, e2, e3;		     	                                //Elements 1, 2, 3
}Vec3d;

typedef struct Vector2d
{
	int e1, e2;			                                    	//Elements 1, 2
}Vec2d;

typedef struct _Vert3xf
{
    float x,y,z;
}Vert3xf;

typedef struct _Vert3xd
{
    int x,y,z;
}Vert3xd;

typedef struct _Vert2xd				                            //Vertex 2x: 2D, d: integer
{
    int x,y;
}Vert2xd;

typedef struct _Vert2x
{
    float x,y;
}Vert2x;

typedef struct _Vert2xf
{
    float x,y;
}Vert2xf;

typedef struct _point
{
  float x,y;
  //point(){x=y=0.0;                                            //Shud include this?
} Point2x;

typedef struct _Face4xV				   //A three vertice face. For triangular faces.
{
	Vert3xf v1, v2, v3, v4;
}Face4xV;

/*
typedef struct Face3x				   //A three vertice face. For triangular faces.
{
	int a1, b2, c3, id;				   //NO NEED FOR THIS
}Face3x;
*/
typedef struct _Face4x				   //Type to hold faces.
{
	int a1[2], b2[2], c3[2], d4[2];	   //The elements are individual verticies,
									   //Two elements: [0]: Vertex number(1-numVerts),
									   //[1]:Texture Coord num(1-4)
	GLint tId;						   //tId: is the faces' texture id.
	char cnfg;						   //cnfg: either P: polygon, or T: triangle. (its the face type)
}Face4x;

typedef struct _vertexList				// Holds a list of verticies, texture coordinates, and colour values
{
	float *vertexList, *textureCoordList, *colourValueList;

	_vertexList(void)
	{
		vertexList = NULL;
		textureCoordList = NULL;
		colourValueList = NULL;
	}
}vertexList;

typedef struct _drawableObj
{
	vertexList vertList;
	int *triFaceVerts, *quadFaceVerts, *textureCoordVerts, *colourValVerts;		// Index values to verticies in list
	int glTexID;

	_drawableObj(void)
	{
		triFaceVerts = NULL;
		quadFaceVerts = NULL;
		textureCoordVerts = NULL;
		colourValVerts = NULL;

		glTexID = -1;
	}
}drawableObj;

typedef struct _Bounds
{
	float minX, maxX, minY, maxY, minZ, maxZ;
}Bounds;


#endif /* COMMONTYPES_H_ */
