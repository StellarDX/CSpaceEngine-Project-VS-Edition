#pragma once

#ifndef _CSE_MATH_GEOMETRY
#define _CSE_MATH_GEOMETRY

#include "../Core/CSECore.h"
#include "../gl/gltypes.h"

_CSE_BEGIN

extern CSELog MathLog;

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

#include "../../astromath/VecGeom.Template.inl"

#endif