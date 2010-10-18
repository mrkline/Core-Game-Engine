#include "PostProcessingManager.h"
#include "IPostProcess.h"
#include "GlobalContext.h"
#include "ErrorHandling.h"

namespace Graphics
{
	PostProcessingManager::PostProcessingManager(ECOLOR_FORMAT bufferType)
	{
		vd = GlobalContext::DeviceContext.GetVideoDriver();
		sm = GlobalContext::DeviceContext.GetSceneManager();

		//No need to allocate double (since this won't be being changed in real-time)
		postProcesses.setAllocStrategy(core::ALLOC_STRATEGY_SAFE);

		const ECOLOR_FORMAT format = vd->getColorFormat();

		const dimension2d<u32>& ss = vd->getScreenSize();

		outputRT = 0;

		rts[0] = vd->addRenderTargetTexture(ss, "PostProcessRT1", bufferType);
		rts[1] = vd->addRenderTargetTexture(ss, "PostProcessRT2", bufferType);

		if(rts[0] == nullptr || rts[1] == nullptr)
			throw new Exception("Render targets couldn't be created for post processing manager.", __FUNCTION__);
	
	}

	void PostProcessingManager::Render(ITexture* __restrict source)
	{
		//Apply post processes
		u32 sz = postProcesses.size();

		if(sz > 0)
		{
			postProcesses[0]->Render(source, rts[!outputRT]);

			for(u32 c = 1; c < sz; c++)
			{
				postProcesses[c]->Render(rts[!outputRT], rts[outputRT]);

				//Flip RTs (the target becomes the source and vice versa)
				outputRT = !outputRT;
			}
			
		outputRT = !outputRT;

		quad.SetTexture(rts[outputRT]);
	
		}
		else
		{
			quad.SetTexture(source);
		}

		quad.Render();
	}

	void PostProcessingManager::Render(ITexture* __restrict source, ITexture* __restrict output)
	{
		//Apply post processes
		u32 sz = postProcesses.size();

		if(sz > 0)
		{
			postProcesses[0]->Render(source, rts[!outputRT]);

			for(u32 c = 1; c < sz; c++)
			{
				postProcesses[c]->Render(rts[!outputRT], rts[outputRT]);

				//Flip RTs (the target becomes the source and vice versa)
				outputRT = !outputRT;
			}
			
		outputRT = !outputRT;

		quad.SetTexture(rts[outputRT]);
	
		}
		else
		{
			quad.SetTexture(source);
		}

		quad.Render(output);
	}

	void PostProcessingManager::AddPostProcess(IPostProcess* newPP)
	{
		postProcesses.push_back(newPP);
	}

	void PostProcessingManager::AddPostProcess(IPostProcess* newPP, u32 index)
	{
		postProcesses.insert(newPP, index);
	}

	void PostProcessingManager::RemovePostProcess(IPostProcess* toRemove, bool removeAllInstances)
	{
		//Do a sequential search.  We can't sort and do a binary search because
		//order is important
		for(u32 c = 0; c < postProcesses.size(); c++)
		{
			if(toRemove == postProcesses[c])
			{
				postProcesses.erase(c);

				if(removeAllInstances)
				{
					//Since everything has moved, we'll need to recheck this index
					c--;
					continue;
				}
				else
				{
					return;
				}
			}
		}
	}
} //end namespace Graphics
