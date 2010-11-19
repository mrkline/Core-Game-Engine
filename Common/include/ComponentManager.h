#pragma once
#include <CoreTypes.h>

namespace Core
{
	class ComponentManager : public ReferenceCounted
	{
	public:
		virtual ~ComponentManager() {}

		virtual void Update(u32 gameTime) = 0;
	protected:

	};
} //end namespace Core