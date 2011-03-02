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
		enum EType
		{
			E_GCT_GRAPHICS, //!< A graphics component
			E_GCT_PHYSICS, //!< A physics component
			E_GCT_LOGIC,	//!< A logic component
			E_GCT_UNKNOWN, //!< A default value that generally shouldn't be used
			E_GCT_COUNT //!< Not a type, but a count of the types. Should not be used
		};

		//! Constructor. A component <i>must</i> be tied to a GameObject
		GameComponent(GameObject* objOwner);
		//! Deconstructor. Does not delete any children in the graph as the lifetime of
		//! components are bound to their owning objects
		virtual ~GameComponent();

		/*!
		\brief Gets the type this component is.
		\see EType
		\todo Possibly make this able to return values outside EType, allowing for custom types

		RTTI in C++ is expensive, so in order to allow casting operations from
		the base GameComponent class, this type field is used
		*/
		virtual EType GetComponentType() { return E_GCT_UNKNOWN; }

		//These functions are called when hierarchy changes involving the GameObject owner
		//occur, so that the components can update their own trees.

		//! Called when the owning GameObject added a child. Updates the component's tree
		virtual void OwnerAddedChild(GameObject* child);
		//! Called when the owning GameObject removed a child. Updates the component's tree
		virtual void OwnerRemovedChild(GameObject* removed);
		//! Called when the owning GameObject has a new parent. Updates the component's tree
		virtual void OwnerSetParent(GameObject* parent);
		//! Called when the owning GameObject is removed from its parent. Updates the component's tree
		virtual void OwnerRemovedFromParent(bool updateHierarchy);

		//! Gets the GameObject that owns this component
		GameObject* GetOwner() const { return owner; }

	protected:
		GameObject* owner; //!< Owning game object
	};
} //end namespace Core