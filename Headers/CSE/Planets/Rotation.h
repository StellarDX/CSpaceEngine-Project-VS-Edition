#pragma once

#ifndef _ROTATION_
#define _ROTATION_

#include "CSE/Object.h"

_CSE_BEGIN

class RotationCalculateException : public _STD runtime_error
{
public:
	RotationCalculateException(_STD string e) : _STD runtime_error(e) {}
};

/// <summary>
/// Calculate Flattening of a object.
/// Reference: https://en.wikipedia.org/wiki/Earth_radius#Physics_of_Earth.27s_deformation
/// </summary>
void Oblate(Object* Obj);

_CSE_END

#endif