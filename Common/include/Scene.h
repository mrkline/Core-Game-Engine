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

	protected:
		std::auto_ptr<GameObject> rootObject; //!< Root GameObject. All game objects in the scene link back to this.
		PhysicsManager* physMan; //!< Physics manager, or null if physics are not used in this scene
	};
} // end namespace Core