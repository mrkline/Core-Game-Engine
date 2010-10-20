#include "Orienter.h"

Orienter::Orienter()
	: kWorldUp(0, 1, 0), kStartingDirection(0, 0, 1)
{
}

const vector3df& Orienter::GetEulerRotationDegrees(const vector3df& rotation) 
{ 
	f32 yRad = rotation.Y * DEGTORAD;
	yRot.Z = cos(yRad);
	yRot.X = sin(yRad);

	quatY.rotationFromTo(kStartingDirection, yRot);

	//Orient our direction vector
	direction.set(kStartingDirection);
	direction.rotateYZBy(-rotation.X);
	direction.rotateXZBy(-rotation.Y);

	//Get our x-rotation
	quatX.rotationFromTo(yRot, direction);

	//Get our z rotation

	//Orient our up vector
	up.set(kWorldUp);
	up.rotateXYBy(-rotation.Z);
	up.rotateYZBy(-rotation.X);
	up.rotateXZBy(-rotation.Y);

	//Get rotation to direction and 
	//rotate the world up by it
	rotatedUp.set(kWorldUp);
	rotatedUp.rotateYZBy(-rotation.X);
	rotatedUp.rotateXZBy(-rotation.Y);

	//Get rotation from rotated up to given up 
	quatZ.rotationFromTo(rotatedUp, up);

	//Combine the rotations for the full rotation
	fullRotation = quatOffset * quatY * quatX * quatZ;
	fullRotation.toEuler(eulerRotation);
	eulerRotation *= RADTODEG;
	
	return eulerRotation;
}

const vector3df& Orienter::GetEulerRotationRadians(const vector3df& rotation) 
{ 
	yRot.Z = cos(rotation.Y);
	yRot.X = sin(rotation.Y);

	convertedRotation = rotation * RADTODEG;

	quatY.rotationFromTo(kStartingDirection, yRot);

	//Orient our direction vector
	direction.set(kStartingDirection);
	direction.rotateYZBy(-convertedRotation.X);
	direction.rotateXZBy(-convertedRotation.Y);

	//Get our x-rotation
	quatX.rotationFromTo(yRot, direction);

	//Get our z rotation

	//Orient our up vector
	up.set(kWorldUp);
	up.rotateXYBy(-convertedRotation.Z);
	up.rotateYZBy(-convertedRotation.X);
	up.rotateXZBy(-convertedRotation.Y);

	//Get rotation to direction and 
	//rotate the world up by it
	rotatedUp.set(kWorldUp);
	rotatedUp.rotateYZBy(-convertedRotation.X);
	rotatedUp.rotateXZBy(-convertedRotation.Y);

	//Get rotation from rotated up to given up 
	quatZ.rotationFromTo(rotatedUp, up);

	//Combine the rotations for the full rotation
	fullRotation = quatOffset * quatY * quatX * quatZ;
	fullRotation.toEuler(eulerRotation);
	
	return eulerRotation;
}

void Orienter::SetOffsetDegrees(const vector3df& off)
{
	quatOffset = quaternion(off * DEGTORAD);
}

void Orienter::SetOffsetRadians(const vector3df& off)
{
	quatOffset = quaternion(off);
}
