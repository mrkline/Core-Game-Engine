#include <ResourceManager.h>
#include <GlobalContext.h>

using namespace irr;
using namespace core;
using namespace io;

namespace Core
{
	namespace Resources
	{
		IReadFile* ResourceManager::OpenResource(const path& name)
		{
			//Currently, we're just reading files
			return GlobalContext::DeviceContext.GetFileSystem()->createAndOpenFile(name);
		}
	} //end namespace Resources
} //end namespace Core