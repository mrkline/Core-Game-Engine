#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;

namespace Error
{
	class EHandler
	{
	public:
		//TODO: Currently displays a Windows message box.  We probably want to change this behavior.
		static void ShowErrorDialog(const char* const message, const char* const title);
	};

	//Drop the exception when it has been handled
	//Most exception types are lovingly borrowed from the .NET framework
	class Exception : public IReferenceCounted
	{
	public:
		Exception(const char* const exceptionMessage,
			const char* const callingFunctionName = nullptr,
			const Exception* const internalException = nullptr)
			: message(exceptionMessage),
			cf(callingFunctionName),
			ie(internalException)
		{
		}

		virtual ~Exception()
		{
			if(ie != nullptr)
				ie->drop();
		}

		const char* const GetExceptionMessage() const
		{
			return message;
		}

		const char* const GetCallingFunction() const
		{
			return cf;
		}

		const Exception* const GetInnerException() const
		{
			return ie;
		}

	protected:
		//Exception message
		const char* const message;
		//name of calling function
		const char* const cf;
		//Internal exception
		const Exception* const ie;
	};

	//Thrown if an argument to a method was invalid
	class ArgumentException : public Exception
	{
	public:
		ArgumentException(const char* const exceptionMessage,
			const char* const callingFunctionName = nullptr,
			const Exception* const internalException = nullptr)
			: Exception(exceptionMessage, callingFunctionName, internalException)
		{
		}
	};

	//Thrown if a null argument was passed to a method that doesn't accept it.
	class ArgumentNullException : public ArgumentException
	{
	public:
		ArgumentNullException(const char* const exceptionMessage,
			const char* const callingFunctionName = nullptr,
			const Exception* const internalException = nullptr)
			: ArgumentException(exceptionMessage, callingFunctionName, internalException)
		{
		}
	};

	//Argument value is out of range
	class ArgumentOutOfRangeException : public ArgumentException
	{
	public:
			ArgumentOutOfRangeException(const char* const exceptionMessage,
			const char* const callingFunctionName = nullptr,
			const Exception* const internalException = nullptr)
			: ArgumentException(exceptionMessage, callingFunctionName, internalException)
		{
		}
	};

	//array index is out of bounds
	class IndexOutOfRangeException : public Exception
	{
	public:
		IndexOutOfRangeException(const char* const exceptionMessage,
			const char* const callingFunctionName = nullptr,
			const Exception* const internalException = nullptr)
			: Exception(exceptionMessage, callingFunctionName, internalException)
		{
		}
	};

	//method was called at an invalid time
	class InvalidOperationException : public Exception
	{
	public:
		InvalidOperationException(const char* const exceptionMessage,
			const char* const callingFunctionName = nullptr,
			const Exception* const internalException = nullptr)
			: Exception(exceptionMessage, callingFunctionName, internalException)
		{
		}
	};
} //end namespace Error
