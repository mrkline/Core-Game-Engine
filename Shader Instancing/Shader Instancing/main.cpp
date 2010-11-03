#include <irrlicht.h>
#pragma comment(lib, "irrlicht.lib")

#include <ctime>
#include "ShaderInstancingSceneNode.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

int main()
{
	SIrrlichtCreationParameters cp;
	cp.AntiAlias = 4;
	cp.Bits = 32;
	cp.DriverType = EDT_DIRECT3D9;
//	cp.Fullscreen = true;
	cp.ZBufferBits = 24;

	IrrlichtDevice* nullDevice = createDevice(EDT_NULL);
	//cp.WindowSize = nullDevice->getVideoModeList()->getDesktopResolution();
	nullDevice->drop();

	IrrlichtDevice* device = createDeviceEx(cp);
	IVideoDriver* vd = device->getVideoDriver();
	ISceneManager* sm = device->getSceneManager();

	//Create our transform matrices for the grid of shapes
	const u32 numObjectsAcross = 20;
	const u32 numObjectsSq = numObjectsAcross * numObjectsAcross;
	const u32 numObjectsCubed = numObjectsAcross * numObjectsAcross * numObjectsAcross;
	const f32 objRadius = 5;
	matrix4 matrices[numObjectsCubed];
	for(u32 x = 0; x < numObjectsAcross; ++x)
	{
		for(u32 y = 0; y < numObjectsAcross; ++y)
		{
			for(u32 z = 0; z < numObjectsAcross; ++z)
			{
				matrices[x * numObjectsSq + numObjectsAcross * y + z].setTranslation(
					vector3df(objRadius + 3 * objRadius * x,
					objRadius + 3 * objRadius * y, objRadius + 3 * objRadius * z));
			}
		}
	}

	//Dimensions of the cube of objects
	const f32 megacubeEdgeLen(numObjectsAcross * objRadius * 2
		+ (numObjectsAcross - 1) * objRadius);

	//cursor off
	device->getCursorControl()->setVisible(false);

	vector3df camPosition(-megacubeEdgeLen / 3.0f,
		megacubeEdgeLen * 1.5f, -megacubeEdgeLen / 3.0f);
	vector3df camTarget(megacubeEdgeLen / 2.0f);
	ICameraSceneNode* cam = sm->addCameraSceneNodeFPS(nullptr, 50.0f, 0.3f);
	cam->setPosition(camPosition);
	cam->setTarget(camTarget);

	//ICameraSceneNode* cam = sm->addCameraSceneNodeFPS(NULL, 50, 0.1f);
	//cam->setPosition(vector3df(megacubeEdgeLen / 3.0f, megacubeEdgeLen * 1.5f, -megacubeEdgeLen / 3.0f));
	//device->getCursorControl()->setVisible(false);

	ISceneNode* masterUninstancedNode = sm->addEmptySceneNode();

	IMesh* cubeMesh = sm->getGeometryCreator()->createCubeMesh(vector3df(10));
	Graphics::ShaderInstancingSceneNode* instancer
		= new Graphics::ShaderInstancingSceneNode(cubeMesh, sm, sm->getRootSceneNode());
	instancer->drop();
	cubeMesh->drop();

	SMaterial cubeMat;
	cubeMat.Lighting = false;
	cubeMat.TextureLayer[0].AnisotropicFilter = 16;
	cubeMat.setTexture(0, vd->getTexture("cubeTexture.png"));

	for(u32 c = 0; c < numObjectsCubed; c++)
	{
		ISceneNode* instance = instancer->addInstance(matrices[c], c);
		ISceneNodeAnimator* anim = sm->createRotationAnimator(vector3df(0.5f, 0.5f, 0.0f));
		instance->addAnimator(anim);
		anim->drop();
		ISceneNode* cube = sm->addCubeSceneNode(10, masterUninstancedNode, c);
		cube->setPosition(matrices[c].getTranslation());
		cube->getMaterial(0) = cubeMat;
	}

	//Disable standard drawing for a bit
	masterUninstancedNode->setVisible(false);
	//instancer->setVisible(false);

	clock_t lastFPSCheck = 0;
	clock_t fpsUpdateInterval = CLOCKS_PER_SEC / 3;

	u32 frames = 0;
	while(device->run())
	{
		vd->beginScene(true, true, SColor(255,200,200,255));
		sm->drawAll();
		vd->endScene();
		if (++frames==100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += vd->getName();
			str += L"] Rendering ";
			str += numObjectsCubed;
			str	+= " instances at ";
			str += (s32)vd->getFPS();
			str += " FPS using ";
			str += masterUninstancedNode->isVisible() ?
				"IMeshSceneNodes" : " shader instancing";

			device->setWindowCaption(str.c_str());
			frames=0;
		}
	}
	
	device->drop();
	return 0;
}