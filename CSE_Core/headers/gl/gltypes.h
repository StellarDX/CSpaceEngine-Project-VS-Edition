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

_CSE_BEGIN

#if 0
template<typename genType>struct vecType;
template<>struct vecType<vec2> { typedef vec2 type; };
template<>struct vecType<vec3> { typedef vec3 type; };
template<>struct vecType<vec4> { typedef vec4 type; };
#endif

using std::is_same_v;

template<typename genType> // Only used in C++20
concept vecType =
(
	is_same_v<genType, vec2> ||
	is_same_v<genType, vec3> ||
	is_same_v<genType, vec4>
);

template<typename genIType> // Only used in C++20
concept vecIType =
(
	is_same_v<genIType, ivec2> ||
	is_same_v<genIType, ivec3> ||
	is_same_v<genIType, ivec4>
);

template<typename genUType> // Only used in C++20
concept vecUType =
(
	is_same_v<genUType, uvec2> ||
	is_same_v<genUType, uvec3> ||
	is_same_v<genUType, uvec4>
);

template<typename genBType> // Only used in C++20
concept vecBType =
(
	is_same_v<genBType, bvec2> ||
	is_same_v<genBType, bvec3> ||
	is_same_v<genBType, bvec4>
);

_CSE_END

#endif