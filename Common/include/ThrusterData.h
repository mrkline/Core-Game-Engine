#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;

namespace Core
{
	class ThrusterData
	{
	public:
		//Local space position of the thruster
		const vector3df position;
		//Unit vector pointing in the direction the thruster is.
		//Applied force will be in the opposite direction
		const vector3df orientation;
		//possible magnitude of the thrusters
		const f32 maxMagnitude;

		f32 GetNormalizedForce() { return normalizedForce; }
		f32 GetCurrentMagnitude() { return maxMagnitude * normalizedForce; }

		void SetNormalizedForce(f32 nf) { normalizedForce = clamp(nf, 0.0f, 1.0f); }
		
	protected:
		//Amount the thruster is being used.  Range is between 0 and 1
		f32 normalizedForce;
		//Amount the thruster can influence each axis of rotation, based on the 
		vector3df cachedForce;
	};
} //end namespace Core