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
		len += std::pow(x[i], 2);
	}
	return std::sqrt(len);
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

template<typename genType> requires vecType<genType>
genType faceforward(genType N, genType I, genType Nref)
{
	if (dot(Nref, I) < 0) { return N; }
	else { return vec3(0) - N; }
}

template<typename genType> requires vecType<genType>
genType reflect(genType I, genType N)
{
	// Original code in GLSL (Normal vector need to be normalized)
	/*if (Length(N) != 1.)
	{
		MathLog.Out("Math-Geometry", "WARNING", "Length of normal vector is not 1, automatically nomalized.", SysLogLevel);
		N = normalize(N);
	}
	return I - 2.0 * dot(N, I) * N;*/

	// StellarDX's Implementation (Normal vector doesn't need to be normalized)
	// Reference: https://blog.csdn.net/heyuchang666/article/details/51833893
	genType S = -(dot(I, N) / pow(Length(N), 2)) * N;
	genType P = I + S;
	return 2.0 * P - I;
}

template<typename genType> requires vecType<genType>
genType refract(genType I, genType N, float64 eta)
{
	// StellarDX's Implementation (Incident vector and Normal vector will be normalized automatically)
	// Reference: https://en.wikipedia.org/wiki/Snell%27s_law
	float64 ctet1 = abs(dot(I, N) / (Length(I) * Length(N)));
	float64 ctet2 = std::sqrt(1.0 - std::pow(eta, 2.) * (1.0 - std::pow(ctet1, 2.)));
	if (std::isnan(ctet2)) { return genType(0.); }
	return eta * normalize(I) + (eta * ctet1 - ctet2) * normalize(N);
}

template<typename genType> requires vecType<genType>
genType refract(genType I, genType N, float64 UpperMediaIndex, float64 LowerMediaIndex)
{
	return refract(I, N, UpperMediaIndex / LowerMediaIndex);
}

_CSE_END

#endif