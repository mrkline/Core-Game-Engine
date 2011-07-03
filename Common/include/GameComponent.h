#pragma once

#include "TreeNode.h"

namespace Core
{
	class GameObject;

	/*!
	\brief A component of a GameObject
	\see GameObject

	Each GameObject can hold a collection of components.  Components have their own managers
	and their own trees, which are kept updated to reflect
	the tree structure of their owner GameObjects.
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

		//! Called by a component's owning GameObject when it is added
		void BindToOwner(GameObject* objOwner);

		//! Called by a component's owning GameObject when it is removed
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
		GameComponent* GetParent() { return parent; }
	
		//! Gets a list of the node's children
		std::list<GameComponent*>& GetChildren() { return children; }

		//! Gets a list of the node's children
		const std::list<GameComponent*>& GetChildren() const { return children; }

		/*!
		\brief Sets whether or not the node cares about hierarchy changes that occur to its children
		\see OnHierarchyChange
		*/
		void SetUpdateOnChildren(bool update) { caresAboutChildren = update; }

		/*!
		\brief Gets whether or not the node cares about hierarchy changes that occur to its children
		\see OnHierarchyChange
		*/
		bool GetUpdateOnChildren() const { return caresAboutChildren; }

		/*!
		\brief Used to update any necessary info by a derived class when the tree changes.

		If caresAboutChildren is false, changes to children will not cause this to be called.
		*/
		virtual void OnHierarchyChange() { }

	protected:
		GameObject* owner; //!< Owning game object
		
		//! Component's parent, or null if it has none
		GameComponent* parent; 
		//! Linked list of children of this component
		std::list<GameComponent*> children; 

		//! If this is true, changes to children will not cause OnHierarchyChange to be called
		bool caresAboutChildren;

	private:

		void SetParent(GameComponent* newParent);
		void AddChild(GameComponent* newChild);
		void RemoveChild(GameComponent* toRemove);
		void DeleteAllChildren();
		void RemoveFromParent(bool updateHD);

		//! Called internally to call OnHierarchyChange on all those in the hierarchy who care
		void UpdateHierarchy();
	};
} //end namespace Core