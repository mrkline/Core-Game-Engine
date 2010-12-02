#pragma once
#include <CoreTypes.h>
#include <ErrorHandling.h>

namespace Core
{
	class ILevel;
	class GraphicsThread;

	//The base for the entire engine.  Instantiates all other objects in the engine.
	class CoreBase : public ReferenceCounted, public Error::CanErr
	{
	public:
		CoreBase();
		virtual ~CoreBase();

		ILevel* GetCurrentLevel() { return currentLevel; }
		ILevel* GetNextLevel() { return nextLevel; }
		bool HasNextLevel() { return nextLevel != nullptr; }
		//CoreBase will grab the provided level (if it isn't null)
		void SetNextLevel(ILevel* nLevel);

		GraphicsThread* GetGraphicsThread() { return gThread; }
		//Resets Graphics Thread if it has already been created.
		//If this happens, all textures, shaders, etc. must be reloaded.
		Error::ECode InitGraphicsThread(const irr::SIrrlichtCreationParameters& cp);

		//Run the game, starting with the given level.  The graphics thread must be
		//initialized before this.
		Error::ECode Run(ILevel* firstLevel);


	protected:
		ILevel *currentLevel, *nextLevel;
		GraphicsThread* gThread;
	};
} //end namespace Core