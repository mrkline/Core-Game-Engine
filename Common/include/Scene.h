#pragma once

namespace Core
{
	class GameObject;
	class PhysicsManager;

	//A scene contains everything you would render, be it the game scene, a UI scene, etc.
	class Scene
	{
	public:
		Scene(PhysicsManager* physicsMan);
		virtual ~Scene();

		GameObject* GetRootObject() { return rootObject; }
		PhysicsManager* GetPhysics() { return physMan; }

	protected:
		GameObject* rootObject;
		PhysicsManager* physMan;
	};
} //end namespace Core