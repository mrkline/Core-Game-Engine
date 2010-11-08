#include <core.h>
#include <memory>
#include <irrlicht.h>

using namespace Core;
using namespace irr;

namespace Tests
{
	int OnHierarchyChangeTest()
	{
		std::auto_ptr<GameObjectManager> gom(new GameObjectManager);

		printf("Adding head...\n");
		GameObject* head = new GameObject(nullptr, gom.get(), ~0, "Head");
		//Add some children to the head
		u32 grandchildren = 0;
		for(u32 c = 0 ; c < 3; ++c)
		{
			printf((stringc("Adding child of head number ") + stringc(c) + stringc("\n")).c_str());
			GameObject* toAdd = new GameObject(head, gom.get(), c, stringc("Child of head number ") + stringc(c));
			//It's safe to drop an child in a tree since its parent will grab it
			toAdd->drop();
			//Adding some grandchildren
			for(u32 c = 0 ; c < 2; ++c)
			{
				printf((stringc("Adding grandchild of head number ") + stringc(grandchildren) + stringc("\n")).c_str());
				GameObject* grandchild = new GameObject(toAdd, gom.get(), c, stringc("Grandchild of head number ") + stringc(grandchildren));
				grandchild->drop();
				++grandchildren;
			}
		}

		

		printf("Dropping head...\n");
		head->drop();
		system("PAUSE");

		return 0;
	}

} //end namespace Tests