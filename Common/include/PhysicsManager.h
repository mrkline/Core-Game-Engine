#pragma once

#include <memory>

#include <btBulletDynamicsCommon.h>

#include "ComponentManager.h"
#include "CollisionDetector.h"

class btDynamicsWorld;
class btDefaultCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;

namespace Core
{
	class GameObject;

	/*!
	\brief Manager of all PhysicsComponents.

	Core physics is built on top of the Bullet physics engine.
	This physics manager handles the Bullet world and uses a CollisionDetector
	to detect and dispatch collisions to logic components of colliding GameObjects
	*/
	class PhysicsManager : public ComponentManager
	{
	public:
		/*!
		\brief Constructor.
		\param detector The collision detector to use with this manager.
					The manager will now own the detector and delete it when it deconstructs

		The PhysicsManager uses a CollisionDetector
		to detect and dispatch collisions to logic components of colliding GameObjects
		*/
		PhysicsManager(CollisionDetector* detector);
		virtual ~PhysicsManager() {}

		//! Gets the Bullet world
		btDynamicsWorld* GetWorld() { return physWorld.get(); }
		//! Gets the Bullet default collision configuration
		btDefaultCollisionConfiguration* GetCollisionConfig() { return collisionConfig.get(); }
		//! Gets the Bullet dispatcher
		btDispatcher* GetDispatcher() { return dispatcher.get(); }
		//! Gets the Bullet broadphase collision detector
		btBroadphaseInterface* GetBroadphase() { return broadphase.get(); }
		//! Gets the Bullet constraint solver
		btConstraintSolver* GetConstraintSolver() { return constraintSolver.get(); }

		/*!
		\brief Called by the GameObjectManager each cycle of the game loop
		\param gameTime The current game time, in miliseconds
		*/
		virtual void Update(unsigned int gameTime);

		/*!
		\brief Used by the CollisionDetector to add pairs of colliding GameObjects.
		\param obj1 The GameObject colliding with obj2
		\param obj2 The GameObject colliding with obj1
		\param substepNum The current physics substep. Used to determine
				if collision pairs are new

		CollisionDetector will call this to add collision pairs each Update call.
		Collision pairs must be re-added each Update so that pairs that are no longer
		colliding can be notified.
		*/
		virtual void AddCollisionPair(GameObject* obj1, GameObject* obj2,
			unsigned int substepNum) = 0;

		/*!
		\brief Dispatches collision notifications to caring logic components on colliding GameObjects
		\param substepNum Physics substep number. Collision pairs keep track of the substep
							during which they were created.
		\param dt The number of seconds that has passed since the last call to this function was made

		Dispatches OnCollisionStart, OnCollisionStay, and OnCollisionEnd 
		to caring logic components of the colliding GameObjects
		*/
		virtual void DispatchCollisions(unsigned int substepNum, float dt) = 0;

	protected:
		//! Callback given to Bullet to allow for collision detections each physics tick
		static void TickCallback(btDynamicsWorld *world, float timeStep);

		//! The fixed time step to pass to Bullet's stepSimulation function
		const float kFixedTimeStep;
		//! The maximum number of substeps to pass to Bullet's stepSimulation function
		const int kMaxSubsteps;
		
		//! Used to keep track of the current substep
		unsigned int substepNum;
		//! Used to keep track of the last game time that Update was called
		unsigned int lastTime;

		
		//! The Core collision detector used by this physics manager
		std::auto_ptr<CollisionDetector> collDetector; 

		//! The Bullet dynamics world
		std::auto_ptr<btDynamicsWorld> physWorld;
		//! The Bullet default collision config.
		std::auto_ptr<btDefaultCollisionConfiguration> collisionConfig; 
		//! The Bullet dispatcher
		std::auto_ptr<btDispatcher> dispatcher;
		//! The Bullet broadphase collision detector
		std::auto_ptr<btBroadphaseInterface> broadphase; 
		//! The Bullet constraint solver
		std::auto_ptr<btConstraintSolver> constraintSolver; 
	};
} // end namespace Core