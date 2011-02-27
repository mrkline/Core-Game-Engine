#include <GameObject.h>
#include <ErrorHandling.h>

using namespace std;

namespace Core
{
	using namespace Error;

	GameObject::GameObject(GameObject* parent, GameObjectManager* objMan, int id, const std::string& name)
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
		//Kill the components
		for(list<GameComponent*>::iterator it = components.begin();
			it != components.end(); ++it)
		{
			delete *it;
		}
	}

	void GameObject::Update()
	{
		UpdateAbsoluteTransform();
		for(list<TreeNode*>::iterator it = children.begin();
			it != children.end(); ++it)
		{
			static_cast<GameObject*>(*it)->Update();
		}
	}

	void GameObject::UpdateAbsoluteTransform()
	{
		if(parent == nullptr)
		{
			absTrans = trans;
		}
		else
		{
			absTrans = trans * static_cast<GameObject*>(parent)->absTrans;
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
		for(list<GameComponent*>::iterator it = components.begin();
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
		for(list<GameComponent*>::iterator it = components.begin();
			it != components.end(); ++it)
		{
			if((*it) == toRemove)
			{
				components.erase(it);
				return Error::E_CEK_SUCCESS;
			}
		}
		lastError = Error::E_CEK_BAD_ARG;
		lastErrorFunction = __FUNCTION__;
		customLastErrorMessage = "The game object did not contain the given component.";
		return lastError;
	}

	void GameObject::DeleteComponents()
	{
		for(list<GameComponent*>::iterator it = components.begin();
			it != components.end(); ++it)
		{
			delete *it;
		}
		components.clear();
	}

	GameComponent* GameObject::GetComponentByType(GameComponent::EType type)
	{
		for(list<GameComponent*>::iterator it = components.begin();
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
		ECode ret = TreeNode::SetParent(parent);
		if(Succeeded(ret))
		{
			for(list<GameComponent*>::iterator it = components.begin();
				it != components.end(); ++it)
			{
				(*it)->OwnerSetParent(newParent);
			}
		}
		return ret;
	}

	void GameObject::RemoveFromParent(bool updateHierarchy)
	{
		TreeNode::RemoveFromParent(updateHierarchy);
		for(list<GameComponent*>::iterator it = components.begin();
			it != components.end(); ++it)
		{
			(*it)->OwnerRemovedFromParent(updateHierarchy);
		}
	}

	ECode GameObject::AddChild(GameObject* child)
	{
		ECode ret =	TreeNode::AddChild(child);
		if(Succeeded(ret))
		{
			for(list<GameComponent*>::iterator it = components.begin();
				it != components.end(); ++it)
			{
				(*it)->OwnerAddedChild(child);
			}
		}
		return ret;
	}

	ECode GameObject::RemoveChild(GameObject* child)
	{
		ECode ret = TreeNode::RemoveChild(child);
		if(Succeeded(ret))
		{
			for(list<GameComponent*>::iterator it = components.begin();
				it != components.end(); ++it)
			{
				(*it)->OwnerRemovedChild(child);
			}
		}
		return ret;
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

	GameObject::ComponentList GameObject::FindNearestDescendantComponents(GameComponent::EType compType,
		bool includingThisObject)
	{
		ComponentList ret;
		if(includingThisObject)
		{
			DescendantSearchRecursor(&ret, this, compType);
		}
		else
		{
			for(list<TreeNode*>::iterator it = children.begin();
				it != children.end(); ++it)
			{
				DescendantSearchRecursor(&ret, static_cast<GameObject*>(*it), compType);
			}
		}
		return ret;
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
			list<TreeNode*>& children = obj->GetChildren();
			for(list<TreeNode*>::iterator it = children.begin();
				it != children.end(); ++it)
			{
				DescendantSearchRecursor(compList, static_cast<GameObject*>(*it), compType);
			}
		}
	}
} //end namespace Core