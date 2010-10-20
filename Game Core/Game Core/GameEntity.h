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
		const matrix4& GetLocalTransform() const { return transform; }
		const matrix4& GetAbsoluteTransform() const { return absoluteTransform; }
		//TODO: Get/Set Type?
		//TODO: Get/Set Motion controller
		//TODO: Get thrusterlist
		const vector3df& getVelocity() const { return velocity; }
		const vector3df& getAngularVelocity() { return angularVelocity; }
		f32 getMass() const { return mass; }
		f32 getAbsoluteMass() const { return absoluteMass; }
		const vector3df& GetCOG() const { return cog; }
		const vector3df& GetAbsoluteCOG() const { return absoluteCOG; }
		const list<vector3df>& GetExternalForces() const { return externalForces; }
		list<vector3df>& GetExternalForces() { return externalForces; }

		void SetLocalTransform(const matrix4& newTransform) { transform = newTransform; }
		//This function should generally be avoided.
		//Velocity should be based on forces acting on object
		void SetVelocity(const vector3df& newVelocity) { velocity = newVelocity; }
		//This function should generally be avoided.
		//Angular velocity should be based on forces acting on object
		void SetAngularVelocity(const vector3df& newAVelocity) { angularVelocity = newAVelocity; }
		void SetMass(f32 newMass) { mass = newMass; } 
		void SetCOG(const vector3df& newCOG) { cog = newCOG; }
		//TODO: Access global timer directly		
		void UpdateVelocities(u32 currTime);
		//TODO: Access global timer directly
		void UpdateTransform(u32 currTime);
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
		//6DOF rotation about each axis in rads
		vector3df rotation;
		matrix4 transform;
		matrix4 absoluteTransform;
		//TODO: Add thruster list
		//Velocity of entity in units/sec
		vector3df velocity;
		//Angular velocity in 6DOF system in rads/sec
		//(independant rotations)
		vector3df angularVelocity;
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