#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace video;

#include "ScreenQuad.h"

namespace Graphics
{
	class IPostProcess
	{
	public:

		virtual void Render(ITexture* __restrict source, ITexture* __restrict output) = 0;

	protected:
		ScreenQuad quad;
	};
} //end namespace graphics
