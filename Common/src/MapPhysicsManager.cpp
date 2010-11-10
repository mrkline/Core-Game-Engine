#include <MapPhysicsManager.h>
#include <btBulletDynamicsCommon.h>
#include <LogicComponent.h>
using namespace irr;
using namespace core;

namespace Core
{
	void MapPhysicsManager::DispatchCollisions(float timeStep)
	{
		for(map<PointerKey<GameObject>, SCollisionPairInfo>::ParentFirstIterator it = pairMap.getParentFirstIterator();
			!it.atEnd(); it++)
		{
			map<PointerKey<GameObject>, SCollisionPairInfo>::Node* curr = it.getNode();
			SCollisionPairInfo info = curr->getValue();
			//TODO: Use pointers as key and value
		}
	}

	void MapPhysicsManager::AddCollisionPair(GameObject* obj1, GameObject* obj2)
	{

	}
};