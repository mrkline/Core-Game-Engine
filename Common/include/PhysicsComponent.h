#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;

#include <btBulletDynamicsCommon.h>
#include "GameComponent.h"

namespace GameCore 
{
	class PhysicsManager;

	class PhysicsComponent : public GameComponent
	{
	public:

		PhysicsComponent(GameObject* owner, PhysicsManager* physMan, btCollisionShape* collisionShape,
			const btVector3& COG = btVector3(), f32 objMass = 1.0f);

		//TODO: Get/Set Motion controller
		//TODO: Get thrusterlist
		f32 getMass() const { return mass; }
		f32 getAbsoluteMass() const { return absoluteMass; }
		const btVector3& GetCOG() const { return cog; }
		const btVector3& GetAbsoluteCOG() const { return absoluteCOG; }
		const list<vector3df>& GetExternalForces() const { return externalForces; }
		list<vector3df>& GetExternalForces() { return externalForces; }
		const btTransform& getTransform() const { return transform; }
		btTransform& getTransform() { return transform; }
		void SetMass(f32 newMass) { mass = newMass; } 
		void SetCOG(const btVector3& newCOG) { cog = newCOG; }
		void SetTransform(const btTransform& newTransform) { transform = newTransform; }

		// Updates hierarchical data based on children.
		void OnHierarchyChange();

	protected:

		btRigidBody* body;

		//Scene node we're attached to.
		btCollisionShape* cShape;
		btCollisionShape* absoluteCShape;
		btTransform transform;

		//TODO: Add type field?
		//TODO: Add motion controller
		//TODO: Add thruster list
		f32 mass;
		//Mass of object plus any child entities
		f32 absoluteMass;
		//Center of gravity in local coordinates
		btVector3 cog;
		//Center of gravity of entity with all children
		btVector3 absoluteCOG;
		//Forces which would affect the velocity of the entity
		list<vector3df> externalForces;
	};
} //end namespace GameCore