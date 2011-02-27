#pragma once
#include <GameComponent.h>

namespace Core
{
	class LogicComponent : public GameComponent
	{
	public:
		virtual ~LogicComponent() {}

		EType GetComponentType() { return GameComponent::E_GCT_LOGIC; }

		//Used by collision dispatching system to notify GameObjects that they're hitting each other

		//Called when a collision is first detected
		virtual void OnCollisionStart(GameObject* otherObj) {}
		//Called when a collision is continuing after the initial detection tick
		virtual void OnCollisionStay(float duration, GameObject* otherObj) {}
		//Called when a collision is no longer detected in the current physics tick
		virtual void OnCollisionEnd(float duration, GameObject* otherObj) {}
	};

} //end namespace Core