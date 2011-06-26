#include "Scene.h"

#include "GameObject.h"

namespace Core
{
	Scene::Scene(PhysicsManager* physicsMan)
		: physMan(physicsMan)
	{
		rootObject = std::auto_ptr<GameObject>(new GameObject(nullptr, Transform(), 0, "root"));
	}
} // end namespace Core