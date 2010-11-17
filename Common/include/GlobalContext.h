#pragma once

#include <DeviceContext.h>

namespace Core
{
	namespace GlobalContext
	{
		//Graphics context
		extern IrrlichtDeviceContext DeviceContext;
	} //end namespace GlobalContext
} //end namespace Core

//for ease of typing
#define GLOBAL_DEVICE Core::GlobalContext::DeviceContext.GetDevice()
#define GLOBAL_VD Core::GlobalContext::DeviceContext.GetVideoDriver()
#define GLOBAL_SM Core::GlobalContext::DeviceContext.GetSceneManager()
#define GLOBAL_FS Core::GlobalContext::DeviceContext.GetFileSystem()
#define GLOBAL_TIMER Core::GlobalContext::DeviceContext.GetTimer()
#define GLOBAL_GUIENV Core::GlobalContext::DeviceContext.GetGUIEnvironment() 