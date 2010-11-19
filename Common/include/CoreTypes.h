#pragma once
#include <irrlicht.h>

//This is mostly to avoid always including the irr namespace on type declarations
namespace Core
{
	typedef irr::u32 u32;
	typedef irr::s32 s32;
	typedef irr::s16 s16;
	typedef irr::u16 u16;
	typedef irr::f32 f32;
	typedef irr::f64 f64;
	typedef irr::c8  c8;
	typedef irr::u8  u8;

	typedef irr::IReferenceCounted ReferenceCounted;
	typedef irr::core::stringc stringc;
	typedef irr::core::stringw stringw;
	typedef irr::core::vector3df Vector3;

} //end namespace Core