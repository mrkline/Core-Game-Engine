#pragma once

#include <list>
#include <string>
#include <memory>

#include "GameComponent.h"
#include "NamedClass.h"
#include "CoreTransform.h"

namespace Core
{
	class GameObjectManager;
	class Scene;

	/*! 
	\brief Represents an entity in the game world.

	The GameObject is the core object of the engine.  A tree of GameObjects is managed by the
	scene.  Each object can have a collection of GameComponenents, which do various things
	such as manage physics, sound, scripts, logic, etc.
	*/
	class GameObject : public NamedClass
	{
	public:

		typedef std::list<GameComponent*> ComponentList;

		/*!
		\brief Constructor.
		\param parent The parent of the object, or null if it has none
		\param startingTransform The object's starting local transform
		\param id An ID that can be used to identify the object
		\param name A name that can be used to identify the object
		*/
		GameObject(GameObject* parent, const Transform& startingTransform = Transform(),
			int id = -1, const std::string& name = std::string());

		//! Deconstructor. Deletes all components owned by this object
		virtual ~GameObject();

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

		//! Tests if the object has children or not
		bool HasChildren() { return !children.empty(); }

		/*!
		\brief Tests if this object has a given child
		\param child The object to search for in this object's children
		\return true if this object has the specified child
		*/
		bool HasChild(GameObject* child);

		/*!
		\brief Tests if this object has a given child
		\param childName The name of the object to search for in this object's children
		\return true if this object has a child with the given name

		In case of children with identical names, the first in the list of children is returned
		*/
		bool HasChild(const std::string& childName);

		/*!
		\brief Tests if this object has a given child
		\param childId The ID of the object to search for in this object's children
		\return true if this object has a child with the given ID

		In case of children with identical IDs, the first in the list of children is returned
		*/
		bool HasChild(int childId);

		/*!
		\brief Tests if this object has a given ancestor
		\param ancestor The object to search for in this object's ancestry
		\return true if this object has the specified ancestor
		*/
		bool HasAncestor(GameObject* ancestor);

		/*!
		\brief Tests if this object has a given ancestor
		\param ancestorName The name of the object to search for in this object's ancestry
		\return true if this object has an ancestor with the given name

		In case of ancestors with identical names, the ancestor closest to this object is returned
		*/
		bool HasAncestor(const std::string& ancestorName);

		/*!
		\brief Tests if this object has a given ancestor
		\param ancestorId The ID of the object to search for in this object's ancestry
		\return true if this object has an ancestor with the given ID

		In case of ancestors with identical IDs, the ancestor closest to this object is returned
		*/
		bool HasAncestor(int ancestorId);


		/*!
		\brief Tests if this object has a given descendant
		\param descendant The object to search for in this object's descendance
		\return true if this object has the given descendant

		Performs a BFS for the descendant
		*/
		bool HasDescendant(GameObject* descendant);

		/*!
		\brief Tests if this object has a given descendant
		\param descendantName The name of the object to search for in this object's descendance
		\return true if this object has an descendant with the given name

		In case of descendants with identical names, the descendant first found in a BFS is returned
		*/
		bool HasDescendant(const std::string& descendantName);

		/*!
		\brief Tests if this object has a given descendant
		\param descendantId The ID of the object to search for in this object's descendance
		\return true if this object has an descendant with the given ID

		In case of descendants with identical IDs, the descendant first found in a BFS is returned
		*/
		bool HasDescendant(int descendantId);

		/*!
		\brief Sets the object's parent
		\param newParent A pointer to the new parent, or null if no parent is desired
		
		This method will remove the component from its current parent (if it exists) and
		set it to the new one.
		*/
		void SetParent(GameObject* newParent);

		//! Gets the object's parent
		GameObject* GetParent() const { return parent; }

		//! Gets a list of the node's children
		std::list<GameObject*>& GetChildren() { return children; }

		//! Gets a list of the node's children
		const std::list<GameObject*>& GetChildren() const { return children; }

		//! Removes this object from its parent
		void RemoveFromParent();

		/*!
		\brief Add a child object
		\param child Child object to add

		This method removes the child from its current parent, sets this component
		as its parent, and adds the new child to this component's list of children.
		Adding a child transfers ownership of the child to this game object.
		All children will be deleted on this object's destruction.
		*/
		void AddChild(GameObject* child);

		/*!
		\brief Remove a child object
		\param child Child object to remove

		Removing the child transfers ownership back to the client's code.
		The client code is responsible for deleting the child after this is called.
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
		std::auto_ptr<ComponentList> FindNearestDescendantComponents(GameComponent::EType compType, bool includingThisObject);

	protected:
		ComponentList components; //!< A linked list of this object's components
		Transform trans; //!< The transform for the game object
		Transform absTrans; //!< The cumulative absolute transform for the game object

		//! Node's parent, or null if it has none
		GameObject* parent; 
		//! Linked list of children of this node
		std::list<GameObject*> children; 
	};
} // end namespace Core