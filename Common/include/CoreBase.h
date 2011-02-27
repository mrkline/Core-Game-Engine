#pragma once
#include "ErrorHandling.h"

namespace Core
{
	class ILevel;
	class IGraphicsThread;

	//!The base for the entire engine.  Instantiates all other objects in the engine.
	class CoreBase : public Error::CanErr
	{
	public:
		CoreBase();
		virtual ~CoreBase();

		ILevel* GetCurrentLevel() { return currentLevel; } //!< Returns the current level
		ILevel* GetNextLevel() { return nextLevel; } //!< Returns the next level, or nullptr if there is none
		bool HasNextLevel() { return nextLevel != nullptr; } //!< Returns true if nextLevel is not null
		/*!
		\brief Sets the next level to be run
		\param nLevel The next level to be run
		
		Calling SetNextLevel gives ownership and deletion power of the level to this CoreBase.
		The level will be deleted by this CoreBase when it is done being used, or a new next level is set.
		*/
		void SetNextLevel(ILevel* nLevel);

		IGraphicsThread* GetGraphicsThread() { return gThread; } //!< Returns the graphics thread
		/*!
		\brief Initializes or resets the graphics thread
		\todo Implement parameters
		\return an ECode indicating the outcome of the initialization
		
		Resets Graphics Thread if it has already been created.
		If this happens, all textures, shaders, etc. must be reloaded.
		*/
		Error::ECode InitGraphicsThread(/*TODO: Graphics Params*/);

		/*!
		\brief Runs the game, starting with the level set by SetNextLevel.
		\see InitGraphicsThread
		\return an ECode indicating the outcome of running the engine

		The graphics thread must be initialized before calling Run.
		*/
		Error::ECode Run();


	protected:
		ILevel *currentLevel; //<! The current, running level
		ILevel *nextLevel;	//<! The level to be run after currentLevel has finished
		IGraphicsThread* gThread; //<! The graphics thread
	};
} //end namespace Core