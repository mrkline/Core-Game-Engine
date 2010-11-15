#include <MapPhysicsManager.h>
#include <btBulletDynamicsCommon.h>
#include <LogicComponent.h>
#include <GameObject.h>
using namespace irr;
using namespace core;

namespace Core
{
	void MapPhysicsManager::DispatchCollisions(irr::u32 substepNum, float dt)
	{

		for(CollisionPairMap::ParentFirstIterator it = pairMap.getParentFirstIterator();
			!it.atEnd(); it++)
		{
			PointerKey<GameObject> key = it->getKey();
			SCollisionPairInfo* pairInfo = it->getValue();
			//Get any existing logic components to notify
			LogicComponent* log1 = static_cast<LogicComponent*>(key.GetHigher()->GetComponentByType(GameComponent::E_GCT_LOGIC));
			LogicComponent* log2 = static_cast<LogicComponent*>(key.GetLower()->GetComponentByType(GameComponent::E_GCT_LOGIC));

			//Notify new collisions that they're colliding
			if(pairInfo->startingSubstep == substepNum)
			{
				if(log1 != nullptr) log1->OnCollisionStart(key.GetLower());
				if(log2 != nullptr) log2->OnCollisionStart(key.GetHigher());
			}
			else
			{
				//Otherwise, increase the time of collision
				pairInfo->totalCollisionTime += dt;
				
				if(pairInfo->matchedToManifold)
				{
					if(log1 != nullptr) log1->OnCollisionStay(pairInfo->totalCollisionTime, key.GetLower());
					if(log2 != nullptr) log2->OnCollisionStay(pairInfo->totalCollisionTime, key.GetHigher());
				}
				//If the collision pair no longer exists, this collision no longer exists.
				//Kill it with fire.
				else
				{
					if(log1 != nullptr) log1->OnCollisionEnd(pairInfo->totalCollisionTime, key.GetLower());
					if(log2 != nullptr) log2->OnCollisionEnd(pairInfo->totalCollisionTime, key.GetHigher());
					pairMap.remove(key);
					delete pairInfo;
				}
			}
			//Reset matchedToManifold
			pairInfo->matchedToManifold = false;
		}
	}

	void MapPhysicsManager::AddCollisionPair(GameObject* obj1, GameObject* obj2, u32 substepNum)
	{
		//See if this pair already exists.  If so, just mark matchedToManifold to true again
		PointerKey<GameObject> key(obj1, obj2);
		CollisionPairMap::Node* existingNode =  pairMap.find(key);
		if(existingNode != nullptr)
		{
			existingNode->getValue()->matchedToManifold = true;
		}
		//Otherwise, create a new node and insert it
		else
		{
			pairMap.insert(key, new SCollisionPairInfo(substepNum));
		}
	}
};