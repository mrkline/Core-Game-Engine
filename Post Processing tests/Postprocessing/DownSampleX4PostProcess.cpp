#include "DownSampleX4PostProcess.h"
#include "GlobalContext.h"
#include "ResourceManager.h"
#include "Paths.h"

namespace Graphics
{
	DownSampleX4PostProcess::DownSampleX4PostProcess()
	{
		IReadFile* fh = Resources::ResourceManager::OpenResource(Paths::PostProcesses::DownSampleX4);
		if(fh == NULL)
			throw new Exception("Down-Sample shader file couldn't be opened", __FUNCTION__);

		mt = (E_MATERIAL_TYPE)GlobalContext::DeviceContext.GetVideoDriver()->getGPUProgrammingServices()->
			addHighLevelShaderMaterialFromFiles(nullptr,
			nullptr, video::EVST_VS_1_1,
			fh, "PSDownSample", video::EPST_PS_2_0,
			this);
		if(mt < 0)
		{
			fh->drop();
			throw new Exception("Down-Sample shader couldn't be loaded", __FUNCTION__);
		}
		
		quad.SetMaterialType(mt);

		fh->drop();
	}

	void DownSampleX4PostProcess::OnSetConstants(IMaterialRendererServices* services, s32 userData)
	{
		services->setPixelShaderConstant("dsOffsets", reinterpret_cast<f32*>(dsOffsets), 32);
	}

	void DownSampleX4PostProcess::Render(ITexture* __restrict source, ITexture* __restrict output)
	{
		if(output->getSize() != lastOutputDims)
		{
			//we need to update the shader offsets since the output size
			//has changed
			lastOutputDims = output->getSize();

			s32 idx = 0;
			for( s32 i = -2; i < 2; i++ )
			{
				for( s32 j = -2; j < 2; j++ )
				{
					dsOffsets[idx][0] = (static_cast<f32>( i ) + 0.5f) * (1.0f / static_cast<f32>(lastOutputDims.Width));
					dsOffsets[idx][1] = (static_cast<f32>( j ) + 0.5f) * (1.0f / static_cast<f32>(lastOutputDims.Height));
					idx++;
				}
			}
		}

		quad.SetTexture(source);
		quad.Render(output);
	}
} //end namespace Graphics