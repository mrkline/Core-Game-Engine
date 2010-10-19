#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

//temp for fps
#include <ctime>

#pragma comment(lib, "irrlicht.lib")

#include "ScreenQuad.h"
#include "GlobalContext.h"
#include "DeferredMatTypeManager.h"
#include "DeferredRenderer.h"
#include "BufferViewSwitcher.h"
#include <d3d9.h>

using namespace Graphics;

int main()
{
	SIrrlichtCreationParameters cp;
	cp.AntiAlias = 8;
	cp.Bits = 32;
	cp.DriverType = video::EDT_DIRECT3D9;
	//cp.Vsync = true;

	IrrlichtDevice* nullDevice = createDevice(video::EDT_NULL);
	cp.WindowSize = nullDevice->getVideoModeList()->getDesktopResolution();
	nullDevice->drop();

	GlobalContext::DeviceContext.SetDevice(cp);

	try
	{
		IVideoDriver* vd = GLOBAL_DEVICE->getVideoDriver();
		vd->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
		ISceneManager* sm = GLOBAL_DEVICE->getSceneManager();
		IGUIEnvironment* guienv = GLOBAL_DEVICE->getGUIEnvironment();
		guienv->getSkin()->setFont(guienv->getFont("displayfont.png"));
		guienv->getSkin()->setColor(gui::EGDC_GRAY_TEXT, SColor(200, 255, 255, 255));
		guienv->getSkin()->setColor(gui::EGDC_HIGH_LIGHT_TEXT, SColor(255, 255, 255, 255));
		guienv->getSkin()->setColor(gui::EGDC_BUTTON_TEXT, SColor(255, 255, 255, 255));
		
		
		ScreenQuad quad;

		BufferViewSwitcher bvs;
		DeferredRenderer deferredRenderer(cp.WindowSize);

		GLOBAL_DEVICE->setEventReceiver(&bvs);

		GLOBAL_DEVICE->getCursorControl()->setVisible(false);
		ICameraSceneNode* cam = sm->addCameraSceneNodeFPS(nullptr, 50.0f, 0.02f);
		cam->setPosition(vector3df(0, 0, -5));
		cam->setTarget(vector3df());
		cam->setFarValue(150.0f);
		//sm->addSkyDomeSceneNode(vd->getTexture("spaceskydome.jpg"), 32, 16);


		SMaterial deferred;
		deferred.Lighting = false;
		deferred.MaterialType = deferredRenderer.GetTypeManager()->getType(DeferredMatTypeManager::EDT_UNLIT_COLOR);
		deferred.Shininess = 1.0f;
		deferred.MaterialTypeParam = 200.0f;
		deferred.NormalizeNormals = true;


		//Temp for holding geometry while we create it
		IMesh* mesh;
		IMeshSceneNode* node;

		mesh = sm->addSphereMesh("sun", 100, 100, 50);
		sm->getMeshManipulator()->setVertexColors(mesh, SColor(255, 255, 255, 0));
		node = sm->addMeshSceneNode(mesh, nullptr, -1, vector3df(0, 0, 200));
		node->setRotation(vector3df(-60, 0, 0));
		node->getMaterial(0) = deferred;

		ILightSceneNode* light = sm->addLightSceneNode(node, vector3df(0, 0, 0));
		light->getLightData().AmbientColor = SColorf(0.3f, 0.3f, 0.3f);

		mesh = sm->getMesh("teapot.x");
		sm->getMeshManipulator()->setVertexColors(mesh, SColor(255, 255, 0, 0));
		node = sm->addMeshSceneNode(mesh, nullptr, -1, vector3df(0.0f, 0.0f, 20.0f));
		node->getMaterial(0) = deferred;
		ISceneNodeAnimator* spinner = sm->createRotationAnimator(vector3df(0.1f, 0.5f, 0.0f));
		//node->addAnimator(spinner);
		spinner->drop();
		//node->setDebugDataVisible(scene::EDS_NORMALS);

		clock_t lastFPSCheck = 0;
		clock_t checkInterval = CLOCKS_PER_SEC / 3;

		//quick test
		D3DCAPS9 caps;
		vd->getExposedVideoData().D3D9.D3DDev9->GetDeviceCaps(&caps);
		printf("Max simulteaneous RTs: %d\n", caps.NumSimultaneousRTs);

		while(GLOBAL_DEVICE->run())
		{
			if(lastFPSCheck + checkInterval < clock())
			{
				stringw cap(L"Current FPS is: ");
				cap += vd->getFPS();
				GLOBAL_DEVICE->setWindowCaption(cap.c_str());
				lastFPSCheck = clock();
			}
			
			vd->beginScene(true, true, SColor(255, 127, 127, 127));
			//sm->drawAll();
			deferredRenderer.Render(sm);
			
			quad.SetTexture(deferredRenderer.GetRT(bvs.GetActiveRT()));
			quad.Render();
			//sm->drawAll();
			vd->endScene();
		}
	}
	catch(Exception* e)
	{		
		stringc eString("An error occurred");
		if(e->GetCallingFunction() != nullptr)
		{
			eString += " in the following function: ";
			eString += e->GetCallingFunction();
			eString += "\nThe error is:\n";
		}
		else
		{
			eString += ":\n";
		}

		eString += e->GetExceptionMessage();
		EHandler::ShowErrorDialog(eString.c_str(), "Error");
		e->drop();
		GlobalContext::DeviceContext.ClearDevice();
		return 1;
	}
	catch(...)
	{
		EHandler::ShowErrorDialog("An unspecified fatal error occurred.", "Holy crash Batman!");
		GlobalContext::DeviceContext.ClearDevice();
		return 1;
	}
	GlobalContext::DeviceContext.ClearDevice();
	return 0;
}