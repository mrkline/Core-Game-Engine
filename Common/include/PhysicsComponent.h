#pragma once

#include <btBulletDynamicsCommon.h>

#include "GameComponent.h"

namespace Core 
{
	class PhysicsManager;
	
	/*!
	\brief This component adds physics to a GameObject
	\todo Motion controllers
	\todo Force functions

	When multiple objects with PhysicsComponents are nested in a hierarchy,
	the topmost physics component acts as a rigid body which uses the sum of
	all masses, COGs, and collision shapes of itself and its children
	*/
	class PhysicsComponent : public GameComponent
	{
	public:
		/*!
		\brief Constructs the physics component
		\param physMan The physics manager
		\param collisionShape The Bullet collision shape to use
		\todo Encapsulate or at least typedef the btCollisionShape?
		\param COG The center of gravity of the object (in local coordinates)
		\todo Use Core::Vector3 instead of the btVector3?
		\param objMass The mass of the object
		*/
		PhysicsComponent(PhysicsManager* physMan, btCollisionShape* collisionShape,
			const btVector3& COG = btVector3(), float objMass = 1.0f);
		//! \todo Implement
		virtual ~PhysicsComponent() {}

		//! \see GameComponent::GetComponentType
		EType GetComponentType() { return GameComponent::E_GCT_PHYSICS; }

		//! Gets the mass of the object
		float getMass() const { return mass; }
		//! Gets the mass of the object and all its children
		float getAbsoluteMass() const { return absoluteMass; }
		/*!
		\brief Gets the center of gravity of this object
		\todo Return Core::Vector3?
		*/
		const btVector3& GetCOG() const { return cog; }
		/*!
		\brief Gets the center of gravity of this object and all its children
		\todo Return Core::Vector3?
		*/
		const btVector3& GetAbsoluteCOG() const { return absoluteCOG; }
		//! \todo: How is the best way to link tranforms between components and owning object?
		const btTransform& getTransform() const { return transform; }
		/*!
		\brief Get the Bullet transform of this object
		\todo Needed? Is this local or a copy of the object's transform?
		*/
		btTransform& getTransform() { return transform; }
		//! Sets the mass of this object
		void SetMass(float newMass) { mass = newMass; } 
		/*!
		\brief Sets the center of gravity of this object
		\todo Use Core::Vector3?
		*/
		void SetCOG(const btVector3& newCOG) { cog = newCOG; }
		//! \todo: How is the best way to link tranforms between components and owning object?
		void SetTransform(const btTransform& newTransform) { transform = newTransform; }

		//! Gets the physics manager
		PhysicsManager* GetPhysicsManager() { return physMan; }

		void OnHierarchyChange();

	protected:
		//! Rigid body representing this physics object, if it's at the top of a hierarchy
		btRigidBody* body;

		//! Collision shape of this object
		btCollisionShape* cShape;
		//! Complex collision shape of this object and its children
		btCollisionShape* absoluteCShape;
		//! Bullet transform of this object
		btTransform transform;

		//! Mass of this object
		float mass;
		//! Mass of object plus any child entities
		float absoluteMass;
		//! Center of gravity in local coordinates
		btVector3 cog;
		//! Center of gravity of entity with all children
		btVector3 absoluteCOG;
		//! Local reference to the physics manager
		PhysicsManager* physMan;
	};
} // end namespace Core