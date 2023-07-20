#pragma once

#ifndef _CSE_PROC_STAR_REM
#define _CSE_PROC_STAR_REM

#include "CSE/Stars/Star_Models_Base.hpp"

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

//===========================================================================//
//                                                                           //
//                           Star Remnant models                             //
//                                                                           //
//===========================================================================//

using WhiteDwarfModel = 
class DensityBasedWhiteDwarfModel
{
public:
	using result_type = Object;
	_STD string ModelName = "Density-Based White Dwarf Model";

	struct param_type
	{
		SPECSTR _Spectum;

		void _Init(SPECSTR _Spec)
		{
			_Spectum = _Spec;
		}

		param_type()
		{
			_Init("WD");
		}

		param_type(SPECSTR _Spec)
		{
			_Init(_Spec);
		}

		SPECSTR spec()const { return _Spectum; }
	}_Par;

	DensityBasedWhiteDwarfModel() : _Par() {}

	DensityBasedWhiteDwarfModel(SPECSTR _Spec) : _Par(_Spec)
	{
		_GENERATOR_ASSERT(IsWhiteDwarf(_Spec), ("\"" + _Spec.str() + "\" is not a white dwarf."))
	}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();

		_CSE_GEN_LOG("INFO", "Loading parameters...");
		// Reference: https://en.wikipedia.org/wiki/White_dwarf
		// Mass and radius is calculated by these parameters:
		// Mass Range is 0.17 - 1.33, average is 0.6
		// Majority between 0.5 - 0.7
		_Obj.Mass = _Eng.normal(0.6, 0.14) * MassSol;
		if (_Obj.Mass > ChandrasekharLim) { _Obj.Mass = ChandrasekharLim; }
		float64 Density = _Eng.normal(1E+9, 2.5E+8);
		_Obj.Dimensions = vec3(2. * cbrt((_Obj.Mass / Density) / ((4. / 3.) * CSE_PI)));
		float64 LIndex = _Par.spec().MinType();
		if (LIndex == -1){ _Obj.Teff = _Eng.uniform(4000, 150000); }
		else { _Obj.Teff = 50400. / LIndex; }
		//_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);
		_Obj.KerrSpin = 0;
		_Obj.KerrCharge = 0;
		return _Obj;
	}
};

#if 0 // Scrapped feature.
class CompositionBasedWhiteDwarfModel
{
public:
	using result_type = Object;
	_STD string ModelName = "Composition-Based White Dwarf Model";

	struct param_type
	{
		struct Composition
		{
			float64 Hydrogen;
			float64 Helium;
			float64 Carbon;
			float64 Oxygen;
		}_Comp;

		param_type() : _Comp({0.01, 0.01, 0.8, 0.18}) {}
		param_type(vec4 _New) : _Comp({ _New.x, _New.y, _New.z, _New.w }) {}
		param_type(float64 _H, float64 _He, float64 _C, float64 _O) : _Comp({ _H, _He, _C, _O }) {}
		param_type(Composition _New) : _Comp(_New) {}
	}_Par;

	CompositionBasedWhiteDwarfModel() : _Par() {}
	CompositionBasedWhiteDwarfModel(param_type::Composition Comp) : _Par(Comp) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
	}
};
#endif

class NeutronStarModel
{
public:
	using result_type = Object;
	_STD string ModelName = "Density-Based White Dwarf Model";

	NeutronStarModel() {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = "Q";
		_Obj.Mass = _Eng.uniform(1.1 * MassSol, TOVLimit);
		float64 Density = _Eng.uniform(3.7E+17, 5.9E+17);
		_Obj.Dimensions = vec3(2. * cbrt((_Obj.Mass / Density) / ((4. / 3.) * CSE_PI)));
		_Obj.Teff = 1E+12 - 9.99999E+11 * (_Eng.exponential() / 5.);
		//_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);
		_Obj.KerrSpin = 0;
		_Obj.KerrCharge = 0;
		return _Obj;
	}
};

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#include "CSE/Stars/BlackHole.hpp"

#endif