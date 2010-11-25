#pragma once
#include <irrlicht.h>
#include <ErrorHandling.h>
#include <CoreTypes.h>

namespace Core
{
	//An basic transform class, inspired by and borrowed heavily from Irrlicht's transform matrix. Uses floats.
	class Transform : public Error::CanErr
	{
	public:
		//Type of matrix to construct
		enum ConstructType
		{
			E_MT_EMPTY,
			E_MT_IDENTITY
		};

		//Copy constructor
		Transform(const Transform& other);
		//Creates tranform from an array of 16 floats
		Transform(const f32* matrixArray);
		//Default Constructor
		Transform(ConstructType type = E_MT_IDENTITY);
		
		//Set other matrices:

		Error::ECode GetInverse(Transform& out) const;
		void GetTransposed(Transform& out) const;
		void Interpolate(const Transform& other, f32 t, Transform& out) const;

		//byVal versions of some of these functions
		__forceinline Transform GetTransposed() const
		{
			Transform ret;
			GetTransposed(ret);
			return ret;
		}
		__forceinline Transform Interpolate(const Transform& other, f32 t)
		{
			Transform ret;
			Interpolate(other, t, ret);
			return ret;
		}

		//Accessors:

		bool Equals(const Transform& other, f32 roundingTolerance = irr::core::ROUNDING_ERROR_f32) const;
		bool IsIdentity() const;
		bool IsOrthogonal() const;
		void GetRotationDegrees(Vector3& vecOut) const; 
		void GetRotationRadians(Vector3& vecOut) const;
		//Note that this always returns the absolute values of the scale components
		void GetScale(Vector3& vecOut) const;
		void GetTranslation(Vector3& vecOut) const;
		f32* GetArray() { return matrix; }
		const f32* GetArray() const { return matrix; }

		//byVal versions of some accessors
		__forceinline Vector3 GetRotationDegrees() const
		{
			Vector3 ret;
			GetRotationDegrees(ret);
			return ret;
		}
		__forceinline Vector3 GetRotationRadians() const
		{
			Vector3 ret;
			GetRotationRadians(ret);
			return ret;
		}
		__forceinline Vector3 GetScale() const
		{
			Vector3 ret;
			GetScale(ret);
			return ret;
		}
		__forceinline Vector3 GetTranslation() const
		{
			Vector3 ret;
			GetTranslation(ret);
			return ret;
		}

		//Mutators:

		void SetToIdentity();
		Error::ECode SetToInverse();
		void SetAsProductOf(const Transform& t1, const Transform& t2);
		void SetInverseRotationDegrees(const Vector3& rotation);
		void SetInverseRotationRadians(const Vector3& rotation);
		void SetInverseTranslation(const Vector3& translation);
		void SetRotationDegrees(const Vector3& rotation);
		void SetRotationRadians(const Vector3& rotation);
		void SetScale(const Vector3& rotation);
		void SetFromArray(const f32* transformMatrix);

		//Change point:
		void InverseRotatePoint(Vector3& pointOut) const;
		void InverseTranslatePoint(Vector3& pointOut) const;
		void RotatePoint(Vector3& pointOut) const;
		void TranslatePoint(Vector3& pointOut) const;
		void ScalePoint(Vector3& pointOut) const;
		void TransformPoint(Vector3& pointOut) const;

		//Operators:

		bool operator==(const Transform& other) const { return Equals(other); }
		bool operator!=(const Transform& other) const { return !Equals(other); }
		Transform operator*(const Transform& m2) const;
		Transform operator*(const f32 scalar) const;
		Transform& operator*=(const Transform& other);
		Transform& operator*=(f32 scalar);
		Transform operator+(const Transform& other) const;
		Transform& operator+=(const Transform& other);
		Transform operator-(const Transform& other) const;
		Transform& operator-=(const Transform& other);
		Transform& operator=(const Transform& other);
		
		//Inline accessors
		//force inlined for speed, as they are used frequently by many internal functions.

		__forceinline f32& operator[](u32 index) { return matrix[index]; }
		__forceinline f32 operator[](u32 index) const { return matrix[index]; }
		__forceinline f32& operator()(u32 row, u32 col) { return matrix[row * 4 + col]; }
		__forceinline f32 operator()(u32 row, u32 col) const { return matrix[row * 4 + col]; }

	protected:
		//The array of floats for the matrix
		f32 matrix[16];
	};

} //end namespace Core