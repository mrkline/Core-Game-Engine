#pragma once

#include <CollisionDetector.h>
#include <ComponentManager.h>
#include <CoreBase.h>
#include <CoreMath.h>
#include <CoreTransform.h>
#include <ErrorHandling.h>
#include <GameComponent.h>
#include <GameMotionState.h>
#include <GameObject.h>
#include <IterativeManifoldCollisionDetector.h>
#include <LogicComponent.h>
#include <MapPhysicsManager.h>
#include <NamedClass.h>
#include <PhysicsComponent.h>
#include <PhysicsManager.h>
#include <PointerKey.h>
#include <Scene.h>

// Be sure to define this while building the engine
#ifndef ENGINE_BUILD
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletFileLoader.lib")
#pragma comment(lib, "BulletMultiThreaded.lib")
#pragma comment(lib, "BulletSoftBody.lib")
#pragma comment(lib, "BulletWorldImporter.lib")
#pragma comment(lib, "ConvexDecomposition.lib")
#pragma comment(lib, "GIMPACTUtils.lib")
#pragma comment(lib, "GLUI.lib")
#pragma comment(lib, "LinearMath.lib")
#pragma comment(lib, "MiniCL.lib")
#endif
