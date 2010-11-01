#include "GameObject.h"
#include "ErrorHandling.h"
using namespace irr;
using namespace core;

namespace GameCore
{
	GameObject::GameObject(GameObject* parent, GameObjectManager* objMan, irr::s32 id, const irr::core::stringc& name)
		: NamedClass(id, name)
	{
		if(parent != nullptr)
		{
			parent->AddChild(this);
		}
	}

	GameObject::~GameObject()
	{
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			(*it)->drop();
		}
	}

	void GameObject::AddComponent(GameComponent* newComponent)
	{
		//Make sure it's not null
		if(newComponent == nullptr)
		{
			throw new Error::ArgumentNullException("A game object cannot add a null component.", __FUNCTION__);
		}
		//Make sure we don't already have a component of this type
		GameComponent::EType ncType = newComponent->GetComponentType();
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			if((*it)->GetComponentType() == ncType)
			{
				throw new Error::ArgumentException("A game object can only have one of each type of component", __FUNCTION__);
			}
		}
		components.push_back(newComponent);
		newComponent->grab();
	}

	void GameObject::RemoveComponent(GameComponent* toRemove)
	{
		if(toRemove == nullptr)
		{
			throw new Error::ArgumentNullException("A game object has no null components to remove.", __FUNCTION__);
		}
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			if((*it) == toRemove)
			{
				(*it)->drop();
				components.erase(it);
				return;
			}
		}
		throw new Error::ArgumentException("The game object did not contain the given component.", __FUNCTION__);
	}

	void GameObject::ClearComponents()
	{
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			(*it)->drop();
		}
		components.clear();
	}

	GameComponent* GameObject::GetComponentByType(GameComponent::EType type)
	{
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			GameComponent* curr = (*it);
			if(curr->GetComponentType() == type)
			{
				return curr;
			}
		}
		return nullptr;
	}

	void GameObject::SetParent(GameObject* newParent)
	{
		RefCountedTreeNode::SetParent(parent);
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			(*it)->OwnerSetParent(newParent);
		}
	}

	void GameObject::RemoveFromParent(bool updateHierarchy)
	{
		RefCountedTreeNode::RemoveFromParent(updateHierarchy);
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			(*it)->OwnerRemovedFromParent(updateHierarchy);
		}
	}

	void GameObject::AddChild(GameObject* child)
	{
		RefCountedTreeNode::AddChild(child);
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			(*it)->OwnerAddedChild(child);
		}
	}

	void GameObject::RemoveChild(GameObject* child)
	{
		RefCountedTreeNode::RemoveChild(child);
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			(*it)->OwnerRemovedChild(child);
		}
	}

	void GameObject::RemoveAllChildren()
	{
		RefCountedTreeNode::RemoveAllChildren();
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			(*it)->OwnerRemovedAllChildren();
		}
	}

	GameComponent* GameObject::FindNearestAncestorComponent(GameComponent::EType compType)
	{
		GameComponent* ret;
		for(GameObject* curr = static_cast<GameObject*>(parent); curr != nullptr; curr = static_cast<GameObject*>(curr->parent))
		{
			ret = curr->GetComponentByType(compType);
			if(ret != nullptr)
				return ret;
		}
		return nullptr;
	}

	const list<GameComponent*>& GameObject::FindNearestDescendantComponents(GameComponent::EType compType)
	{
		holder.clear();
		DescendantSearchRecursor(&holder, this, compType);
		return holder;
	}

	void  GameObject::DescendantSearchRecursor(list<GameComponent*>* compList,
			GameObject* obj, GameComponent::EType compType)
	{
		GameComponent* comp = obj->GetComponentByType(compType);
		if(comp != nullptr)
		{
			compList->push_back(comp);
			return;
		}
		else
		{
			list<RefCountedTreeNode*>& children = obj->GetChildren();
			for(list<RefCountedTreeNode*>::Iterator it = children.begin();
				it != children.end(); ++it)
			{
				DescendantSearchRecursor(compList, static_cast<GameObject*>(*it), compType);
			}
		}
	}
} //end namespace GameCore