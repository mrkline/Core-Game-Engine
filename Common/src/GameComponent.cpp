#include <GameComponent.h>
#include <ErrorHandling.h>
#include <GameObject.h>

using namespace std;

namespace Core
{
	GameComponent::GameComponent(GameObject* objOwner)
		: owner(objOwner)
	{
		if(objOwner == nullptr)
		{
			throw new Error::ArgumentException("A game component cannot have a null owner", __FUNCTION__);
		}
		//Automatically link us into this component's hierarchy
		GameObject::ComponentList add = owner->FindNearestDescendantComponents(GetComponentType());
		for(GameObject::ComponentList::const_iterator it = add.begin();
			it != add.end(); ++it)
		{
			AddChild(*it);
		}
		SetParent(owner->FindNearestAncestorComponent(GetComponentType()));
	}

	GameComponent::~GameComponent()
	{
		//Strip ourselves out of the hierarchy by having the parent add our children.
		//The parent will automatically have the children remove us
		if(parent != nullptr)
		{
			for(list<TreeNode*>::iterator it = children.begin();
				it != children.end(); ++it)
			{
				parent->AddChild(*it);
			}
		}
	}

	void GameComponent::OwnerAddedChild(GameObject* added)
	{
		GameObject::ComponentList add = added->FindNearestDescendantComponents(GetComponentType(), true);
		for(GameObject::ComponentList::const_iterator it = add.begin();
			it != add.end(); ++it)
		{
			AddChild(*it);
		}
	}

	void GameComponent::OwnerRemovedChild(GameObject* removed)
	{
		GameObject::ComponentList remove = removed->FindNearestDescendantComponents(GetComponentType(), true);
		for(GameObject::ComponentList::const_iterator it = remove.begin();
			it != remove.end(); ++it)
		{
			RemoveChild(*it);
		}
	}

	void GameComponent::OwnerSetParent(GameObject* parent)
	{
		SetParent(owner->FindNearestAncestorComponent(GetComponentType()));
	}

	void GameComponent::OwnerRemovedFromParent(bool updateHierarchy)
	{
		RemoveFromParent(updateHierarchy);
	}
} //end namespace Core