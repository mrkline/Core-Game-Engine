#include "PhysicsManager.h"
#include <btBulletDynamicsCommon.h>

namespace Physics
{
	PhysicsManager::PhysicsManager()
	{
		broadphase = new btDbvtBroadphase();
		collisionConfig = new btDefaultCollisionConfiguration();
		/*
		/ Do we need multiple contact points per iteration?
		/ collisionConfig->setConvexConvexMultipointIterations();
		*/
		dispatcher = new btCollisionDispatcher(collisionConfig);
		constraintSolver = new btSequentialImpulseConstraintSolver();
		physWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase,
			constraintSolver, collisionConfig);
	}

	PhysicsManager::~PhysicsManager()
	{
		delete physWorld;
		delete constraintSolver;
		delete dispatcher;
		delete collisionConfig;
		delete broadphase;
	}
} //end namespace Physics