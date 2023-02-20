#pragma once

// CSE-Hypercomplex extension header

#ifndef _CSE_HYPERCOMPLEX_BASE
#define _CSE_HYPERCOMPLEX_BASE

#include "CSE/Core.h"
#include <limits>
#include <sstream>
#include <type_traits>
#include <xutility>
#include <ymath.h>

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

template <class _Ty, class _Valbase>
class Hypercomplex : public _Valbase
{
public:
	using value_type = _Ty;
	using _Myctraits = _STD _Ctraits<_Ty>;

	constexpr Hypercomplex(_Valbase _Vec) : _Valbase(_Vec) {}
};

inline namespace hypercomplex_literals {}

#if !defined(_SILENCE_NONFLOATING_HYPERCOMPLEX_DEPRECATION_WARNING)
#define _DEPRECATE_NONFLOATING_HYPERCOMPLEX                                                                   \
    [[deprecated("warning STL4037: "                                                                          \
                 "The effect of instantiating the template cse::Hypercomplex for any "                        \
                 "type other than float, double(cse::float64), or long double(cse::float96) is unspecified. " \
                 "You can define _SILENCE_NONFLOATING_HYPERCOMPLEX_DEPRECATION_WARNING to "                   \
                 "suppress this warning.")]]
#else // ^^^ warning enabled / warning disabled vvv
#define _DEPRECATE_NONFLOATING_HYPERCOMPLEX
#endif // ^^^ warning disabled ^^^

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif