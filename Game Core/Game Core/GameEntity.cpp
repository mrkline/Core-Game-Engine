#include "GameEntity.h"
#include "Orienter.h"

namespace GameCore
{
	//Orienter used to convert 6DOF rotations into a Euler rotation
	static Orienter orient;

	GameEntity::GameEntity()
		: mass(1.0f), absoluteMass(1.0f),
		lastVelocityUpdateTime(0), lastTransformUpdateTime(0)
	{
	}

	void GameEntity::SetParent(GameEntity* newParent)
	{
		if(newParent == this)
		{
			//TODO: Throw exception
		}
		parent = newParent;
	}

	void GameEntity::AddChild(GameEntity* newChild)
	{
		if(newChild == nullptr || newChild == this)
		{
			//TODO: Throw Exception
		}
		for(list<GameEntity*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			if(*it == newChild)
			{
				//TODO: Throw exception
			}
		}
		newChild->grab();
		children.push_back(newChild);
	}

	void GameEntity::RemoveChild(GameEntity* toRemove)
	{
		if(toRemove == nullptr || toRemove == this)
		{
			//TODO: Throw Exception
		}
		for(list<GameEntity*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			if(*it == toRemove)
			{
				(*it)->SetParent(nullptr);
				(*it)->drop();
				children.erase(it);
			}
		}
		//TODO: Throw Exception
	}

	void GameEntity::RemoveAllChildren()
	{
		for(list<GameEntity*>::Iterator it = children.begin();
			it != children.end(); ++it)
		{
			(*it)->SetParent(nullptr);
			(*it)->drop();
		}

		children.clear();
	}

	void GameEntity::UpdateTransform(u32 currTime)
	{
		f32 dt = static_cast<f32>(currTime - lastTransformUpdateTime)
			/ 1000.0f;

		transform.setTranslation(transform.getTranslation() + velocity * dt);
		rotation += angularVelocity * dt;
		transform.setRotationRadians(orient.GetEulerRotationRadians(rotation));
		lastTransformUpdateTime = currTime;
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