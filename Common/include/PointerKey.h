#pragma once

namespace Core
{
	/*!
	\brief A key for associative collections that uses two pointers to compose the key

	This was designed with the collision system in mind, and is used to hold pointers
	to two colliding objects.
	*/
	template <class T>
	class PointerKey
	{
	public:
		//! Construct the key using two pointers
		PointerKey(T* p1, T* p2)
		{
			if(p1 > p2)
			{
				higher = p1;
				lower = p2;
			}
			else
			{
				lower = p1;
				higher = p2;
			}
		}


		bool operator>(const PointerKey& other) const
		{
			if(higher > other.higher)
				return true;
			else if(higher == other.higher && lower > other.lower)
				return true;

			return false;
		}
		bool operator<(const PointerKey& other) const
		{
			if(higher < other.higher)
				return true;
			else if(higher == other.higher && lower < other.lower)
				return true;

			return false;
		}
		bool operator==(const PointerKey& other) const
		{
			return higher == other.higher && lower == other.lower;
		}
		bool operator>=(const PointerKey& other) const { return *this == other || *this > other; }
		bool operator<=(const PointerKey& other) const { return *this == other || *this < other; }

		T* GetHigher() const { return higher; } //!< Gets the higher pointer
		T* GetLower() const { return lower; } //!< Gets the lower pointer

	private:
		T* higher; //!< Pointer in the key with a higher value
		T* lower;  //!< Pointer in the key with a lower value
	};
} // end namespace Core