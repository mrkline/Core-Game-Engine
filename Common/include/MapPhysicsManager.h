#pragma once
#include <PhysicsManager.h>
#include <PointerKey.h>
#include <irrlicht.h>

namespace Core
{
	//A physics manager that dispatches collisions via a map.
	//The key in the map is a combination of two pointers.
	class MapPhysicsManager : public PhysicsManager
	{
	public:
		void DispatchCollisions(float timeStep);

	protected:
		struct SCollisionPairInfo
		{
			//Start of collision in game time
			irr::u32 collisionStartTime;
			//Tracks if collision pair has been matched to a manifold yet.
			bool matchedToManifold;
		};

		irr::core::map<PointerKey, SCollisionPairInfo> collisionPairMap;
	};
} //end namespace Core