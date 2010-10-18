#pragma once

#include "IPostProcess.h"

namespace Graphics
{
	class InvertPostProcess : public IPostProcess
	{
	public:
		InvertPostProcess();
		void Render(ITexture* __restrict source, ITexture* __restrict output);

	private:
		E_MATERIAL_TYPE mt;
	};
} //end namespace Graphics
