#pragma once

namespace UnitTesting
{
	//! Thrown out of Test::Run() if a test fails in an anticipated manner
	class TestFailedException
	{
	public:
		TestFailedException(char* failureMessage) : message(failureMessage) {}

		char* GetMessage() const { return message; }

	private:
		char* message;
	};

	//! Represents a single test, a series of which is run by each TestUnit
	class Test
	{
	public:
		//! Returns the name of the test
		virtual char* GetName() = 0;

		/*!
		\brief Runs the test. Error reporting is done via exceptions
		\throws TestFailedException if the test fails in an anticipated manner.
		*/
		virtual void Run() = 0;
	};
} // end namespace UnitTesting