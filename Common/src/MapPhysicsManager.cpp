#include "MapPhysicsManager.h"

#include <btBulletDynamicsCommon.h>

#include "ErrorHandling.h"
#include "GameObject.h"
#include "LogicComponent.h"

namespace Core
{
	void MapPhysicsManager::DispatchCollisions(unsigned int substepNum, float dt)
	{

		for(CollisionPairMap::iterator it = pairMap.begin();
			it != pairMap.end(); ++it)
		{
			PointerKey<GameObject> key = it->first;
			SCollisionPairInfo* pairInfo = it->second;
			// Get any existing logic components to notify
			LogicComponent* log1 = static_cast<LogicComponent*>(key.GetHigher()->GetComponentByType(GameComponent::E_GCT_LOGIC));
			LogicComponent* log2 = static_cast<LogicComponent*>(key.GetLower()->GetComponentByType(GameComponent::E_GCT_LOGIC));

			// Notify new collisions that they're colliding
			if(pairInfo->startingSubstep == substepNum)
			{
				if(log1 != nullptr) log1->OnCollisionStart(key.GetLower());
				if(log2 != nullptr) log2->OnCollisionStart(key.GetHigher());
			}
			else
			{
				// Otherwise, increase the time of collision
				pairInfo->totalCollisionTime += dt;
				
				if(pairInfo->matchedToManifold)
				{
					if(log1 != nullptr) log1->OnCollisionStay(pairInfo->totalCollisionTime, key.GetLower());
					if(log2 != nullptr) log2->OnCollisionStay(pairInfo->totalCollisionTime, key.GetHigher());
				}
				// If the collision pair no longer exists, this collision no longer exists.
				// Kill it with fire.
				else
				{
					if(log1 != nullptr) log1->OnCollisionEnd(pairInfo->totalCollisionTime, key.GetLower());
					if(log2 != nullptr) log2->OnCollisionEnd(pairInfo->totalCollisionTime, key.GetHigher());
					pairMap.erase(it);
					delete pairInfo;
				}
			}
			// Reset matchedToManifold
			pairInfo->matchedToManifold = false;
		}
	}

	void MapPhysicsManager::AddCollisionPair(GameObject* obj1, GameObject* obj2, unsigned int substepNum)
	{
		// See if this pair already exists.  If so, just mark matchedToManifold to true again
		PointerKey<GameObject> key(obj1, obj2);
		CollisionPairMap::iterator existingNode =  pairMap.find(key);
		if(existingNode != pairMap.end())
		{
			existingNode->second->matchedToManifold = true;
		}
		// Otherwise, create a new node and insert it
		else
		{
			pairMap.insert(CollisionPairMap::value_type(key, new SCollisionPairInfo(substepNum)));
		}
	}
} // end namespace Core