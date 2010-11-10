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

		//Dispatch OnCollisionStart, OnCollisionStay, and OnCollisionEnd
		//to caring logic components of the colliding GameObjects
		void DispatchCollisions(float timeStep);

	protected:
		struct SCollisionPairInfo
		{
			//Elapsed collision time
			float totalCollisionTime;
			//Tracks if collision pair has been matched to a manifold yet.
			bool matchedToManifold;
		};

		virtual void AddCollisionPair(GameObject* obj1, GameObject* obj2);

		irr::core::map<PointerKey<GameObject>, SCollisionPairInfo> pairMap;
	};
} //end namespace Core