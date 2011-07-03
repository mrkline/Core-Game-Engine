#include "ObjectHierarchyTests.h"

#include <Core.h>

using namespace Core;

namespace UnitTesting
{
	namespace ObjectHierarchy
	{
		void BasicHierarchyTest::Run()
		{
			// Save some typing
			typedef Core::GameObject GO;
			
			// Needed for the creation of GameObjects
			Scene scene(nullptr);
			GO* root = scene.GetRootObject();
			
			GO* c1_1 = new GO(root);

			if(c1_1->GetParent() != root || !root->HasChild(c1_1))
				throw TestFailedException("GameObject::AddChild (called from contructor) failed.");

			root->RemoveChild(c1_1);

			if(root->HasChildren() || c1_1->GetParent() != nullptr)
				throw TestFailedException("GameObject::RemoveChild failed.");

			c1_1->SetParent(root);

			if(c1_1->GetParent() != root || !root->HasChild(c1_1))
				throw TestFailedException("GameObject::SetParent failed.");

			GO* c2_1 = new GO(c1_1, Transform(), 1337, "desc");

			if(!c2_1->HasAncestor(0) || !c2_1->HasAncestor("root") || !c2_1->HasAncestor(root))
				throw TestFailedException("GameObject::HasAncestor failed.");

			if(!root->HasDescendant(c2_1) || !root->HasDescendant(1337) || !root->HasDescendant("desc"))
				throw TestFailedException("GameObject::HasDescendant failed.");

			// Clean up here
			root->DeleteAllChildren();
		}

		void ComponentTreeTest::Run()
		{
			typedef class TestComponent : public GameComponent
			{
			public:
				bool hChange;

				TestComponent() : hChange(false) { }

				bool HasChild(TestComponent* child)
				{
					for(auto it = children.begin(); it != children.end(); ++it)
					{
						if(*it == child)
							return true;
					}

					return false;
				}

				EType GetComponentType() const { return (EType)0x1337; }

				static bool CheckComponentLink(GameObject* parentObj, GameObject* childObj)
				{
					TestComponent* parent = static_cast<TestComponent*>(parentObj->GetComponentByType((EType)0x1337));
					TestComponent* child = static_cast<TestComponent*>(childObj->GetComponentByType((EType)0x1337));

					return child->GetParent() == parent && parent->HasChild(child);
				}

			private:
				void OnHierarchyChange() { hChange = true; }
			} TC;

			// Save some typing
			typedef Core::GameObject GO;

			std::list<TC*> testComps;
			TC* temp;
			
			// Needed for the creation of GameObjects
			Scene scene(nullptr);
			GO* root = scene.GetRootObject();

			GO* o0_1 = new GO(root);
			temp = new TC();
			o0_1->AddComponent(temp);
			testComps.push_back(temp);

			// We'll add the component on to this one later
			GO* o1_1 = new GO(o0_1);

			// We're testing to see if the search for descendants seraches past this object which has no component
			GO* o1_2 = new GO(o0_1);

			GO* o2_1 = new GO(o1_1);
			temp = new TC(); 
			o2_1->AddComponent(temp);
			testComps.push_back(temp);

			GO* o2_2 = new GO(o1_2);
			temp = new TC();
			o2_2->AddComponent(temp);
			testComps.push_back(temp);

			GO* o3_1 = new GO(o2_1);
			temp = new TC();
			o3_1->AddComponent(temp);
			testComps.push_back(temp);

			// We're going to reset all the hierarchy change flags and add the last node,
			// and they should all be set again

			for(auto it = testComps.begin(); it != testComps.end(); ++it)
				(*it)->hChange = false;

			GO* o3_2 = new GO(o2_1);
			temp = new TC();
			o3_2->AddComponent(temp);
			testComps.push_back(temp);

			for(auto it = testComps.begin(); it != testComps.end(); ++it)
			{
				TC* curr = *it;

				if(curr->hChange == false)
					throw new TestFailedException("All components were not notified of a hierarchy update");

				curr->hChange = false;
			}

			temp = new TC();
			o1_1->AddComponent(temp);
			testComps.push_back(temp);

			for(auto it = testComps.begin(); it != testComps.end(); ++it)
			{
				TC* curr = *it;

				if(curr->hChange == false)
					throw new TestFailedException("All components were not notified of a hierarchy update on component insertion");

				curr->hChange = false;
			}

			// Test our links
			if(!TestComponent::CheckComponentLink(o0_1, o1_1)
				|| !TestComponent::CheckComponentLink(o0_1, o2_2)
				|| !TestComponent::CheckComponentLink(o1_1, o2_1)
				|| !TestComponent::CheckComponentLink(o2_1, o3_1)
				|| !TestComponent::CheckComponentLink(o2_1, o3_2))
			{
				throw new TestFailedException("Component tree not properly linked on on component insertion");
			}
		}
	} // end namespace ObjectHierarchy
} // end namespace UnitTesting