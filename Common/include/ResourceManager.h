#pragma once

#include <irrlicht.h>
using namespace irr;
using namespace io;

//Since resources could be loaded in different ways (i.e. resource file), provide a resource manager
namespace Resources
{
	class ResourceManager
	{
	public:
		static IReadFile* OpenResource(const path& name);
	};
} //end namespace Resources
