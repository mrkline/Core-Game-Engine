#pragma once

#include <btBulletDynamicsCommon.h>
#include "GameComponent.h"

namespace Physics
{
	class PhysicsManager;
} //end namespace Physics

namespace GameCore 
{
	class PhysicsComponent : public GameComponent
	{
	public:

		PhysicsComponent(GameObject* owner, Physics::PhysicsManager* physMan, btCollisionShape* collisionShape,
			const btVector3& COG = btVector3(), irr::f32 objMass = 1.0f);

		EType GetComponentType() { return GameComponent::E_GCT_PHYSICS; }

		//TODO: Get/Set Motion controller
		//TODO: Get thrusterlist
		irr::f32 getMass() const { return mass; }
		irr::f32 getAbsoluteMass() const { return absoluteMass; }
		const btVector3& GetCOG() const { return cog; }
		const btVector3& GetAbsoluteCOG() const { return absoluteCOG; }
		const irr::core::list<btVector3>& GetExternalForces() const { return externalForces; }
		irr::core::list<btVector3>& GetExternalForces() { return externalForces; }
		const btTransform& getTransform() const { return transform; }
		btTransform& getTransform() { return transform; }
		void SetMass(irr::f32 newMass) { mass = newMass; } 
		void SetCOG(const btVector3& newCOG) { cog = newCOG; }
		void SetTransform(const btTransform& newTransform) { transform = newTransform; }

		Physics::PhysicsManager* GetPhysicsManager() { return physMan; }

		// Updates hierarchical data based on children.
		void OnHierarchyChange(bool goingUp);

	protected:

		btRigidBody* body;

		//Scene node we're attached to.
		btCollisionShape* cShape;
		btCollisionShape* absoluteCShape;
		btTransform transform;

		//TODO: Add type field?
		//TODO: Add motion controller
		//TODO: Add thruster list
		irr::f32 mass;
		//Mass of object plus any child entities
		irr::f32 absoluteMass;
		//Center of gravity in local coordinates
		btVector3 cog;
		//Center of gravity of entity with all children
		btVector3 absoluteCOG;
		//Forces which would affect the velocity of the entity
		irr::core::list<btVector3> externalForces;

		Physics::PhysicsManager* physMan;
	};
} //end namespace GameCore