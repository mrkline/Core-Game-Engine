#pragma once
#include <ErrorHandling.h>

namespace Core
{
	class IGraphicsThread;

	//A level contains one or more scenes.
	//One scene is presumably the main scene, and others could be 3D UIs, etc.
	class ILevel
	{
	public:
		virtual ~ILevel();

		//Load the entire level.
		//This level will be deleted if E_CEK_SUCCESS is not returned.
		virtual Error::ECode Load(IGraphicsThread* gThread) = 0;
		//Called if the GraphicsThread was reset (due to resolution change, etc.).
		//This level should exit out of its Run method if E_CEK_SUCCESS is not returned.
		virtual Error::ECode OnGraphicsReset(IGraphicsThread* newGThread) = 0;

		//Called after Load.  Game loop here.
		virtual Error::ECode Run() = 0;
	};
}