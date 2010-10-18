#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;

namespace Graphics
{
	class GaussianBlurBase
	{
	public:
		GaussianBlurBase();

		f32 GetGaussianCoefficient() { return gaussCoeff; }
		f32 GetGaussianMean() { return gaussMean; }
		f32 GetGaussianStandardDeviation() { return gaussStandDev; }
		
		void SetGaussianCoefficient(f32 newCoeff)
		{
			gaussCoeff = newCoeff;
			CalculateWeights();
		}
		void SetGaussianMean(f32 newMean)
		{
			gaussMean = newMean;
			CalculateWeights();
		}
		void SetGaussianStandardDeviation(f32 newStandDev)
		{
			gaussStandDev = newStandDev;
			CalculateWeights();
		}

	protected:
		f32 blurWeights[9];

		f32 gaussCoeff;
		f32 gaussMean;
		f32 gaussStandDev;

		void CalculateWeights();
	};
} //end namespace Graphics
