#include "GaussianBlurBase.h"

namespace Graphics
{
	GaussianBlurBase::GaussianBlurBase()
		: gaussCoeff(0.4f),
		gaussMean(1.0f),
		gaussStandDev(0.8f)
	{
		CalculateWeights();
	}

	void GaussianBlurBase::CalculateWeights()
	{
		f32 x;
		for(u32 c = 0; c < 9; c++)
		{
			//Map 0-9 to -4 to 4
			x = (static_cast<f32>(c) - 4.0f ) / 4.0f;

			blurWeights[c] = gaussCoeff * ( 1.0f / sqrt( 2.0f * PI * gaussStandDev * gaussStandDev ) )
				* expf( ( -( ( x - gaussMean ) * ( x - gaussMean ) ) ) / ( 2.0f * gaussStandDev * gaussStandDev ) );
		}
	}
} //end namespace Graphics