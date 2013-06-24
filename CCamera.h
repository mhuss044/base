/*
 * CCamera.h
 *
 *  Created on: Aug 20, 2012
 *      Author: hp
 *
 *
 *		Translate left/right seems to degrade after a while? or under some condition?
 *		Performance++; MakeRoteMat(), store em, instead of make new one every req for rotation
 *		Support camera animation
 *		Combine all rotation matricies to one;
 *
 *		roll/pitch operations transform forward vec, right vec no longer corresponds
 *			- recalc right vec every roll/pitch operation; costly
 *			- have readjust func; checks angle b/w fwd vec and rightvec->inverts right vec
 *
 *		strat;
 *			define 3 axes; forward, right, up
 *			Yaw operation; rot fwd/right about up axis
 *			Pitch operation; rot fwd/up about right axis
 *			Roll operation; rot up/right about fws axis
 *			axes must be normalized after each operation?
 *
 *			rot axis about axis?
 *
 *			build flying game; ++fwd vec
 *
 *		build scripted cam; cinematics gets
 *			transcript class; holds sequence of operations; move_fwd over 5s, rot 40deg over 2s, then moveright and pitch down over 3s
 */

#ifndef CCAMERA_H_
#define CCAMERA_H_

#define CAMERA_ZOOM 1000                                          	// Cam distance from refpt
#define CAMERA_TRANSLATE_SPEED	3									// Cam move speed (wasd), remember, depends on fps: high fps = more translate/sec and vice versa
#define CAMERA_ROTATE_DEGREES 0.1									// Cam degress to rorate, remember, depends on fps: high fps = more translate/sec and vice versa

enum moveDirections													// Directions to translate camera
{
	CAMERA_MOVE_FORWARD = 0,
	CAMERA_MOVE_BACKWARD,
	CAMERA_MOVE_LEFT,
	CAMERA_MOVE_RIGHT
};

enum lookOperations													// Directions to rotate camera view
{
	CAMERA_LOOK_UP = 0,
	CAMERA_LOOK_DOWN,
	CAMERA_LOOK_LEFT,
	CAMERA_LOOK_RIGHT,
	CAMERA_LOOK_ROLL_CWISE,
	CAMERA_LOOK_ROLL_CCWISE
};

// FIXED PITCH CAMERA
// Camera class; allows
//		free yaw, restricted pitch, manipulation of camPos
//		Must use ::Look when request rotation operation to be under restricitons, YAW/Roll/Pitch arent restricted
//		May want to disable proccessCamera(): rotate cam view via mouse. bcs messes up the restricted look axis
// FIXED PITCH CAMERA
class CFixedPitchCamera
{
public:
    Vert3xf camPos, viewPt;							// viewPt; point at which cam rotates around, cam looks at
    Vert3xf camForwardVec, camRightVec, camUpVec;	// Vector added to camPos when translate pos, rotated when ever rotate viewing window (mouse, arrow keys)
    Mat3x4 camPitch, camYaw, camRoll;				// Matrices holding rotation of 1 deg in each respective axis; mult by pos/viewpt vectors to rotate view window
    float maxPitch, rotationSpeed, translateSpeed;					// Greatest angle at which user can look up/down

    CFixedPitchCamera(float camViewPtX, float camViewPtY, float camViewPtZ, float pitchMax, float rotSpeed, float transSpeed);
    void translatePos(float x, float y, float z);
    void Move(moveDirections direction);
    void Look(lookOperations operation);
    void scalePos(float k);                                             // Scale/Zoom
    void Pitch(float degrees);    				                        // Rotate about the x-axis
    void Yaw(float degrees);             			                    // Rotate about the y-axis
    void Roll(float degrees);         				                    // Rotate about the z-axis
    void RotAboutRightVec(float degrees);								// Rotate about right vector
    void processCamera(int x, int y);
    void reportData(void);
};

