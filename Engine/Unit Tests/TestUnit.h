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

			printf("Loading Tests for the %s unit\n", name);
			LoadTests();

			printf("Running Tests for the %s unit\n", name);
			for(auto iter = unrunTests.begin(); iter != unrunTests.end(); ++iter)
			{
				Test* curr = *iter;
				char* currName = curr->GetName();

				printf("Running %s test\n");
				try
				{
					curr->Run();
					printf("%s test succeeded\n");
					succeededTests.push_back(curr);
				}
				catch(TestFailedException&)
				{
					printf("%s test failed\n");
					failedTests.push_back(curr);
				}
				catch(...)
				{
					printf("%s test failed and thew an exception besides TestFailedException\n");
					bombedTests.push_back(curr);
				}
			}

			printf("Results for %s\n", name);
			printf("\tSucceeded Tests:\n");
			for(auto iter = succeededTests.begin(); iter != succeededTests.end(); ++iter)
			{
				printf("\t\t%s\n", (*iter)->GetName());
			}

			printf("\tFailed Tests:\n");
			for(auto iter = failedTests.begin(); iter != failedTests.end(); ++iter)
			{
				printf("\t\t%s\n", (*iter)->GetName());
			}

			printf("\tEpically failed tests (threw an exception besides TestFailure):\n");
			for(auto iter = bombedTests.begin(); iter != bombedTests.end(); ++iter)
			{
				printf("\t\t%s\n", (*iter)->GetName());
			}
			printf("\n\n");
		}

	protected:
		//! Loads tests into unrunTests. Tests will automatically be deleted on this unit's destruction
		virtual void LoadTests() = 0;

	private:
		std::list<Test*> unrunTests;
		std::list<Test*> succeededTests;
		std::list<Test*> failedTests;
		//! Tests which throw an exception besides TestFailedException
		std::list<Test*> bombedTests;
	};
} // end namespace UnitTesting
