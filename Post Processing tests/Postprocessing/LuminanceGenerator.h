#pragma once

#include "IPostProcessHelper.h"

namespace Graphics
{
	class LuminanceGenerator : public IPostProcessHelper
	{
	public:
		static const s32 kNumLumSteps = 6;

		//Requires only a red and green channel (for storing max and average luminances)
		LuminanceGenerator(ECOLOR_FORMAT bufferType = video::ECF_R32F);
		virtual ~LuminanceGenerator();

		void Render(ITexture* __restrict source);
		ITexture* GetOutput();

		ITexture* GetLuminanceStep(u32 step);

	protected:
		class LuminanceCallback : public IShaderConstantSetCallBack
		{
		public:
			void OnSetConstants(IMaterialRendererServices* services, s32 userData);
			void UpdateSourceDimensions(const dimension2d<u32>& sourceDims);

		protected:
			dimension2d<u32> lastSourceDims;
			f32 lumOffsets[4][2];
		};

		class DownsampleCallback : public IShaderConstantSetCallBack
		{
		public:
			void OnSetConstants(IMaterialRendererServices* services, s32 userData);
			void UpdateSourceDimensions(const dimension2d<u32>& sourceDims);
			void UpdateDestDimensions(u32 destWidth);

		protected:
			f32 dsOffsets[9][2];
			f32 halfDestPixelSize;
		};


		LuminanceCallback* lumCB;
		DownsampleCallback* dsCB;

		//Luminance steps - ramp down from 243 x 243 to 1x1
		ITexture* luminanceSteps[kNumLumSteps];

		//Initial luminance shader that scales down to 243 x 243 and calculates luminance
		E_MATERIAL_TYPE luminanceShader;
		//3x3 downsampler
		E_MATERIAL_TYPE downsampler;
	};
} //end namespace Graphics
