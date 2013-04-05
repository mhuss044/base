/*
 * CCamera.h
 *
 *  Created on: Aug 20, 2012
 *      Author: hp
 */

#ifndef CCAMERA_H_
#define CCAMERA_H_

#define CAMERA_ZOOM 1000                                          	// Cam distance from refpt
#define CAMERA_TRANSLATE_SPEED	3									// Cam move speed (wasd), remember, depends on fps: high fps = more translate/sec and vice versa
#define CAMERA_ROTATE_DEGREES	0.1									// Cam degress to rorate, remember, depends on fps: high fps = more translate/sec and vice versa

// Camera class.
// TODO: rotate at extreme y, low y: rotate around right/left vec; as of now looks like viewpt rot around circle infront??

enum moveDirections
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT
};

class CCamera
{
public:
    Vert3xf camPos, viewPt;				// viewPt; point at which cam rotates around, cam looks at
    Vert3xf camForwardVec, camRightVec; // Vector added to camPos when translate pos, rotated when ever rotate viewing window (mouse, arrow keys)
    Mat3x4 camPitch, camYaw, camRoll;	// Matrices holding rotation of 1 deg in each respective axis; mult by pos/viewpt vectors to rotate view window

    CCamera(void);
    void translatePos(float x, float y, float z);
    void Move(moveDirections direction);
    void scalePos(float k);                                             // Scale/Zoom
    void Pitch(float degrees);    				                        // Rotate about the x-axis
    void Yaw(float degrees);             			                    // Rotate about the y-axis
    void Roll(float degrees);         				                    // Rotate about the z-axis
    void processCamera(int x, int y);
};

CCamera::CCamera(void)                      	                        // Initialize.
{
    // Init mats;
	InitMat(camPitch);
    InitMat(camYaw);
    InitMat(camRoll);

	viewPt.x = GAME_MAP_SIZE_X/2;										// Mid map
	viewPt.y = CAMERA_ZOOM;
	viewPt.z = GAME_MAP_SIZE_Z/2;

    camPos.x = viewPt.x + CAMERA_ZOOM;
    camPos.y = viewPt.y + CAMERA_ZOOM;
    camPos.z = viewPt.z + CAMERA_ZOOM;

    // Get normal pointing towards viewPt from camPos;
    camForwardVec.x = (viewPt.x - camPos.x)/abs(int(viewPt.x - camPos.x));
    camForwardVec.y = (viewPt.y - camPos.y)/abs(int(viewPt.y - camPos.y));
    camForwardVec.z = (viewPt.z - camPos.z)/abs(int(viewPt.z - camPos.z));

    // Right dir vec is 90 from forward vec; must be better way doing this, get rightvec from forward?
    //		so instead of multmat for right vec, maybe derive it from forward?!?
    camRightVec.x = camForwardVec.x;
    camRightVec.y = camForwardVec.y;
    camRightVec.z = camForwardVec.z;

    MakeRotMat3x4(camYaw, 90, 0, 1, 0);                                 // 90 Degree, y-axis
    MultMat3x4xV(camYaw, &camRightVec);

    //cout << INS << "Init cam. : forwad, right: " << camForwardVec.x << ", " << camForwardVec.z;
    //cout << INS << camRightVec.x << camRightVec.z;
    //cin.get();
}

void CCamera::translatePos(float x, float y, float z)
{
	// Move the camera position;
    camPos.x += x;
    camPos.y += y;
    camPos.z += z;
    // Move the view point;
    viewPt.x += x;
    viewPt.y += y;
    viewPt.z += z;
}

