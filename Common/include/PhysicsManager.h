#pragma once

class btDynamicsWorld;
class btDefaultCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;

namespace Physics
{
	class PhysicsManager
	{
	public:
		PhysicsManager();
		virtual ~PhysicsManager();

		__forceinline btDynamicsWorld* GetWorld() { return physWorld; }
		__forceinline btDefaultCollisionConfiguration* GetCollisionConfig() { return collisionConfig; }
		__forceinline btDispatcher* GetDispatcher() { return dispatcher; }
		__forceinline btBroadphaseInterface* GetBroadphase() { return broadphase; }
		__forceinline btConstraintSolver* GetConstraintSolver() { return constraintSolver; }

	protected:
		btDynamicsWorld *physWorld;
		btDefaultCollisionConfiguration* collisionConfig;
		btDispatcher* dispatcher;
		btBroadphaseInterface* broadphase;
		btConstraintSolver*	constraintSolver;

		void InitBullet();
	};
} //end namespace Physics