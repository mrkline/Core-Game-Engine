#include "GameEntity.h"
#include "Orienter.h"
#include "ErrorHandling.h"

namespace GameCore
{
	GameEntity::GameEntity(ISceneNode* sceneNode,  btCollisionShape* collisionShape,
		const btTransform& trans, GameEntity* parentEnt, s32 entId, const stringc& entName)
		: sNode(sceneNode), cShape(collisionShape), absoluteCShape(nullptr), transform(trans), mass(1.0f),
		id(entId), name(entName)
	{
		if(sceneNode == nullptr)
		{
			throw new ArgumentException("A game entity must be tied to a scene node.", __FUNCTION__);
		}
		if(collisionShape == nullptr)
		{
			throw new ArgumentException("A game entity must have a rigid body (to hold the transform at the very least)", __FUNCTION__);
		}
		SetParent(parentEnt);
	}

	void GameEntity::SetParent(GameEntity* newParent)
	{
		if(newParent == this)
		{
			throw new ArgumentException("A game entity cannot be its own parent.", __FUNCTION__);
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

	void GameEntity::AddChild(GameEntity* newChild)
	{
		if(newChild == nullptr || newChild == this)
		{
			throw new ArgumentException("A child of an entity cannot be null or itself.", __FUNCTION__);
		}
		for(list<GameEntity*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			if(*it == newChild)
			{
				throw new ArgumentException("The provided entity is already a child of this entity.", __FUNCTION__);
			}
		}
		sNode->addChild(newChild->sNode);
		newChild->grab();
		newChild->RemoveFromParent();
		children.push_back(newChild);
		newChild->parent = this;
		UpdateHierarchicalData();
	}

	void GameEntity::RemoveChild(GameEntity* toRemove)
	{
		if(toRemove == nullptr)
		{
			throw new ArgumentException("You cannot remove a null child.", __FUNCTION__);
		}
		for(list<GameEntity*>::Iterator it = children.begin();
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
		throw new ArgumentException("The given entity is not a child of this entity.", __FUNCTION__);
	}

	void GameEntity::RemoveAllChildren()
	{
		sNode->removeAll();
		for(list<GameEntity*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			(*it)->parent = nullptr;
			(*it)->UpdateHierarchicalData();
			(*it)->drop();
		}

		children.clear();
		UpdateHierarchicalData();
	}

	void GameEntity::RemoveFromParent(bool updateHD)
	{
		sNode->remove();
		if (parent != nullptr)
				parent->RemoveChild(this);
		if(updateHD)
			UpdateHierarchicalData();
	}

	void GameEntity::UpdateHierarchicalData()
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

			list<GameEntity*>::Iterator it = children.begin();
			for(; it != children.end(); ++it)
			{
				GameEntity* curr = *it;
				curr->UpdateHierarchicalData();
				compAbsShape->addChildShape(curr->getTransform(), curr->absoluteCShape);
				absoluteMass += curr->absoluteMass;
			}
			it = children.begin();

			absoluteCOG.set(0.0f, 0.0f, 0.0f);
			absoluteCOG += cog * (mass / absoluteMass);
			for(; it != children.end(); ++it)
			{
				GameEntity* curr = *it;
				absoluteCOG += curr->cog
					* (curr->absoluteMass / absoluteMass);
			}
		}
	}
} //end namespace GameCore