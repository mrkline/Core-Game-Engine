#pragma once

// Only build in for Windows
#ifdef CORE_WINDOWS_BUILD

#include <Windows.h>

#include "IGraphicsThread.h"

namespace Core
{
	//! A Windows implementation of IGraphicsThread
	class WindowsGraphicsThread : public IGraphicsThread
	{
	public:
		WindowsGraphicsThread();

		// We're trusting the thread to close itself for now, so no deconstructor
		// ~WindowsGraphicsThread();

		/*!
		\brief The actual procedure of the thread
		\param thisPtr The pointer to this class, since the procedure function must be static
		\return The exit value of the thread
		*/
		static DWORD __stdcall ThreadProc(void* thisPtr);

	private:
		//! Thread handle
		HANDLE threadHandle;
	};
} // end namespace Core

#endif
