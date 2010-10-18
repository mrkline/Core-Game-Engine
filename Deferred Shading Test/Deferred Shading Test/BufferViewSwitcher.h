#pragma once

#include <irrlicht.h>
using namespace irr;

#include "DeferredRenderer.h"
using namespace Graphics;

//Quick and dirty class; just for test purposes
class BufferViewSwitcher : public IEventReceiver
{
public:
	BufferViewSwitcher() : activeRT(DeferredRenderer::EDR_MRT_0)
	{
	}

	bool OnEvent(const SEvent& e)
	{
		if(e.EventType == irr::EET_KEY_INPUT_EVENT
			&& e.KeyInput.Key >= irr::KEY_KEY_0
			&& e.KeyInput.Key <= irr::KEY_KEY_9)
		{
			activeRT = static_cast<DeferredRenderer::E_DEFERRED_RTS>(e.KeyInput.Key - irr::KEY_KEY_0 - 1);
		}

		return false;
	}

	DeferredRenderer::E_DEFERRED_RTS GetActiveRT() { return activeRT; }

private:
	DeferredRenderer::E_DEFERRED_RTS activeRT;
};