CFixedPitchCamera::CFixedPitchCamera(float camViewPtX, float camViewPtY, float camViewPtZ, float pitchMax, float rotSpeed = CAMERA_ROTATE_DEGREES, float transSpeed = CAMERA_TRANSLATE_SPEED)// Initialize.
{
	maxPitch = pitchMax;
	rotationSpeed = rotSpeed;
	translateSpeed = transSpeed;

    // Init mats;
	InitMat(camPitch);
    InitMat(camYaw);
    InitMat(camRoll);

	viewPt.x = camViewPtX;										// Mid map
	viewPt.y = camViewPtY;
	viewPt.z = camViewPtZ;

    camPos.x = viewPt.x + CAMERA_ZOOM;
    camPos.y = viewPt.y + CAMERA_ZOOM;
    camPos.z = viewPt.z + CAMERA_ZOOM;

    // Define up vector
    camUpVec.x = 0.0;
    camUpVec.y = 1.0;
    camUpVec.z = 0.0;

    // Get normal pointing towards viewPt from camPos;
    camForwardVec.x = (viewPt.x - camPos.x)/abs(int(viewPt.x - camPos.x));
    camForwardVec.y = (viewPt.y - camPos.y)/abs(int(viewPt.y - camPos.y));
    camForwardVec.z = (viewPt.z - camPos.z)/abs(int(viewPt.z - camPos.z));

    // Right dir vec is 90 from forward vec; must be better way doing this, get rightvec from forward?
    //		so instead of multmat for right vec, maybe derive it from forward?!?
    camRightVec.x = camForwardVec.x;
    camRightVec.y = camForwardVec.y;
    camRightVec.z = camForwardVec.z;

    MakeRotMat3x4(camYaw, 270, 0, 1, 0);                                 // 270 Degree(RHS wrt forward vec), rotate around y-axis
    MultMat3x4xV(camYaw, &camRightVec);
}

void CFixedPitchCamera::translatePos(float x, float y, float z)
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

void CFixedPitchCamera::Move(moveDirections direction)
{
	if(direction == CAMERA_MOVE_FORWARD)
		translatePos(camForwardVec.x*translateSpeed, camForwardVec.y*translateSpeed, camForwardVec.z*translateSpeed);
	else if(direction == CAMERA_MOVE_BACKWARD)
		translatePos(-camForwardVec.x*translateSpeed, -camForwardVec.y*translateSpeed, -camForwardVec.z*translateSpeed);
	else if(direction == CAMERA_MOVE_RIGHT)		// Involves no translation in y direction
		translatePos(camRightVec.x*translateSpeed, 0, camRightVec.z*translateSpeed);
	else if(direction == CAMERA_MOVE_LEFT)
		translatePos(-camRightVec.x*translateSpeed, 0, -camRightVec.z*translateSpeed);
}

void CFixedPitchCamera::Look(lookOperations operation)
{
	static float angle = 0.0;

	if(operation == CAMERA_LOOK_UP)
	{
		if(angle > maxPitch)
			return;
		else
		{
			angle += rotationSpeed;
			RotAboutRightVec(rotationSpeed);
		}
	}
	else if(operation == CAMERA_LOOK_DOWN)
	{
		if(angle < -maxPitch)
			return;
		else
		{
			angle -= rotationSpeed;
			RotAboutRightVec(-rotationSpeed);
		}
	}
	else if(operation == CAMERA_LOOK_RIGHT)
		Yaw(-rotationSpeed);
	else if(operation == CAMERA_LOOK_LEFT)
		Yaw(rotationSpeed);
}

void CFixedPitchCamera::scalePos(float k)
{
    camPos.x += k*CAMERA_ZOOM;
    camPos.y += k*CAMERA_ZOOM;
    camPos.z += k*CAMERA_ZOOM;
}

void CFixedPitchCamera::Pitch(float degrees)									// Rotate viewPt degrees around camPos
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

void CFixedPitchCamera::Yaw(float degrees)										// Rotate viewPt degrees around camPos
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

void CFixedPitchCamera::Roll(float degrees)										// Rotate viewPt degrees around camPos
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

void CFixedPitchCamera::RotAboutRightVec(float degrees)							// Rotate about right vector, proper nodding effect
{
	viewPt.x -= camPos.x;
	viewPt.y -= camPos.y;
	viewPt.z -= camPos.z;

	// Make a mat to rot about rightVec axis, camPitch is place holder
	MakeArbitraryAxisRotMat3x4(camPitch, degrees, camRightVec.x, camRightVec.y, camRightVec.z);

	MultMat3x4xV(camPitch, &viewPt);                                	// Apply transform.
	MultMat3x4xV(camPitch, &camForwardVec);        			            // Apply transform to forward vector.

	/*
	// Get correct right vec;
	camRightVec.x = camForwardVec.x;
	camRightVec.y = camForwardVec.y;
	camRightVec.z = camForwardVec.z;


	MakeRotMat3x4(camYaw, 270, 0, 1, 0);                                 // 270 Degree(RHS wrt forward vec), rotate around y-axis
	MultMat3x4xV(camYaw, &camRightVec);
*/
	MultMat3x4xV(camPitch, &camRightVec);                     	   	    // Rotate right vec about right vec.. sounds fucked but its necessary, cant figure out why


	viewPt.x += camPos.x;
	viewPt.y += camPos.y;
	viewPt.z += camPos.z;
}

