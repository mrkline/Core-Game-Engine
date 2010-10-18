#pragma once

#include "IPostProcess.h"

namespace Graphics
{
	class BrightPassPostProcess : public IPostProcess, public IShaderConstantSetCallBack
	{
	public:
		BrightPassPostProcess();
		void Render(ITexture* __restrict source, ITexture* __restrict output);

		//Set actual brightness threshold
		void SetBrightnessThreshold(f32 newThreshold)
		{
			brightThreshold = newThreshold;
		}

		f32 GetBrightessThreshold() { return brightThreshold; }

	private:
		E_MATERIAL_TYPE mt;

		//Dimensions of last target (for a check if a switch is necessary)
		dimension2d<u32> lastSourceDims;

		//Offsets for scaling in shader
		f32 dsOffsets[4][2];

		//Actual brightness threshold
		f32 brightThreshold;

		void OnSetConstants(IMaterialRendererServices* services, s32 userData);
	};
} //End namespace Graphics