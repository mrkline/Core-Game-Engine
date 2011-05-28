#include "CoreBase.h"

#include "ErrorHandling.h"
#include "IGraphicsThread.h"
#include "ILevel.h"

namespace Core
{
	CoreBase::CoreBase()
		: currentLevel(nullptr), nextLevel(nullptr), gThread(nullptr)
	{
	}

	CoreBase::~CoreBase()
	{
		if(currentLevel != nullptr)
		{
			delete currentLevel;
		}
		if(nextLevel != nullptr)
		{
			delete nextLevel;
		}
		if(gThread != nullptr)
		{
			delete gThread;
		}
	}

	void CoreBase::SetNextLevel(ILevel* nLevel)
	{
		if(nextLevel != nullptr)
		{
			delete nextLevel;
		}
		nextLevel = nLevel;
	}

	void CoreBase::InitGraphicsThread(/*need params here*/)
	{
		if(gThread != nullptr)
		{
			delete gThread;
		}

		//! \todo Implement platform-specific thread
		gThread = new IGraphicsThread();

		// Notify the current level that we've changed graphics threads, if it exists
		if(currentLevel != nullptr)
		{
			currentLevel->OnGraphicsReset(gThread);
		}
	}

	void CoreBase::Run()
	{
		if(gThread == nullptr)
		{
			throw new InvalidOperationException("The graphics thread must be initialized before calling CoreBase::Run.",
				__FUNCTION__);
		}

		currentLevel = nextLevel;
		
		while(currentLevel != nullptr)
		{
			currentLevel->Run();
			delete currentLevel;
			currentLevel = nextLevel;
		}
	}
} // end namespace Core