#pragma once

class btDynamicsWorld;
class btDefaultCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;

namespace Physics
{
	//Manager of all PhysicsComponents.  Uses Bullet to manage them
	class PhysicsManager
	{
	public:
		PhysicsManager();
		virtual ~PhysicsManager();

		btDynamicsWorld* GetWorld() { return physWorld; }
		btDefaultCollisionConfiguration* GetCollisionConfig() { return collisionConfig; }
		btDispatcher* GetDispatcher() { return dispatcher; }
		btBroadphaseInterface* GetBroadphase() { return broadphase; }
		btConstraintSolver* GetConstraintSolver() { return constraintSolver; }

	protected:
		btDynamicsWorld *physWorld;
		btDefaultCollisionConfiguration* collisionConfig;
		btDispatcher* dispatcher;
		btBroadphaseInterface* broadphase;
		btConstraintSolver*	constraintSolver;

		void InitBullet();
	};
} //end namespace Physics