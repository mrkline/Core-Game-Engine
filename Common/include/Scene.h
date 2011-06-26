#pragma once

#include <memory>

namespace Core
{
	class GameObject;
	class PhysicsManager;

	/*!
	\brief A scene contains everything you would render, be it the game scene, a UI scene, etc.
	\todo Replace ObjectManager with Scene. Game objects should be tied to the scene and its root object.

	The Scene also contains the component managers
	*/
	class Scene
	{
	public:
		/*!
		\brief Construct the scene and attach any needed managers
		\param physicsMan The physics manager to use for the scene, or null if no physics are used
		\todo Allow addition of more managers as they are added
		*/
		Scene(PhysicsManager* physicsMan);

		//! Get the physics manager, or null if physics are not being used
		PhysicsManager* GetPhysics() { return physMan; }

		/*!
		\brief Gets the GameObject. All game objects in the scene should link back to this to be updated
		\return The root GameObject

		The root GameObject has the id 0 and the name "root"
		*/
		GameObject* GetRootObject() { return rootObject.get(); }

	protected:
		//! Root GameObject; should be the ancestor of every active object in the scene
		std::auto_ptr<GameObject> rootObject;
		//! Physics manager, or null if physics are not used in this scene
		PhysicsManager* physMan;
	};
} // end namespace Core