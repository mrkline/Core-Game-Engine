#pragma once
#include <ErrorHandling.h>

namespace Core
{
	class IGraphicsThread;

	//!A level contains one or more scenes.
	//!One scene is presumably the main scene, and others could be 3D UIs, etc.
	class ILevel
	{
	public:
		virtual ~ILevel();

		/*!
		\brief Load the entire level.
		\return An ECode that indicates the outcome of loading the level
		\warning This level will be deleted if E_CEK_SUCCESS is not returned.
		*/
		virtual Error::ECode Load(IGraphicsThread* gThread) = 0;
		/*!
		\brief Called if the GraphicsThread was reset (due to resolution change, etc.).
		\todo Is this needed for OGRE?
		
		This level should exit out of its Run method if E_CEK_SUCCESS is not returned.
		*/
		virtual Error::ECode OnGraphicsReset(IGraphicsThread* newGThread) = 0;

		//! Called after Load.  Game loop should reside here.
		virtual Error::ECode Run() = 0;
	};
}