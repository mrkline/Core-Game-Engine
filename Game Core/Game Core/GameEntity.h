#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;

namespace GameCore 
{
	//Thre base object for all game entities
	//Massive TODO: Chuck all the custom physics stuff and put in Bullet stuff.
	class GameEntity : public IReferenceCounted
	{
	public:
		GameEntity();

		void SetParent(GameEntity* newParent);
		GameEntity* GetParent() const { return parent; }
		//Throws an exception if the pointer is null or
		//the child was already in the list
		void AddChild(GameEntity* newChild);
		//Throws an exception if the pointer is null or
		//the child was not in the list
		void RemoveChild(GameEntity* toRemove);
		void RemoveAllChildren();
		//TODO: Get/Set Type?
		//TODO: Get/Set Motion controller
		//TODO: Get thrusterlist
		f32 getMass() const { return mass; }
		f32 getAbsoluteMass() const { return absoluteMass; }
		const vector3df& GetCOG() const { return cog; }
		const vector3df& GetAbsoluteCOG() const { return absoluteCOG; }
		const list<vector3df>& GetExternalForces() const { return externalForces; }
		list<vector3df>& GetExternalForces() { return externalForces; }
		void SetMass(f32 newMass) { mass = newMass; } 
		void SetCOG(const vector3df& newCOG) { cog = newCOG; }
		//Updates absolute transform based on parents.
		//Parents should call this before children
		//as this does not recursively call its parents
		void UpdateAbsoluteTransform();
		/*
		/ Updates absolute mass and center of Mass based on children.
		/ This recursively runs on all children (since unlike
		/ the transform, it is dependant on the children.
		/ Therefore, this should only be called on all tier-1
		/ entities by the manager.
		*/
		void UpdateAbsoluteMassAndCOG();

	protected:
		GameEntity* parent;
		list<GameEntity*> children;
		//TODO: Add type field?
		//TODO: Add motion controller
		//TODO: Add thruster list
		f32 mass;
		//Mass of object plus any child entities
		f32 absoluteMass;
		//Center of gravity in local coordinates
		vector3df cog;
		//Center of gravity of entity with all children
		vector3df absoluteCOG;
		//Forces which would affect the velocity of the entity
		list<vector3df> externalForces;
		u32 lastVelocityUpdateTime;
		u32 lastTransformUpdateTime;
	};
} //end namespace GameCore