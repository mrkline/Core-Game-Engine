#pragma once

#include <btBulletDynamicsCommon.h>

namespace Physics
{
	class PhysicsManager
	{
	public:
		PhysicsManager();
		virtual ~PhysicsManager();

		//Will completely delete and re-initialize Bullet physics.
		//Any existing entities using physics are now invalid.
		void ResetBullet();

	protected:
		btDynamicsWorld *physWorld;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btDispatcher* dispatcher;
		btBroadphaseInterface* pairCache;
		btConstraintSolver*	constraintSolver;

		void InitBullet();
	};
} //end namespace Physics

#define GLOBAL_PHYSICS Physics::PhysicsMan
#define GLOBAL_PHYSICS_WORLD Physics::PhysicsMan
