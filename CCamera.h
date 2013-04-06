// Camera class.

// TODO: W hen at extremes of window, auto-rotate.

class CCamera
{
private:
    float Zoom;

public:
    Vert3xf refPt, camPos, viewPt;
    Mat3x4 camPitch, camYaw, camRoll;

    CCamera(float Zoom);
    void translatePos(float x, float y, float z);
    void scalePos(float k);                                             // Scale/Zoom
    void Pitch(float degrees);                                          // Rotate about the x-axis
    void Yaw(float degrees);                                            // Rotate about the y-axis
    void Roll(float degrees);                                           // Rotate about the z-axis
    void processCamera(int x, int y);
}MainGameCamera(200);

void CCamera::translatePos(float x, float y, float z)
{
    camPos.x += x;
    camPos.y += y;
    camPos.z += z;
}

void CCamera::scalePos(float k)
{
    camPos.x *= k;
    camPos.y *= k;
    camPos.z *= k;
}

void CCamera::Pitch(float degrees)
{
    // Must translate to local coordinates first:
    translatePos(-refPt.x, -refPt.y, -refPt.z);

    MakeRotMat3x4(camPitch, degrees, 1, 0, 0);
    MultMat3x4xV(camPitch, &camPos);                                    // Apply transform.

    // Cam follows refPoint
    camPos.x += refPt.x;
    camPos.y += refPt.y;
    camPos.z += refPt.z;

    viewPt.x = (camPos.x * -1);
    viewPt.y = (camPos.y * -1);
    viewPt.z = (camPos.z * -1);
}

void CCamera::Yaw(float degrees)
{
    // Must translate to local coordinates first:
    translatePos(-refPt.x, -refPt.y, -refPt.z);

    MakeRotMat3x4(camYaw, degrees, 0, 1, 0);
    MultMat3x4xV(camYaw, &camPos);                                      // Apply transform.

    // Cam follows refPoint
    camPos.x += refPt.x;
    camPos.y += refPt.y;
    camPos.z += refPt.z;

    viewPt.x = (camPos.x * -1);
    viewPt.y = (camPos.y * -1);
    viewPt.z = (camPos.z * -1);
}

void CCamera::Roll(float degrees)
{
    // Must translate to local coordinates first:
    translatePos(-refPt.x, -refPt.y, -refPt.z);

    MakeRotMat3x4(camRoll, degrees, 0, 0, 1);
    MultMat3x4xV(camRoll, &camPos);                                     // Apply transform.

    // Cam follows refPoint
    camPos.x += refPt.x;
    camPos.y += refPt.y;
    camPos.z += refPt.z;

    viewPt.x = (camPos.x * -1);
    viewPt.y = (camPos.y * -1);
    viewPt.z = (camPos.z * -1);
}

CCamera::CCamera(float Zoom)                                            // Initialize.
{
    refPt.x = 0;
    refPt.y = 0;
    refPt.z = 0;

    InitMat(camPitch);
    InitMat(camYaw);
    InitMat(camRoll);

    MakeRotMat3x4(camPitch, 1, 1, 0, 0);                                // 1 Degree, x-axis
    MakeRotMat3x4(camYaw,   1, 0, 1, 0);                                // 1 Degree, y-axis
    MakeRotMat3x4(camRoll,  1, 0, 0, 1);                                // 1 Degree, z-axis

    camPos.x = 0;
    camPos.y = 0;
    camPos.z = refPt.x + Zoom;

    viewPt.x = (camPos.x * -1);
    viewPt.y = (camPos.y * -1);
    viewPt.z = (camPos.z * -1);
}

void CCamera::processCamera(int x, int y)
{
	static int prevX = WINWidth/2, prevY = WINHeight/2;

	// Auto rotate:
	if(y > (WINHeight - 50))											// Extreme bottom edge
		MainGameCamera.Pitch(1);
	if(y < 50)															// Extreme top edge
		MainGameCamera.Pitch(-1);
    if(x > WINWidth - 50)												// Extreme right edge
		MainGameCamera.Yaw(-1);
	if(x < 50)															// Extreme left edge
		MainGameCamera.Yaw(1);



	//if(y < prevY)                                                       // Mouse going up.
		MainGameCamera.Pitch(-0.1*(prevY-y));                                     // Rotate 1 degree counter-clockwise.

/*    if(y > prevY)                                                       // Mouse going down.
        MainGameCamera.Pitch(0.5);                                      // Rotate 1 degree clockwise.
*/
//    if(x < prevX)                                                       // Mouse moving left.
        MainGameCamera.Yaw(0.1*(prevX-x));                                        // Rotate 1 degree clockwise.
/*
    if(x > prevX)                                                       // Mouse moving left.
        MainGameCamera.Yaw(-0.5);                                       // Rotate 1 degree clockwise.
*/



    prevX = x;
    prevY = y;

}


