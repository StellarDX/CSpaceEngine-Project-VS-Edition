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



_CSE_END

#include "../../astromath/Algorithms.Template.inl"

#endif