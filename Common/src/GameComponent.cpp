#include "GameComponent.h"
#include "ErrorHandling.h"
#include "GameObject.h"

namespace GameCore
{
	GameComponent::GameComponent(GameObject* objOwner)
		: owner(objOwner)
	{
		if(objOwner == nullptr)
		{
			throw new Error::ArgumentException("A game component cannot have a null owner", __FUNCTION__);
		}
	}

	void GameComponent::OwnerAddedChild(GameObject* added)
	{
		const list<GameComponent*>& add = added->FindNearestDescendantComponents(GetComponentType());
		for(list<GameComponent*>::ConstIterator it = add.begin();
			it != add.end(); ++it)
		{
			AddChild(*it);
		}
	}

	void GameComponent::OwnerRemovedChild(GameObject* removed)
	{
		const list<GameComponent*>& remove = removed->FindNearestDescendantComponents(GetComponentType());
		for(list<GameComponent*>::ConstIterator it = remove.begin();
			it != remove.end(); ++it)
		{
			RemoveChild(*it);
		}
	}

	void GameComponent::OwnerRemovedAllChildren()
	{
		RemoveAllChildren();
	}

	void GameComponent::OwnerSetParent(GameObject* parent)
	{
		SetParent(owner->FindNearestAncestorComponent(GetComponentType()));
	}

	void GameComponent::OwnerRemovedFromParent(bool updateHierarchy)
	{
		RemoveFromParent(updateHierarchy);
	}
} //end namespace GameCore