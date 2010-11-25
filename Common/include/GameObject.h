#pragma once
#include <CoreTypes.h>
#include <GameComponent.h>
#include <RefCountedTreeNode.h>
#include <NamedClass.h>
#include <list>
#include <CoreTransform.h>

namespace Core
{
	class GameObjectManager;

	//The GameObject is the core object of the engine.  A tree of GameObjects is managed by the
	//GameObjectManager.  Each object can have a collection of GameComponenents, which do various things
	//such as manage physics, sound, scripts, logic, etc.
	class GameObject : public RefCountedTreeNode, public NamedClass
	{
	public:
		GameObject(GameObject* parent, GameObjectManager* objMan, s32 id = -1, const stringc& name = irr::core::stringc());
		virtual ~GameObject();

		GameObjectManager* getObectManager() const { return man; }

		virtual void Update();
	
		//Transform manipulation

		void SetTransform(const Transform& newTransform) { trans = newTransform; }
		Transform& GetTransform() { return trans; }
		const Transform& GetTransform() const { return trans; }
		//Only updates based on parent, does not recursively travel up the hierarchy.
		//Because of this, call on object tree from root down
		void UpdateAbsoluteTransform();
		const Transform& GetAbsoluteTransform() const { return absTrans; }

		//Each component type can only have one instance per object
		Error::ECode AddComponent(GameComponent* newComponent);
		Error::ECode RemoveComponent(GameComponent* toRemove);
		void ClearComponents();
		
		//Returns nullptr if the GameObject does not have this type
		GameComponent* GetComponentByType(GameComponent::EType type);
		
		const std::list<GameComponent*>& GetComponents() const { return components; }
		std::list<GameComponent*>& GetComponents() { return components; }

		//Functions to manipulate the object tree

		virtual Error::ECode SetParent(GameObject* newParent);
		virtual GameObject* GetParent() { return static_cast<GameObject*>(parent); }
		virtual void RemoveFromParent(bool updateHierarchy = true);

		virtual Error::ECode AddChild(GameObject* child);
		virtual Error::ECode RemoveChild(GameObject* child);
		virtual void RemoveAllChildren();

		//Notify children of changes
		//Called recursively on all children
		virtual void OnHierarchyChange(bool goingUp)
		{
			RefCountedTreeNode::OnHierarchyChange(goingUp);
			//TEST:
			/*if(!goingUp)
			{
				printf("OnHierarchyChange (going down) called for ");
				printf(name.c_str());
				printf("\n");
			}*/
		}

		//Used by components searching for a parent

		GameComponent* FindNearestAncestorComponent(GameComponent::EType compType);
		const std::list<GameComponent*>& FindNearestDescendantComponents(GameComponent::EType compType);

	protected:
		GameObjectManager* man;
		std::list<GameComponent*> components;
		std::list<GameComponent*> holder; //Used for finding descendants with certain components
		Transform trans; //Transform for the game object
		Transform absTrans; //Cumulative absolute transform for the game object

		//Recursive function used to find descendants with a given component
		static void  DescendantSearchRecursor(std::list<GameComponent*>* compList,
			GameObject* obj, GameComponent::EType compType);
	};
} //end namespace Core