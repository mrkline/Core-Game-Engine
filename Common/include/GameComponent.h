#pragma once
#include <RefCountedTreeNode.h>

namespace Core
{
	class GameObject;

	//Each GameObject can hold a collection of components.  Components have their own managers
	//within the GameObjectManager and their own trees, which are kept updated to reflect the tree
	//structure of their owner GameObjects
	//Tree node functions should be managed through the Onwer... functions.
	//Because of this, we're making the tree node base class protected
	class GameComponent : protected RefCountedTreeNode, virtual public ReferenceCounted
	{
	public:
		//The types of Game components to be returned by GetComponentType()
		enum EType
		{
			E_GCT_GRAPHICS, //A graphics component
			E_GCT_PHYSICS, //A physics component
			E_GCT_LOGIC,	//A logic component
			E_GCT_UNKNOWN, //A default value that generally shouldn't be used
			E_GCT_COUNT //Not a type, but a count of the types
		};

		GameComponent(GameObject* objOwner);
		virtual ~GameComponent();

		//See EType
		virtual EType GetComponentType() { return E_GCT_UNKNOWN; }

		//These functions are called when hierarchy changes involving the GameObject owner
		//occur, so that the components can update their own trees.

		virtual void OwnerAddedChild(GameObject* child);
		virtual void OwnerRemovedChild(GameObject* removed);
		virtual void OwnerRemovedAllChildren();
		virtual void OwnerSetParent(GameObject* parent);
		virtual void OwnerRemovedFromParent(bool updateHierarchy);


		GameObject* GetOwner() const { return owner; }

	protected:
		GameObject* owner;
	};
} //end namespace Core