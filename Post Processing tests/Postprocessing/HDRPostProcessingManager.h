#ifndef HDR_POST_PROCESSING_MANAGER_H_
#define HDR_POST_PROCESSING_MANAGER_H_

#include "AAPostProcessingManager.h"


namespace Graphics
{
	class DSBloomGenerator;

	class HDRPostProcessingManager : public PostProcessingManager
	{
	public:
		HDRPostProcessingManager(ECOLOR_FORMAT bufferType = video::ECF_A32B32G32R32F);
		virtual ~HDRPostProcessingManager();
		virtual void RenderScene();

		DSBloomGenerator* GetBloomGenerator() { return bloomGen; }

	protected:
		DSBloomGenerator* bloomGen;
	};
} //end namespace Graphics

#endif