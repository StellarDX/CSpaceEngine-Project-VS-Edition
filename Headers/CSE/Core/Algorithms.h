#pragma once

#ifndef _CSE_MATH_ALGORITHMS
#define _CSE_MATH_ALGORITHMS

#include "CSE/Core/CSECore.h"
#include "CSE/Core/gltypes.h"
#include <complex>
#include <initializer_list>

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

using float96 = long double; // Add 96-bit floating point type.
using complex64 = std::complex<float64>;
using std::literals::operator""i;

/// <summary>
/// Returns the absolute value of x.
/// </summary>
_NODISCARD _Check_return_ float64 abs(_In_ float64 _Xx)noexcept;
_NODISCARD _Check_return_ int64 abs(_In_ int64 _Xx)noexcept;
_NODISCARD _Check_return_ float64 abs(_In_ complex64 _Xx)noexcept;

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType __cdecl abs(_In_ genType _Xx)noexcept;

template<typename genIType> requires vecIType<genIType>
_NODISCARD _Check_return_ genIType __cdecl abs(_In_ genIType _Xx)noexcept;

/// <summary>
/// Returns -1.0 if x is less than 0.0, 0.0 if x is equal to 0.0, and +1.0 if x is greater than 0.0.
/// </summary>
_NODISCARD _Check_return_ float64 sgn(_In_ float64 _Xx)noexcept;
_NODISCARD _Check_return_ int64 sgn(_In_ int64 _Xx)noexcept;

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType __cdecl sgn(_In_ genType _Xx)noexcept;

template<typename genIType> requires vecIType<genIType>
_NODISCARD _Check_return_ genIType __cdecl sgn(_In_ genIType _Xx)noexcept;

/// <summary>
/// Returns a value equal to the nearest integer that is less than or equal to x.
/// </summary>
_NODISCARD _Check_return_ int64 floor(_In_ float64 _Xx)noexcept;

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType floor(_In_ genType _Xx)noexcept;

/// <summary>
/// Returns a value equal to the nearest integer that is greater than or equal to x.
/// </summary>
_NODISCARD _Check_return_ int64 ceil(_In_ float64 x)noexcept;

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType ceil(_In_ genType _Xx)noexcept;

/// <summary>
/// Returns the fractional part of x.
/// </summary>
_NODISCARD _Check_return_ float64 FractionalPart(float64 _Xx)noexcept;

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType FractionalPart(genType _Xx)noexcept;

_NODISCARD _Check_return_ float64 fract(float64 _Xx)noexcept;

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType fract(genType _Xx)noexcept;

/// <summary>
/// Returns the value of x modulo y.
/// </summary>
_NODISCARD _Check_return_ float64 mod(float64 _Left, float64 _Right)noexcept;

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType mod(genType _Left, float64 _Right)noexcept;

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType mod(genType _Left, genType _Right)noexcept;

/// <summary>
/// returns the minimum of the two parameters or array.
/// </summary>
_NODISCARD constexpr float64 min(float64 _Left, float64 _Right) noexcept(noexcept(_Left < _Right));
_NODISCARD constexpr int64 min(int64 _Left, int64 _Right) noexcept(noexcept(_Left < _Right));
_NODISCARD constexpr uint64 min(uint64 _Left, uint64 _Right) noexcept(noexcept(_Left < _Right));

template<typename genType> requires vecType<genType>
_NODISCARD constexpr genType min(genType _Left, genType _Right)noexcept;
template<typename genType> requires vecType<genType>
_NODISCARD constexpr genType min(genType _Left, float64 _Right)noexcept;

template<typename genIType> requires vecIType<genIType>
_NODISCARD constexpr genIType min(genIType _Left, genIType _Right)noexcept;
template<typename genIType> requires vecIType<genIType>
_NODISCARD constexpr genIType min(genIType _Left, int64 _Right)noexcept;

template<typename genUType> requires vecUType<genUType>
_NODISCARD constexpr genUType min(genUType _Left, genUType _Right)noexcept;
template<typename genUType> requires vecUType<genUType>
_NODISCARD constexpr genUType min(genUType _Left, uint64 _Right)noexcept;

template<typename iterator> requires std::random_access_iterator<iterator>
_NODISCARD constexpr iterator min(iterator begin, iterator end)noexcept;

template<typename genType>
_NODISCARD constexpr genType min(std::initializer_list<genType> _Ilist)noexcept;

/// <summary>
/// returns the maximum of the two parameters or array.
/// </summary>
_NODISCARD constexpr float64 max(float64 _Left, float64 _Right) noexcept(noexcept(_Right < _Left));
_NODISCARD constexpr int64 max(int64 _Left, int64 _Right) noexcept(noexcept(_Right < _Left));
_NODISCARD constexpr uint64 max(uint64 _Left, uint64 _Right) noexcept(noexcept(_Right < _Left));

template<typename genType> requires vecType<genType>
_NODISCARD constexpr genType max(genType _Left, genType _Right)noexcept;
template<typename genType> requires vecType<genType>
_NODISCARD constexpr genType max(genType _Left, float64 _Right)noexcept;

