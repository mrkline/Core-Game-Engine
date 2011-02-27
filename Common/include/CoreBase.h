#pragma once
#include <ErrorHandling.h>

namespace Core
{
	class ILevel;
	class IGraphicsThread;

	//The base for the entire engine.  Instantiates all other objects in the engine.
	class CoreBase : public Error::CanErr
	{
	public:
		CoreBase();
		virtual ~CoreBase();

		ILevel* GetCurrentLevel() { return currentLevel; }
		ILevel* GetNextLevel() { return nextLevel; }
		bool HasNextLevel() { return nextLevel != nullptr; }
		//Calling SetNextLevel gives ownership and deletion power of the level to
		//this CoreBase.
		void SetNextLevel(ILevel* nLevel);

		IGraphicsThread* GetGraphicsThread() { return gThread; }
		//Resets Graphics Thread if it has already been created.
		//If this happens, all textures, shaders, etc. must be reloaded.
		Error::ECode InitGraphicsThread(/*TODO: Graphics Params*/);

		//Run the game, starting with the level ste.  The graphics thread must be
		//initialized before this.
		Error::ECode Run();


	protected:
		ILevel *currentLevel, *nextLevel;
		IGraphicsThread* gThread;
	};
} //end namespace Core