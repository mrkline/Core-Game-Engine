#include "Scene.h"

#include "GameObject.h"

namespace Core
{
	Scene::Scene(PhysicsManager* physicsMan)
		: rootObject(new GameObject(nullptr, (Scene*)1)), physMan(physicsMan)
	{
		rootObject->scn = this;
	}
} // end namespace Core