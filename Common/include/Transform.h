#pragma once
#include <irrlicht.h>
#include <ErrorHandling.h>

namespace Core
{
	//An basic transform class, inspired by Irrlicht's transform. Uses floats
	class Transform : public Error::CanErr
	{
	public:
		//Type of matrix to construct
		enum ConstructType
		{
			E_MT_NOTHING, 	
			E_MT_COPY, 	
			E_MT_IDENTITY, 	
			E_MT_TRANSPOSED,
			E_MT_INVERSE, 	
			E_MT_INVERSE_TRANSPOSED 	
		};

		//Copy constructor
		Transform(const Transform& other, ConstructType type = E_MT_COPY);
		//Default 
		Transform(ConstructType type = E_MT_IDENTITY);
		
		//Set other matrices:

		Error::ECode GetInverse(const Transform& out) const;
		void GetTransposed(Transform& out) const;
		void Interpolate(const Transform& other, f32 t, Transform& out) const;
		void Mul(f32 scalar, Transform& out) const;
		void Mul(const Transform& other, Transform& out) const;
		void Add(const Transform& other, Transform& out) const;
		void Sub(const Transform& other, Transform& out) const;

		//Accessors:

		bool Equals(const Transform& other, f32 roundingTolerance = irr::core::ROUNDING_ERROR_f32) const;
		bool IsIdentity() const;
		bool IsOrthogonal() const;
		void GetRotationDegrees(Vector3& vecOut) const; 
		void GetRotationRads(Vector3& vecOut) const;
		void GetScale(Vector3& vecOut) const;
		void GetTranslation(Vector3& vecOut) const;
		float* GetArray() { return matrix; }
		const float* GetArray() const { return matrix; }


		//Mutators:

		void SetToIdentity();
		Error::ECode SetToInverse();
		void SetAsProductOf(const Transform& t1, const Transform& t2);
		void SetInverseRotationDegrees(const Vector3& invRotation);
		void SetInverseRotationRads(const Vector3& invRotation);
		void SetInverseTranslation(const Vector3& invTranslation);
		void SetRotationDegrees(const Vector3& rotation);
		void SetRotationRadians(const Vector3& rotation);
		void SetScale(const Vector3& rotation);
		void SetFromArray(const float* matrix);

		//Change point:
		void InverseRotatePoint(Vector3& pointOut) const;
		void InverseTranslatePoint(Vector3& pointOut) const;
		void InverseScalePoint(Vector3& pointOut) const;
		void RotatePoint(Vector3& pointOut) const;
		void TranslatePoint(Vector3& pointOut) const;
		void ScalePoint(Vector3& pointOut) const;
		void TransformPoint(Vector3& pointOut) const;

		//Operators:

		bool operator==(const Transform& other) const { return Equals(other); }
		bool operator!=(const Transform& other) const { return !Equals(other); }
		Transform operator*(const Transform& other) const;
		Transform operator*(f32 scalar) const;
		Transform& operator*=(const Transform& other);
		Transform& operator*=(f32 scalar);
		Transform operator+(const Transform& other) const;
		Transform& operator+=(const Transform& other);
		Transform operator-(const Transform& other) const;
		Transform& operator-=(const Transform& other);
		Transform& operator=(const Transform& other);
		float& operator[](u32 index);
		float operator[](u32 index) const;
		float& operator()(u32 row, u32 col);
		float operator()(u32 row, u32 col) const;

	protected:
		//The array of floats for the matrix
		float matrix[16];
	};

} //end namespace Core