#pragma once
#include <CoreTypes.h>
#include <ErrorHandling.h>

namespace Core
{
	class GraphicsThread;

	//A level contains one or more scenes.
	//One scene is presumably the main scene, and others could be 3D UIs, etc.
	class Level : public ReferenceCounted
	{
	public:
		virtual ~Level();

		//Load the entire level.
		//This level will be deleted if E_CEK_SUCCESS is not returned.
		virtual Error::ECode Load(GraphicsThread* gThread) = 0;
		//Called if the GraphicsThread was reset (due to resolution change, etc.).
		//This level will be deleted if E_CEK_SUCCESS is not returned.
		virtual Error::ECode OnGraphicsReset(GraphicsThread* newGThread) = 0;

		//Called after Load.  Game loop here.
		virtual Error::ECode Run() = 0;
	};
}