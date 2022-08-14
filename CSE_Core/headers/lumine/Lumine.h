////////////////////////////////////////////////////////////
//                                                        //
//               Luminosity and Magnitudes                //
//                                                        //
////////////////////////////////////////////////////////////

#pragma once

#ifndef __LUMINE__
#define __LUMINE__

#include "../Core/CSECore.h"
#include "../Core/CelObject.h"
#include "../math/AsMath.h"

#include "SpTypes.h"

_CSE_BEGIN

// Basic luminosity functions

/// <summary>
/// Computing luminosity and apply to an star object
/// </summary>
float64 GetLuminosity(float64 Radius, float64 Teff);
void GetLuminosity(Object* Src);

float64 GetStarAbsMagn(float64 _AppMagn, float64 _Dist, float64 _Extinction = 0);

_CSE_END

#endif