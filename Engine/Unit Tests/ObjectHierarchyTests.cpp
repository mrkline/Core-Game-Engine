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
	}
} // end namespace UnitTesting