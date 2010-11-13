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
			LogicComponent* log2 = static_cast<LogicComponent*>(key.GetHigher()->GetComponentByType(GameComponent::E_GCT_LOGIC));

			//Notify new collisions that they're colliding
			if(pairInfo->startingSubstep == substepNum)
			{
				log1->OnCollisionStart();
				log2->OnCollisionStart();
			}
			else
			{
				//Otherwise, increase the time of collision
				pairInfo->totalCollisionTime += dt;
				
				if(pairInfo->matchedToManifold)
				{
					log1->OnCollisionStay(pairInfo->totalCollisionTime);
					log2->OnCollisionStay(pairInfo->totalCollisionTime);
				}
				//If the collision pair no longer exists, this collision no longer exists.
				//Kill it with fire.
				else
				{
					log1->OnCollisionEnd(pairInfo->totalCollisionTime);
					log2->OnCollisionEnd(pairInfo->totalCollisionTime);
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

	}
};