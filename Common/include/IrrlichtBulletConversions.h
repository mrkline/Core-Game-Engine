#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>

//Converts Irrlicht data structures to Bullet ones and vice versa

namespace Core
{
	namespace Convert
	{
		//Should we be using __forceinline or just inline?

		inline void VectorBTI(const btVector3& bVec, irr::core::vector3df& iVec)
		{
			iVec.X = bVec.getX();
			iVec.Y = bVec.getY();
			iVec.Z = bVec.getZ();
		}

		inline void VectorITB(const irr::core::vector3df& iVec, btVector3& bVec)
		{
			bVec.setX(iVec.X);
			bVec.setY(iVec.Y);
			bVec.setZ(iVec.Z);
		}

		inline void QuaternionBTI(const btQuaternion& bQuat, irr::core::quaternion& iQuat)
		{
			iQuat.X = bQuat.getX();
			iQuat.Y = bQuat.getY();
			iQuat.Z = bQuat.getZ();
			iQuat.W = bQuat.getW();
		}

		inline void QuaternionITB(const irr::core::quaternion& iQuat, btQuaternion& bQuat)
		{
			bQuat.setX(iQuat.X);
			bQuat.setY(iQuat.Y);
			bQuat.setZ(iQuat.Z);
			bQuat.setW(iQuat.W);
		}

		inline void TransformBTI(const btTransform& bTransform, irr::core::matrix4& iTransform)
		{
			bTransform.getOpenGLMatrix(iTransform.pointer());
		}

		inline void TransformITB(const irr::core::matrix4& iTransform, btTransform& bTransform)
		{
			bTransform.setFromOpenGLMatrix(iTransform.pointer());
		}
	} //end namespace Convert
} //end namespace Core