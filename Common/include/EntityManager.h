#pragma once

namespace Physics
{
	class PhysicsManager;
} //end namespace Physics

namespace GameCore
{
	//Manages GameEntities, the core pieces of the 
	class EntityManager
	{
	public:
		EntityManager();
		virtual ~EntityManager();

		__forceinline Physics::PhysicsManager* GetPhysicsManager() { return physMan; }

	protected:
		Physics::PhysicsManager* physMan;
	};
} //end namespace GameCore