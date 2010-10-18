#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace video;
using namespace scene;
using namespace io;

class PhongShaderManager : public IShaderConstantSetCallBack
{
public:

	PhongShaderManager(IVideoDriver* __restrict driver,
		const path& shaderDirectory);

	virtual ~PhongShaderManager(){}
	
	E_MATERIAL_TYPE getMaterialType();

protected:
	IVideoDriver* vd;

	s32 colorMatType;

	float specularAmount, specularExponent;
	
	virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData);
	virtual void OnSetMaterial(const SMaterial& mat);
};
