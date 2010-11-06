#include "PhysicsComponent.h"
#include "EntityManager.h"
#include "PhysicsManager.h"
#include "ErrorHandling.h"
#include "IrrlichtBulletConversions.h"

namespace GameCore
{
	PhysicsComponent::PhysicsComponent(GameObject* owner, PhysicsManager* physMan, btCollisionShape* collisionShape,
			const btVector3& COG, f32 objMass)
			: GameComponent(owner), cog(COG), mass(objMass)
	{

	}

	void PhysicsComponent::OnHierarchyChange()
	{
		//Clear out old data
		if(absoluteCShape != nullptr)
		{
			delete absoluteCShape;
		} 

		absoluteMass = mass;
		if(children.getSize() == 0)
		{
			absoluteCOG = cog;
			absoluteCShape = cShape;
		}
		else
		{
			absoluteCShape = new btCompoundShape();
			btCompoundShape* compAbsShape = static_cast<btCompoundShape*>(absoluteCShape);
			compAbsShape->addChildShape(btTransform(), cShape);

			list<PhysicsComponent*>::Iterator it = children.begin();
			for(; it != children.end(); ++it)
			{
				PhysicsComponent* curr = *it;
				curr->UpdateHierarchicalData();
				compAbsShape->addChildShape(curr->getTransform(), curr->absoluteCShape);
				absoluteMass += curr->absoluteMass;
			}
			it = children.begin();

			absoluteCOG.set(0.0f, 0.0f, 0.0f);
			absoluteCOG += cog * (mass / absoluteMass);
			for(; it != children.end(); ++it)
			{
				PhysicsComponent* curr = *it;
				absoluteCOG += curr->cog
					* (curr->absoluteMass / absoluteMass);
			}
		}

		//Only entities with no parents should have rigid bodies.
		if(parent == nullptr)
		{
			//TODO: Update body mass and possibly motionstate

			/*
			// If the body already exists, this entity was already a parent
			// beforehand.  We just need to update it's collision shape
			// with the new absolute shape.
			*/
			if(body != nullptr)
			{
				body->setCollisionShape(absoluteCShape);
			}
			//Otherwise we need to instantiate a new rigid body and register
			//it with the physics world.
			else
			{
				//TODO: Add rigid body
			}
		}
		//If the entitiy is no longer a parent, remove it from the world
		else if(body != nullptr)
		{
			manager->GetPhysicsManager()->GetWorld()->removeRigidBody(body);
			delete body;
			body = nullptr;
		}
	}
} //end namespace GameCore