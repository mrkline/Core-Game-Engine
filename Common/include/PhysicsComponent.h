#pragma once

#include <btBulletDynamicsCommon.h>
#include <GameComponent.h>

namespace Core 
{
	class PhysicsManager;

	class PhysicsComponent : public GameComponent
	{
	public:

		PhysicsComponent(GameObject* owner, PhysicsManager* physMan, btCollisionShape* collisionShape,
			const btVector3& COG = btVector3(), float objMass = 1.0f);

		EType GetComponentType() { return GameComponent::E_GCT_PHYSICS; }

		//TODO: Get/Set Motion controller
		//TODO: Get thrusterlist
		float getMass() const { return mass; }
		float getAbsoluteMass() const { return absoluteMass; }
		const btVector3& GetCOG() const { return cog; }
		const btVector3& GetAbsoluteCOG() const { return absoluteCOG; }
		const std::list<btVector3>& GetExternalForces() const { return externalForces; }
		std::list<btVector3>& GetExternalForces() { return externalForces; }
		const btTransform& getTransform() const { return transform; }
		btTransform& getTransform() { return transform; }
		void SetMass(float newMass) { mass = newMass; } 
		void SetCOG(const btVector3& newCOG) { cog = newCOG; }
		void SetTransform(const btTransform& newTransform) { transform = newTransform; }

		PhysicsManager* GetPhysicsManager() { return physMan; }

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
		float mass;
		//Mass of object plus any child entities
		float absoluteMass;
		//Center of gravity in local coordinates
		btVector3 cog;
		//Center of gravity of entity with all children
		btVector3 absoluteCOG;
		//Forces which would affect the velocity of the entity
		std::list<btVector3> externalForces;

		PhysicsManager* physMan;
	};
} //end namespace Core