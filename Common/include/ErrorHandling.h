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
			static void ShowErrorDialog(const char* const message, const char* const title);
		};

		//! Error codes that could be returned by functions if they fail
		enum ECode
		{
			E_CEK_SUCCESS, //!< No errors occurred
			E_CEK_FAILURE, //!< General failure.  Try to avoid using this (be more specific).
			E_CEK_BAD_ARG, //!< Bad argument
			E_CEK_NULL_ARG, //null argument
			E_CEK_ARG_OUT_OF_RANGE, //!< argument is out of range
			E_CEK_INDEX_OUT_OF_RANGE, //!< index is out of range
			E_CEK_INVALID_STATE, //!< Object is not in the correct state to perform this operation
			E_CEK_COUNT //!< Count of error codes, not an actual error code (do not use)
		};

		//! Returns true if the error code indicates failure.
		static inline bool Failed(ECode code) { return code != E_CEK_SUCCESS; }
		//! Returns true if the error code indicates success.
		static inline bool Succeeded(ECode code) { return code == E_CEK_SUCCESS; }

		/*!
		\brief Class which can set and return error codes and messages for itself

		Instead of having a global GetLastError function, this class allows error codes
		and messages to be done on a per-instance level so that different threads
		don't have to wait on a global GetLastError call.
		*/
		class CanErr
		{
		public:
			//! The constructor initializes the last error to E_CEK_SUCCESS (no error)
			CanErr() : lastError(E_CEK_SUCCESS), customLastErrorMessage(nullptr),
							lastErrorFunction(nullptr) {}

			virtual ~CanErr() {}

			//! Get the error code from the last error that occurred in this object
			ECode GetLastError() const { return lastError; }
			//! Get the nme of the function that last errored out in this object
			char* GetLastErrorFunction() const { return lastErrorFunction; }
			//! Get the error message pertaining to the last error code
			char* GetLastErrorMessage() const
			{
				switch(lastError)
				{
				case E_CEK_SUCCESS:
					return "Operation succeeded.";
				case E_CEK_FAILURE:
					return "An unspecified problem occurred.";
				case E_CEK_BAD_ARG:
					return "A bad argument was passed to the function.";
				case E_CEK_NULL_ARG:
					return "An invalid null argument was passed to the function";
				case E_CEK_INVALID_STATE:
					return "Object is not in the correct state to perform the function";
				case E_CEK_ARG_OUT_OF_RANGE:
					return "Argument is out of range";
				case E_CEK_INDEX_OUT_OF_RANGE:
					return "An index is out of range";
				default:
					return "Last error code is not recognized";
				};
			}

			/*!
			\brief Returns a custom error message with more specific information, if it exists.
			Otherwise returns the same as GetLastErrorMessage.
			*/
			char* GetCustomErrorMessage() const
			{
				return customLastErrorMessage == nullptr ?
					GetLastErrorMessage() : customLastErrorMessage;
			}

		protected:
			mutable ECode lastError; //!< Functions should set this when they error out
			mutable char* customLastErrorMessage; //!< User can set a custom last error message.
			mutable char* lastErrorFunction; //!< The function that errored out should set this to itself
		};
	} //end namespace Error
} //end namespace Core