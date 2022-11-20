#pragma once

//===========================================================================//
//                                                                           //
//            CSpaceEngine Procedural Planet Generator Header                //
//                                                                           //
//===========================================================================//

#ifndef _CSE_PROC_PLANET_GEN
#define _CSE_PROC_PLANET_GEN

#include "../Core/CSECore.h"
#include "../gl/gltypes.h"
#include "../date/Date_Time.h"
#include "../random/random.h"

#include "../Core/ConstLists.h"

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

/// <summary>
/// Making orbit for companion
/// </summary>
/// <param name="_Primary">Primary Object</param>
/// <param name="_Companion">Secondary Object</param>
/// <param name="_DefaultData">Default data</param>
void
__CRTDECL MakeOrbit
(
	Object& _Primary, Object& _Companion,
	OrbitParam _DefaultData = 
	{
		.RefPlane = "Extrasolar",
		.Epoch = 2451545
	}
);

/// <summary>
/// Making binary orbit for two objects.
/// </summary>
/// <param name="_Primary">Primary Object</param>
/// <param name="_Companion">Secondary Object</param>
/// <param name="_DefaultData">Default data</param>
/// <returns>The barycenter of system</returns>
_Check_return_ Object
__CRTDECL MakeBinary
(
	Object& _Primary, Object& _Companion, 
	OrbitParam _DefaultData =
	{
		.RefPlane = "Extrasolar",
		.Epoch = 2451545
	}
);

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif