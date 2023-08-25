#pragma once

#ifndef _CSE_MATH_GEOMETRY
#define _CSE_MATH_GEOMETRY

#include "CSE/Core/IEEE754/MathFuncs.h"

_CSE_BEGIN

// These functions can only used for vecs.
// No base functions, all the definations are in templates.

/// <summary>
/// Returns the length of the vector
/// </summary>
template<typename genType> requires vecType<genType>
float64 Length(genType x);

/// <summary>
/// Returns the distance between the two points p0 and p1.
/// </summary>
/// <param name="p0">Specifies the first of two points</param>
/// <param name="p1">Specifies the second of two points</param>
template<typename genType> requires vecType<genType>
float64 Distance(genType p0, genType p1);

inline float64 DistancePolar(vec2 p0, vec2 p1)
{
	return sqrt(pow(p0.x, 2) + pow(p1.x, 2) - 2. * p0.x * p1.x * cse::cos(p0.y - p1.y));
}

inline float64 AngularSeparation(vec3 p0, vec3 p1, float64 RABase = 24)
{
	p0.x *= 360. / RABase;
	p1.x *= 360. / RABase;
	return arccos(cse::cos(p0.y) * cse::cos(p1.y) * cse::sin(p0.x - p1.x) + cse::sin(p0.y) * cse::sin(p1.y));
}

/// <summary>
/// Returns the dot product of two vectors
/// </summary>
/// <param name="x">Specifies the first of two vectors</param>
/// <param name="y">Specifies the second of two vectors</param>
/// <returns></returns>
template<typename genType> requires vecType<genType>
float64 dot(genType x, genType y);

/// <summary>
/// Returns the cross product of two vectors
/// </summary>
/// <param name="x">Specifies the first of two vectors</param>
/// <param name="y">Specifies the second of two vectors</param>
inline vec3 cross(vec3 x, vec3 y)
{
	return vec3
	(
		x[1] * y[2] - y[1] * x[2],
		x[2] * y[0] - y[2] * x[0],
		x[0] * y[1] - y[0] * x[1]
	);
}

/// <summary>
/// Corss product on 4-dimension, using 3 vectors
/// Refernece: https://www.researchgate.net/publication/318543243_Vector_Cross_Product_in_4D_Euclidean_Space_A_Maple_worksheet
/// </summary>
inline vec4 cross(vec4 u, vec4 v, vec4 t)
{
	mat4 U
	({
		{0, -u.w * v.z + u.z * v.w, +u.w * v.y - u.y * v.w, +u.y * v.z - u.z * v.y},
		{+u.w * v.z - u.z * v.w, 0, -u.w * v.x + u.x * v.w, -u.x * v.z + u.z * v.x},
		{-u.w * v.y + u.y * v.w, +u.w * v.x - u.x * v.w, 0, +u.x * v.y - u.y * v.x},
		{-u.y * v.z + u.z * v.y, +u.x * v.z - u.z * v.x, -u.x * v.y + u.y * v.x, 0}
	});
	matrix<float64, 1, 4> T({t.x, t.y, t.z, t.w});
	return vec4((U * T)[0]);
}

/// <summary>
/// Returns a vector with the same direction as its parameter, v, but with length 1.
/// </summary>
template<typename genType> requires vecType<genType>
genType normalize(genType v);

/// <summary>
/// If dot(Nref, I) less than 0 faceforward returns N, otherwise it returns -N.
/// </summary>
/// <param name="N">Specifies the vector to orient.</param>
/// <param name="I">Specifies the incident vector.</param>
/// <param name="Nref">Specifies the reference vector.</param>
template<typename genType> requires vecType<genType>
genType faceforward(genType N, genType I, genType Nref);

/// <summary>
/// For a given incident vector I and surface normal N reflect returns the reflection direction.
/// N should be normalized in order to achieve the desired result.
/// </summary>
/// <param name="I">Specifies the incident vector.</param>
/// <param name="N">Specifies the normal vector.</param>
template<typename genType> requires vecType<genType>
genType reflect(genType I, genType N);

/// <summary>
/// For a given incident vector I, surface normal N and ratio of indices of refraction, eta, returns the refraction vector, R.
/// The direction of normal vector is the upper media.
/// </summary>
/// <param name="I">Specifies the incident vector.</param>
/// <param name="N">Specifies the normal vector.</param>
/// <param name="eta">Specifies the ratio of indices of refraction. (n2 / n1)</param>
template<typename genType> requires vecType<genType>
genType refract(genType I, genType N, float64 eta);

template<typename genType> requires vecType<genType>
genType refract(genType I, genType N, float64 UpperMediaIndex, float64 LowerMediaIndex);

_CSE_END

#include "CSE/Core/Templates/VecGeom.Template.inl"

#endif