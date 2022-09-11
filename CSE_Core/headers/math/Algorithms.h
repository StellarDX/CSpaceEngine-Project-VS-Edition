#pragma once

#ifndef _CSE_MATH_ALGORITHMS
#define _CSE_MATH_ALGORITHMS

#include "../Core/CSECore.h"
#include "../gl/gltypes.h"

#include <iterator>
#include <array>
#include <vector>
#include <initializer_list>

using std::array;
using std::vector;
using std::initializer_list;

_CSE_BEGIN

extern CSELog MathLog;

// These functions can used for floats ints and vecs, complexes are not supported.

/// <summary>
/// Returns the absolute value of x.
/// </summary>
float64 abs(float64 x); // Base
int64 abs(int64 x);

template<typename genType> requires vecType<genType>
genType abs(genType x);

template<typename genIType> requires vecIType<genIType>
genIType abs(genIType x);

/// <summary>
/// Returns -1.0 if x is less than 0.0, 0.0 if x is equal to 0.0, and +1.0 if x is greater than 0.0.
/// </summary>
float64 sgn(float64 x); // Base
int64 sgn(int64 x);

template<typename genType> requires vecType<genType>
genType sgn(genType x);

template<typename genIType> requires vecIType<genIType>
genIType sgn(genIType x);

/// <summary>
/// Returns a value equal to the nearest integer that is less than or equal to x.
/// </summary>
float64 floor(float64 x); // Base

template<typename genType> requires vecType<genType>
genType floor(genType x);

/// <summary>
/// Returns a value equal to the nearest integer that is greater than or equal to x.
/// </summary>
float64 ceil(float64 x); // Base

template<typename genType> requires vecType<genType>
genType ceil(genType x);

/// <summary>
/// Returns the fractional part of x.
/// </summary>
float64 FractionalPart(float64 x); // Base

template<typename genType> requires vecType<genType>
genType FractionalPart(genType x);

/// <summary>
/// Returns the value of x modulo y.
/// </summary>
float64 mod(float64 x, float64 y); // Base

template<typename genType> requires vecType<genType>
genType mod(genType x, genType y);

/// <summary>
/// returns the minimum of the two parameters or array.
/// </summary>
float64 min(float64 x, float64 y); // Base
float64 min(float64* x, uint64 size);
int64 min(int64 x, int64 y);
int64 min(int64* x, uint64 size);
uint64 min(uint64 x, uint64 y);
uint64 min(uint64* x, uint64 size);

float64 min(uint64 size, ...); // UNSTABLE

template<typename genType> requires vecType<genType> genType min(genType x, genType y);
template<typename genIType> requires vecIType<genIType> genIType min(genIType x, genIType y);
template<typename genUType> requires vecUType<genUType> genUType min(genUType x, genUType y);

template<typename iterator> requires std::random_access_iterator<iterator> iterator min(iterator begin, iterator end);
template<typename genType> genType min(initializer_list<genType> Array);

/// <summary>
/// returns the maximum of the two parameters or array.
/// </summary>
float64 max(float64 x, float64 y); // Base
float64 max(float64* x, uint64 size);
int64 max(int64 x, int64 y);
int64 max(int64* x, uint64 size);
uint64 max(uint64 x, uint64 y);
uint64 max(uint64* x, uint64 size);

float64 max(uint64 size, ...); // UNSTABLE

template<typename genType> requires vecType<genType> genType max(genType x, genType y);
template<typename genIType> requires vecIType<genIType> genIType max(genIType x, genIType y);
template<typename genUType> requires vecUType<genUType> genUType max(genUType x, genUType y);

template<typename iterator> requires std::random_access_iterator<iterator> iterator max(iterator begin, iterator end);
template<typename genType> genType max(initializer_list<genType> Array);

// StellarDX: All the possibilities of parameters that I can think of has been listed here...

/// <summary>
/// Returns the value of x constrained to the range minVal to maxVal. 
/// </summary>
/// <param name="x">Specify the value to constrain.</param>
/// <param name="MinVal">Specify the lower end of the range into which to constrain x.</param>
/// <param name="MaxVal">Specify the upper end of the range into which to constrain x.</param>
float64 clamp(float64 x, float64 MinVal, float64 MaxVal);
int64 clamp(int64 x, int64 MinVal, int64 MaxVal);
uint64 clamp(uint64 x, uint64 MinVal, uint64 MaxVal);

template<typename genType> requires vecType<genType> genType clamp(genType x, genType MinVal, genType MaxVal);
template<typename genIType> requires vecIType<genIType> genIType clamp(genIType x, genIType MinVal, genIType MaxVal);
template<typename genUType> requires vecUType<genUType> genUType clamp(genUType x, genUType MinVal, genUType MaxVal);

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

template<typename genType> requires vecType<genType> genType mix(genType x, genType y, genType a);
template<typename genType, typename genBType> requires vecType<genType> && vecBType<genBType> genType mix(genType x, genType y, genBType a);
template<typename genIType, typename genBType> requires vecIType<genIType> && vecBType<genBType> genIType mix(genIType x, genIType y, genBType a);
template<typename genUType, typename genBType> requires vecUType<genUType> && vecBType<genBType> genUType mix(genUType x, genUType y, genBType a);
template<typename genBType> requires vecBType<genBType> genBType mix(genBType x, genBType y, genBType a);

/// <summary>
/// Generates a step function by comparing x to edge.
/// </summary>
/// <param name="edge">Specifies the location of the edge of the step function.</param>
/// <param name="x">Specify the value to be used to generate the step function.</param>
float64 step(float64 edge, float64 x);

template<typename genType> requires vecType<genType>
genType step(genType edge, genType x);

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

template<typename genType> requires vecType<genType> genType smoothstep(genType edge0, genType edge1, genType x);

float64 isinf(float64 x);
float64 isnan(float64 x);

_CSE_END

#include "../../astromath/Algorithms.Template.inl"

#endif