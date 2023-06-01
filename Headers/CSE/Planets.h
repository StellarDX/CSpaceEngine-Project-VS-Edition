#pragma once

#ifndef _CSE_PROC_PLANET_GEN
#define _CSE_PROC_PLANET_GEN

#include "CSE/Planets/EOS.hpp"
#include "CSE/Planets/Planet_Base.hpp"
#include "CSE/Planets/Orbit.h"
#include "CSE/Planets/Rotation.h"

//#ifdef _USE_EXTERNAL_PHS_FUNC
// Functions for planet's physical characteristics
_CSE_BEGIN
inline namespace planet
{
	inline float64 MeanRadius(Object Obj){return Obj.Radius();}
	inline float64 EquatorialRadius(Object Obj) { return sqrt((Obj.Dimensions.x / 2.)*(Obj.Dimensions.z / 2.)); }
	inline float64 PolarRadius(Object Obj) { return Obj.Dimensions.y / 2.; }
	inline float64 Flattening(Object Obj) { return (EquatorialRadius(Obj) - PolarRadius(Obj)) / EquatorialRadius(Obj); }

	inline float64 CircumferenceE(Object Obj)
	{
		float64 a = max(Obj.Dimensions.x, Obj.Dimensions.z) / 2.;
		float64 b = min(Obj.Dimensions.x, Obj.Dimensions.z) / 2.;
		float64 Eccentricity = sqrt(1. - (pow(b, 2) / pow(a, 2)));
		IntegralFunction<float64(float64)> E = [Eccentricity](float64 x)->float64
		{
			return sqrt(1. - pow(Eccentricity, 2) * pow(sin(degrees(x)), 2));
		};
		return 4. * a * E(0, CSE_PI_D2);
	}

	inline float64 CircumferenceM(Object Obj)
	{
		float64 a = max(Obj.Dimensions.x, Obj.Dimensions.y) / 2.;
		float64 b = min(Obj.Dimensions.x, Obj.Dimensions.y) / 2.;
		float64 Eccentricity = sqrt(1. - (pow(b, 2) / pow(a, 2)));
		IntegralFunction<float64(float64)> E = [Eccentricity](float64 x)->float64
		{
			return sqrt(1. - pow(Eccentricity, 2) * pow(sin(degrees(x)), 2));
		};
		return 4. * a * E(0, CSE_PI_D2);
	}

	inline float64 SurfaceArea(Object Obj)
	{
		_STD array<float64, 3> Radiuses = Obj.Dimensions / 2.;
		_STD sort(Radiuses.begin(), Radiuses.end(), [](float64 A, float64 B){return A > B;});
		float64 a = Radiuses[0], b = Radiuses[1], c = Radiuses[2];
		float64 phi = radians(arccos(c / a)),
			k = sqrt((pow(a, 2) * (pow(b, 2) - pow(c, 2))) / (pow(b, 2) * (pow(a, 2) - pow(c, 2))));
		IntegralFunction<float64(float64)> F = [k](float64 x)->float64
		{
			return 1. / sqrt(1. - pow(k, 2) * pow(sin(degrees(x)), 2));
		}, 
		E = [k](float64 x)->float64
		{
			return sqrt(1. - pow(k, 2) * pow(sin(degrees(x)), 2));
		};
		return 2. * CSE_PI * pow(c, 2) +
			((2. * CSE_PI * a * b) / sin(degrees(phi))) *
			(E(0, phi) * pow(sin(degrees(phi)), 2) + F(0, phi) * pow(cos(degrees(phi)), 2));
	}

	inline float64 Volume(Object Obj) { return (4. / 3.) * CSE_PI * (Obj.Dimensions.x / 2.) * (Obj.Dimensions.y / 2.) * (Obj.Dimensions.z / 2.); }
	inline float64 MeanDensity(Object Obj) { return Obj.Mass / Volume(Obj); }
	inline float64 SurfaceGravity(Object Obj) { return (GravConstant * Obj.Mass) / pow(Obj.Radius(), 2); }
}
_CSE_END
//#endif

#endif