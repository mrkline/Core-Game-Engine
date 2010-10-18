#pragma once
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;

#include "GlobalContext.h"

namespace Graphics
{
	class DeferredRenderer
	{
	public:
		struct CreationParams
		{
			//Format of the MRT textures
			ECOLOR_FORMAT MRTFormat;
			//Format of the reconstructed position buffer
			ECOLOR_FORMAT positionFormat;
			//Format of the final output
			ECOLOR_FORMAT outputFormat;

			CreationParams()
			{
				MRTFormat = video::ECF_A16B16G16R16F;
				positionFormat = video::ECF_A32B32G32R32F;
				outputFormat = video::ECF_A16B16G16R16F;
			}
		};

		//TODO: create more useful names when the RTs become more useful
		enum E_DEFERRED_RTS
		{
			EDR_MRT_0,
			EDR_MRT_1,
			EDR_MRT_2,
			EDR_MRT_3,
			EDR_POSITION,
			EDR_OUTPUT,
			EDR_COUNT
		};

		DeferredRenderer(const dimension2d<u32>& displayRes, const CreationParams& params = CreationParams());
		virtual ~DeferredRenderer();

		//If sm is null, the default scene manager is used
		void Render(ISceneManager* __restrict sm = nullptr);

		//If we have 
		ITexture* GetRT(E_DEFERRED_RTS which)
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

	protected:
		static const u32 kMRTCount = 4;

		ITexture* mrts[kMRTCount];
		ITexture* output;
		ITexture* positionBuff;
		array<IRenderTarget> rtArray;
	};
} //end namespace Graphics
