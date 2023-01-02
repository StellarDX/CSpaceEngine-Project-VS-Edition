﻿#pragma once

#ifndef _CSE_PROC_PLANET_BASE
#define _CSE_PROC_PLANET_BASE

#include "CSE/Core.h"
#include "CSE/Object.h"
#include "CSE/Random.h"
#include <map>

_CSE_BEGIN

//===========================================================================//
//                                                                           //
//                CSpaceEngine procedural planet generator                   //
//                                                                           //
//===========================================================================//

// Reference:
// Wikipedia. Hydrostatic equilibrium
// https://en.wikipedia.org/wiki/Hydrostatic_equilibrium
// S. Seager, M. Kuchner, C. A. Hier-Majumder, and B. Militzer "MASS-RADIUS RELATIONSHIPS FOR SOLID EXOPLANETS"
// https://arxiv.org/abs/0707.2895

// Normal Approximation-based models, not physically realistic
class TerrestrialPlanetBase
{
public:
	using result_type = Object;
	_STD string ModelName = "Terrestrial Planet Model";

	struct param_type
	{
		vec2 RadiusRange; // in Earth radius (*6378137)
		float64 MassScale;
		float64 RadScale;
		float64 PressurePow;

		param_type(float64 MinRadius, float64 MaxRadius, float64 MScale, float64 RScale, float64 PPow) : 
			RadiusRange(MinRadius, MaxRadius), MassScale(MScale), RadScale(RScale), PressurePow(PPow){}
	}_Par;

	TerrestrialPlanetBase() : _Par(0.15, 2, 6.41, 2.84, 0){}
	TerrestrialPlanetBase(float64 MinRadius, float64 MaxRadius, float64 MScale = 6.41, float64 RScale = 2.84, float64 PPow = 0) :
		_Par(MinRadius, MaxRadius, MScale, RScale, PPow) {}

	template <class _Engine> // Procedural planet generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Planet";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} b", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
		_Obj.Class = "Terra";

		float64 BaseRadius = _Eng.uniform(_Par.RadiusRange.x, _Par.RadiusRange.y);
		_Obj.Dimensions = vec3(BaseRadius * RadEarth * 2.);
		if (_Par.PressurePow == 0) { _Par.PressurePow = _Eng.uniform(0.51, 0.55); } // n ranges typically from 0.5-0.6
		float64 BaseMassScaled = MassScale(BaseRadius / _Par.RadScale, _Par.PressurePow) * _Par.MassScale;
		_Obj.Mass = BaseMassScaled * MassEarth;
		return _Obj;
	};

protected:
	float64 MassScale(float64 RadiusScale, float64 n)
	{
		return (4. / 3.) * CSE_PI * pow(RadiusScale, 3) * (1. + (1. - (3. / 5.) * n) * pow((2. / 3.) * CSE_PI * pow(RadiusScale, 2), n));
	}
};

// A physical realistic model, based on composition.
// It is still incomplete because this model is too difficaut to solve.
#if 0
class UBox_PlanetBase_Composition
{
public:
	using result_type = Object;
	_STD string ModelName = "Custom Planet Model";
	using interior = _STD map<_STD string, float64>;

	struct param_type // Based on compositions
	{
		float64 Metals;
		float64 Silicates;
		float64 Ices;
		float64 Hydrogen;
	}_Par;

	UBox_PlanetBase_Composition() : _Par() {}

	interior Interior()const;

	// CUMULATIVE PROPERTIES
	
	_CONSTEXPR20 float64 Mass()const;
	_CONSTEXPR20 float64 Radius()const;
	_CONSTEXPR20 float64 Density()const;
	_CONSTEXPR20 float64 Volume()const;
	_CONSTEXPR20 float64 RadialMassGain()const;
	_CONSTEXPR20 float64 SurfaceGravity()const;
	_CONSTEXPR20 float64 EscapeVelocity()const;

	// CORE PROPERTIES

	//_CONSTEXPR20 float64 CoreDensity()const;
	//_CONSTEXPR20 float64 CorePressure()const;
	//_CONSTEXPR20 float64 CoreTemperature()const;


};
#endif

_CSE_END

#endif