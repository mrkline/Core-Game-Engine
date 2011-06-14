#include "GameObject.h"

#include <stack>

#include "ErrorHandling.h"

using namespace std;

namespace Core
{
	GameObject::GameObject(GameObject* parent, Scene* owningScene, int id, const std::string& name)
		: NamedClass(id, name), scn(owningScene)
	{
		if(owningScene == nullptr)
			throw ArgumentNullException("A GameObject needs a non-null pointer to its manager",
				__FUNCTION__);

		if(parent != nullptr)
			parent->AddChild(this);
	}

	GameObject::~GameObject()
	{
		// Kill the components
		for(auto it = components.begin(); it != components.end(); ++it)
			delete *it;
	}

	void GameObject::Update()
	{
		UpdateAbsoluteTransform();
		for(auto it = children.begin(); it != children.end(); ++it)
			static_cast<GameObject*>(*it)->Update();
	}

	void GameObject::UpdateAbsoluteTransform()
	{
		if(parent == nullptr)
			absTrans = trans;
		else
			absTrans = trans * static_cast<GameObject*>(parent)->absTrans;
	}

	void GameObject::AddComponent(GameComponent* newComponent)
	{
		// Make sure it's not null
		if(newComponent == nullptr)
			throw ArgumentNullException("A game object cannot add a null component.",
				__FUNCTION__);

		// Make sure we don't already have a component of this type
		GameComponent::EType ncType = newComponent->GetComponentType();
		for(auto it = components.begin(); it != components.end(); ++it)
		{
			if((*it)->GetComponentType() == ncType)
				throw ArgumentException("A game object can only have one of each type of component",
					__FUNCTION__);
		}

		components.push_back(newComponent);
	}

	void GameObject::RemoveComponent(GameComponent* toRemove)
	{
		if(toRemove == nullptr)
			throw ArgumentNullException("A game object has no null components to remove.",
				__FUNCTION__);

		for(auto it = components.begin(); it != components.end(); ++it)
		{
			if((*it) == toRemove)
			{
				components.erase(it);
				return;
			}
		}

		throw ArgumentNullException("The game object did not contain the given component.",
			__FUNCTION__);
	}

	void GameObject::DeleteComponents()
	{
		for(list<GameComponent*>::iterator it = components.begin();
			it != components.end(); ++it)
			delete *it;

		components.clear();
	}

	GameComponent* GameObject::GetComponentByType(GameComponent::EType type)
	{
		for(auto it = components.begin(); it != components.end(); ++it)
		{
			GameComponent* curr = (*it);
			if(curr->GetComponentType() == type)
				return curr;
		}
		return nullptr;
	}

	void GameObject::SetParent(GameObject* newParent)
	{
		TreeNode::SetParent(parent);
		for(auto it = components.begin(); it != components.end(); ++it)
			(*it)->OwnerSetParent(newParent);
	}

	void GameObject::RemoveFromParent(bool updateHierarchy)
	{
		TreeNode::RemoveFromParent(updateHierarchy);
		for(auto it = components.begin(); it != components.end(); ++it)
			(*it)->OwnerRemovedFromParent(updateHierarchy);
	}

	void GameObject::AddChild(GameObject* child)
	{
		TreeNode::AddChild(child);
		for(auto it = components.begin(); it != components.end(); ++it)
			(*it)->OwnerAddedChild(child);
	}

	void GameObject::RemoveChild(GameObject* child)
	{
		TreeNode::RemoveChild(child);
		for(auto it = components.begin(); it != components.end(); ++it)
			(*it)->OwnerRemovedChild(child);
	}

	void GameObject::DeleteAllChildren()
	{
		TreeNode::DeleteAllChildren();
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

	auto_ptr<GameObject::ComponentList> GameObject::FindNearestDescendantComponents(GameComponent::EType compType,
		bool includingThisObject)
	{
		ComponentList* ret = new ComponentList();
		stack<GameObject*> s;
		
		if(includingThisObject)
		{
			s.push(this);
		}
		else
		{
			for(auto it = children.begin(); it != children.end(); ++it)
				s.push(static_cast<GameObject*>(*it));
		}

		while(!s.empty())
		{
			GameObject* curr = s.top();
			s.pop();

			GameComponent* comp = curr->GetComponentByType(compType);
			if(comp != nullptr)
			{
				ret->push_back(comp);
			}
			else
			{
				list<TreeNode*>& children = curr->GetChildren();
				for(auto it = children.begin(); it != children.end(); ++it)
					s.push(static_cast<GameObject*>(*it));
			}
		}

		return auto_ptr<ComponentList>(ret);
	}
} // end namespace Core