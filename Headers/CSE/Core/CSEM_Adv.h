#pragma once

#ifndef _CSE_MATH_ADVANCED
#define _CSE_MATH_ADVANCED

// These functions are used to solve some very difficaut problems.
// But its accuracy maybe very low.

#include "CSE/Core/CSECore.h"

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

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
_Check_return_ float64 _cdecl integral(_In_ float64(*f)(float64 x), _In_ float64 a, _In_ float64 b, float64 Logn = 5);

_END_EXTERN_C

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif