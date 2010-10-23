#include "GameEntity.h"
#include "Orienter.h"
#include "ErrorHandling.h"

namespace GameCore
{
	//Orienter used to convert 6DOF rotations into a Euler rotation
	static Orienter orient;

	GameEntity::GameEntity()
		: sNode(nullptr), parent(nullptr),
		mass(1.0f), absoluteMass(1.0f),
		lastVelocityUpdateTime(0), lastTransformUpdateTime(0)
	{
	}

	void GameEntity::SetParent(GameEntity* newParent)
	{
		if(newParent == this)
		{
			throw new ArgumentException("A game entity cannot be its own parent.", __FUNCTION__);
		}
		sNode->setParent(newParent->sNode);
		grab();
		RemoveFromParent();
		parent = newParent;
		if(parent != nullptr)
		{
			parent->AddChild(this);
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
		UpdateAbsoluteMassAndCOG();
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
				toRemove->drop();
				children.erase(it);
				UpdateAbsoluteMassAndCOG();
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
			(*it)->drop();
		}

		children.clear();
	}

	void GameEntity::RemoveFromParent()
	{
		sNode->remove();
		if (parent != nullptr)
				parent->RemoveChild(this);
	}

	void GameEntity::UpdateAbsoluteMassAndCOG()
	{
		absoluteMass = mass;
		list<GameEntity*>::Iterator it = children.begin();
		for(; it != children.end(); ++it)
		{
			(*it)->UpdateAbsoluteMassAndCOG();
			absoluteMass += (*it)->getAbsoluteMass();
		}
		it = children.begin();

		absoluteCOG.set(0.0f, 0.0f, 0.0f);
		absoluteCOG += cog * (mass / absoluteMass);
		for(; it != children.end(); ++it)
		{
			GameEntity* curr = *it;
			absoluteCOG += curr->GetAbsoluteCOG()
				* (curr->getAbsoluteMass() / absoluteMass);
		}
	}
} //end namespace GameCore