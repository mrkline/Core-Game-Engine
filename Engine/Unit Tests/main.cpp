#pragma comment(lib, "Core.lib")

#include <list>

#include <Core.h>

#include "TestUnit.h"

using namespace UnitTesting;

std::list<TestUnit*> testUnits;

int main()
{
	//! \todo Load units into testUnits here

	for(auto iter = testUnits.begin(); iter != testUnits.end(); ++iter)
	{
		TestUnit* curr = *iter;
		try
		{
			printf("Starting new test unit: %s\n", curr->GetUnitName());

		}
		catch(...)
		{
			printf("The test unit %s throw an exception. Bad test unit. Bad.\n");
		}
		delete *iter;
	}
	return 0;
}