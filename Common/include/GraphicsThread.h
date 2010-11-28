#pragma once

namespace irr
{
	class SIrrlichtCreationParameters;
}

namespace Core
{
	class GraphicsThread
	{
		GraphicsThread(const irr::SIrrlichtCreationParameters& cp);
		virtual ~GraphicsThread();
	};
};