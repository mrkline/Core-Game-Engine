#include "DeferredRenderer.h"
#include "GlobalContext.h"

namespace Graphics
{
	DeferredRenderer::DeferredRenderer(const dimension2d<u32>& displayRes, const CreationParams& params)
	{
		IVideoDriver* vd = GLOBAL_VD;
		for(u8 c = 0; c < kMRTCount; ++c)
		{
			mrts[c] = GLOBAL_VD->addRenderTargetTexture(displayRes, "DeferredRT" + c, params.MRTFormat);
			IRenderTarget toAdd(mrts[c]);
			rtArray.push_back(toAdd);
		}
		positionBuff = vd->addRenderTargetTexture(displayRes, "PositionBuffer", params.positionFormat);
		output = vd->addRenderTargetTexture(displayRes, "DeferredOutput", params.outputFormat);
	}

	DeferredRenderer::~DeferredRenderer()
	{
		IVideoDriver* vd = GLOBAL_VD;
		for(u8 c = 0; c < EDR_COUNT; ++c)
		{
			vd->removeTexture(mrts[c]);
		}
		vd->removeTexture(output);
		vd->removeTexture(positionBuff);
	}

	void DeferredRenderer::Render(ISceneManager* __restrict sm)
	{
		bool res = GLOBAL_VD->setRenderTarget(rtArray);
		if(sm == nullptr)
		{
			GLOBAL_SM->drawAll();
		}
		else
		{
			sm->drawAll();
		}
	}
} //end namespace Grahpics
