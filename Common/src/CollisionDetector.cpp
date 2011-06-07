#include "CollisionDetector.h"

#include <btBulletDynamicsCommon.h>

#include "ErrorHandling.h"
#include "PhysicsManager.h"

namespace Core
{
	void CollisionDetector::SetPhysicsManager(PhysicsManager* manager)
	{
		if(manager == nullptr)
		{
			throw ArgumentNullException("The provided physics manager cannot be null.",
				__FUNCTION__);
		}

		physMan = manager;
		collWorld = manager->GetWorld();
	}
} // end namespace Core