#pragma once
#include <btBulletDynamicsCommon.h>
#include <CollisionDetector.h>

namespace Core
{
	/*!
	This collision detector works by examining each collision pair in the manifold.
	It then checks if the objects have compound shapes, and if so, iterates through the subshapes
	in order to determine which one is colliding with the other.
	*/
	class IterativeManifoldCollisionDetector : public CollisionDetector, private btCollisionWorld::ContactResultCallback
	{
	public:
		IterativeManifoldCollisionDetector();
		~IterativeManifoldCollisionDetector();

		void GetCollisionPairs(unsigned int substep);

	private:
		//! Used as temporary holders for collision shapes we iterate through.
		btCollisionObject *co0, *co1;

		//! Used as a temporary store for info returned by addSingleResult
		struct
		{
			const btCollisionObject * 	colObj0;
			int 	partId0;
			int 	index0;
			const btCollisionObject * 	colObj1;
			int 	partId1;
			int 	index1;
		} lastResult;

		float addSingleResult(	btManifoldPoint & 	cp,
								const btCollisionObject * 	colObj0,
								int 	partId0,
								int 	index0,
								const btCollisionObject * 	colObj1,
								int 	partId1,
								int 	index1);
	};
}