#pragma once
#include <string>

namespace Core
{
	//A useful base class that gives its children a name and ID value
	class NamedClass
	{
	public:
		NamedClass(int cid = -1, const std::string& cName = std::string())
			: id(cid), name(cName)
		{}

		virtual ~NamedClass() {}

		void SetID(int newID) { id = newID; }
		void SetName(const std::string& newName) { name = newName; }
		void SetName(const char* newName) { name = newName; }

		int GetID() const { return id; }
		const std::string& GetName() const { return name; }
		std::string& GetName() { return name; }

	protected:
		int id;
		std::string name;
	};
} //end namespace Core