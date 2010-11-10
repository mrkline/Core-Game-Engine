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
		virtual ~MapPhysicsManager() {}

		void DispatchCollisions(float timeStep);

	protected:
		struct SCollisionPairInfo
		{
			//Elapsed collision time
			float totalCollisionTime;
			//Tracks if collision pair has been matched to a manifold yet.
			bool matchedToManifold;
		};

		irr::core::map<PointerKey<GameObject>, SCollisionPairInfo> collisionPairMap;
	};
} //end namespace Core