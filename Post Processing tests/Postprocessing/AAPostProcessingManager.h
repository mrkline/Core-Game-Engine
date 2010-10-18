#include "PostProcessingManager.h"

struct IDirect3DDevice9;

namespace Graphics
{
	class AAPostProcessingManager : public PostProcessingManager
	{
	public:
		AAPostProcessingManager(ECOLOR_FORMAT bufferType = video::ECF_A32B32G32R32F);
		virtual void RenderScene();

	protected:
		//RT the same format as the frame buffer for copying
		ITexture* frameToRT;
	};
} //end namespace graphics
