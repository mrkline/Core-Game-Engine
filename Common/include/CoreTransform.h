#pragma once
#include <ErrorHandling.h>
#include <Vector3.h>

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
		Transform(const float* matrixArray);
		//Default Constructor
		Transform(ConstructType type = E_MT_IDENTITY);
		
		//Set other matrices:

		Error::ECode GetInverse(Transform& out) const;
		void GetTransposed(Transform& out) const;
		void Interpolate(const Transform& other, float t, Transform& out) const;

		//byVal versions of some of these functions
		Transform GetTransposed() const
		{
			Transform ret;
			GetTransposed(ret);
			return ret;
		}
		Transform Interpolate(const Transform& other, float t)
		{
			Transform ret;
			Interpolate(other, t, ret);
			return ret;
		}

		//Accessors:

		bool Equals(const Transform& other, float roundingTolerance = Math::kFloatRoundError) const;
		bool IsIdentity() const;
		bool IsOrthogonal() const;
		void GetRotationDegrees(Vector3& vecOut) const; 
		void GetRotationRadians(Vector3& vecOut) const;
		//Note that this always returns the absolute values of the scale components
		void GetScale(Vector3& vecOut) const;
		void GetTranslation(Vector3& vecOut) const;
		float* GetArray() { return matrix; }
		const float* GetArray() const { return matrix; }

		//byVal versions of some accessors
		Vector3 GetRotationDegrees() const
		{
			Vector3 ret;
			GetRotationDegrees(ret);
			return ret;
		}
		Vector3 GetRotationRadians() const
		{
			Vector3 ret;
			GetRotationRadians(ret);
			return ret;
		}
		Vector3 GetScale() const
		{
			Vector3 ret;
			GetScale(ret);
			return ret;
		}
		Vector3 GetTranslation() const
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
		void SetFromArray(const float* transformMatrix);

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
		Transform operator*(const float scalar) const;
		Transform& operator*=(const Transform& other);
		Transform& operator*=(float scalar);
		Transform operator+(const Transform& other) const;
		Transform& operator+=(const Transform& other);
		Transform operator-(const Transform& other) const;
		Transform& operator-=(const Transform& other);
		Transform& operator=(const Transform& other);
		
		//Inline accessors
		//force inlined for speed, as they are used frequently by many internal functions.

		float& operator[](unsigned int index) { return matrix[index]; }
		float operator[](unsigned int index) const { return matrix[index]; }
		float& operator()(unsigned int row, unsigned int col) { return matrix[row * 4 + col]; }
		float operator()(unsigned int row, unsigned int col) const { return matrix[row * 4 + col]; }

	protected:
		//The array of floats for the matrix
		float matrix[16];
	};

} //end namespace Core