#pragma comment(lib, "Core.lib")

#include <list>

#include "ObjectHierarchyUnit.h"

using namespace UnitTesting;

std::list<TestUnit*> testUnits;

int main()
{
	testUnits.push_back(new ObjectHierarchyUnit());
	//! \todo Load units into testUnits here

	for(auto iter = testUnits.begin(); iter != testUnits.end(); ++iter)
	{
		TestUnit* curr = *iter;
		try
		{
			printf("Beginning test unit %s...\n\n", curr->GetUnitName());
			curr->RunUnit();
		}
		catch(...)
		{
			printf("The test unit %s throw an exception. Bad test unit. Bad.\n");
		}
		delete *iter;
	}
	printf("\n");
	system("pause");
	return 0;
}