#pragma once

#include "IPostProcess.h"

namespace Graphics
{
	class LuminanceGenerator;
	class DSBloomGenerator;
	class TextureAdder;

	class HDRPostProcess : public IPostProcess, public IShaderConstantSetCallBack
	{
	public:
		//Set useHalfBuffers true to use 16-bit buffers instead of 32-bit ones
		HDRPostProcess(const dimension2d<u32>& sourceSize, bool useHalfBuffers = false);
		virtual ~HDRPostProcess();
		void Render(ITexture* __restrict source, ITexture* __restrict output);

		LuminanceGenerator* GetLuminanceGenerator() { return lg; }

		//Be aware that the brightness threshold is set dynamically each frame
		DSBloomGenerator* GetBloomGenerator() { return bg; }

		void SetExposure(f32 exposureLevel) { exposure = exposureLevel; }
		f32 GetExposure() { return exposure; }

		void SetMinimumLuminance(f32 min) { min < 0.0f ? minLuminance = 0.0f : minLuminance = min; }
		f32 GetMinimumLuminance() { return minLuminance; }
		
		void SetMaximumLuminance(f32 max) { max <= minLuminance ? maxLuminance = minLuminance + 0.1f : maxLuminance = max; }
		f32 GetMaximumLuminance() { return maxLuminance; }

		void SetLuminanceIncreaseRate(f32 rate) { lumIncreaseRate = rate; }
		f32 GetLuminanceIncreaseRate() { return lumIncreaseRate; }

		void SetLuminanceDecreaseRate(f32 rate) { lumDecreaseRate = rate; }
		f32 GetLumnanceDecreaseRate() { return lumDecreaseRate; }

	protected:
		LuminanceGenerator* lg;
		DSBloomGenerator* bg;
		TextureAdder* ta;

		E_MATERIAL_TYPE mt;

		f32 exposure;

		u32 lastLuminanceAdjustment;

		f32 currLuminance;
		f32 minLuminance;
		f32 maxLuminance;

		//Amount per second luminance can decrease
		f32 lumIncreaseRate;
		//Amount per second luminance can increase
		f32 lumDecreaseRate;
		//Luminance sent to the shader
		f32 outputLuminance;

		virtual void OnSetConstants(IMaterialRendererServices* services, s32 userData);
	};
} //End namespace Graphics
