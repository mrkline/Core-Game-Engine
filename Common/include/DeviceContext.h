#pragma once

#include <irrlicht.h>

#include <ErrorHandling.h>

namespace Core
{
	namespace GlobalContext
	{
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

			//Creates a graphics device based on creation parameters
			void SetDevice(const irr::SIrrlichtCreationParameters& cp)
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

			//Drop the device and set all pointers to null.
			//Should be called at the end of the program.
			void ClearDevice()
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
			void SetSceneManager(irr::scene::ISceneManager* newSceneManager)
			{
				if(newSceneManager == nullptr)
					throw new Error::ArgumentNullException("SetSceneManager does not accept null scene managers",
						__FUNCTION__);

				sm = newSceneManager;
			}

			__forceinline irr::IrrlichtDevice* GetDevice() { return device; }
			__forceinline irr::video::IVideoDriver* GetVideoDriver() { return vd; }
			__forceinline irr::scene::ISceneManager* GetSceneManager() { return sm; }
			__forceinline irr::io::IFileSystem* GetFileSystem() { return fs; }
			__forceinline irr::ITimer* GetTimer() { return timer; }
			__forceinline irr::gui::IGUIEnvironment* GetGUIEnvironment() { return guienv; }

	
		private:
			irr::IrrlichtDevice* device;
			irr::video::IVideoDriver* vd;
			irr::scene::ISceneManager* sm;
			irr::io::IFileSystem* fs;
			irr::ITimer* timer;
			irr::gui::IGUIEnvironment* guienv;
		};
	} //end namespace GlobalContext
} //end namespace Core