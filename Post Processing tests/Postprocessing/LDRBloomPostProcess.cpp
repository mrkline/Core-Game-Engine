#include "LDRBloomPostProcess.h"
#include "GlobalContext.h"
#include "ResourceManager.h"
#include "Paths.h"

namespace Graphics
{
	LDRBloomPostProcess::LDRBloomPostProcess(const dimension2d<u32>& sourceSize)
		: bloomLimit(0.3f)
	{
		IReadFile* fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::Bloom);
		if(fh == NULL)
			throw new Exception("Bloom shader file couldn't be opened", __FUNCTION__);

		mt = (E_MATERIAL_TYPE)GlobalContext::DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->
			addHighLevelShaderMaterialFromFiles(nullptr,
			nullptr, video::EVST_VS_1_1,
			fh, "PSBloom", video::EPST_PS_2_0,
			this);
		if(mt < 0)
		{
			fh->drop();
			throw new Exception("Bloom shader couldn't be loaded", __FUNCTION__);
		}

		quad.SetMaterialType(mt);

		fh->drop();

		bloomGen = new DSBloomGenerator(sourceSize);
		quad.SetTexture(bloomGen->GetOutput(), 1);
	}

	LDRBloomPostProcess::~LDRBloomPostProcess()
	{
		delete bloomGen;
	}

	void LDRBloomPostProcess::OnSetConstants(IMaterialRendererServices* services, s32 userData)
	{
		services->setPixelShaderConstant("bloomLimit", &bloomLimit, 1);
	}

	void LDRBloomPostProcess::Render(ITexture* __restrict source, ITexture* __restrict output)
	{
		bloomGen->Render(source);
		quad.SetTexture(source, 0);
		quad.Render(output);
	}
} //end namespace Graphics