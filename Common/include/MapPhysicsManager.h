#pragma once
#include <PhysicsManager.h>
#include <PointerKey.h>
#include <map>

namespace Core
{
	/*! 
	\brief A physics manager that dispatches collisions via an std::map.
	
	The key in the map is a class that holds pointers to the two colliding objects.
	The value in the map contains information about the collision between the two objects.
	*/
	class MapPhysicsManager : public PhysicsManager
	{
	public:
		virtual ~MapPhysicsManager() {}

		//! \see PhysicsManager::DispatchCollisions
		void DispatchCollisions(unsigned int substepNum, float dt);

	protected:
		//! The value class in the collision map
		struct SCollisionPairInfo
		{
			//! Used to determine if collision is new or not
			unsigned int startingSubstep;
			//! Elapsed collision time
			float totalCollisionTime;
			//! Tracks if collision pair has been matched to a manifold yet.
			bool matchedToManifold;

			SCollisionPairInfo(unsigned int startingSS)
				: startingSubstep(startingSS), totalCollisionTime(0.0f),
				matchedToManifold(true)
			{}
		};

		typedef std::map<PointerKey<GameObject>, SCollisionPairInfo*> CollisionPairMap;

		//! \see PhysicsManager::AddCollisionPair
		virtual void AddCollisionPair(GameObject* obj1, GameObject* obj2, unsigned int substepNum);

		//! A map that keeps track of collision pairs
		CollisionPairMap pairMap;
	};
} //end namespace Core