#include <ResourceManager.h>
#include <GlobalContext.h>

using namespace GlobalContext;

namespace Resources
{
	IReadFile* ResourceManager::OpenResource(const path& name)
	{
		//Currently, we're just reading files
		return DeviceContext.GetFileSystem()->createAndOpenFile(name);
	}
} //end namespace Resources