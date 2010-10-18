#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

//temp for fps
#include <ctime>

#pragma comment(lib, "irrlicht.lib")

#include "ScreenQuad.h"
#include "PhongShaderManager.h"
#include "GlobalContext.h"
#include "PostProcessingManager.h"
#include "HDRPostProcess.h"
#include "DSBloomGenerator.h"
#include "LuminanceGenerator.h"
#include "Amplifier.h"

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
		IVideoDriver* vd = GLOBAL_VD;
		vd->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
		ISceneManager* sm = GLOBAL_SM;
		IGUIEnvironment* guienv = GLOBAL_GUIENV;
		guienv->getSkin()->setFont(guienv->getFont("displayfont.png"));
		guienv->getSkin()->setColor(gui::EGDC_GRAY_TEXT, SColor(200, 255, 255, 255));
		guienv->getSkin()->setColor(gui::EGDC_HIGH_LIGHT_TEXT, SColor(255, 255, 255, 255));
		guienv->getSkin()->setColor(gui::EGDC_BUTTON_TEXT, SColor(255, 255, 255, 255));

		ITexture* rt = vd->addRenderTargetTexture(cp.WindowSize, "rt", video::ECF_A32B32G32R32F);
		ITexture* processedRT = vd->addRenderTargetTexture(cp.WindowSize, "rt", video::ECF_A8R8G8B8);
		
		
		ScreenQuad quad;

		PhongShaderManager psm(vd, GLOBAL_FS->getWorkingDirectory());

		Amplifier amp;

		PostProcessingManager ppm;
		HDRPostProcess pp(cp.WindowSize);
		ppm.AddPostProcess(&pp);

		pp.GetBloomGenerator()->SetGaussianCoefficient(0.3f);


		GLOBAL_DEVICE->getCursorControl()->setVisible(false);
		ICameraSceneNode* cam = sm->addCameraSceneNodeFPS(nullptr, 50.0f, 0.02f);
		cam->setPosition(vector3df(0, 0, -20));
		cam->setTarget(vector3df());

		sm->addSkyDomeSceneNode(vd->getTexture("spaceskydome.jpg"), 32, 16);


		SMaterial phong;
		phong.Lighting = false;
		phong.MaterialType = psm.getMaterialType();
		phong.Shininess = 1.0f;
		phong.MaterialTypeParam = 200.0f;


		//Temp for holding geometry while we create it
		IMesh* mesh;
		IMeshSceneNode* node;

		mesh = sm->addSphereMesh("sun", 100, 50, 50);
		sm->getMeshManipulator()->setVertexColors(mesh, SColor(255, 255, 255, 0));
		node = sm->addMeshSceneNode(mesh, nullptr, -1, vector3df(0, 0, -200));
		node->getMaterial(0).Lighting = false;
		node->getMaterial(0).MaterialType = amp.getMaterialType();

		ILightSceneNode* light = sm->addLightSceneNode(node, vector3df(0, 0, 0));
		light->getLightData().AmbientColor = SColorf(0.3f, 0.3f, 0.3f);

		mesh = sm->getMesh("teapot.x");
		sm->getMeshManipulator()->setVertexColors(mesh, SColor(255, 255, 0, 0));
		node = sm->addMeshSceneNode(mesh, nullptr, -1, vector3df());
		node->getMaterial(0) = phong;
		ISceneNodeAnimator* spinner = sm->createRotationAnimator(vector3df(0.1f, 0.5f, 0.0f));
		node->addAnimator(spinner);
		spinner->drop();

		clock_t lastFPSCheck = 0;
		clock_t checkInterval = CLOCKS_PER_SEC / 3;

		rect<s32> quarters[4];
		quarters[0].LowerRightCorner.Y = quarters[1].LowerRightCorner.Y
			= quarters[2].UpperLeftCorner.Y = quarters[3].UpperLeftCorner.Y = cp.WindowSize.Height / 2;
		quarters[2].LowerRightCorner.Y = quarters[3].LowerRightCorner.Y = cp.WindowSize.Height;
		quarters[0].LowerRightCorner.X = quarters[1].UpperLeftCorner.X
			= quarters[2].LowerRightCorner.X = quarters[3].UpperLeftCorner.X = cp.WindowSize.Width / 2;
		quarters[1].LowerRightCorner.X = quarters[3].LowerRightCorner.X = cp.WindowSize.Width;


		rect<s32> lumStepView[LuminanceGenerator::kNumLumSteps];

		const dimension2d<u32> viewDim(110, 110);
		for(s32 c = LuminanceGenerator::kNumLumSteps - 1; c >= 0; --c)
		{
			//Get dims of current luminance step
			//get x val
			s32 ulX = 0;
			for(s32 i = c + 1; i < LuminanceGenerator::kNumLumSteps; ++i)
			{
				ulX += viewDim.Width;
			}
			lumStepView[c].UpperLeftCorner.X = ulX;
			lumStepView[c].LowerRightCorner.X = ulX + viewDim.Width;
			lumStepView[c].LowerRightCorner.Y = viewDim.Height;
		}

		//Set up labels
		guienv->addStaticText(L"Luminance Steps\n(sampled down to 1 x 1 average luminance)",
			rect<s32>(vector2di(10,lumStepView[0].LowerRightCorner.Y + 10),
			dimension2di(quarters[0].getWidth(), 100)));
		guienv->addStaticText(L"Bloom",
			rect<s32>(quarters[1].UpperLeftCorner + 10,
			dimension2di(quarters[0].getWidth(), 100)));
		guienv->addStaticText(L"Original Scene",
			rect<s32>(quarters[2].UpperLeftCorner + 10,
			dimension2di(quarters[0].getWidth(), 100)));
		guienv->addStaticText(L"Final Pass\n(with HDR Tone mapping and bloom)",
			rect<s32>(quarters[3].UpperLeftCorner + 10,
			dimension2di(quarters[0].getWidth(), 100)));

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
			vd->setRenderTarget(rt);
			sm->drawAll();
			ppm.Render(rt, processedRT);
			vd->setRenderTarget(video::ERT_FRAME_BUFFER);
			for(s32 c = LuminanceGenerator::kNumLumSteps - 1; c >= 0; --c)
			{
				quad.SetTexture(pp.GetLuminanceGenerator()->GetLuminanceStep(c));
				vd->setViewPort(lumStepView[c]);
				quad.Render(false);
			}
			quad.SetTexture(pp.GetBloomGenerator()->GetOutput());
			vd->setViewPort(quarters[1]);
			quad.Render(false);
			quad.SetTexture(rt);
			vd->setViewPort(quarters[2]);
			quad.Render(false);
			quad.SetTexture(processedRT);
			vd->setViewPort(quarters[3]);
			quad.Render(false);
			vd->setViewPort(rect<s32>(vector2d<s32>(), vector2d<s32>(cp.WindowSize.Width, cp.WindowSize.Height))); 
			guienv->drawAll();
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
		GLOBAL_DEVICE->drop();
		return 1;
	}
	GlobalContext::DeviceContext.ClearDevice();
	return 0;
}