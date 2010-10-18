#include "InvertPostProcess.h"
#include "GlobalContext.h"
#include "ResourceManager.h"
#include "Paths.h"

namespace Graphics
{
	InvertPostProcess::InvertPostProcess()
	{
		IReadFile* fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::Invert);
		if(fh == NULL)
			throw new Exception("Invert shader file couldn't be opened", __FUNCTION__);

		mt = (E_MATERIAL_TYPE)GlobalContext::DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->
			addHighLevelShaderMaterialFromFiles(nullptr,
			nullptr, video::EVST_VS_1_1,
			fh, "PSInvert", video::EPST_PS_1_1);
		if(mt < 0)
		{
			fh->drop();
			throw new Exception("Invert shader couldn't be loaded", __FUNCTION__);
		}
		
		quad.SetMaterialType(mt);

		fh->drop();
	}

	void InvertPostProcess::Render(ITexture* __restrict source, ITexture* __restrict output)
	{	
		quad.SetTexture(source);
		quad.Render(output);
	}
} //end namespace Graphics

