#ifndef __GEOMETRY_TEMP__
#define __GEOMETRY_TEMP__

#include "../headers/math/Geometry.h"

_CSE_BEGIN

template<typename genType> requires vecType<genType>
float64 Length(genType x)
{
	float64 len = 0;
	for (size_t i = 0; i < x.size(); i++)
	{
		len += pow(x[i], 2);
	}
	return sqrt(len);
}

template<typename genType> requires vecType<genType>
float64 Distance(genType p0, genType p1)
{
	return Length(p0 - p1);
}

template<typename genType> requires vecType<genType>
float64 dot(genType x, genType y)
{
	float64 Result = 0;
	for (size_t i = 0; i < x.size(); i++)
	{
		Result += x[i] * y[i];
	}
	return Result;
}

template<typename genType> requires vecType<genType>
genType normalize(genType v)
{
	return v / Length(v);
}

_CSE_END

#endif