///////////////////////////////////////////////////////////
//                 Data types from GLSL                  //
///////////////////////////////////////////////////////////

// some functions taken from GLM library by G-Truc(MIT Lic.)

/*
The MIT License
--------------------------------------------------------------------------------
Copyright (c) 2005 - 2014 G-Truc Creation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
*/

#pragma once

#ifndef __GLTYPES__
#define __GLTYPES__

#include "gl_vec2.h"
#include "gl_vec3.h"
#include "gl_vec4.h"
#include "matrix.h"

_CSE_BEGIN

// CG/HLSL/Unity style vecs and mats

using fixed2 = _GL gl_vec2<lfixed>;
using fixed3 = _GL gl_vec3<lfixed>;
using fixed4 = _GL gl_vec4<lfixed>;

using half2 = _GL gl_vec2<lhalf>;
using half3 = _GL gl_vec3<lhalf>;
using half4 = _GL gl_vec4<lhalf>;

using float2 = _GL gl_vec2<lfloat>;
using float3 = _GL gl_vec3<lfloat>;
using float4 = _GL gl_vec4<lfloat>;

using fixed2x2 = _GL basic_matrix<lfixed, 2, 2>;
using fixed2x3 = _GL basic_matrix<lfixed, 2, 3>;
using fixed2x4 = _GL basic_matrix<lfixed, 2, 4>;
using fixed3x2 = _GL basic_matrix<lfixed, 3, 2>;
using fixed3x3 = _GL basic_matrix<lfixed, 3, 3>;
using fixed3x4 = _GL basic_matrix<lfixed, 3, 4>;
using fixed4x2 = _GL basic_matrix<lfixed, 4, 2>;
using fixed4x3 = _GL basic_matrix<lfixed, 4, 3>;
using fixed4x4 = _GL basic_matrix<lfixed, 4, 4>;

using half2x2 = _GL basic_matrix<lhalf, 2, 2>;
using half2x3 = _GL basic_matrix<lhalf, 2, 3>;
using half2x4 = _GL basic_matrix<lhalf, 2, 4>;
using half3x2 = _GL basic_matrix<lhalf, 3, 2>;
using half3x3 = _GL basic_matrix<lhalf, 3, 3>;
using half3x4 = _GL basic_matrix<lhalf, 3, 4>;
using half4x2 = _GL basic_matrix<lhalf, 4, 2>;
using half4x3 = _GL basic_matrix<lhalf, 4, 3>;
using half4x4 = _GL basic_matrix<lhalf, 4, 4>;

using float2x2 = _GL basic_matrix<lfloat, 2, 2>;
using float2x3 = _GL basic_matrix<lfloat, 2, 3>;
using float2x4 = _GL basic_matrix<lfloat, 2, 4>;
using float3x2 = _GL basic_matrix<lfloat, 3, 2>;
using float3x3 = _GL basic_matrix<lfloat, 3, 3>;
using float3x4 = _GL basic_matrix<lfloat, 3, 4>;
using float4x2 = _GL basic_matrix<lfloat, 4, 2>;
using float4x3 = _GL basic_matrix<lfloat, 4, 3>;
using float4x4 = _GL basic_matrix<lfloat, 4, 4>;

#if 0
template<typename genType>struct vecType;
template<>struct vecType<vec2> { typedef vec2 type; };
template<>struct vecType<vec3> { typedef vec3 type; };
template<>struct vecType<vec4> { typedef vec4 type; };
#endif

template<typename genType> // Only used in C++20
concept vecType =
(
	_STD is_same_v<genType, vec2> ||
	_STD is_same_v<genType, vec3> ||
	_STD is_same_v<genType, vec4>
);

template<typename genIType> // Only used in C++20
concept vecIType =
(
	_STD is_same_v<genIType, ivec2> ||
	_STD is_same_v<genIType, ivec3> ||
	_STD is_same_v<genIType, ivec4>
);

template<typename genUType> // Only used in C++20
concept vecUType =
(
	_STD is_same_v<genUType, uvec2> ||
	_STD is_same_v<genUType, uvec3> ||
	_STD is_same_v<genUType, uvec4>
);

template<typename genBType> // Only used in C++20
concept vecBType =
(
	_STD is_same_v<genBType, bvec2> ||
	_STD is_same_v<genBType, bvec3> ||
	_STD is_same_v<genBType, bvec4>
);

// ----------Vector Functions---------- //

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 LessThan(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 LessThan(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 LessThan(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 LessThanEqual(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 LessThanEqual(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 LessThanEqual(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 GreaterThan(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 GreaterThan(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 GreaterThan(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 GreaterThanEqual(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 GreaterThanEqual(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 GreaterThanEqual(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 Equal(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 Equal(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 Equal(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 NotEqual(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 NotEqual(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 NotEqual(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y);

template<typename bvec> requires vecBType<bvec>
bool any(bvec x);

template<typename bvec> requires vecBType<bvec>
bool all(bvec x);

template<typename bvec> requires vecBType<bvec>
bvec operator!(bvec x); // because of "not" is already exist as a operator and can't be overloaded, so use this substitusion.

//template<typename bvec> requires vecBType<bvec>
//bvec operator not(bvec x);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 isinf(_GL gl_vec2<genType> x);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 isinf(_GL gl_vec3<genType> x);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 isinf(_GL gl_vec4<genType> x);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 isnan(_GL gl_vec2<genType> x);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 isnan(_GL gl_vec3<genType> x);

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 isnan(_GL gl_vec4<genType> x);

_CSE_END

#include "vector.inl"

#endif