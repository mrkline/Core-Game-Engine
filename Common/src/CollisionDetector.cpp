#include "CollisionDetector.h"

#include <btBulletDynamicsCommon.h>

#include "PhysicsManager.h"

namespace Core
{
	void CollisionDetector::SetPhysicsManager(PhysicsManager* manager)
	{
		physMan = manager;
		collWorld = manager->GetWorld();
	}
} //end namespace Core