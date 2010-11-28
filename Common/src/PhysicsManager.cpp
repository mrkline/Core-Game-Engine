#include <PhysicsManager.h>
#include <BulletPragmas.h>
#include <btBulletDynamicsCommon.h>
#include <CollisionDetector.h>
#include <ErrorHandling.h>
using namespace irr;

namespace Core
{
	PhysicsManager::PhysicsManager(CollisionDetector* detector)
		: kFixedTimeStep(1.0f / 60.0f), kMaxSubsteps(3),
		substepNum(0), lastTime(0), collDetector(detector)
	{
		//TODO:
		//Does this call the destructor?  This will cause problems if it doesn't (since all
		//the things being deleted haven't been allocated yet)
		if(detector == nullptr)
		{
			throw new Error::ArgumentNullException("Physics manager cannot receive a null CollisionDetector",
				__FUNCTION__);
		}
		detector->SetPhysicsManager(this);

		broadphase = new btDbvtBroadphase();
		collisionConfig = new btDefaultCollisionConfiguration();
		//Do we need multiple contact points per iteration?
		//If so, uncomment the below line:
		//collisionConfig->setConvexConvexMultipointIterations();
		dispatcher = new btCollisionDispatcher(collisionConfig);
		constraintSolver = new btSequentialImpulseConstraintSolver();
		physWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase,
			constraintSolver, collisionConfig);
		//Set a callback up for ticks
		physWorld->setInternalTickCallback(TickCallback, this);
	}

	PhysicsManager::~PhysicsManager()
	{
		delete physWorld;
		delete constraintSolver;
		delete dispatcher;
		delete collisionConfig;
		delete broadphase;
	}
	
	void PhysicsManager::Update(u32 gameTime)
	{
		f32 dt = static_cast<f32>(gameTime - lastTime) / static_cast<f32>(1000);
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
} //end namespace Core