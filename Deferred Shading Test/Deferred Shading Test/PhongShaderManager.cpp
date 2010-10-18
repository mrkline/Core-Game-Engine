#include "PhongShaderManager.h"

using namespace core;

PhongShaderManager::PhongShaderManager(IVideoDriver* __restrict driver,
	const path& shaderDirectory)
	: vd(driver)
{
	IGPUProgrammingServices* gpu = vd->getGPUProgrammingServices();

	colorMatType = gpu->addHighLevelShaderMaterialFromFiles(
		shaderDirectory + "\\ColorPhong.fx", "ColorPhongVS", video::EVST_VS_1_1,
		shaderDirectory + "\\ColorPhong.fx", "ColorPhongPS", video::EPST_PS_2_0,
		this);
}

E_MATERIAL_TYPE PhongShaderManager::getMaterialType()
{
		return (E_MATERIAL_TYPE)colorMatType;
}

void PhongShaderManager::OnSetConstants(
	IMaterialRendererServices* services, s32 userData)
{
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

	matrix4 viewInverse;
	vd->getTransform(video::ETS_VIEW).getInverse(viewInverse);
	services->setVertexShaderConstant("viewInverse", viewInverse.pointer(), 16);
	SLight light = vd->getDynamicLight(0);
	f32 arr[3];

	arr[0] = light.Position.X;
	arr[1] = light.Position.Y;
	arr[2] = light.Position.Z;
	services->setVertexShaderConstant("lightPosition", arr, 3);

	arr[0] = light.AmbientColor.r;
	arr[1] = light.AmbientColor.g;
	arr[2] = light.AmbientColor.b;
	services->setPixelShaderConstant("ambientColor", arr, 3);

	arr[0] = light.DiffuseColor.r;
	arr[1] = light.DiffuseColor.g;
	arr[2] = light.DiffuseColor.b;
	services->setPixelShaderConstant("lightColor", arr, 3);

	services->setPixelShaderConstant("specularAmount",
		&specularAmount, 1);

	services->setPixelShaderConstant("specularExponent",
		&specularExponent, 1);
}

void PhongShaderManager::OnSetMaterial(const SMaterial& mat)
{
	specularAmount = mat.Shininess;
	specularExponent = mat.MaterialTypeParam;
}