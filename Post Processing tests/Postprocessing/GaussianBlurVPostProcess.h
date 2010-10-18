#pragma once

#include "IPostProcess.h"
#include "GaussianBlurBase.h"

namespace Graphics
{
	class GaussianBlurVPostProcess : public GaussianBlurBase, public IPostProcess, public IShaderConstantSetCallBack
	{
	public:
		GaussianBlurVPostProcess();
		void Render(ITexture* __restrict source, ITexture* __restrict output);

	private:
		E_MATERIAL_TYPE mt;

		//Dimensions of last target (for a check if a switch is necessary)
		dimension2d<u32> lastOutputDims;

		f32 blurOffsets[9];

		void OnSetConstants(IMaterialRendererServices* services, s32 userData);
	};
} //End namespace Graphics
