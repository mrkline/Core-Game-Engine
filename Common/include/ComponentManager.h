#pragma once

namespace Core
{
	//!Provides the basis for component managers from which they can be derived
	class ComponentManager
	{
	public:
		virtual ~ComponentManager() {}

		/*!
		\brief Updates all components owned by the manager.
		\param gameTime The current game time in milliseconds.
		*/
		virtual void Update(unsigned int gameTime) = 0;
	protected:

	};
} //end namespace Core