void CCamera::Move(moveDirections direction)
{
	if(direction == FORWARD)
	{
		// Move the camera position;s
		camPos.x += camForwardVec.x*CAMERA_TRANSLATE_SPEED;
		camPos.y += camForwardVec.y*CAMERA_TRANSLATE_SPEED;
		camPos.z += camForwardVec.z*CAMERA_TRANSLATE_SPEED;

		// Move the view point;
		viewPt.x += camForwardVec.x*CAMERA_TRANSLATE_SPEED;
		viewPt.y += camForwardVec.y*CAMERA_TRANSLATE_SPEED;
		viewPt.z += camForwardVec.z*CAMERA_TRANSLATE_SPEED;
	}
	else if(direction == BACKWARD)
	{
		// Move the camera position;s
		camPos.x += -camForwardVec.x*CAMERA_TRANSLATE_SPEED;
		camPos.y += -camForwardVec.y*CAMERA_TRANSLATE_SPEED;
		camPos.z += -camForwardVec.z*CAMERA_TRANSLATE_SPEED;

		// Move the view point;
		viewPt.x += -camForwardVec.x*CAMERA_TRANSLATE_SPEED;
		viewPt.y += -camForwardVec.y*CAMERA_TRANSLATE_SPEED;
		viewPt.z += -camForwardVec.z*CAMERA_TRANSLATE_SPEED;
	}
	else if(direction == RIGHT)
	{
		// Move the camera position;s
		camPos.x += -camRightVec.x*CAMERA_TRANSLATE_SPEED;
//		camPos.y += -camRightVec.y*CAMERA_TRANSLATE_SPEED;
		camPos.z += -camRightVec.z*CAMERA_TRANSLATE_SPEED;

		// Move the view point;
		viewPt.x += -camRightVec.x*CAMERA_TRANSLATE_SPEED;
//		viewPt.y += -camRightVec.y*CAMERA_TRANSLATE_SPEED;
		viewPt.z += -camRightVec.z*CAMERA_TRANSLATE_SPEED;
	}
	else if(direction == LEFT)
	{
		// Move the camera position;s
		camPos.x += camRightVec.x*CAMERA_TRANSLATE_SPEED;
//		camPos.y += camRightVec.y*CAMERA_TRANSLATE_SPEED;
		camPos.z += camRightVec.z*CAMERA_TRANSLATE_SPEED;

		// Move the view point;
		viewPt.x += camRightVec.x*CAMERA_TRANSLATE_SPEED;
//		viewPt.y += camRightVec.y*CAMERA_TRANSLATE_SPEED;
		viewPt.z += camRightVec.z*CAMERA_TRANSLATE_SPEED;
	}
}

void CCamera::scalePos(float k)
{
    camPos.x += k*CAMERA_ZOOM;
    camPos.y += k*CAMERA_ZOOM;
    camPos.z += k*CAMERA_ZOOM;
}

void CCamera::Pitch(float degrees)										// Rotate viewPt degrees around camPos
{
	viewPt.x -= camPos.x;
	viewPt.y -= camPos.y;
	viewPt.z -= camPos.z;

	MakeRotMat3x4(camPitch, degrees, 1, 0, 0);
	MultMat3x4xV(camPitch, &viewPt);                                    // Apply transform to viewpt.
	MultMat3x4xV(camPitch, &camForwardVec);                   		    // Apply transform to forward vector.
	MultMat3x4xV(camPitch, &camRightVec);                  				// Apply transform to right vector.

	viewPt.x += camPos.x;
	viewPt.y += camPos.y;
	viewPt.z += camPos.z;
}

void CCamera::Yaw(float degrees)										// Rotate viewPt degrees around camPos
{
	viewPt.x -= camPos.x;
	viewPt.y -= camPos.y;
	viewPt.z -= camPos.z;

	MakeRotMat3x4(camYaw, degrees, 0, 1, 0);
	MultMat3x4xV(camYaw, &viewPt);                                      // Apply transform.
	MultMat3x4xV(camYaw, &camForwardVec);                       		// Apply transform to forward vector.
	MultMat3x4xV(camYaw, &camRightVec);                     			// Apply transform to right vector.

	viewPt.x += camPos.x;
	viewPt.y += camPos.y;
	viewPt.z += camPos.z;
}

void CCamera::Roll(float degrees)										// Rotate viewPt degrees around camPos
{
	viewPt.x -= camPos.x;
	viewPt.y -= camPos.y;
	viewPt.z -= camPos.z;

	MakeRotMat3x4(camRoll, degrees, 0, 0, 1);
	MultMat3x4xV(camRoll, &viewPt);                                	    // Apply transform.
	MultMat3x4xV(camRoll, &camForwardVec);        			            // Apply transform to forward vector.
	MultMat3x4xV(camRoll, &camRightVec);                     	   	    // Apply transform to right vector.

	viewPt.x += camPos.x;
	viewPt.y += camPos.y;
	viewPt.z += camPos.z;
}

void CCamera::processCamera(int x, int y)	// Rotate using info from mouse input
{
	static int prevX = WINWidth/2, prevY = WINHeight/2;					// Cursor origin center screen
//	static int prevX = x, prevY = y;					// Cursor origin center screen


	// Auto rotate:
	if(y > (WINHeight - 50))											// Extreme bottom edge
		Roll(0.1);
	if(y < 50)															// Extreme top edge
		Roll(-0.1);
    if(x > (WINWidth - 50))												// Extreme right edge
		Yaw(-0.1);
	if(x < 50)															// Extreme left edge
		Yaw(0.1);

	// Rotate by magnitude of mouse movement:
	// Mouse going up.
	Pitch(-0.1*(prevY-y));                         			            // Rotate 1 degree counter-clockwise.
	// Mouse moving left.
	Yaw(0.1*(prevX-x));                           			            // Rotate 1 degree clockwise.

    prevX = x;
    prevY = y;
}

#endif /* CCAMERA_H_ */
