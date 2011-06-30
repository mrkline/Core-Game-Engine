#include "GameComponent.h"

#include "ErrorHandling.h"
#include "GameObject.h"

using namespace std;

namespace Core
{
	void GameComponent::BindToOwner(GameObject* objOwner)
	{
		if(objOwner == nullptr)
			throw ArgumentException("A game component cannot have a null owner", __FUNCTION__);

		owner = objOwner;

		// Automatically link us into this component's hierarchy
		auto_ptr<GameObject::ComponentList> add = owner->FindNearestDescendantComponents(GetComponentType(), false);

		for(auto it = add->begin(); it != add->end(); ++it)
			AddChild(*it);

		SetParent(owner->FindNearestAncestorComponent(GetComponentType()));
	}

	void GameComponent::UnbindFromOwner()
	{
		if(owner != nullptr)
			throw InvalidOperationException("You cannot unbind a game component that was not bound in the first place", __FUNCTION__);
		
		if(parent != nullptr)
		{
			for(auto it = children.begin(); it != children.end(); ++it)
				parent->AddChild(*it);
		}

		owner = nullptr;
		parent = nullptr;
		children.clear();
	}

	void GameComponent::OwnerAddedChild(GameObject* added)
	{
		auto_ptr<GameObject::ComponentList> add = added->FindNearestDescendantComponents(GetComponentType(), true);

		for(auto it = add->begin(); it != add->end(); ++it)
			AddChild(*it);
	}

	void GameComponent::OwnerRemovedChild(GameObject* removed)
	{
		auto_ptr<GameObject::ComponentList> remove = removed->FindNearestDescendantComponents(GetComponentType(), true);

		for(auto it = remove->begin(); it != remove->end(); ++it)
			RemoveChild(*it);
	}

	void GameComponent::OwnerSetParent(GameObject* parent)
	{
		SetParent(owner->FindNearestAncestorComponent(GetComponentType()));
	}

	void GameComponent::OwnerRemovedFromParent(bool updateHierarchy)
	{
		RemoveFromParent(updateHierarchy);
	}
} // end namespace Core