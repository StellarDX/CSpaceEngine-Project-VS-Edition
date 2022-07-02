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

_CSE_BEGIN

// Basic luminosity functions

float64 _Lum_Stefan_Boltzmann_Law(float64 _Radius, float64 _Eff_Temp);

/// <summary>
/// Computing luminosity and apply to an star object
/// </summary>
float64 GetLuminosity(float64 Radius, float64 Teff);
void GetLuminosity(Object* Src);

_CSE_END

#endif