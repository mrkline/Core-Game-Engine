#pragma once

#include "Test.h"

namespace UnitTesting
{
	namespace ObjectHierarchy
	{
		class BasicHierarchyTest : public Test
		{
		public:
			char* GetName() { return  "Basic Game Hierarchy"; }
			void Run();
		};

		class ComponentTreeTest : public Test
		{
		public:
			char* GetName() { return "Component Tree"; }
			void Run();
		};
	} // end namespace ObjectHierarchy
} // end namespace UnitTests
