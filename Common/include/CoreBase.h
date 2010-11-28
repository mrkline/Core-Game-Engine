#pragma once
#include <CoreTypes.h>
#include <ErrorHandling.h>

namespace Core
{
	class Level;
	class GraphicsThread;

	//The base for the entire engine.  Instantiates all other objects in the engine.
	class CoreBase : public ReferenceCounted, public Error::CanErr
	{
	public:
		CoreBase();
		virtual ~CoreBase();

		Level* GetCurrentLevel() { return currentLevel; }
		Level* GetNextLevel() { return nextLevel; }
		bool HasNextLevel() { return nextLevel != nullptr; }
		void SetNextLevel(Level* nLevel) { nextLevel = nLevel; }

		GraphicsThread* GetGraphicsThread() { return gThread; }
		//Resets Graphics Thread if it has already been created.
		//If this happens, all textures, shaders, etc. must be reloaded.
		Error::ECode InitGraphicsThread(const irr::SIrrlichtCreationParameters& cp);

		//Run the game, starting with the given level.  The graphics thread must be
		//initialized before this.
		Error::ECode Run(Level* firstLevel);


	protected:
		Level *currentLevel, *nextLevel;
		GraphicsThread* gThread;
	};
};