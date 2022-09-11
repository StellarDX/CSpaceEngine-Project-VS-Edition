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

_EXTERN_C

// Functions from earlier version of Star Luminosity and Magnitude calculator
// Reference:
// https://en.wikipedia.org/wiki/Magnitude_(astronomy)
// https://en.wikipedia.org/wiki/Absolute_magnitude

/// <summary>
/// Returns Apparent magnitude based on Absolute magnitude and Distance
/// </summary>
/// <param name="AbsMagn">Absolute magnitude</param>
/// <param name="Dist">Distance in Parsec</param>
/// <param name="Extinction">Offsets caused by Interstellar Extinction</param>
float64 ToAppMagn1(float64 AbsMagn, float64 Dist, float64 Extinction = 0);

/// <summary>
/// Returns Absolute magnitude based on Apparent magnitude and Distance
/// </summary>
/// <param name="AppMagn">Apparent magnitude</param>
/// <param name="Dist">Distance in Parsec</param>
/// <param name="Extinction">Offsets caused by Interstellar Extinction</param>
float64 ToAbsMagn1(float64 AppMagn, float64 Dist, float64 Extinction = 0);

/// <summary>
/// Returns Absolute magnitude based on Parallax and Apparent magnitude
/// </summary>
/// <param name="Parallax">Parallax</param>
/// <param name="AppMagn">Apparent magnitude</param>
/// <param name="Extinction">Offsets caused by Interstellar Extinction</param>
float64 ToAbsMagn2(float64 Parallax, float64 AppMagn, float64 Extinction = 0);

//float64 ToAbsMagn3(float64 AppMagn, float64 DistModulus, float64 Extinction = 0);// Scrapped feature, because this is same as ToAbsMagn1.

/// <summary>
/// Returns Absolute bolometric magnitude based on bolometric luminosity
/// </summary>
/// <param name="LumBol">bolometric luminosity in Watts</param>
float64 ToAbsMagn4(float64 LumBol);

/// <summary>
/// Returns Star luminosity using Stefan–Boltzmann law
/// </summary>
/// <param name="Radius">Mean radius of star, using metres</param>
/// <param name="Teff">Temperature in Kelvin</param>
float64 ToLuminosity1(float64 Radius, float64 Teff);

//float64 ToLuminosity2(float64 Mass);// Moved into star generator module.

/// <summary>
/// Returns bolometric luminosity in Watts based on Absolute bolometric magnitude
/// </summary>
/// <param name="AbsMagnBol">Absolute bolometric magnitude</param>
/// <returns></returns>
float64 ToLuminosity3(float64 AbsMagnBol);

_END_EXTERN_C

_CSE_END

#endif