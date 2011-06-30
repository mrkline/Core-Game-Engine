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
	Tree node functions should be managed through the Onwer... functions.
	Because of this, we're making the tree node base class protected
	*/
	class GameComponent : protected TreeNode
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
		virtual EType GetComponentType() { return E_GCT_BASE; }

		//These functions are called when hierarchy changes involving the GameObject owner
		//occur, so that the components can update their own trees.

		//! Called when the owning GameObject added a child. Updates the component's tree
		void OwnerAddedChild(GameObject* child);

		//! Called when the owning GameObject removed a child. Updates the component's tree
		void OwnerRemovedChild(GameObject* removed);

		//! Called when the owning GameObject has a new parent. Updates the component's tree
		void OwnerSetParent(GameObject* parent);

		//! Called when the owning GameObject is removed from its parent. Updates the component's tree
		void OwnerRemovedFromParent(bool updateHierarchy);

		//! Gets the GameObject that owns this component
		GameObject* GetOwner() const { return owner; }

	protected:
		GameObject* owner; //!< Owning game object
	};
} //end namespace Core