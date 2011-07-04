#pragma once

#include <list>

namespace Core
{
	class GameObject;

	/*!
	\brief A component of a GameObject
	\see GameObject

	Each GameObject can hold a collection of components.  Components have their own managers
	and their own trees, which are kept updated to reflect
	the tree structure of their owner GameObjects.
	Components, once attached to an object, are owned by the object and will be deleted
	by the object's deconstructor, unless they are unattached from the object prior to its destruction.
	*/
	class GameComponent
	{
	public:
		//! The types of Game components to be returned by GetComponentType()
		enum EType : unsigned int
		{
			E_GCT_BASE, //!< A default value that generally shouldn't be used
			E_GCT_GRAPHICS, //!< A graphics component
			E_GCT_PHYSICS, //!< A physics component
			E_GCT_LOGIC	//!< A logic component
		};

		GameComponent(bool updateOnChildrenChange = true);

		virtual ~GameComponent() { }

		//! Called by a component's owning GameObject when it is added.
		//! This should only be called by the object itself
		void BindToOwner(GameObject* objOwner);

		//! Called by a component's owning GameObject when it is removed
		//! This should only be called by the object itself
		void UnbindFromOwner();

		/*!
		\brief Gets the type this component is.

		RTTI in C++ is expensive, so in order to allow casting operations from
		the base GameComponent class, this type field is used
		*/
		virtual EType GetComponentType() const { return E_GCT_BASE; }

		//These functions are called when hierarchy changes involving the GameObject owner
		//occur, so that the components can update their own trees.

		//! Called when the owning GameObject added a child. Updates the component's tree
		void OwnerAddedChild(GameObject* child);

		//! Called when the owning GameObject removed a child. Updates the component's tree
		void OwnerRemovedChild(GameObject* removed);

		//! Called when the owning GameObject has a new parent. Updates the component's tree
		void OwnerSetParent(GameObject* parent);

		//! Called when the owning GameObject is removed from its parent. Updates the component's tree
		void OwnerRemovedFromParent();

		//! Gets the GameObject that owns this component
		GameObject* GetOwner() const { return owner; }

		//! Gets the tree node's parent, or null if it has none
		GameComponent* GetParent() const { return parent; }
	
		//! Gets a list of the node's children
		std::list<GameComponent*>& GetChildren() { return children; }

		//! Gets a list of the node's children
		const std::list<GameComponent*>& GetChildren() const { return children; }

		/*!
		\brief Sets whether or not the node cares about hierarchy changes that occur to its children
		\see OnHierarchyChange
		*/
		void SetPostCallbaksOnChildrenChanges(bool update) { caresAboutChildren = update; }

		/*!
		\brief Gets whether or not the node cares about hierarchy changes that occur to its children
		\see OnHierarchyChange
		*/
		bool GetPostCallbaksOnChildrenChanges() const { return caresAboutChildren; }

		/*!
		\brief Used to update any necessary info by a derived class when the tree changes.

		If caresAboutChildren is false, changes to children will not cause this to be called.
		*/
		virtual void OnHierarchyChange() { }

	protected:
		//! Owning game object
		GameObject* owner;
		
		//! Component's parent, or null if it has none
		GameComponent* parent; 

		//! Linked list of children of this component
		std::list<GameComponent*> children; 

		//! If this is true, changes to children will not cause OnHierarchyChange to be called
		bool caresAboutChildren;

	private:

		/*!
		\brief Used internally to set the parent of the component
		\param newParent The new parent component of this component

		This method will remove the component from its current parent (if it exists) and
		set it to the new one.
		*/
		void SetParent(GameComponent* newParent);

		/*! 
		\brief Used internally to add a child to this component
		\param newChild The child to add to this component

		This method removes the child from its current parent, sets this component
		as its parent, and adds the new child to this component's list of children.
		*/
		void AddChild(GameComponent* newChild);

		/*!
		\brief Used internally to remove a child of this component
		\param toRemove The component to remove
		*/
		void RemoveChild(GameComponent* toRemove);

		/*!
		\brief Used internally to remove this component from its parent
		\param updateHD true to post hierarchy change callbacks

		The callbacks are optional since this method is sometimes used by other hierarchy manipulation
		methods which already post callbacks
		*/
		void RemoveFromParent(bool updateHD);

		//! Called internally to call OnHierarchyChange on all those in the hierarchy who care
		void UpdateHierarchy();
	};
} //end namespace Core