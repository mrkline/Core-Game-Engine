#include "LuminanceGenerator.h"
#include "GlobalContext.h"
#include "ResourceManager.h"
#include "Paths.h"

namespace Graphics
{
	LuminanceGenerator::LuminanceGenerator(ECOLOR_FORMAT bufferType)
	{
		//Set up our callbacks
		lumCB = new LuminanceCallback;
		dsCB = new DownsampleCallback;

		IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();

		IReadFile* fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::Luminance);
		if(fh == NULL)
			throw new Exception("Luminance calculator shader file couldn't be opened", __FUNCTION__);

		luminanceShader = (E_MATERIAL_TYPE)vd->getGPUProgrammingServices()->
			addHighLevelShaderMaterialFromFiles(nullptr,
			nullptr, video::EVST_VS_1_1,
			fh, "PSLuminance", video::EPST_PS_2_0,
			lumCB);
		if(luminanceShader < 0)
		{
			fh->drop();
			throw new Exception("Luminance calculator shader couldn't be loaded", __FUNCTION__);
		}
		fh->drop();

		fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::Luminance);
		if(fh == NULL)
			throw new Exception("Luminance downsampler shader file couldn't be opened", __FUNCTION__);

		downsampler = (E_MATERIAL_TYPE)vd->getGPUProgrammingServices()->
			addHighLevelShaderMaterialFromFiles(nullptr,
			nullptr, video::EVST_VS_1_1,
			fh, "PSLuminanceDownsample", video::EPST_PS_2_0,
			dsCB);
		if(downsampler < 0)
		{
			fh->drop();
			throw new Exception("Luminance downsampler shader couldn't be loaded", __FUNCTION__);
		}
		fh->drop();

		//set up render targets
		for(s32 c = 0; c < kNumLumSteps; c++)
		{
			u32 len = static_cast<u32>(pow(static_cast<f32>(3), c));
			luminanceSteps[c] = vd->addRenderTargetTexture(dimension2d<u32>(len, len),
				path("LuminanceGeneratorStep") + path(c), bufferType);
			if(luminanceSteps[c] == nullptr)
				throw new Exception("Luminance RT couldn't be set up.", __FUNCTION__);
		}
	}

	LuminanceGenerator::~LuminanceGenerator()
	{
		IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();
		for(u32 c = 0; c < kNumLumSteps; c++)
		{
			vd->removeTexture(luminanceSteps[c]);
		}

		lumCB->drop();
		dsCB->drop();
	}

	void LuminanceGenerator::Render(ITexture* __restrict source)
	{
		lumCB->UpdateSourceDimensions(source->getSize());
		quad.SetMaterialType(luminanceShader);
		quad.SetTexture(source);
		quad.Render(luminanceSteps[kNumLumSteps - 1]);
		quad.SetMaterialType(downsampler);
		for(s32 c = kNumLumSteps - 1; c > 0; c--)
		{
			quad.SetTexture(luminanceSteps[c]);
			dsCB->UpdateSourceDimensions(luminanceSteps[c]->getSize());
			dsCB->UpdateDestDimensions(luminanceSteps[c - 1]->getSize().Width);
			quad.Render(luminanceSteps[c - 1]);
		}
	}

	ITexture* LuminanceGenerator::GetOutput()
	{
		return luminanceSteps[0];
	}

	ITexture* LuminanceGenerator::GetLuminanceStep(u32 step)
	{
		if(step >= kNumLumSteps)
			throw new IndexOutOfRangeException("The maximum luminance step is lower than the requested step", __FUNCTION__);

		return luminanceSteps[step];
	}

	//Callback fun
	void LuminanceGenerator::LuminanceCallback::OnSetConstants(IMaterialRendererServices* services, s32 userData)
	{
		services->setPixelShaderConstant("lumOffsets", reinterpret_cast<f32*>(lumOffsets), 8);
	}

	void LuminanceGenerator::LuminanceCallback::UpdateSourceDimensions(const dimension2d<u32>& sourceDims)
	{
		if(sourceDims != lastSourceDims)
		{
			lastSourceDims = sourceDims;

			f32 sU = (1.0f / static_cast<f32>(sourceDims.Width));
			f32 sV = (1.0f / static_cast<f32>(sourceDims.Height));

			lumOffsets[0][0] = -0.5f * sU;
			lumOffsets[0][1] = 0.5f * sV;
			lumOffsets[1][0] = 0.5f * sU;
			lumOffsets[1][1] = 0.5f * sV;
			lumOffsets[2][0] = -0.5f * sU;
			lumOffsets[2][1] = -0.5f * sV;
			lumOffsets[3][0] = 0.5f * sU;
			lumOffsets[3][1] = -0.5f * sV;
		}
	}

	void LuminanceGenerator::DownsampleCallback::OnSetConstants(IMaterialRendererServices* services, s32 userData)
	{
		services->setPixelShaderConstant("dsOffsets", reinterpret_cast<f32*>(dsOffsets), 18);
		services->setPixelShaderConstant("halfDestPixelSize", &halfDestPixelSize, 1);
	}

	void LuminanceGenerator::DownsampleCallback::UpdateSourceDimensions(const dimension2d<u32>& sourceDims)
	{
		s32 idx = 0;
		for( int x = -1; x < 2; x++ )
		{
			for( int y = -1; y < 2; y++ )
			{
				dsOffsets[idx][0] = static_cast<f32>(x) / static_cast<f32>( sourceDims.Width );
				dsOffsets[idx][1] =	static_cast<f32>(y) / static_cast<f32>( sourceDims.Height );
				++idx;
			}
		}
	}

	void LuminanceGenerator::DownsampleCallback::UpdateDestDimensions(u32 destWidth)
	{
		halfDestPixelSize = 0.5f / static_cast<f32>(destWidth);
	}
} //end namespace Graphics
