#include <IterativeManifoldCollisionDetector.h>
#include <PhysicsManager.h>

namespace Core
{
	IterativeManifoldCollisionDetector::IterativeManifoldCollisionDetector()
	{
		co0 = new btCollisionObject();
		co1 = new btCollisionObject();
	}

	IterativeManifoldCollisionDetector::~IterativeManifoldCollisionDetector()
	{
		delete co0;
		delete co1;
	}

	void IterativeManifoldCollisionDetector::GetCollisionPairs(unsigned int substep)
	{
		btDispatcher* dis = collWorld->getDispatcher();
		btPersistentManifold* man;
		int sz = dis->getNumManifolds();
		for(int c = 0; c < sz; ++c)
		{
			man = dis->getManifoldByIndexInternal(c);
			btCollisionShape* curr0 = static_cast<btCollisionObject*>(man->getBody0())->getCollisionShape();
			btCollisionShape* curr1 = static_cast<btCollisionObject*>(man->getBody1())->getCollisionShape();

			//If neither is compound, we're all done here
			if(!curr0->isCompound() && !curr1->isCompound())
			{
				physMan->AddCollisionPair(static_cast<GameObject*>(curr0->getUserPointer()), 
						static_cast<GameObject*>(curr1->getUserPointer()), substep);
			}
			
			//TODO: Carry on here
			//Iterate through each object to get the specific collision shape colliding

			while(curr0->isCompound())
			{
			}

			while(curr1->isCompound())
			{
			}
			
			physMan->AddCollisionPair(static_cast<GameObject*>(curr0->getUserPointer()), 
					static_cast<GameObject*>(curr1->getUserPointer()), substep);
		}
	}

	float IterativeManifoldCollisionDetector::addSingleResult(	btManifoldPoint & 	cp,
																const btCollisionObject * 	colObj0,
																int 	partId0,
																int 	index0,
																const btCollisionObject * 	colObj1,
																int 	partId1,
																int 	index1)
	{
		lastResult.colObj0 = colObj0;
		lastResult.partId0 = partId0;
		lastResult.colObj1 = colObj1;
		lastResult.partId1 = partId1;
		lastResult.index1 = index1;
		return 0.0f;
	}
} //end namespace Core