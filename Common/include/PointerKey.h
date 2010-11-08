namespace Core
{
	class PointerKey
	{
	public:
		PointerKey(void* p1, void* p2)
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

		void* GetHigher() { return higher; }
		void* GetLower() { return lower; }

	private:
		void* higher;
		void* lower;
	};
} //end namespace Core