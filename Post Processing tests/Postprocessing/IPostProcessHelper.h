#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace video;

#include "ScreenQuad.h"

namespace Graphics
{
	class IPostProcessHelper
	{
	public:
		IPostProcessHelper(bool requiresDepthOrNormalBuff = false)
			: requiresDepthOrNormBuff(requiresDepthOrNormalBuff)
		{
		}

		virtual void Render(ITexture* __restrict source) = 0;
		virtual ITexture* GetOutput() = 0;

		bool GetRequiresDepthOrNormBuff() { return requiresDepthOrNormBuff; }

	protected:
		bool requiresDepthOrNormBuff;
		ScreenQuad quad;
	};
} //end namespace graphics
