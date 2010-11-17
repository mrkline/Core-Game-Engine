#include <GameObject.h>
#include <ErrorHandling.h>
using namespace irr;
using namespace core;

namespace Core
{
	using namespace Error;

	GameObject::GameObject(GameObject* parent, GameObjectManager* objMan, irr::s32 id, const irr::core::stringc& name)
		: NamedClass(id, name)
	{
		if(objMan == nullptr)
		{
			throw new Error::ArgumentNullException("A GameObject needs a non-null pointer to its manager", __FUNCTION__);
		}
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

	ECode GameObject::AddComponent(GameComponent* newComponent)
	{
		//Make sure it's not null
		if(newComponent == nullptr)
		{
			lastError = Error::E_CEK_NULL_ARG;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "A game object cannot add a null component.";
			return lastError;
		}
		//Make sure we don't already have a component of this type
		GameComponent::EType ncType = newComponent->GetComponentType();
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			if((*it)->GetComponentType() == ncType)
			{
				lastError = Error::E_CEK_BAD_ARG;
				lastErrorFunction = __FUNCTION__;
				customLastErrorMessage = "A game object can only have one of each type of component";
				return lastError;
			}
		}
		components.push_back(newComponent);
		newComponent->grab();
		return Error::E_CEK_SUCCESS;
	}

	ECode GameObject::RemoveComponent(GameComponent* toRemove)
	{
		if(toRemove == nullptr)
		{
			lastError = Error::E_CEK_NULL_ARG;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "A game object has no null components to remove.";
			return lastError;
		}
		for(list<GameComponent*>::Iterator it = components.begin();
			it != components.end(); ++it)
		{
			if((*it) == toRemove)
			{
				(*it)->drop();
				components.erase(it);
				return Error::E_CEK_SUCCESS;
			}
		}
		lastError = Error::E_CEK_BAD_ARG;
		lastErrorFunction = __FUNCTION__;
		customLastErrorMessage = "The game object did not contain the given component.";
		return lastError;
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

	ECode GameObject::SetParent(GameObject* newParent)
	{
		ECode ret = RefCountedTreeNode::SetParent(parent);
		if(Succeeded(ret))
		{
			for(list<GameComponent*>::Iterator it = components.begin();
				it != components.end(); ++it)
			{
				(*it)->OwnerSetParent(newParent);
			}
		}
		return ret;
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

	ECode GameObject::AddChild(GameObject* child)
	{
		ECode ret =	RefCountedTreeNode::AddChild(child);
		if(Succeeded(ret))
		{
			for(list<GameComponent*>::Iterator it = components.begin();
				it != components.end(); ++it)
			{
				(*it)->OwnerAddedChild(child);
			}
		}
		return ret;
	}

	ECode GameObject::RemoveChild(GameObject* child)
	{
		ECode ret = RefCountedTreeNode::RemoveChild(child);
		if(Succeeded(ret))
		{
			for(list<GameComponent*>::Iterator it = components.begin();
				it != components.end(); ++it)
			{
				(*it)->OwnerRemovedChild(child);
			}
		}
		return ret;
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
} //end namespace Core