void CFixedPitchCamera::processCamera(int x, int y)	// Rotate using info from mouse input
{
	static int prevX = WINWidth/2, prevY = WINHeight/2;					// Cursor origin center screen
//	static int prevX = x, prevY = y;					// Cursor origin center screen


	// Auto rotate:
	if(y > (resizeWINHeight - 50))											// Extreme bottom edge
		Roll(rotationSpeed);
	if(y < 50)															// Extreme top edge
		Roll(-0.1);
    if(x > (resizeWINWidth - 50))												// Extreme right edge
		Yaw(-rotationSpeed);
	if(x < 50)															// Extreme left edge
		Yaw(rotationSpeed);

	// Rotate by magnitude of mouse movement:
	// Mouse going up.
	Pitch(-0.1*(prevY-y));                         			            // Rotate 1 degree counter-clockwise.
	// Mouse moving left.
	Yaw(0.1*(prevX-x));                           			            // Rotate 1 degree clockwise.

    prevX = x;
    prevY = y;
}

void CFixedPitchCamera::reportData(void)
{
	cout << INS << "Forward vec; " << camForwardVec.x << ", " << camForwardVec.y << ", " << camForwardVec.z;
	cout << INS << "Right vec; " << camRightVec.x << ", " << camRightVec.y << ", " << camRightVec.z;
	cout << INS << "Sum: " << camForwardVec.x + camRightVec.x << ", " << camForwardVec.y + camRightVec.y  << ", " << camForwardVec.z + camRightVec.z;
}

// Arbitrary Axis Camera
//		All axes are manipulated, camPos manip allowed
//		No mouse control allowed; seems too chaotic
//
class CArbAxisCamera
{
public:
	Vert3xf camPos, viewPt;							// viewPt; point at which cam rotates around, cam looks at
    Vert3xf camForwardVec, camRightVec, camUpVec;	// Vector added to camPos when translate pos, rotated when ever rotate viewing window (mouse, arrow keys)
    Mat3x4 rotMat;									// Matrix holding rotation
    float rotationSpeed, translateSpeed;			// Amount to rotate by

    CArbAxisCamera(float camViewPtX, float camViewPtY, float camViewPtZ, float rotSpeed, float transSpeed);
    void translatePos(float x, float y, float z);
    void Move(moveDirections direction);
    void Look(lookOperations operation);
    void scalePos(float k);                                             				// Scale/Zoom
    void ArbAxisRotation(float degrees, Vert3xf axis, Vert3xf *V1, Vert3xf *V2);		// Rotate viewPt and V1/V2 about axis
    void reportData(void);
};

CArbAxisCamera::CArbAxisCamera(float camViewPtX, float camViewPtY, float camViewPtZ, float rotSpeed = CAMERA_ROTATE_DEGREES, float transSpeed = CAMERA_TRANSLATE_SPEED)// Initialize.
{
	rotationSpeed = rotSpeed;
	translateSpeed = transSpeed;

    // Init mats;
	InitMat(rotMat);											// Make identity

	viewPt.x = camViewPtX;										// Mid map
	viewPt.y = camViewPtY;
	viewPt.z = camViewPtZ;

    camPos.x = viewPt.x + CAMERA_ZOOM;
    camPos.y = viewPt.y + CAMERA_ZOOM;
    camPos.z = viewPt.z + CAMERA_ZOOM;

    // Define up vector
    camUpVec.x = 0.0;
    camUpVec.y = 1.0;
    camUpVec.z = 0.0;

    // Get normal pointing towards viewPt from camPos;
    camForwardVec.x = (viewPt.x - camPos.x)/abs(int(viewPt.x - camPos.x));
    camForwardVec.y = (viewPt.y - camPos.y)/abs(int(viewPt.y - camPos.y));
    camForwardVec.z = (viewPt.z - camPos.z)/abs(int(viewPt.z - camPos.z));

    // Right dir vec is 90 from forward vec; must be better way doing this, get rightvec from forward?
    //		so instead of multmat for right vec, maybe derive it from forward?!?
    camRightVec.x = camForwardVec.x;
    camRightVec.y = camForwardVec.y;
    camRightVec.z = camForwardVec.z;

    MakeRotMat3x4(rotMat, 270, 0, 1, 0);                                 // 270 Degree(RHS wrt forward vec), rotate around y-axis
    MultMat3x4xV(rotMat, &camRightVec);
}

