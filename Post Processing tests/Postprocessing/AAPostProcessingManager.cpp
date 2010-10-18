#include "AAPostProcessingManager.h"
#include "IPostProcess.h"
#include "ErrorHandling.h"

namespace Graphics
{

	AAPostProcessingManager::AAPostProcessingManager(ECOLOR_FORMAT bufferType)
		: PostProcessingManager(bufferType)
	{
		frameToRT = vd->addRenderTargetTexture(rts[0]->getSize(), "frameToRTIntermediate",
			vd->getColorFormat());

		if(frameToRT == nullptr)
			throw new Exception("Intermediate RT could not be created", __FUNCTION__);
	}

	void AAPostProcessingManager::RenderScene()
	{

		vd->setRenderTarget(video::ERT_FRAME_BUFFER);

	
		//Draw the scene onto the frame buffer, then copy it to our first source
		sm->drawAll();

		vd->copyBackBufferTo(frameToRT);

		quad.SetTexture(frameToRT);
		quad.Render(rts[outputRT]);

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

			//quad.SetTexture(rts[outputRT]);
			//quad.Render();
			//Sleep(300);
		}

		//static bool saved = false;
		//if(!saved)
		//{
		//	IImage* toSave = vd->createImage(rts[outputRT], vector2di(0, 0), rts[outputRT]->getSize());
		//	vd->writeImageToFile(toSave, "C:\\Test1.png");
		//	toSave->drop();
		//	toSave = vd->createImage(rts[!outputRT], vector2di(0, 0), rts[!outputRT]->getSize());
		//	vd->writeImageToFile(toSave, "C:\\Test2.png");
		//	toSave->drop();
		//	saved = true;
		//}


		//Render the final image to the frame buffer
		outputRT = !outputRT;
		
		quad.SetTexture(rts[outputRT]);
		quad.Render();
	}
} //End namespace graphics