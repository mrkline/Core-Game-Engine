#pragma once

#include "GameComponent.h"

namespace Core
{
	//! A component that executes game logic for its parent object.
	class LogicComponent : public GameComponent
	{
	public:
		virtual ~LogicComponent() {}

		//! \see GameComponent::GetComponentType
		EType GetComponentType() { return GameComponent::E_GCT_LOGIC; }

		// The following functions are used by the collision dispatching system
		// to notify GameObjects that they're hitting each other

		//! Called when a collision is first detected
		virtual void OnCollisionStart(GameObject* otherObj) {}
		//! Called when a collision is continuing after the initial detection tick
		virtual void OnCollisionStay(float duration, GameObject* otherObj) {}
		//! Called when a collision is no longer detected in the current physics tick
		virtual void OnCollisionEnd(float duration, GameObject* otherObj) {}
	};

} // end namespace Core