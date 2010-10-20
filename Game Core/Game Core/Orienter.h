#ifndef ORIENTER_H_
#define ORIENTER_H_

#include <irrlicht.h>
using namespace irr;
using namespace core;

//This class calculates the correct Euler rotations to
//correctly orient a scene node given independant rotations
//around each axis, allowing for (6DOF motion).  While the member
//functions of this class could be static, they are not so to save
//speed by eliminating constant vector declaration and destruction
//on each call.  It also allows you to get the last used up vector
//to save time on recalculation.
class Orienter
{
public:
	Orienter();
	virtual ~Orienter() {}

	const vector3df& GetEulerRotationDegrees(const vector3df& rotation);
	const vector3df& GetEulerRotationRadians(const vector3df& rotation);
	void SetOffsetDegrees(const vector3df& off);
	void SetOffsetRadians(const vector3df& off);

protected:
	
	//Used in GetEurlerRotationFromRadians to convert to degrees rotation as needed
	vector3df convertedRotation;

	//The direction the rotated mesh will point
	vector3df direction;

	//The y-rotation component of the above direction
	vector3df yRot;

	//"Up" from the perspective of the rotated mesh
	vector3df up;
	//World up to be rotated by the direction quaternion
	vector3df rotatedUp;

	//Staring offset of the model
	quaternion quatOffset;

	quaternion quatX;
	quaternion quatY;
	quaternion quatZ;

	quaternion fullRotation;

	//Last calculated rotation
	vector3df eulerRotation;

	//The world-space up (0, 1, 0)
	const vector3df kWorldUp;
	//Starting direction (0, 0, 1)
	const vector3df kStartingDirection;
};

#endif