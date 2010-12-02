#pragma once

namespace irr
{
	struct SIrrlichtCreationParameters;
}

namespace Core
{
	//Graphics thread is not reference counted since only the CoreBase should handle it
	class GraphicsThread
	{
	public:
		GraphicsThread(const irr::SIrrlichtCreationParameters& cp);
		virtual ~GraphicsThread();
	};
};