#pragma once

#include "IPostProcess.h"

namespace Graphics
{
	class DownSampleX4PostProcess : public IPostProcess, public IShaderConstantSetCallBack
	{
	public:
		DownSampleX4PostProcess();
		void Render(ITexture* __restrict source, ITexture* __restrict output);

	private:
		E_MATERIAL_TYPE mt;

		//Dimensions of last target (for a check if a switch is necessary)
		dimension2d<u32> lastOutputDims;

		//Offsets for downsample shader
		f32 dsOffsets[16][2];

		void OnSetConstants(IMaterialRendererServices* services, s32 userData);
	};
} //end namespace Graphics