void CArbAxisCamera::translatePos(float x, float y, float z)
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

void CArbAxisCamera::Move(moveDirections direction)
{
	if(direction == CAMERA_MOVE_FORWARD)
		translatePos(camForwardVec.x*translateSpeed, camForwardVec.y*translateSpeed, camForwardVec.z*translateSpeed);
	else if(direction == CAMERA_MOVE_BACKWARD)
		translatePos(-camForwardVec.x*translateSpeed, -camForwardVec.y*translateSpeed, -camForwardVec.z*translateSpeed);
	else if(direction == CAMERA_MOVE_RIGHT)		// Involves not translation in y direction
		translatePos(camRightVec.x*translateSpeed, 0, camRightVec.z*translateSpeed);
	else if(direction == CAMERA_MOVE_LEFT)
		translatePos(-camRightVec.x*translateSpeed, 0, -camRightVec.z*translateSpeed);
}

void CArbAxisCamera::Look(lookOperations operation)
{
	switch(operation)
	{
		case CAMERA_LOOK_UP:
			ArbAxisRotation(rotationSpeed, camRightVec, &camForwardVec, &camUpVec);		// Rotate fwd/Up about camRightVec
		break;
		case CAMERA_LOOK_DOWN:
			ArbAxisRotation(-rotationSpeed, camRightVec, &camForwardVec, &camUpVec);	// Rotate fwd/Up about camRightVec
			break;
		case CAMERA_LOOK_RIGHT:
			ArbAxisRotation(-rotationSpeed, camUpVec, &camForwardVec, &camRightVec);		// Rotate fwd/right about camUpVec
		break;
		case CAMERA_LOOK_LEFT:
			ArbAxisRotation(rotationSpeed, camUpVec, &camForwardVec, &camRightVec);	// Rotate fwd/right about camUpVec
		break;
		case CAMERA_LOOK_ROLL_CCWISE:
			ArbAxisRotation(rotationSpeed, camForwardVec, &camUpVec, &camRightVec);		// Rotate Up/right about camForwardVec
		break;
		case CAMERA_LOOK_ROLL_CWISE:
			ArbAxisRotation(-rotationSpeed, camForwardVec, &camUpVec, &camRightVec);	// Rotate Up/right about camForwardVec
		break;
	}
}

void CArbAxisCamera::scalePos(float k)
{
    camPos.x += k*CAMERA_ZOOM;
    camPos.y += k*CAMERA_ZOOM;
    camPos.z += k*CAMERA_ZOOM;
}

void CArbAxisCamera::ArbAxisRotation(float degrees, Vert3xf axis, Vert3xf *V1, Vert3xf *V2)		// Rotate viewPt and V1/V2 about axis
{
	viewPt.x -= camPos.x;
	viewPt.y -= camPos.y;
	viewPt.z -= camPos.z;

	MakeArbitraryAxisRotMat3x4(rotMat, degrees, axis.x, axis.y, axis.z);						// Make a mat to rot about axis
	MultMat3x4xV(rotMat, &viewPt);                                								// Apply transform.
	MultMat3x4xV(rotMat, V1);		        			            							// Apply transform
	MultMat3x4xV(rotMat, V2);		        			            							// Apply transform

	viewPt.x += camPos.x;
	viewPt.y += camPos.y;
	viewPt.z += camPos.z;
}

void CArbAxisCamera::reportData(void)
{
	cout << INS << "Forward vec; " << camForwardVec.x << ", " << camForwardVec.y << ", " << camForwardVec.z;
	cout << INS << "Right vec; " << camRightVec.x << ", " << camRightVec.y << ", " << camRightVec.z;
	cout << INS << "Sum: " << camForwardVec.x + camRightVec.x << ", " << camForwardVec.y + camRightVec.y  << ", " << camForwardVec.z + camRightVec.z;
}


#endif /* CCAMERA_H_ */
