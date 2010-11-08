#pragma once

#include <DeviceContext.h>

namespace GlobalContext
{
	//Graphics context
	extern IrrlichtDeviceContext DeviceContext;
} //end namespace GlobalContext

//for ease of typing
#define GLOBAL_DEVICE GlobalContext::DeviceContext.GetDevice()
#define GLOBAL_VD GlobalContext::DeviceContext.GetVideoDriver()
#define GLOBAL_SM GlobalContext::DeviceContext.GetSceneManager()
#define GLOBAL_FS GlobalContext::DeviceContext.GetFileSystem()
#define GLOBAL_TIMER GlobalContext::DeviceContext.GetTimer()
#define GLOBAL_GUIENV GlobalContext::DeviceContext.GetGUIEnvironment() 