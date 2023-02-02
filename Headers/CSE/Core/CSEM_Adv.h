#pragma once

#ifndef _CSE_MATH_ADVANCED
#define _CSE_MATH_ADVANCED

// These functions are used to solve some very difficaut problems.
// But its accuracy maybe very low.

#include <functional>
#include "CSE/Core/CSECore.h"
#include "CSE/Core/gltypes.h"

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

/****************************************************************************************\
*                                     XYZ -> Polar                                       *
\****************************************************************************************/

/// <summary>
/// Convert XY coordinate to polar coordinate, in format (r, Theta)
/// </summary>
_Check_return_ vec2 _cdecl XYToPolar(_In_ vec2 XY);

/// <summary>
/// Convert XYZ coordinate (Based on X-Z plane) to polar coordinate, in format (Lon, Lat, Dist)
/// </summary>
_Check_return_ vec3 _cdecl XYZToPolar(_In_ vec3 XYZ);

/****************************************************************************************\
*                                     Polar -> XYZ                                       *
\****************************************************************************************/

/// <summary>
/// Convert polar coordinate in format (r, Theta) to XY coordinate
/// </summary>
_Check_return_ vec2 _cdecl PolarToXY(_In_ vec2 Polar);

/// <summary>
/// Convert polar coordinate in format (Lon, Lat, Dist) to XYZ coordinate (Based on X-Z plane)
/// </summary>
_Check_return_ vec3 _cdecl PolarToXYZ(_In_ vec3 Polar);

_EXTERN_C

/// <summary>
/// Compute Numerical Integral Using Function Pointers
/// Reference: https://helloacm.com/c-function-to-compute-numerical-integral-using-function-pointers/
/// </summary>
/// <param name="f">Input Functions</param>
/// <param name="a">Lower limit</param>
/// <param name="b">Upper limit</param>
/// <param name="n">Number of samples, expressed as logarithm</param>
/// <returns>result of integral</returns>
_Check_return_ float64 _cdecl integral(_In_ _STD function<float64(float64)> f, _In_ float64 a, _In_ float64 b, float64 Logn = 5);

_END_EXTERN_C

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif