#pragma once

#include <list>
#include <string>

#include "GameComponent.h"
#include "TreeNode.h"
#include "NamedClass.h"
#include "CoreTransform.h"

namespace Core
{
	class GameObjectManager;

	/*! 
	\brief Represents an ntity in the game world.

	The GameObject is the core object of the engine.  A tree of GameObjects is managed by the
	scene.  Each object can have a collection of GameComponenents, which do various things
	such as manage physics, sound, scripts, logic, etc.
	*/
	class GameObject : public TreeNode, public NamedClass
	{
	public:
		typedef std::list<GameComponent*> ComponentList;

		/*!
		\brief Constructor.
		\param parent The parent of the object, or null if it has none
		*/
		GameObject(GameObject* parent, GameObjectManager* objMan,
			int id = -1, const std::string& name = std::string());
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
		void DeleteComponents();
		
		//Returns nullptr if the GameObject does not have this type
		GameComponent* GetComponentByType(GameComponent::EType type);
		
		const ComponentList& GetComponents() const { return components; }
		ComponentList& GetComponents() { return components; }

		//Functions to manipulate the object tree

		virtual Error::ECode SetParent(GameObject* newParent);
		virtual GameObject* GetParent() { return static_cast<GameObject*>(parent); }
		virtual void RemoveFromParent(bool updateHierarchy = true);

		virtual Error::ECode AddChild(GameObject* child);
		virtual Error::ECode RemoveChild(GameObject* child);
		virtual void DeleteAllChildren();

		//Notify children of changes
		//Called recursively on all children
		virtual void OnHierarchyChange(bool goingUp)
		{
			TreeNode::OnHierarchyChange(goingUp);
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
		ComponentList FindNearestDescendantComponents(GameComponent::EType compType, bool includingThisObject = false);

	protected:
		GameObjectManager* man;
		std::list<GameComponent*> components;
		Transform trans; //Transform for the game object
		Transform absTrans; //Cumulative absolute transform for the game object

		//Recursive function used to find descendants with a given component
		static void  DescendantSearchRecursor(std::list<GameComponent*>* compList,
			GameObject* obj, GameComponent::EType compType);
	};
} //end namespace Core