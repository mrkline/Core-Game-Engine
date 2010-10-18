#include "HDRPostProcessingManager.h"
#include "IPostProcess.h"
#include "ErrorHandling.h"
#include "DSBloomGenerator.h"
#include "GlobalContext.h"

namespace Graphics
{
	HDRPostProcessingManager::HDRPostProcessingManager(ECOLOR_FORMAT bufferType)
		: PostProcessingManager(bufferType)
	{
		bloomGen = new DSBloomGenerator(GlobalContext::DeviceContext.GetVideoDriver()->getScreenSize());
		quad.SetTexture(bloomGen->GetOutput(), 1);
	}

	HDRPostProcessingManager::~HDRPostProcessingManager()
	{
		delete bloomGen;
	}

	void HDRPostProcessingManager::RenderScene()
	{
		vd->setRenderTarget(rts[outputRT]);
		sm->drawAll();

		//calculate bloom
		bloomGen->Render(rts[outputRT]);

		//Apply post processes
		u32 sz = postProcesses.size();

		if(sz > 0)
		{
			//Flip RTs (the target becomes the source and vice versa)
			outputRT = !outputRT;
		}

		for(u32 c = 0; c < sz; c++)
		{
			postProcesses[c]->Render(rts[!outputRT], rts[outputRT]);

			//Flip RTs (the target becomes the source and vice versa)
			outputRT = !outputRT;
		}

		//Render the final image to the frame buffer
		outputRT = !outputRT;
		
		quad.SetTexture(rts[outputRT]);
		quad.Render();
	}
} //end namespace Graphics
