#pragma once
#include <irrlicht.h>

namespace Core
{
	class ComponentManager
	{
	public:
		virtual ~ComponentManager() {}

		virtual void Update(irr::u32 gameTime) = 0;
	protected:

	};
} //end namespace Core