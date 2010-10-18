#include "TextureAdder.h"
#include "GlobalContext.h"
#include "ResourceManager.h"
#include "Paths.h"

namespace Graphics
{
	TextureAdder::TextureAdder(const dimension2d<u32>& outputSize, ECOLOR_FORMAT bufferType)
	{
		IReadFile* fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::TextureAdder);
		if(fh == NULL)
			throw new Exception("Texture Adder shader file couldn't be opened", __FUNCTION__);

		IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();

		mt = (E_MATERIAL_TYPE)vd->getGPUProgrammingServices()->
			addHighLevelShaderMaterialFromFiles(nullptr,
			nullptr, video::EVST_VS_1_1,
			fh, "PSTextureAdder", video::EPST_PS_2_0);
		if(mt < 0)
		{
			fh->drop();
			throw new Exception("Texture Adder shader couldn't be loaded", __FUNCTION__);
		}

		out = vd->addRenderTargetTexture(outputSize, "textureAdderRT", bufferType);
		
		quad.SetMaterialType(mt);

		fh->drop();
	}

	void TextureAdder::Render(ITexture* __restrict source)
	{
		quad.SetTexture(source);
		quad.Render(out);
	}

	void TextureAdder::Render(ITexture* __restrict tex0, ITexture* __restrict tex1)
	{
		quad.SetTexture(tex0, 0);
		quad.SetTexture(tex1, 1);
		quad.Render(out);
	}
} //end namespace Graphics