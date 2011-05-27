#pragma once

#include <list>
#include <string>
#include <memory>

#include "GameComponent.h"
#include "TreeNode.h"
#include "NamedClass.h"
#include "CoreTransform.h"

namespace Core
{
	class GameObjectManager;

	/*! 
	\brief Represents an entity in the game world.

	The GameObject is the core object of the engine.  A tree of GameObjects is managed by the
	scene.  Each object can have a collection of GameComponenents, which do various things
	such as manage physics, sound, scripts, logic, etc.
	*/
	class GameObject : public TreeNode, public NamedClass
	{
	public:
		typedef std::list<GameComponent*> ComponentList;

		/*!
		\brief Constructor.
		\param parent The parent of the object, or null if it has none
		\param objMan The object manager
		\todo Replace objMan with scene
		\param id An ID that can be used to identify the object
		\param name A name that can be used to identify the object
		*/
		GameObject(GameObject* parent, GameObjectManager* objMan,
			int id = -1, const std::string& name = std::string());

		//! Deconstructor. Deletes all components owned by this object
		virtual ~GameObject();

		/*!
		\brief Gets the object manager
		\todo Replace with scene
		*/
		GameObjectManager* getObectManager() const { return man; }

		//! Updates the absolute transform of the object
		void Update();
	
		//! Sets the object's transform to a new one
		void SetTransform(const Transform& newTransform) { trans = newTransform; }
		//! Gets the object's transform
		Transform& GetTransform() { return trans; }
		//! Gets the object's transform
		const Transform& GetTransform() const { return trans; }
		
		/*!
		\brief Called by Update to update the absolute transform
		\see Update

		Only updates based on parent, does not recursively travel up the hierarchy.
		Because of this update should be calld on object tree from root down
		*/
		void UpdateAbsoluteTransform();
		//! Gets the absolute transform
		const Transform& GetAbsoluteTransform() const { return absTrans; }

		/*!
		\brief Add a component to the object.
		\param newComponent Component to add to this object

		A game object can only hold one of each type of component.
		Adding the component transfers ownership of the component to the game object.
		After this is called, this game object is responsible for the deletion of the component.
		*/
		void AddComponent(GameComponent* newComponent);

		/*!
		\brief Remove a component from the object
		\param toRemove The component to remove

		Once a component is removed, the client code is responsible for its deletion again.
		*/
		void RemoveComponent(GameComponent* toRemove);
		//! Deletes all components
		void DeleteComponents();
		
		//Returns nullptr if the GameObject does not have this type
		GameComponent* GetComponentByType(GameComponent::EType type);
		
		//! Gets a list of the components owned by this object
		const ComponentList& GetComponents() const { return components; }
		//! Gets a list of the components owned by this object
		ComponentList& GetComponents() { return components; }

		// Functions to manipulate the object tree

		/*!
		\brief Sets the object's parent
		\param newParent A pointer to the new parent, or null if no parent is desired
		\todo Automatically tie to some sort of root?
		*/
		void SetParent(GameObject* newParent);

		//! Gets the object's parent
		GameObject* GetParent() { return static_cast<GameObject*>(parent); }

		/*!
		\brief Removes this object from its parent
		\param updateHierarchy Set to true to notify the surrounding tree of the change
		*/
		void RemoveFromParent(bool updateHierarchy = true);

		/*!
		\brief Add a child object
		\param child Child object to add
		\return An ECode that indicates the outcome of the additon
		\warning If this object is deleted, all of its children are deleted as well.
		*/
		void AddChild(GameObject* child);

		/*!
		\brief Remove a child object
		\param child Child object to remove
		\return An ECode that indicates the outcome of the removal
		*/
		void RemoveChild(GameObject* child);

		//! Delete all children
		void DeleteAllChildren();

		/*!
		\brief Used by components to find their nearest ancestor while assembling their own trees
		\param compType The component type to search for
		\return The nearest ancestor component of compType. The search starts with this object's parent.
		*/
		GameComponent* FindNearestAncestorComponent(GameComponent::EType compType);

		/*!
		\brief Used by components to find their nearest descendants while assembling their own trees
		\param compType THe component type to search for
		\param includingThisObject If true, start the search with this object, otherwise start with this object's children.
		\return An auto_ptr to a list of nearest descendant componentes of compType.
				This ensures that the list will be deleted when it goes out of scope.
		*/
		std::auto_ptr<ComponentList> FindNearestDescendantComponents(GameComponent::EType compType, bool includingThisObject = false);

	protected:
		//! brief The game object manager
		//! \todo Replace with Scene
		GameObjectManager* man;
		std::list<GameComponent*> components; //!< A linked list of this object's components
		Transform trans; //!< The transform for the game object
		Transform absTrans; //!< The cumulative absolute transform for the game object
	};
} // end namespace Core