#ifndef __ALGORITHMS_TEMP__
#define __ALGORITHMS_TEMP__

#include "../headers/math/Algorithms.h"

_CSE_BEGIN

// Abs Template
template<typename genType> requires vecType<genType>
genType abs(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = abs(x[i]);
	}
	return x;
}

template<typename genIType> requires vecIType<genIType>
genIType abs(genIType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = abs(x[i]);
	}
	return x;
}

// Sign Template
template<typename genType> requires vecType<genType>
genType sgn(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = sgn(x[i]);
	}
	return x;
}

template<typename genIType> requires vecIType<genIType>
genIType sgn(genIType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = sgn(x[i]);
	}
	return x;
}

// Floor Template
template<typename genType> requires vecType<genType>
genType floor(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = floor(x[i]);
	}
	return x;
}

// Ceil Template
template<typename genType> requires vecType<genType>
genType ceil(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = ceil(x[i]);
	}
	return x;
}

// Fract Template
template<typename genType> requires vecType<genType>
genType FractionalPart(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = FractionalPart(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType mod(genType x, genType y)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = mod(x[i], y[i]);
	}
	return x;
}

_CSE_END

#endif