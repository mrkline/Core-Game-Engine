#pragma once

#include <cstdio>
#include <list>

#include "Test.h"

namespace UnitTesting
{
	/*!
	\brief Provides a base abstract class for test units.

	A test unit is a group of related tests related to a given component.
	*/
	class TestUnit
	{
	public:
		//! Deletes all tests
		virtual ~TestUnit()
		{
			for(auto iter = succeededTests.begin(); iter != succeededTests.end(); ++iter)
			{
				delete *iter;
			}
			for(auto iter = failedTests.begin(); iter != failedTests.end(); ++iter)
			{
				delete *iter;
			}
			for(auto iter = bombedTests.begin(); iter != bombedTests.end(); ++iter)
			{
				delete *iter;
			}
		}

		//! Returns this unit's name
		virtual char* GetUnitName() = 0;

		//! Executes all Tests and prints a summary of the unit
		void RunUnit()
		{
			char* name = GetUnitName();

			LoadTests();

			for(auto iter = unrunTests.begin(); iter != unrunTests.end(); ++iter)
			{
				Test* curr = *iter;
				char* currName = curr->GetName();
				try
				{
					curr->Run();
					printf("SUCCESS: %s test succeeded\n", currName);
					succeededTests.push_back(curr);
				}
				catch(const TestFailedException& ex)
				{
					printf("FAILURE: %s test failed with the message:\n\t%s", currName, ex.GetMessage());
					failedTests.push_back(curr);
				}
				catch(...)
				{
					printf("FAILURE: %s test failed and thew an exception besides TestFailedException\n", currName);
					bombedTests.push_back(curr);
				}
			}

			printf("\nResults for %s\n", name);

			if(!succeededTests.empty())
			{
				printf("\tSucceeded Tests:\n");
				for(auto iter = succeededTests.begin(); iter != succeededTests.end(); ++iter)
				{
					printf("\t\t%s\n", (*iter)->GetName());
				}
			}

			if(!failedTests.empty())
			{
				printf("\tFailed Tests:\n");
				for(auto iter = failedTests.begin(); iter != failedTests.end(); ++iter)
				{
					printf("\t\t%s\n", (*iter)->GetName());
				}
			}


			if(!bombedTests.empty())
			{
				printf("\tEpically failed tests (threw an exception besides TestFailure):\n");
				for(auto iter = bombedTests.begin(); iter != bombedTests.end(); ++iter)
				{
					printf("\t\t%s\n", (*iter)->GetName());
				}
				printf("\n\n");
			}
		}

	protected:
		//! Loads tests into unrunTests. Tests will automatically be deleted on this unit's destruction
		virtual void LoadTests() = 0;
		
		std::list<Test*> unrunTests;

	private:
		std::list<Test*> succeededTests;
		std::list<Test*> failedTests;
		//! Tests which throw an exception besides TestFailedException
		std::list<Test*> bombedTests;
	};
} // end namespace UnitTesting
