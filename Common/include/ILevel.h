#pragma once

namespace Core
{
	class IGraphicsThread;

	/*!
	\brief The base unit of operation for the engine.

	A level contains one or more scenes.
	One scene is presumably the main scene, and others could be 3D UIs, etc.
	*/
	
	class ILevel
	{
	public:
		virtual ~ILevel();

		/*!
		\brief Load the entire level.
		\param gThread The graphics thread, possibly used to display loading screen
		*/
		virtual void Load(IGraphicsThread* gThread) = 0;
		/*!
		\brief Called if the GraphicsThread was reset (due to resolution change, etc.).
		\todo Is this needed for OGRE?
		*/
		virtual void OnGraphicsReset(IGraphicsThread* newGThread) = 0;

		//! Called after Load.  Game loop should reside here.
		virtual void Run() = 0;
	};
} // end namespace Core