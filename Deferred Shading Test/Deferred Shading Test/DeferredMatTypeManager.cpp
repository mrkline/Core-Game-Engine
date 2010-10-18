#include "DeferredMatTypeManager.h"
#include "Paths.h"
#include "ResourceManager.h"
#include "GlobalContext.h"
using namespace Paths;
using namespace Resources;

namespace Graphics
{
	DeferredMatTypeManager::DeferredMatTypeManager(E_VERTEX_SHADER_TYPE vsVersion, E_PIXEL_SHADER_TYPE psVersion)
	{
		baseCB = new BaseCallback;
		positionCB = new PositionCreatorCallback;

		IVideoDriver* vd = GLOBAL_VD;

		IReadFile* fh = ResourceManager::OpenResource(DeferredMatTypes::UnlitMaterial);
		if(fh == nullptr)
			throw new Exception("Unlit Deferred type shader file couldn't be opened", __FUNCTION__);

		mts[EDT_UNLIT] = static_cast<E_MATERIAL_TYPE>(vd->getGPUProgrammingServices()->
			addHighLevelShaderMaterialFromFiles(fh,
			"VSUnlit", vsVersion,
			fh, "PSUnlit", psVersion,
			baseCB));
		if(mts[EDT_UNLIT] < 0)
		{
			fh->drop();
			throw new Exception("Unlit Deferred type shader couldn't be loaded", __FUNCTION__);
		}
		fh->drop();

		fh = ResourceManager::OpenResource(DeferredHelperTypes::PositionCreator);
		if(fh == nullptr)
			throw new Exception("Position Creator shader file couldn't be opened", __FUNCTION__);

		protectedMTs[EPT_POSITION_CREATOR] = static_cast<E_MATERIAL_TYPE>(vd->getGPUProgrammingServices()->
			addHighLevelShaderMaterialFromFiles(nullptr,
			nullptr, video::EVST_VS_1_1,
			fh, "PSPositionCreator", psVersion,
			positionCB));
		if(protectedMTs[EPT_POSITION_CREATOR] < 0)
		{
			fh->drop();
			throw new Exception("Position Creator shader couldn't be loaded", __FUNCTION__);
		}
		fh->drop();
	}

	DeferredMatTypeManager::~DeferredMatTypeManager()
	{
		baseCB->drop();
		positionCB->drop();
	}

	//Callbacks
	void DeferredMatTypeManager::BaseCallback::OnSetConstants(IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* vd = GLOBAL_VD;

		const ICameraSceneNode* cam = GLOBAL_SM->getActiveCamera();
		
		//Write camera direction vector (used for calculating depth map)
		vector3df cameraVector(0.0f, 0.0f, 1.0f);
		f32 cameraArray[3];		
		const matrix4& camTransform = cam->getAbsoluteTransformation();
		camTransform.rotateVect(cameraVector);
		cameraVector.setLength(1.0f / cam->getFarValue());
		cameraVector.getAs3Values(cameraArray);
		services->setVertexShaderConstant("cameraDirection", cameraArray, 3);
		//Write camera position (used for calculating depth map)
		camTransform.getTranslation().getAs3Values(cameraArray);
		services->setVertexShaderConstant("cameraPosition", cameraArray, 3);

		matrix4 worldViewProjection = vd->getTransform(video::ETS_PROJECTION);
		worldViewProjection *= vd->getTransform(video::ETS_VIEW);
		worldViewProjection *= vd->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("worldViewProjection",
			worldViewProjection.pointer(), 16);

		matrix4 world = vd->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("world", world.pointer(), 16);
		matrix4 worldTranspose;
		world.getTransposed(worldTranspose);
		matrix4 worldInverseTranspose;
		worldTranspose.getInverse(worldInverseTranspose);
		services->setVertexShaderConstant("worldInverseTranspose", worldInverseTranspose.pointer(), 16);
	}

	void DeferredMatTypeManager::BaseCallback::OnSetMaterial(const SMaterial& mat)
	{
		//TODO: Access velocity info here
	}

	void DeferredMatTypeManager::PositionCreatorCallback::OnSetConstants(IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* vd = GLOBAL_VD;
		ISceneManager* sm = GLOBAL_SM;

		ICameraSceneNode* cam = sm->getActiveCamera();
		//cam->getViewFrustum
	}
} //end namespace Graphics