#include "EntityManager.h"
#include "PhysicsManager.h"

namespace GameCore
{
	EntityManager::EntityManager()
	{
		physMan = new Physics::PhysicsManager();
	}

	EntityManager::~EntityManager()
	{
		delete physMan;
	}
} //end namespace GameCore