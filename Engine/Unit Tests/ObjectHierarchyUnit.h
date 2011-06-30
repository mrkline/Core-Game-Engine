#pragma once

#include "TestUnit.h"
#include "ObjectHierarchyTests.h"

namespace UnitTesting
{
	class ObjectHierarchyUnit : public TestUnit
	{
		char* GetUnitName() { return "GameObject Hierarchy"; }

		void LoadTests()
		{
			unrunTests.push_back(new ObjectHierarchy::BasicHierarchyTest());
			unrunTests.push_back(new ObjectHierarchy::ComponentTreeTest());
		}
	};
} // end namespace UnitTesting