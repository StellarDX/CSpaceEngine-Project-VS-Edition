#pragma once

#ifndef __ORBIT__
#define __ORBIT__

#include "CSE/Core.h"
#include "CSE/Object.h"
#include "CSE/Random.h"

// Orbit models and functions for CSE

_CSE_BEGIN

class OrbitCalculateException : public _STD runtime_error
{
public:
	OrbitCalculateException(_STD string e) : _STD runtime_error(e) {}
};

// ---------- Functions ---------- //

void BinaryOrbitToKeplerianElems(OrbitParam* Par);

/// <summary>
/// Calculate roche limit of two objects
/// </summary>
/// <param name="Primary">Primary object</param>
/// <param name="Companion">Companion object</param>
/// <param name="Mode">Mode of function, 0 = Rigid, 1 = Fluid, default is 0</param>
float64 RocheLimit(const Object* Primary, const Object* Companion, int Mode = 0);

/// <summary>
/// Calculate Hill sphere of two objects
/// </summary>
/// <param name="Primary">Primary object</param>
/// <param name="Companion">Companion object</param>
/// <returns>Hill sphere of the companion</returns>
float64 HillSphere(const Object* Primary, const Object* Companion);

_Check_return_opt_ int __CRTDECL MakeOrbit(Object* Primary, Object* Companion, Object* ThirdGravSourse = nullptr, OrbitParam Args = { .RefPlane = "Extrasolar", .Epoch = J2000 });
_Check_return_ _STD shared_ptr<Object> __CRTDECL MakeBinary(Object* Primary, Object* Companion, Object* ThirdGravSourse = nullptr, OrbitParam Args = { .RefPlane = "Extrasolar", .Epoch = J2000 });

_CSE_END

#endif