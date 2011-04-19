#pragma once

class btCollisionWorld;

namespace Core
{
	class PhysicsManager;

	/*!
	\brief An interface that identifies collisions between GameObjects using Bullet

	Since Bullet by default only specifies collisions between objects
	and we need to determine collisions on a per-shape level
	(since each PhysicsComponent is tied to a shape).
	This interface gives us an easy way to swap out different methods of doing so.
	*/
	class CollisionDetector
	{
	public:
		virtual ~CollisionDetector() {}

		/*!
		\brief Will be called by the physics manager itself
		\param manager The physics manager

		This allows the detector to be initialized without a reference
		to the physics manager. When the physics manager is constructed,
		it takes a pointer to a collision detector, and calls this function,
		passing itself as the argument.
		*/
		void SetPhysicsManager(PhysicsManager* manager);
		
		/*!
		\brief Gets pairs of colliding GameObjects
		\param substep The current physics substep

		Will be called by the physics manager every Bullet substep.
		Looks at the collision world and calls AddCollisionPair
		for each pair of collision shapes (linked to PhysicsComponents)
		needs the substep number to pass on to AddCollisionPair
		*/
		virtual void GetCollisionPairs(unsigned int substep) = 0;

	protected:

		PhysicsManager* physMan; //!< The owning physics manager
		btCollisionWorld* collWorld; //!< The Bullet collision world
	};
}; // end namepsace Core