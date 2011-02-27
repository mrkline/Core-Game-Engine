#pragma once

namespace Core
{
	class ComponentManager
	{
	public:
		virtual ~ComponentManager() {}

		virtual void Update(unsigned int gameTime) = 0;
	protected:

	};
} //end namespace Core