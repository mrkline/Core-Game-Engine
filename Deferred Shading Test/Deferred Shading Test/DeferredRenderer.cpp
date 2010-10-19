#include "DeferredRenderer.h"
#include "GlobalContext.h"
#include "DeferredMatTypeManager.h"

namespace Graphics
{
	DeferredRenderer::DeferredRenderer(const dimension2d<u32>& displayRes, const CreationParams& params)
	{
		IVideoDriver* vd = GLOBAL_VD;

		typeMan = new DeferredMatTypeManager;

		for(u8 c = 0; c < kMRTCount; ++c)
		{
			mrts[c] = GLOBAL_VD->addRenderTargetTexture(displayRes, "DeferredRT" + c, params.MRTFormat);
			IRenderTarget toAdd(mrts[c]);
			rtArray.push_back(toAdd);
		}
		positionBuff = vd->addRenderTargetTexture(displayRes, "PositionBuffer", params.positionFormat);
		output = vd->addRenderTargetTexture(displayRes, "DeferredOutput", params.outputFormat);
		quad.SetTexture(GetRT(EDR_MRT_1));
		quad.SetMaterialType(typeMan->getProtectedType(DeferredMatTypeManager::EPT_POSITION_CREATOR));
	}

	DeferredRenderer::~DeferredRenderer()
	{
		delete typeMan;

		IVideoDriver* vd = GLOBAL_VD;
		for(u8 c = 0; c < EDR_COUNT; ++c)
		{
			vd->removeTexture(mrts[c]);
		}
		vd->removeTexture(output);
		vd->removeTexture(positionBuff);
	}

	ITexture* DeferredRenderer::GetRT(E_DEFERRED_RTS which)
	{
		switch(which)
		{
		case EDR_MRT_0:
		case EDR_MRT_1:
		case EDR_MRT_2:
		case EDR_MRT_3:
			return mrts[which];

		case EDR_POSITION:
			return positionBuff;

		case EDR_OUTPUT:
		default:
			return output;
		}
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
		quad.Render(positionBuff);
	}
} //end namespace Grahpics
