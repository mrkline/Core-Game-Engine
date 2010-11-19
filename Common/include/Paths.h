#pragma once

#include <irrlicht.h>

//Used to store directory and file paths

namespace Core
{
	namespace Paths
	{
		//Post processes
		namespace PostProcesses
		{
			const irr::io::path PostProcessDir("PostProcesses\\");
			const irr::io::path Invert(PostProcessDir + "Invert.fx");
			const irr::io::path DownSampleX4(PostProcessDir + "DownSampleX4.fx");
			const irr::io::path BrightPass(PostProcessDir + "BrightPass.fx");
			const irr::io::path Bloom(PostProcessDir + "Bloom.fx");
			const irr::io::path GaussianBlur(PostProcessDir + "GaussianBlur.fx");
			const irr::io::path Luminance(PostProcessDir + "Luminance.fx");
			const irr::io::path HDR(PostProcessDir + "HDR.fx");
			const irr::io::path TextureAdder(PostProcessDir + "TextureAdder.fx");
		}

		namespace DeferredMatTypes
		{
			const irr::io::path DeferredMatTypesDir("DeferredMatTypes\\");
			const irr::io::path UnlitColorMaterial(DeferredMatTypesDir + "UnlitColorMaterial.fx");
		}

		namespace DeferredHelperTypes
		{
			const irr::io::path DeferredHelperTypesDir("DeferredHelperTypes\\");
			const irr::io::path PositionCreator(DeferredHelperTypesDir + "PositionCreator.fx");
		}
	} //end namespace Graphics
} //end namespace Core