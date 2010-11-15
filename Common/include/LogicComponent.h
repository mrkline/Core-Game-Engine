#pragma once
#include <GameComponent.h>
#include <irrlicht.h>

namespace Core
{
	class LogicComponent : public GameComponent
	{
	public:
		virtual ~LogicComponent() {}

		EType GetComponentType() { return GameComponent::E_GCT_LOGIC; }

		//Used by collision dispatching system to notify GameObjects that they're hitting each other

		virtual void OnCollisionStart(GameObject* otherObj) {}
		virtual void OnCollisionStay(float duration, GameObject* otherObj) {}
		virtual void OnCollisionEnd(float duration, GameObject* otherObj) {}
	};

} //end namespace Core