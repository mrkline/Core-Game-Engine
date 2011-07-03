#include "GameObject.h"

#include <stack>

#include "ErrorHandling.h"
#include "Scene.h"

using namespace std;

namespace Core
{
	GameObject::GameObject(GameObject* parent, const Transform& startingTransform,
		int id, const std::string& name)
		: NamedClass(id, name), trans(startingTransform), parent(parent)
	{
		// We can't call AddChild here because it then calls RemoveFromParent, which doesn't work well at this stage
		// (since we're trying to add it to the parent)
		if(parent != nullptr)
		{
			parent->children.push_back(this);

			ComponentList& pComps = parent->components;

			for(auto it = pComps.begin(); it != pComps.end(); ++it)
				(*it)->OwnerAddedChild(this);
		}
	}

	GameObject::~GameObject()
	{
		// Kill the components
		for(auto it = components.begin(); it != components.end(); ++it)
			delete *it;

		// Kill all children.
		DeleteAllChildren();
	}

	void GameObject::Update()
	{
		UpdateAbsoluteTransform();
		for(auto it = children.begin(); it != children.end(); ++it)
			(*it)->Update();
	}

	void GameObject::UpdateAbsoluteTransform()
	{
		if(parent == nullptr)
			absTrans = trans;
		else
			absTrans = trans * parent->absTrans;
	}

	bool GameObject::HasChild(GameObject* child)
	{
		for(auto it = children.begin(); it != children.end(); ++it)
		{
			if(*it == child)
				return true;
		}
		return false;
	}

	bool GameObject::HasChild(const std::string& childName)
	{
		for(auto it = children.begin(); it != children.end(); ++it)
		{
			if((*it)->name == childName)
				return true;
		}
		return false;
	}

	bool GameObject::HasChild(int childId)
	{
		for(auto it = children.begin(); it != children.end(); ++it)
		{
			if((*it)->id == childId)
				return true;
		}
		return false;
	}

	bool GameObject::HasAncestor(GameObject* ancestor)
	{
		for(GameObject* curr = parent; curr != nullptr; curr = curr->parent)
		{
			if(curr == ancestor)
				return true;
		}
		return false;
	}

	bool GameObject::HasAncestor(const std::string& ancestorName)
	{
		for(GameObject* curr = parent; curr != nullptr; curr = curr->parent)
		{
			if(curr->name == ancestorName)
				return true;
		}
		return false;
	}
	
	bool GameObject::HasAncestor(int ancestorId)
	{
		for(GameObject* curr = parent; curr != nullptr; curr = curr->parent)
		{
			if(curr->id == ancestorId)
				return true;
		}
		return false;
	}

	bool GameObject::HasDescendant(GameObject* descendant)
	{
		stack<GameObject*> s;
		
		for(auto it = children.begin(); it != children.end(); ++it)
			s.push(*it);

		while(!s.empty())
		{
			GameObject* curr = s.top();
			s.pop();

			if(curr == descendant)
			{
				return true;
			}
			else
			{
				const list<GameObject*>& children = curr->children;
				for(auto it = children.begin(); it != children.end(); ++it)
					s.push(*it);
			}
		}

		return false;
	}
	
	bool GameObject::HasDescendant(const std::string& descendantName)
	{
		stack<GameObject*> s;
		
		for(auto it = children.begin(); it != children.end(); ++it)
			s.push(*it);

		while(!s.empty())
		{
			GameObject* curr = s.top();
			s.pop();

			if(curr->GetName() == descendantName)
			{
				return true;
			}
			else
			{
				const list<GameObject*>& children = curr->children;
				for(auto it = children.begin(); it != children.end(); ++it)
					s.push(*it);
			}
		}

		return false;
	}
	
	bool GameObject::HasDescendant(int descendantId)
	{
		stack<GameObject*> s;
		
		for(auto it = children.begin(); it != children.end(); ++it)
			s.push(*it);

		while(!s.empty())
		{
			GameObject* curr = s.top();
			s.pop();

			if(curr->GetID() == descendantId)
			{
				return true;
			}
			else
			{
				const list<GameObject*>& children = curr->children;
				for(auto it = children.begin(); it != children.end(); ++it)
					s.push(*it);
			}
		}

		return false;
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
				throw ArgumentException("A game object can only have one of each type of component.",
					__FUNCTION__);
		}

		components.push_back(newComponent);
		newComponent->BindToOwner(this);
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
		if(newParent == this)
			throw ArgumentException("A game object cannot set itself as its parent.",
				__FUNCTION__);

		RemoveFromParent();
		if(newParent != nullptr)
			newParent->AddChild(this);

		for(auto it = components.begin(); it != components.end(); ++it)
			(*it)->OwnerSetParent(newParent);
	}

	void GameObject::RemoveFromParent()
	{
		if (parent != nullptr)
				parent->RemoveChild(this);

		for(auto it = components.begin(); it != components.end(); ++it)
			(*it)->OwnerRemovedFromParent();
	}

	void GameObject::AddChild(GameObject* child)
	{
		if(child == nullptr)
			throw ArgumentNullException("A game object cannot add a null child.",
				__FUNCTION__);

		if(child == this)
			throw ArgumentException("A game object cannot add itself as a child.",
				__FUNCTION__);

		for(auto it = children.begin(); it != children.end(); ++it)
		{
			// We're trying to add a duplicate child
			if(*it == child)
				throw ArgumentException("A game object cannot have duplicate children.",
					__FUNCTION__);
		}

		child->RemoveFromParent();
		children.push_back(child);
		child->parent = this;

		for(auto it = components.begin(); it != components.end(); ++it)
			(*it)->OwnerAddedChild(child);
	}

	void GameObject::RemoveChild(GameObject* child)
	{
		if(child == nullptr)
			throw ArgumentNullException("A tree node cannot remove a null child.",
				__FUNCTION__);

		for(auto it = children.begin(); it != children.end(); ++it)
		{
			if(*it == child)
			{
				child->parent = nullptr;
				children.erase(it);
				return;
			}
		}
		throw ArgumentException("A tree node could not find the child that was to be removed.",
			__FUNCTION__);

		for(auto it = components.begin(); it != components.end(); ++it)
			(*it)->OwnerRemovedChild(child);
	}

	void GameObject::DeleteAllChildren()
	{
		for(list<GameObject*>::iterator it = children.begin();
			it != children.end(); ++it)
			delete *it;

		children.clear();
	}

	GameComponent* GameObject::FindNearestAncestorComponent(GameComponent::EType compType)
	{
		GameComponent* ret;
		for(GameObject* curr = parent; curr != nullptr; curr = curr->parent)
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
				s.push(*it);
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
				list<GameObject*>& children = curr->children;
				for(auto it = children.begin(); it != children.end(); ++it)
					s.push(*it);
			}
		}

		return auto_ptr<ComponentList>(ret);
	}
} // end namespace Core