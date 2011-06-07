#pragma once

#include "Test.h"

namespace UnitTesting
{
	namespace ObjectHierarchy
	{
		class BasicHierarchyTest : public Test
		{
			char* GetName() { return  "Basic Game Hierarchy"; }
			
			void Run();
		};
	} // end namespace ObjectHierarchy
} // end namespace UnitTests
