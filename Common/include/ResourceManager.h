#pragma once

#include <irrlicht.h>

namespace Core
{
	//Since resources could be loaded in different ways (i.e. resource file), provide a resource manager
	namespace Resources
	{
		class ResourceManager
		{
		public:
			static irr::io::IReadFile* OpenResource(const irr::io::path& name);
		};
	} //end namespace Resources
} //end namespace Core