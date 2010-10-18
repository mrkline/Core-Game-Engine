#pragma once

#include "IPostProcessHelper.h"
#include "BrightPassPostProcess.h"
#include "DownSampleX4PostProcess.h"
#include "GaussianBlurHPostProcess.h"
#include "GaussianBlurVPostProcess.h"

namespace Graphics
{
	class DSBloomGenerator : public IPostProcessHelper
	{
	public:
		DSBloomGenerator(const dimension2d<u32>& sourceSize, ECOLOR_FORMAT bufferType = video::ECF_A32B32G32R32F);
		virtual ~DSBloomGenerator();
	
		void Render(ITexture* __restrict source);
		ITexture* GetOutput() { return blurRT1; }

		void SetBrightnessThreshold(f32 newThreshold)
		{
			brightPass->SetBrightnessThreshold(newThreshold);
		}

		f32 GetBrightessThreshold() { return brightPass->GetBrightessThreshold(); }

		f32 GetGaussianCoefficient() { return blurH->GetGaussianCoefficient(); }
		f32 GetGaussianMean() { return blurH->GetGaussianMean(); }
		f32 GetGaussianStandardDeviation() { return blurH->GetGaussianStandardDeviation(); }
		
		void SetGaussianCoefficient(f32 newCoeff)
		{
			blurH->SetGaussianCoefficient(newCoeff);
			blurV->SetGaussianCoefficient(newCoeff);
		}
		void SetGaussianMean(f32 newMean)
		{
			blurH->SetGaussianMean(newMean);
			blurV->SetGaussianMean(newMean);
		}
		void SetGaussianStandardDeviation(f32 newStandDev)
		{
			blurH->SetGaussianStandardDeviation(newStandDev);
			blurV->SetGaussianStandardDeviation(newStandDev);
		}
		
	protected:
		ITexture* brightPassRT;
		ITexture* blurRT1;
		ITexture* blurRT2;

		BrightPassPostProcess* brightPass;
		DownSampleX4PostProcess* downSample;
		GaussianBlurHPostProcess* blurH;
		GaussianBlurVPostProcess* blurV;
	};
} //end namespace Graphics
