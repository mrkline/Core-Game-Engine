#pragma once

#include "IPostProcessHelper.h"

namespace Graphics
{
	class TextureAdder : public IPostProcessHelper
	{
	public:
		TextureAdder(const dimension2d<u32>& outputSize, ECOLOR_FORMAT bufferType = video::ECF_A32B32G32R32F);

		ITexture* GetOutput() { return out; }

		//Render, adding the texture set in SetTexToAdd
		void Render(ITexture* __restrict source);
		//Render, giving both textures
		void Render(ITexture* __restrict tex0, ITexture* __restrict tex1);

		void SetTexToAdd(ITexture* texToAdd) { quad.SetTexture(texToAdd, 1); }
		ITexture* GetTexToAdd() { return quad.GetTexture(1); }
		

	protected:
		ITexture* out;
		E_MATERIAL_TYPE mt;
	};
} //end namespace graphics