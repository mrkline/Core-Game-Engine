#include "DSBloomGenerator.h"
#include "GlobalContext.h"

namespace Graphics
{
	DSBloomGenerator::DSBloomGenerator(const dimension2d<u32>& sourceSize, ECOLOR_FORMAT bufferType)
	{
		brightPass = new BrightPassPostProcess;
		downSample = new DownSampleX4PostProcess;
		blurH = new GaussianBlurHPostProcess;
		blurV = new GaussianBlurVPostProcess;

		IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();
		brightPassRT = vd->addRenderTargetTexture(sourceSize / 2,
			"bloomBrightPassRT", bufferType);

		dimension2d<u32> bloomTextureSize = sourceSize / 8;

		blurRT1 = vd->addRenderTargetTexture(bloomTextureSize, 
			"bloomBlurRT1", bufferType);
		blurRT2 = vd->addRenderTargetTexture(bloomTextureSize,
			"bloomBlurRT2", bufferType);

		if(brightPassRT == nullptr
			|| blurRT1 == nullptr
			|| blurRT2 == nullptr)
			throw new Exception("Render targets for bloom could not be created.", __FUNCTION__);
	}

	DSBloomGenerator::~DSBloomGenerator()
	{
		brightPass->drop();
		downSample->drop();
		blurH->drop();
		blurV->drop();

		IVideoDriver* vd = GlobalContext::DeviceContext.GetVideoDriver();
		vd->removeTexture(brightPassRT);
		vd->removeTexture(blurRT1);
		vd->removeTexture(blurRT2);
	}

	void DSBloomGenerator::Render(ITexture* __restrict source)
	{
		brightPass->Render(source, brightPassRT);
		downSample->Render(brightPassRT, blurRT1);
		blurH->Render(blurRT1, blurRT2);
		blurV->Render(blurRT2, blurRT1);
	}
} //end namespace Graphics
