#include <CoreBase.h>
#include <ILevel.h>
#include <IGraphicsThread.h>

namespace Core
{
	using namespace Error;

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

	ECode CoreBase::InitGraphicsThread(/*need params here*/)
	{
		if(gThread != nullptr)
		{
			delete gThread;
		}
		try
		{
			//TODO: Implement platform-specific thread
			gThread = new IGraphicsThread();
		}
		//This will get thrown if creation fails
		catch(Error::Exception*)
		{
			lastError = Error::E_CEK_FAILURE;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "The new graphics thread could not be initialized.";
			return lastError;
		}
		//Notify the current level that we've changed graphics threads, if it exists
		if(currentLevel != nullptr)
		{
			currentLevel->OnGraphicsReset(gThread);
		}

		return Error::E_CEK_SUCCESS;
	}

	Error::ECode CoreBase::Run()
	{
		if(gThread == nullptr)
		{
			lastError = Error::E_CEK_INVALID_STATE;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "The graphics thread must be initialized before calling CoreBase::Run.";
			return lastError;
		}

		currentLevel = nextLevel;
		
		while(currentLevel != nullptr)
		{
			if(Succeeded(currentLevel->Load(gThread)))
			{
				currentLevel->Run();
			}
			delete currentLevel;
			currentLevel = nextLevel;
		}

		return Error::E_CEK_SUCCESS;
	}
} //end namespace Core