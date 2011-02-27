#pragma once
#include <ComponentManager.h>

class btDynamicsWorld;
class btDefaultCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;

namespace Core
{
	class GameObject;
	class CollisionDetector;

	//!Manager of all PhysicsComponents.  Uses Bullet to manage them
	class PhysicsManager : public ComponentManager
	{
	public:

		PhysicsManager(CollisionDetector* detector);
		virtual ~PhysicsManager();

		btDynamicsWorld* GetWorld() { return physWorld; }
		btDefaultCollisionConfiguration* GetCollisionConfig() { return collisionConfig; }
		btDispatcher* GetDispatcher() { return dispatcher; }
		btBroadphaseInterface* GetBroadphase() { return broadphase; }
		btConstraintSolver* GetConstraintSolver() { return constraintSolver; }

		//! Called by the GameObjectManager each cycle of the game loop
		virtual void Update(unsigned int gameTime);

		//! CollisionDetector will call this to add collision pairs each Update call.
		//! Collision pairs must be re-added each Update so that pairs that are no longer
		//! colliding can be notified.
		virtual void AddCollisionPair(GameObject* obj1, GameObject* obj2,
			unsigned int subsetNum) = 0;

		//! Dispatches OnCollisionStart, OnCollisionStay, and OnCollisionEnd
		//! to caring logic components of the colliding GameObjects
		//The argument is the ms passed since the last call, not total time
		virtual void DispatchCollisions(unsigned int currentTime, float dt) = 0;

	protected:

		static void TickCallback(btDynamicsWorld *world, float timeStep);

		const float kFixedTimeStep;
		const int kMaxSubsteps;
		
		unsigned int substepNum;
		unsigned int lastTime;

		btDynamicsWorld *physWorld;
		btDefaultCollisionConfiguration* collisionConfig;
		btDispatcher* dispatcher;
		btBroadphaseInterface* broadphase;
		btConstraintSolver*	constraintSolver;

		CollisionDetector* collDetector;

		void InitBullet();
	};
} //end namespace Core