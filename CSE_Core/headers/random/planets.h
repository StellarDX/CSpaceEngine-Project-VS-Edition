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
/// <param name="_RefSystem">Reference System</param>
/// <param name="_Epoch">Epoch, default is 2451545 (2000.01.01)</param>
/// <param name="_Separation">Distance between two objects. Generate randomly if zero or ignored</param>
/// <param name="_PositionAngle">Position angle at epoch</param>
void
__CRTDECL MakeOrbit(Object& _Primary, Object& _Companion,
	std::string _RefSystem = "Extrasolar", float64 _Epoch = 2451545, float64 _Separation = 0, float64 _PositionAngle = 0);

/// <summary>
/// Making binary orbit for two objects.
/// </summary>
/// <param name="_Primary">Primary Object</param>
/// <param name="_Companion">Secondary Object</param>
/// <param name="_RefSystem">Reference System</param>
/// <param name="_Epoch">Epoch, default is 2451545 (2000.01.01)</param>
/// <param name="_Separation">Distance between two objects. Generate randomly if zero or ignored</param>
/// <param name="_PositionAngle">Position angle at epoch</param>
/// <returns></returns>
_Check_return_ Object
__CRTDECL MakeBinary(Object& _Primary, Object& _Companion, 
	std::string _RefSystem = "Extrasolar", float64 _Epoch = 2451545, float64 _Separation = 0, float64 _PositionAngle = 0);

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif