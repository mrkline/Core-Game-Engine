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
		void DispatchCollisions(irr::u32 substepNum, float dt);

	protected:
		struct SCollisionPairInfo
		{
			//Used to determine if collision is new or not
			irr::u32 startingSubstep;
			//Elapsed collision time
			float totalCollisionTime;
			//Tracks if collision pair has been matched to a manifold yet.
			bool matchedToManifold;
		};

		typedef irr::core::map<PointerKey<GameObject>, SCollisionPairInfo*> CollisionPairMap;

		virtual void AddCollisionPair(GameObject* obj1, GameObject* obj2, irr::u32 subsetNum);

		CollisionPairMap pairMap;
	};
} //end namespace Core