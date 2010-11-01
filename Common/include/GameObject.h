#pragma once
#include <irrlicht.h>
#include "GameComponent.h"
#include "RefCountedTreeNode.h"
#include "NamedClass.h"

namespace GameCore
{
	class GameObjectManager;

	class GameObject : public RefCountedTreeNode, public NamedClass
	{
	public:
		GameObject(GameObject* parent, GameObjectManager* objMan, irr::s32 id = -1, const irr::core::stringc& name = irr::core::stringc());
		virtual ~GameObject();

		GameObjectManager* getObectManager() const { return man; }

		//Each component type can only have on instance per object
		void AddComponent(GameComponent* newComponent);
		void RemoveComponent(GameComponent* toRemove);
		void ClearComponents();
		
		//Returns nullptr if the GameObject does not have this type
		GameComponent* GetComponentByType(GameComponent::EType type);

		const irr::core::list<GameComponent*>& GetComponents() const { return components; }
		irr::core::list<GameComponent*>& GetComponents() { return components; }

		virtual void SetParent(GameObject* newParent);
		virtual GameObject* GetParent() { return static_cast<GameObject*>(parent); }
		virtual void RemoveFromParent(bool updateHierarchy = true);

		virtual void AddChild(GameObject* child);
		virtual void RemoveChild(GameObject* child);
		virtual void RemoveAllChildren();

		//Notify children of changes
		//Called recursively on all children
		virtual void OnHierarchyChange()
			{ return RefCountedTreeNode::OnHierarchyChange(); }

		//Used by components searching for a parent
		GameComponent* FindNearestAncestorComponent(GameComponent::EType compType);
		const irr::core::list<GameComponent*>& FindNearestDescendantComponents(GameComponent::EType compType);

	protected:
		GameObjectManager* man;
		irr::core::list<GameComponent*> components;
		irr::core::list<GameComponent*> holder; //Used for finding descendants with certain components

		static void  DescendantSearchRecursor(irr::core::list<GameComponent*>* compList,
			GameObject* obj, GameComponent::EType compType);
	};
} //end namespace GameCore