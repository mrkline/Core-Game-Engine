#pragma once

#include "IPostProcess.h"
#include "DSBloomGenerator.h"

namespace Graphics
{
	class LDRBloomPostProcess : public IPostProcess, public IShaderConstantSetCallBack
	{
	public:
		LDRBloomPostProcess(const dimension2d<u32>& sourceSize);
		virtual ~LDRBloomPostProcess();
		void Render(ITexture* __restrict source, ITexture* __restrict output);

		DSBloomGenerator* GetBloomGenerator() { return bloomGen; }

	private:
		E_MATERIAL_TYPE mt;

		f32 bloomLimit;

		DSBloomGenerator* bloomGen;

		void OnSetConstants(IMaterialRendererServices* services, s32 userData);
	};
} //End namespace Graphics
