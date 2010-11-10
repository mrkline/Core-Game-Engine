#pragma once

namespace Core
{
	template <class T>
	class PointerKey
	{
	public:
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

		bool operator>(const PointerKey& other)
		{
			if(higher > other.higher)
			{
				return true;
			}
			else if(higher == other.higher && lower > other.lower)
			{
				return true;
			}
			return false;
		}

		bool operator<(const PointerKey& other)
		{
			if(higher < other.higher)
			{
				return true;
			}
			else if(higher == other.higher && lower < other.lower)
			{
				return true;
			}
			return false;
		}

		bool operator==(const PointerKey& other)
		{
			return higher == other.higher && lower == other.lower;
		}

		bool operator>=(const PointerKey& other) { return *this == other || *this > other; }
		bool operator<=(const PointerKey& other) { return *this == other || *this < other; }

		T* GetHigher() { return higher; }
		T* GetLower() { return lower; }

	private:
		T* higher;
		T* lower;
	};
} //end namespace Core