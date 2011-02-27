#pragma once
#include <PhysicsManager.h>
#include <PointerKey.h>
#include <map>

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
		void DispatchCollisions(unsigned int substepNum, float dt);

	protected:
		struct SCollisionPairInfo
		{
			//Used to determine if collision is new or not
			unsigned int startingSubstep;
			//Elapsed collision time
			float totalCollisionTime;
			//Tracks if collision pair has been matched to a manifold yet.
			bool matchedToManifold;

			SCollisionPairInfo(unsigned int startingSS)
				: startingSubstep(startingSS), totalCollisionTime(0.0f),
				matchedToManifold(true)
			{}
		};

		typedef std::map<PointerKey<GameObject>, SCollisionPairInfo*> CollisionPairMap;

		virtual void AddCollisionPair(GameObject* obj1, GameObject* obj2, unsigned int subsetNum);

		CollisionPairMap pairMap;
	};
} //end namespace Core