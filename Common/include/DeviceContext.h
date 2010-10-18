#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace gui;

#include "ErrorHandling.h"
//#include "RTCache.h"

namespace GlobalContext
{
	//extern RTCache RenderTargetCache;

	//Since the Irrlicht device is pretty much a global in itself, make it globally accessible.
	class IrrlichtDeviceContext
	{
	public:
		IrrlichtDeviceContext()
			: device(nullptr),
			vd(nullptr),
			sm(nullptr),
			fs(nullptr),
			timer(nullptr),
			guienv(nullptr)
		{
		}

		inline void SetDevice(const SIrrlichtCreationParameters& cp)
		{
			//Clear out a previous device if we're creating a new one
			ClearDevice();

			device = createDeviceEx(cp);
			vd = device->getVideoDriver();
			sm = device->getSceneManager();
			fs = device->getFileSystem();
			timer = device->getTimer();
			guienv = device->getGUIEnvironment();
			//RenderTargetCache.ClearForNewDevice();
		}

		inline void ClearDevice()
		{
			if(device != nullptr)
			{
				device->drop();
				device = nullptr;
				vd = nullptr;
				sm = nullptr;
				fs = nullptr;
				timer = nullptr;
				guienv = nullptr;
			}
		}

		//The scene manager could be set to something besides the
		//default scene manager
		inline void SetSceneManager(ISceneManager* newSceneManager)
		{
			if(newSceneManager == nullptr)
				throw new ArgumentNullException("SetSceneManager does not accept null scene managers",
					__FUNCTION__);

			sm = newSceneManager;
		}

		__forceinline IrrlichtDevice* GetDevice() { return device; }
		__forceinline IVideoDriver* GetVideoDriver() { return vd; }
		__forceinline ISceneManager* GetSceneManager() { return sm; }
		__forceinline IFileSystem* GetFileSystem() { return fs; }
		__forceinline ITimer* GetTimer() { return timer; }
		__forceinline IGUIEnvironment* GetGUIEnvironment() { return guienv; }

	
	private:
		IrrlichtDevice* device;
		IVideoDriver* vd;
		ISceneManager* sm;
		IFileSystem* fs;
		ITimer* timer;
		IGUIEnvironment* guienv;
	};
} //end namespace GlobalContext