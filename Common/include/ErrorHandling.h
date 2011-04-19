#pragma once

namespace Core
{
	//! Provides functionality for error handling and reporting
	namespace Error
	{
		//! Static class with error handling utilities
		class EHandler
		{
		public:
			/*!
			\brief Shows an error dialog.
			\param message The message to display in the dialog
			\param title The title of the dialog to display
			\todo Currently displays a Windows message box.  We probably want to change this behavior.
			*/
			static void ShowErrorDialog(const char* message, const char* title);
		};

		/*!
		\brief A base Exception class

		An exception should be deleted when it has been handled.
		Most exception types are lovingly borrowed from the .NET framework.
		Exceptions will only be used in constructors, since they will be handled by factory functions.
		*/
		class Exception
		{
		public:
			/*!
			\brief Initializes an exception with a message and other optional data
			\param exceptionMessage The message to contain within the exception.
			\param callingFunctionName The name of the exception the function comes from.
			\param internalException An internal exception, if one exists

			All strings must not point to dynamic memory, as no free or delete is called on them.
			It is assumed that they will point to static strings that were created at compile time.
			*/
			Exception(const char* exceptionMessage,	const char* callingFunctionName = nullptr,
				const Exception* internalException = nullptr)
				: message(exceptionMessage), cf(callingFunctionName),
				ie(internalException) { }

			//! Deconstructor. Deletes the internal exception, if necessary
			virtual ~Exception()
			{
				if(ie != nullptr)
					delete ie;
			}

			//! Returns the exception message
			const char* GetExceptionMessage() const { return message; }

			//! Returns the name of the calling function, or null if it is not set
			const char* GetCallingFunction() const { return cf;	}

			//! Returns the inner exception, or null if there isn't one
			const Exception* GetInnerException() const { return ie;	}

		private:
			const char* message; //!< Exception message
			const char* cf; //!< Name of calling function
			const Exception* ie; //!< Internal exception
		};

		//! Thrown if an argument to a method was invalid
		class ArgumentException : public Exception
		{
		public:
			ArgumentException(const char* exceptionMessage,
				const char* callingFunctionName = nullptr,
				const Exception* internalException = nullptr)
				: Exception(exceptionMessage, callingFunctionName, internalException)
			{ }
		};

		//! Thrown if a null argument was passed to a method that doesn't accept it.
		class ArgumentNullException : public ArgumentException
		{
		public:
			ArgumentNullException(const char* exceptionMessage,
				const char* callingFunctionName = nullptr,
				const Exception* internalException = nullptr)
				: ArgumentException(exceptionMessage, callingFunctionName, internalException)
			{ }
		};

		//! Thrown if an argument value is out of range
		class ArgumentOutOfRangeException : public ArgumentException
		{
		public:
				ArgumentOutOfRangeException(const char* exceptionMessage,
				const char* callingFunctionName = nullptr,
				const Exception* internalException = nullptr)
				: ArgumentException(exceptionMessage, callingFunctionName, internalException)
			{ }
		};

		//! Thrown if an array index is out of bounds
		class IndexOutOfRangeException : public Exception
		{
		public:
			IndexOutOfRangeException(const char* exceptionMessage,
				const char* callingFunctionName = nullptr,
				const Exception* internalException = nullptr)
				: Exception(exceptionMessage, callingFunctionName, internalException)
			{ }
		};

		//! Thrown if a method was called at an improper time
		class InvalidOperationException : public Exception
		{
		public:
			InvalidOperationException(const char* exceptionMessage,
				const char* callingFunctionName = nullptr,
				const Exception* internalException = nullptr)
				: Exception(exceptionMessage, callingFunctionName, internalException)
			{ }
		};
	} // end namespace Error
} // end namespace Core