#include "WindowsGraphicsThread.h"

#ifdef CORE_WINDOWS_BUILD

namespace Core
{
	WindowsGraphicsThread::WindowsGraphicsThread()
	{
		threadHandle = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
	}

	DWORD WindowsGraphicsThread::ThreadProc(void* thisPtr)
	{
		return 0;
	}
} // end namespace Core
#endif
