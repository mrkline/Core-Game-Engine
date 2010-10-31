#pragma once
#include "RefCountedTreeNode.h"

namespace GameCore
{
	class GameObject;

	//Tree node functions should be managed through the Onwer... functions.
	//Because of this, we're making the tree node base class protected
	class GameComponent : protected RefCountedTreeNode, virtual public irr::IReferenceCounted
	{
	public:
		enum EType
		{
			E_GCT_SCENE_NODE,
			E_GCT_PHYSICS,
			E_GCT_COUNT //Not a type, but a count of the types
		};

		GameComponent(GameObject* objOwner);
		virtual ~GameComponent() {}

		virtual EType GetComponentType() = 0;
		virtual void OwnerAddedChild(GameObject* child) {}
		virtual void OwnerRemovedChild(GameObject* removed) {}
		virtual void OwnerRemovedAllChildren() {}
		virtual void OwnerSetParent(GameObject* parent) {}
		virtual void OwnerRemovedFromParent() {}

		GameObject* GetOwner() { return owner; }

	protected:
		GameObject* owner;
	};
} //end namespace GameCore