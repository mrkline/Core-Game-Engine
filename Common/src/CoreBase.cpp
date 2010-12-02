#include <CoreBase.h>
#include <ILevel.h>
#include <GraphicsThread.h>

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
			currentLevel->drop();
		}
		if(nextLevel != nullptr)
		{
			nextLevel->drop();
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
			nextLevel->drop();
		}
		nextLevel = nLevel;
		if(nextLevel != nullptr)
		{
			nextLevel->grab();
		}
	}

	ECode CoreBase::InitGraphicsThread(const irr::SIrrlichtCreationParameters& cp)
	{
		if(gThread != nullptr)
		{
			delete gThread;
		}
		try
		{
			gThread = new GraphicsThread(cp);
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

	Error::ECode CoreBase::Run(ILevel* firstLevel)
	{
		if(gThread == nullptr)
		{
			lastError = Error::E_CEK_INVALID_STATE;
			lastErrorFunction = __FUNCTION__;
			customLastErrorMessage = "The graphics thread must be initialized before calling CoreBase::Run.";
			return lastError;
		}

		currentLevel = firstLevel;
		if(currentLevel != nullptr)
		{
			currentLevel->grab();
		}
		
		while(currentLevel != nullptr)
		{
			if(Succeeded(currentLevel->Load(gThread)))
			{
				currentLevel->Run();
			}
			currentLevel->drop();
			currentLevel = nextLevel;
		}

		return Error::E_CEK_SUCCESS;
	}
} //end namespace Core