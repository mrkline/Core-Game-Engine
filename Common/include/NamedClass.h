#pragma once
#include <irrlicht.h>

namespace Core
{
	//A useful base class that gives its children a name and ID value
	class NamedClass
	{
	public:
		NamedClass(irr::s32 cid = -1, const irr::core::stringc& cName = irr::core::stringc())
			: id(cid), name(cName)
		{}

		virtual ~NamedClass() {}

		void SetID(irr::s32 newID) { id = newID; }
		void SetName(const irr::core::stringc& newName) { name = newName; }
		void SetName(const irr::c8* newName) { name = newName; }

		irr::s32 GetID() const { return id; }
		const irr::core::stringc& GetName() const { return name; }
		irr::core::stringc& GetName() { return name; }

	protected:
		irr::s32 id;
		irr::core::stringc name;
	};
} //end namespace Core