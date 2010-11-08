#include <PhysicsComponent.h>
#include <PhysicsManager.h>
#include <ErrorHandling.h>
#include <IrrlichtBulletConversions.h>

using namespace irr;
using namespace core;

namespace Core
{
	PhysicsComponent::PhysicsComponent(GameObject* owner, PhysicsManager* physManager, btCollisionShape* collisionShape,
			const btVector3& COG, f32 objMass)
			: GameComponent(owner), body(nullptr), cShape(collisionShape), absoluteCShape(nullptr),
				mass(objMass), absoluteMass(objMass),
				cog(COG), absoluteCOG(COG), physMan(physManager)
	{
		if(physManager == nullptr)
		{
			throw new Error::ArgumentNullException("The physics manager cannot be null.", __FUNCTION__);
		}
		if(collisionShape == nullptr)
		{
			throw new Error::ArgumentNullException("Each physics component must be given a collision shape");
		}
	}

	void PhysicsComponent::OnHierarchyChange(bool goingUp)
	{
		//Call this on our children
		RefCountedTreeNode::OnHierarchyChange(goingUp);

		if(!goingUp)
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

				list<RefCountedTreeNode*>::Iterator it = children.begin();
				for(; it != children.end(); ++it)
				{
					PhysicsComponent* curr = static_cast<PhysicsComponent*>(*it);
					compAbsShape->addChildShape(curr->getTransform(), curr->absoluteCShape);
					absoluteMass += curr->absoluteMass;
				}

				it = children.begin();
				absoluteCOG.setValue(0.0f, 0.0f, 0.0f);
				absoluteCOG += cog * (mass / absoluteMass);
				for(; it != children.end(); ++it)
				{
					PhysicsComponent* curr = static_cast<PhysicsComponent*>(*it);
					absoluteCOG += curr->cog
						* (curr->absoluteMass / absoluteMass);
				}
			}

			//Only entities with no parents should have rigid bodies.
			if(parent == nullptr)
			{
				//TODO: Update body mass and possibly motionstate

			
				// If the body already exists, this entity was already a parent
				// beforehand.  We just need to update it's collision shape
				// with the new absolute shape.
				if(body != nullptr)
				{
					body->setCollisionShape(absoluteCShape);
					body->setMassProps(absoluteMass, btVector3());
				}
				//Otherwise we need to instantiate a new rigid body and register
				//it with the physics world.
				else
				{
					btRigidBody::btRigidBodyConstructionInfo ci(absoluteMass, nullptr,
						absoluteCShape);
					body = new btRigidBody(ci);

				}

				//Set new center of gravity
				btTransform comTrans;
				comTrans.setOrigin(absoluteCOG);
				body->setCenterOfMassTransform(comTrans);
			}
			//If the entitiy is no longer a parent, remove it from the world
			else if(body != nullptr)
			{
				physMan->GetWorld()->removeRigidBody(body);
				delete body;
				body = nullptr;
			}
		}
	}
} //end namespace Core