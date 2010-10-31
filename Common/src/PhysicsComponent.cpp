#include "PhysicsComponent.h"
#include "EntityManager.h"
#include "PhysicsManager.h"
#include "ErrorHandling.h"
#include "IrrlichtBulletConversions.h"

namespace GameCore
{
	PhysicsComponent::PhysicsComponent(EntityManager* entManager, ISceneNode* sceneNode,  btCollisionShape* collisionShape,
		const btTransform& trans, PhysicsComponent* parentEnt, s32 entId, const stringc& entName)
		: manager(entManager), body(nullptr), sNode(sceneNode), cShape(collisionShape), absoluteCShape(nullptr),
		transform(trans), mass(1.0f), id(entId), name(entName)
	{
		if(sceneNode == nullptr)
		{
			throw new Error::ArgumentNullException("A game entity must be tied to a scene node.", __FUNCTION__);
		}
		if(collisionShape == nullptr)
		{
			throw new Error::ArgumentNullException("A game entity must have a collision shape, even if collision will be disabled.", __FUNCTION__);
		}
		SetParent(parentEnt);
		matrix4 irrTransform;
		Convert::TransformBTI(transform, irrTransform);
		sNode->setRotation(irrTransform.getRotationDegrees());
		sNode->setPosition(irrTransform.getTranslation());
	}

	void PhysicsComponent::SetParent(PhysicsComponent* newParent)
	{
		if(newParent == this)
		{
			throw new Error::ArgumentException("A game entity cannot be its own parent.", __FUNCTION__);
		}
		sNode->setParent(newParent->sNode);
		grab();
		RemoveFromParent(false);
		if(newParent != nullptr)
		{
			newParent->AddChild(this);
		}
		else
		{
			UpdateHierarchicalData();
		}
		drop();
	}

	void PhysicsComponent::AddChild(PhysicsComponent* newChild)
	{
		if(newChild == nullptr || newChild == this)
		{
			throw new Error::ArgumentException("A child of an entity cannot be null or itself.", __FUNCTION__);
		}
		for(list<PhysicsComponent*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			if(*it == newChild)
			{
				throw new Error::ArgumentException("The provided entity is already a child of this entity.", __FUNCTION__);
			}
		}
		sNode->addChild(newChild->sNode);
		newChild->grab();
		newChild->RemoveFromParent();
		children.push_back(newChild);
		newChild->parent = this;
		UpdateHierarchicalData();
	}

	void PhysicsComponent::RemoveChild(PhysicsComponent* toRemove)
	{
		if(toRemove == nullptr)
		{
			throw new Error::ArgumentNullException("You cannot remove a null child.", __FUNCTION__);
		}
		for(list<PhysicsComponent*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			if(*it == toRemove)
			{
				sNode->removeChild(toRemove->sNode);
				toRemove->parent = nullptr;
				toRemove->UpdateHierarchicalData();
				toRemove->drop();
				children.erase(it);
				UpdateHierarchicalData();
				return;
			}
		}
		throw new Error::ArgumentException("The given entity is not a child of this entity.", __FUNCTION__);
	}

	void PhysicsComponent::RemoveAllChildren()
	{
		sNode->removeAll();
		for(list<PhysicsComponent*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			(*it)->parent = nullptr;
			(*it)->UpdateHierarchicalData();
			(*it)->drop();
		}

		children.clear();
		UpdateHierarchicalData();
	}

	void PhysicsComponent::RemoveFromParent(bool updateHD)
	{
		sNode->remove();
		if (parent != nullptr)
				parent->RemoveChild(this);
		if(updateHD)
			UpdateHierarchicalData();
	}

	void PhysicsComponent::UpdateHierarchicalData()
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