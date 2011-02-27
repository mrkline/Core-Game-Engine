#include <Core.h>

namespace Core
{
	extern "C" DLL_LINKAGE CoreBase* CORE_DLL_CALLCONV CreateCoreBase()
	{
		return new CoreBase();
	}
} //end namespace Core