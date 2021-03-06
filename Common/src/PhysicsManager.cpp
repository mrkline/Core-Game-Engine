#include "PhysicsManager.h"

#include "CollisionDetector.h"
#include "ErrorHandling.h"

namespace Core
{
	PhysicsManager::PhysicsManager(CollisionDetector* detector)
		: kFixedTimeStep(1.0f / 60.0f), kMaxSubsteps(3),
		substepNum(0), lastTime(0), collDetector(detector),
		broadphase(new btDbvtBroadphase()),
		collisionConfig(new btDefaultCollisionConfiguration()),
		dispatcher(new btCollisionDispatcher(collisionConfig.get())),
		constraintSolver(new btSequentialImpulseConstraintSolver()),
		physWorld(new btDiscreteDynamicsWorld(dispatcher.get(), broadphase.get(),
			constraintSolver.get(), collisionConfig.get()))

	{
		/*!
		\todo Does this call the destructor?  This will cause problems if it doesn't (since all
				the things being deleted haven't been allocated yet)
		*/
		if(detector == nullptr)
			throw ArgumentNullException("Physics manager cannot receive a null CollisionDetector",
				__FUNCTION__);

		detector->SetPhysicsManager(this);

		//Set a callback up for ticks
		physWorld->setInternalTickCallback(TickCallback, this);
	}
	
	void PhysicsManager::Update(unsigned int gameTime)
	{
		float dt = static_cast<float>(gameTime - lastTime) / 1000.0f;
		lastTime = gameTime;
		physWorld->stepSimulation(dt, kMaxSubsteps, kFixedTimeStep);
	}

	void PhysicsManager::TickCallback(btDynamicsWorld *world, float timeStep)
	{
		PhysicsManager* physMan = static_cast<PhysicsManager*>(world->getWorldUserInfo());
		physMan->collDetector->GetCollisionPairs(physMan->substepNum);
		physMan->DispatchCollisions(physMan->substepNum, timeStep);
		++(physMan->substepNum);
	}
} // end namespace Core