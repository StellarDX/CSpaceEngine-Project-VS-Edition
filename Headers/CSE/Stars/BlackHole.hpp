#pragma once

#ifndef _BLACK_HOLE_
#define _BLACK_HOLE_

#include "CSE/Core.h"
#include "CSE/Object.h"

_CSE_BEGIN

//===========================================================================//
//                                                                           //
//                          Black Holes implements                           //
//                                                                           //
//===========================================================================//

// According to The no-hair theorem(https://en.wikipedia.org/wiki/No-hair_theorem), 
// A black hole can be completely characterized by three(and only three) quantities:
// M - Mass
// J - angular momentum (KerrSpin)
// Q - electric charge (KerrCharge)

struct BLACKHOLE
{
	float64 _M_m;
	float64 _M_j;
	float64 _M_q;

	operator Object() { return { .Mass = _M_m, .KerrSpin = _M_j, .KerrCharge = _M_q }; }
};

// Then the blackhole can be classified into 4 types:
// J == 0, Q == 0  -->  Schwarzschild black hole      (https://en.wikipedia.org/wiki/Schwarzschild_metric)
// J != 0, Q == 0  -->  Kerr black hole               (https://en.wikipedia.org/wiki/Kerr_metric)
// J == 0, Q != 0  -->  Reissner-Nordstrom black hole (https://en.wikipedia.org/wiki/Reissner_Nordstrom_metric)
// J != 0, Q != 0  -->  Kerr-Newman black hole        (https://en.wikipedia.org/wiki/Kerr_Newman_metric)

// Reference: 
// https://en.wikipedia.org/wiki/Black_hole
// https://en.wikipedia.org/wiki/Einstein_field_equations#Einstein%E2%80%93Maxwell_equations

/// <summary>
/// Returns the Schwarzschild Radius of a black hole
/// </summary>
/// <param name="Mass">Mass of the black hole</param>
/// <returns>Schwarzschild Radius</returns>
float64 SchwarzschildRadius(float64 Mass);

/// <summary>
/// Returns the Schwarzschild Density of a black hole
/// </summary>
/// <param name="Mass">Mass of the black hole</param>
/// <returns>Schwarzschild Density</returns>
float64 SchwarzschildDensity(float64 Mass);

/// <summary>
/// Check the data of blackhole. 
/// </summary>
/// <returns>smaller than or equals 0 is valid and other is not.</returns>
float64 BHIsValid1(BLACKHOLE BH);
float64 BHIsValid2(BLACKHOLE BH);



_CSE_END

#endif