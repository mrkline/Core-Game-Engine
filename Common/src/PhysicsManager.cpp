#include <PhysicsManager.h>
#include <btBulletDynamicsCommon.h>
using namespace irr;

namespace Core
{
	PhysicsManager::PhysicsManager()
		: kFixedTimeStep(1.0f / 60.0f), kMaxSubsteps(3)
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
		//Set a callback up for ticks
		physWorld->setInternalTickCallback(PhysicsManager::TickCallback, this);
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
		physMan->DispatchCollisions(timeStep);
	}
} //end namespace Core