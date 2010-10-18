#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace io;
using namespace video;
using namespace scene;

#include "ScreenQuad.h"

namespace Graphics
{
	class IPostProcess;

	class PostProcessingManager
	{
	public:

		PostProcessingManager(ECOLOR_FORMAT bufferType = video::ECF_A32B32G32R32F);

		//Render the scene to the frame buffer with all post processes applied
		virtual void Render(ITexture* __restrict source);

		//Render to an output texture
		virtual void Render(ITexture* __restrict source, ITexture* __restrict output);

		//Add a post process to the end of the post processing routine
		void AddPostProcess(IPostProcess* newPP);

		//Add a post process in the middle of the routine (after a given index)
		void AddPostProcess(IPostProcess* newPP, u32 index);

		//Remove a post process.  Optional param: remove all instances
		//(default behavior is to just remove the first one found)
		void RemovePostProcess(IPostProcess* toRemove, bool removeAllInstances = false);

	protected:
		IVideoDriver* vd;
		ISceneManager* sm;

		ScreenQuad quad;

		//List of post-process effects, in order
		core::array<IPostProcess*> postProcesses;

		//Tracks which rt to use
		bool outputRT;

		//Primary render targets to flip between during post processing
		ITexture* rts[2];
	};
} //end namespace Graphics
