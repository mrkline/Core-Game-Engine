#pragma once
#include <ComponentManager.h>

class btDynamicsWorld;
class btDefaultCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;

namespace Core
{
	//Manager of all PhysicsComponents.  Uses Bullet to manage them
	class PhysicsManager : public ComponentManager
	{
	public:
		PhysicsManager();
		virtual ~PhysicsManager();

		btDynamicsWorld* GetWorld() { return physWorld; }
		btDefaultCollisionConfiguration* GetCollisionConfig() { return collisionConfig; }
		btDispatcher* GetDispatcher() { return dispatcher; }
		btBroadphaseInterface* GetBroadphase() { return broadphase; }
		btConstraintSolver* GetConstraintSolver() { return constraintSolver; }

		void Update(irr::u32 gameTime);
		virtual void DispatchCollisions(float timeStep) = 0;

	protected:
		static void TickCallback(btDynamicsWorld *world, float timeStep);

		const float kFixedTimeStep;
		const int kMaxSubsteps;

		btDynamicsWorld *physWorld;
		btDefaultCollisionConfiguration* collisionConfig;
		btDispatcher* dispatcher;
		btBroadphaseInterface* broadphase;
		btConstraintSolver*	constraintSolver;

		irr::u32 lastTime;

		void InitBullet();
	};
} //end namespace Core