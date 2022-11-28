#include "CSE/Core/gltypes.h"

#include <cmath>

#ifndef __GLTYPES_INL_
#define __GLTYPES_INL_

_CSE_BEGIN

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 LessThan(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y)
{
	return bvec2(x[0] < y[0], x[1] < y[1]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 LessThan(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y)
{
	return bvec3(x[0] < y[0], x[1] < y[1], x[2] < y[2]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 LessThan(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y)
{
	return bvec4(x[0] < y[0], x[1] < y[1], x[2] < y[2], x[3] < y[3]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 LessThanEqual(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y)
{
	return bvec2(x[0] <= y[0], x[1] <= y[1]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 LessThanEqual(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y)
{
	return bvec3(x[0] <= y[0], x[1] <= y[1], x[2] <= y[2]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 LessThanEqual(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y)
{
	return bvec4(x[0] <= y[0], x[1] <= y[1], x[2] <= y[2], x[3] <= y[3]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 GreaterThan(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y)
{
	return bvec2(x[0] > y[0], x[1] > y[1]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 GreaterThan(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y)
{
	return bvec3(x[0] > y[0], x[1] > y[1], x[2] > y[2]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 GreaterThan(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y)
{
	return bvec4(x[0] > y[0], x[1] > y[1], x[2] > y[2], x[3] > y[3]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 GreaterThanEqual(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y)
{
	return bvec2(x[0] >= y[0], x[1] >= y[1]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 GreaterThanEqual(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y)
{
	return bvec3(x[0] >= y[0], x[1] >= y[1], x[2] >= y[2]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 GreaterThanEqual(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y)
{
	return bvec4(x[0] >= y[0], x[1] >= y[1], x[2] >= y[2], x[3] >= y[3]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 Equal(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y)
{
	return bvec2(x[0] == y[0], x[1] == y[1]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 Equal(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y)
{
	return bvec3(x[0] == y[0], x[1] == y[1], x[2] == y[2]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 Equal(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y)
{
	return bvec4(x[0] == y[0], x[1] == y[1], x[2] == y[2], x[3] == y[3]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 NotEqual(_GL gl_vec2<genType> x, _GL gl_vec2<genType> y)
{
	return bvec2(x[0] != y[0], x[1] != y[1]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 NotEqual(_GL gl_vec3<genType> x, _GL gl_vec3<genType> y)
{
	return bvec3(x[0] != y[0], x[1] != y[1], x[2] != y[2]);
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 NotEqual(_GL gl_vec4<genType> x, _GL gl_vec4<genType> y)
{
	return bvec4(x[0] != y[0], x[1] != y[1], x[2] != y[2], x[3] != y[3]);
}

template<typename bvec> requires vecBType<bvec>
bool any(bvec x)
{     // bvec can be bvec2, bvec3 or bvec4
	bool result = false;
	int i;
	for (i = 0; i < x.size(); ++i)
	{
		result |= x[i];
	}
	return result;
}

template<typename bvec> requires vecBType<bvec>
bool all(bvec x)       // bvec can be bvec2, bvec3 or bvec4
{
	bool result = true;
	int i;
	for (i = 0; i < x.size(); ++i)
	{
		result &= x[i];
	}
	return result;
}

template<typename bvec> requires vecBType<bvec>
bvec operator!(bvec x)
{
	bvec result;
	for (size_t i = 0; i < x.size(); ++i)
	{
		result[i] = !x[i];
	}
	return result;
}

//template<typename bvec>
//bvec operator not(bvec x)
//{
//	return !x;
//}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 isinf(_GL gl_vec2<genType> x)
{
	return bvec2(_STD isinf(x.x), _STD isinf(x.y));
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 isinf(_GL gl_vec3<genType> x)
{
	return bvec3(_STD isinf(x.x), _STD isinf(x.y), _STD isinf(x.z));
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 isinf(_GL gl_vec4<genType> x)
{
	return bvec4(_STD isinf(x.x), _STD isinf(x.y), _STD isinf(x.z), _STD isinf(x.w));
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec2 isnan(_GL gl_vec2<genType> x)
{
	return bvec2(_STD isnan(x.x), _STD isnan(x.y));
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec3 isnan(_GL gl_vec3<genType> x)
{
	return bvec3(_STD isnan(x.x), _STD isnan(x.y), _STD isnan(x.z));
}

template<typename genType>
requires _STD convertible_to<genType, float64> || _STD convertible_to<genType, int64> || _STD convertible_to<genType, uint64>
bvec4 isnan(_GL gl_vec4<genType> x)
{
	return bvec4(_STD isnan(x.x), _STD isnan(x.y), _STD isnan(x.z), _STD isnan(x.w));
}

_CSE_END

#endif