template<typename genIType> requires vecIType<genIType>
_NODISCARD constexpr genIType max(genIType _Left, genIType _Right)noexcept;
template<typename genIType> requires vecIType<genIType>
_NODISCARD constexpr genIType max(genIType _Left, int64 _Right)noexcept;

template<typename genUType> requires vecUType<genUType>
_NODISCARD constexpr genUType max(genUType _Left, genUType _Right)noexcept;
template<typename genUType> requires vecUType<genUType>
_NODISCARD constexpr genUType max(genUType _Left, uint64 _Right)noexcept;

template<typename iterator> requires std::random_access_iterator<iterator>
_NODISCARD constexpr iterator max(iterator begin, iterator end)noexcept;

template<typename genType>
_NODISCARD constexpr genType max(std::initializer_list<genType> _Ilist)noexcept;

/// <summary>
/// Returns the value of x constrained to the range minVal to maxVal. 
/// </summary>
/// <param name="x">Specify the value to constrain.</param>
/// <param name="MinVal">Specify the lower end of the range into which to constrain x.</param>
/// <param name="MaxVal">Specify the upper end of the range into which to constrain x.</param>
float64 clamp(float64 x, float64 MinVal, float64 MaxVal);
int64 clamp(int64 x, int64 MinVal, int64 MaxVal);
uint64 clamp(uint64 x, uint64 MinVal, uint64 MaxVal);

template<typename genType> requires vecType<genType>
genType clamp(genType x, genType MinVal, genType MaxVal);
template<typename genType> requires vecType<genType>
genType clamp(genType x, float64 MinVal, float64 MaxVal);

template<typename genIType> requires vecIType<genIType>
genIType clamp(genIType x, genIType MinVal, genIType MaxVal);
template<typename genIType> requires vecIType<genIType>
genIType clamp(genIType x, int64 MinVal, int64 MaxVal);

template<typename genUType> requires vecUType<genUType>
genUType clamp(genUType x, genUType MinVal, genUType MaxVal);
template<typename genUType> requires vecUType<genUType>
genUType clamp(genUType x, uint64 MinVal, uint64 MaxVal);

/// <summary>
/// Performs a linear interpolation between x and y using a to weight between them.
/// </summary>
/// <param name="x">Specify the start of the range in which to interpolate.</param>
/// <param name="y">Specify the end of the range in which to interpolate.</param>
/// <param name="a">Specify the value to use to interpolate between x and y.</param>
float64 mix(float64 x, float64 y, float64 a);
float64 mix(float64 x, float64 y, bool a);
int64 mix(int64 x, int64 y, bool a);
uint64 mix(uint64 x, uint64 y, bool a);
bool mix(bool x, bool y, bool a);

template<typename genType> requires vecType<genType>
genType mix(genType x, genType y, genType a);
template<typename genType> requires vecType<genType>
genType mix(genType x, genType y, float64 a);

template<typename genType, typename genBType> requires vecType<genType> && vecBType<genBType>
genType mix(genType x, genType y, genBType a);

template<typename genIType, typename genBType> requires vecIType<genIType> && vecBType<genBType>
genIType mix(genIType x, genIType y, genBType a);

template<typename genUType, typename genBType> requires vecUType<genUType> && vecBType<genBType>
genUType mix(genUType x, genUType y, genBType a);

template<typename genBType> requires vecBType<genBType>
genBType mix(genBType x, genBType y, genBType a);

/// <summary>
/// Generates a step function by comparing x to edge.
/// </summary>
/// <param name="edge">Specifies the location of the edge of the step function.</param>
/// <param name="x">Specify the value to be used to generate the step function.</param>
float64 step(float64 edge, float64 x);

template<typename genType> requires vecType<genType>
genType step(genType edge, genType x);
template<typename genType> requires vecType<genType>
genType step(float64 edge, genType x);

/// <summary>
/// Performs smooth Hermite interpolation between 0 and 1 when x is between edge0 and edge1.
/// https://en.wikipedia.org/wiki/Smoothstep
/// </summary>
/// <param name="edge0">Specifies the value of the lower edge of the Hermite function.</param>
/// <param name="edge1">Specifies the value of the upper edge of the Hermite function.</param>
/// <param name="x"></param>
float64 smoothstep(float64 edge0, float64 edge1, float64 x); // Original function in GLSL
float64 smoothstepAMD(float64 edge0, float64 edge1, float64 x); // Another version provide by AMD
float64 smoothstepKP(float64 edge0, float64 edge1, float64 x); // Ken Perlin's improved version
float64 inverse_smoothstep(float64 x); // Inverse function of smoothstep

template<typename genType> requires vecType<genType>
genType smoothstep(genType edge0, genType edge1, genType x);
template<typename genType> requires vecType<genType>
genType smoothstep(float64 edge0, float64 edge1, genType x);

// -------------------------------------------------------------------
// inf and nan
_Check_return_ int64 isinf(_In_ float64 _X)throw();
_Check_return_ int64 isnan(_In_ float64 _X)throw();

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#include "CSE/Core/Templates/Algorithms.Template.inl"

#endif