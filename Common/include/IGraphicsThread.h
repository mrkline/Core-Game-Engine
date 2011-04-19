#pragma once

namespace Core
{
	/*!
	\brief An interface for the OGRE graphics thread. This allows for multiple threading implementations,
			making it easier to port to different OSes.
	*/
	class IGraphicsThread
	{
	public:
		virtual ~IGraphicsThread() {};
	};
}; // end namespace Core