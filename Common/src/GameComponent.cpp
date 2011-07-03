#include "GameComponent.h"

#include <queue>

#include "ErrorHandling.h"
#include "GameObject.h"

using namespace std;

namespace Core
{
	GameComponent::GameComponent(bool updateOnChildrenChange)
		: parent(nullptr), caresAboutChildren(updateOnChildrenChange)
	{ }

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

	void GameComponent::OwnerRemovedFromParent()
	{
		RemoveFromParent(true);
	}

	void GameComponent::SetParent(GameComponent* newParent)
	{
		if(newParent == this)
			throw ArgumentException("A game component cannot set itself as its parent.",
				__FUNCTION__);

		RemoveFromParent(false);
		if(newParent != nullptr)
			newParent->AddChild(this);
		else
			UpdateHierarchy();
	}

	void GameComponent::AddChild(GameComponent* newChild)
	{
		if(newChild == nullptr)
			throw ArgumentNullException("A game component cannot add a null child.",
				__FUNCTION__);

		if(newChild == this)
			throw ArgumentException("A game component cannot add itself as a child.",
				__FUNCTION__);

		for(auto it = children.begin(); it != children.end(); ++it)
		{
			// We're trying to add a duplicate child
			if(*it == newChild)
				throw ArgumentException("A game component cannot have duplicate children.",
					__FUNCTION__);
		}

		newChild->RemoveFromParent(false);
		children.push_back(newChild);
		newChild->parent = this;
		UpdateHierarchy();
	}

	void GameComponent::RemoveChild(GameComponent* toRemove)
	{
		if(toRemove == nullptr)
			throw ArgumentNullException("A game component cannot remove a null child.",
				__FUNCTION__);

		for(auto it = children.begin(); it != children.end(); ++it)
		{
			if(*it == toRemove)
			{
				toRemove->parent = nullptr;
				toRemove->UpdateHierarchy();
				children.erase(it);
				UpdateHierarchy();
				return;
			}
		}
		throw ArgumentException("A game component could not find the child that was to be removed.",
			__FUNCTION__);
	}

	void GameComponent::DeleteAllChildren()
	{
		for(auto it = children.begin(); it != children.end(); ++it)
			delete *it;

		children.clear();
		UpdateHierarchy();
	}

	void GameComponent::RemoveFromParent(bool updateHD)
	{
		if (parent != nullptr)
				parent->RemoveChild(this);
		if(updateHD)
			UpdateHierarchy();
	}

	void GameComponent::UpdateHierarchy()
	{
		GameComponent* top = this;

		// Keep walking up the tree until it's time to stop and go back down
		while(top->parent != nullptr && top->parent->caresAboutChildren)
			top = top->parent;
		
		// Walk down the tree (level order), updating all children
		queue<GameComponent*> q;
		q.push(top);
		while(!q.empty())
		{
			GameComponent* curr = q.front();
			q.pop();

			curr->OnHierarchyChange();

			for(auto it = curr->children.begin(); it != curr->children.end(); ++it)
				q.push(*it);
		}
	}
} // end namespace Core