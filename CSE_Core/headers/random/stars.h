#pragma once

//===========================================================================//
//                                                                           //
//             CSpaceEngine Procedural Star Generator Header                 //
//                                                                           //
//===========================================================================//

#ifndef _CSE_PROC_STAR_GEN
#define _CSE_PROC_STAR_GEN

#include "../Core/CSECore.h"
#include "../gl/gltypes.h"
#include "../lumine/Lumine.h"
#include "../random/random.h"

#include "../Core/ConstLists.h"

#include <array>
#include <format>
#include <iostream>

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

using STPARS = _STD array<float64, 6>;

enum StarTableCoeffs
{
	SpT,      // Subtype of Spectal Type
	Teff,     // Effective Temperature
	BCv,      // Bolometric Correction in V-band
	Mbol,     // Absolute Bolometric Magnitude
	R_RSun,   // Radius in Solar Radius
	MSun      // Mass in Solar Mass
};

const STPARS* GetStarTable(LSTARCLS::SpecClass Class, size_t* Tyc);

//===========================================================================//
//                                                                           //
//                               Star Models                                 //
//                                                                           //
//===========================================================================//

                              // O,   B,      A,       F,        G,        K,        M
#define _MAIN_SEQUENCE_WEIGHTS { 143, 536250, 2681250, 13000000, 33000000, 53625000, 326157357 } // Calculated among 3000000 stars and converted to integers
SPECSTR _Random_Main_Sequence_types();

class StarModelBase
{
public:
	using result_type = Object;

	struct param_type
	{
		using model_type = StarModelBase;

		SPECSTR _Spectum;
		const STPARS* _TableParams;
		size_t _TableSize;

		void _Init(SPECSTR _Spec)
		{
			_Spectum = _Spec;
			_TableParams = GetStarTable(_Spec.SClass(), &_TableSize);
		}

		param_type()
		{
			_Init("M2V");
		}

		param_type(SPECSTR _Spec)
		{
			_Init(_Spec);
		}

		SPECSTR spec()const { return _Spectum; }
		const STPARS* param()const { return _TableParams; }
		size_t tablesize()const { return _TableSize; }
	} _Par;

	StarModelBase() : _Par() {}

	explicit StarModelBase(SPECSTR _Spec) : _Par(_Spec) {}

	explicit StarModelBase(param_type _Par2) : _Par(_Par2) {}
};

class MainSequenceStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	MainSequenceStarModel() : _Mybase() {}
	explicit MainSequenceStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_STL_ASSERT(IsMainSequence(_Spec), ("\"" + _Spec.str() + "\" is not a main-sequence star type."));
	}

	explicit MainSequenceStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();
		const STPARS* _NextTable;
		size_t _NextTableSize;
		if (_Par.spec().SClass() < 6)
		{
			_NextTable = GetStarTable(static_cast<LSTARCLS::SpecClass>(_Par.spec().SClass() + 1), &_NextTableSize);
		}
		else { _NextTable = nullptr; }

		// Locating params
		STPARS BaseParams;
		STPARS NextParams;
		float64 Offset;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.uniform(_Par.param()[0][StarTableCoeffs::SpT], lround(_Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]) + 0.9);
		}
		_STL_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");

		size_t i = 0;
		while
		(
			i < _Par.tablesize() - 1 &&
			!(SType >= _Par.param()[i][StarTableCoeffs::SpT] && SType < _Par.param()[i + 1][StarTableCoeffs::SpT])
		)
		{++i;}
		BaseParams = _Par.param()[i];
		if (i == _Par.tablesize() - 1 && _NextTable)
		{
			NextParams = _NextTable[0];
			NextParams[StarTableCoeffs::SpT] = 10.0;
		}
		else { NextParams = _Par.param()[i + 1]; }

		Offset = (SType - BaseParams[StarTableCoeffs::SpT]) / (NextParams[StarTableCoeffs::SpT] - BaseParams[StarTableCoeffs::SpT]);

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun] + 
			(NextParams[StarTableCoeffs::MSun] - BaseParams[StarTableCoeffs::MSun]) * Offset;
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * _Eng.normal(BaseMass, BaseMass * 0.2 / 3.);

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun] +
			(NextParams[StarTableCoeffs::R_RSun] - BaseParams[StarTableCoeffs::R_RSun]) * Offset;
		_Obj.Dimensions = vec3(_Eng.normal(BaseRadius, BaseRadius * 0.15 / 3.) * 2. * RadSol);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff] +
			(NextParams[StarTableCoeffs::Teff] - BaseParams[StarTableCoeffs::Teff]) * Offset;
		_Obj.Teff = _Eng.normal(BaseTEff, BaseTEff * 0.2 / 3.);

		// Luminosities

		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol] +
			(NextParams[StarTableCoeffs::Mbol] - BaseParams[StarTableCoeffs::Mbol]) * Offset;
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv] +
			(NextParams[StarTableCoeffs::BCv] - BaseParams[StarTableCoeffs::BCv]) * Offset;
		_Obj.AbsMagn = _Eng.normal(BaseMBol, 0.01) - BaseBCorr;
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);

		return _Obj;
	}
};

class HPMainSequenceStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	HPMainSequenceStarModel() : _Mybase() {}
	explicit HPMainSequenceStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_STL_ASSERT(IsMainSequence(_Spec), ("\"" + _Spec.str() + "\" is not a main-sequence star type."));
	}

	explicit HPMainSequenceStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back("CSE-RS" + _STD vformat(" {}", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = "CSE-RS" + _STD vformat(" {}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();
		const STPARS* _NextTable;
		size_t _NextTableSize;
		if (_Par.spec().SClass() < 6)
		{
			_NextTable = GetStarTable(static_cast<LSTARCLS::SpecClass>(_Par.spec().SClass() + 1), &_NextTableSize);
		}
		else { _NextTable = nullptr; }

		// Locating params
		STPARS BaseParams;
		STPARS NextParams;
		float64 Offset;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();

		if (SType == -1)
		{
			SType = (float)_Eng.uniform(_Par.param()[0][StarTableCoeffs::SpT], lround(_Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]) + 0.9);
		}
		_STL_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");

		size_t i = 0;
		while
		(
			i < _Par.tablesize() - 1 &&
			!(SType >= _Par.param()[i][StarTableCoeffs::SpT] && SType < _Par.param()[i + 1][StarTableCoeffs::SpT])
		)
		{++i;}
		BaseParams = _Par.param()[i];
		if (i == _Par.tablesize() - 1 && _NextTable)
		{
			NextParams = _NextTable[0];
			NextParams[StarTableCoeffs::SpT] = 10.0;
		}
		else { NextParams = _Par.param()[i + 1]; }

		Offset = (SType - BaseParams[StarTableCoeffs::SpT]) / (NextParams[StarTableCoeffs::SpT] - BaseParams[StarTableCoeffs::SpT]);

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun] +
			(NextParams[StarTableCoeffs::MSun] - BaseParams[StarTableCoeffs::MSun]) * Offset;
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * BaseMass;//_Eng.normal(BaseMass, BaseMass * 0.2 / 3.);

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun] +
			(NextParams[StarTableCoeffs::R_RSun] - BaseParams[StarTableCoeffs::R_RSun]) * Offset;
		_Obj.Dimensions = vec3(BaseRadius * 2. * RadSol);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff] +
			(NextParams[StarTableCoeffs::Teff] - BaseParams[StarTableCoeffs::Teff]) * Offset;
		_Obj.Teff = BaseTEff;

		// Luminosities

		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol] +
			(NextParams[StarTableCoeffs::Mbol] - BaseParams[StarTableCoeffs::Mbol]) * Offset;
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv] +
			(NextParams[StarTableCoeffs::BCv] - BaseParams[StarTableCoeffs::BCv]) * Offset;
		_Obj.AbsMagn = BaseMBol - BaseBCorr;
		_Obj.LumBol = ToLuminosity3(BaseMBol);

		return _Obj;
	}
};

Object RandomMainSequenceStar();

class WolfRayetStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	WolfRayetStarModel() : _Mybase() {}
	explicit WolfRayetStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_STL_ASSERT(IsWolfRayet(_Spec), ("\"" + _Spec.str() + "\" is not a Wolf-Rayet star type."));
	}

	explicit WolfRayetStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();

		// Locating params
		STPARS BaseParams;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.randint(_Par.param()[0][StarTableCoeffs::SpT], _Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]);
		}
		_STL_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");
		_STL_ASSERT(SType <= _Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");

		size_t i = 0;
		while
		(
			i < _Par.tablesize() - 1 &&
			!(SType >= _Par.param()[i][StarTableCoeffs::SpT] && SType < _Par.param()[i + 1][StarTableCoeffs::SpT])
		)
		{++i;}
		BaseParams = _Par.param()[i];

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun];
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * _Eng.normal(BaseMass, BaseMass * 0.2 / 3.);

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun];
		_Obj.Dimensions = vec3(_Eng.normal(BaseRadius, BaseRadius * 0.15 / 3.) * 2. * RadSol);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff];
		_Obj.Teff = _Eng.normal(BaseTEff, BaseTEff * 0.2 / 3.);

		// Luminosities

		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol];
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv];
		_Obj.AbsMagn = _Eng.normal(BaseMBol, 0.01) - BaseBCorr;
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);

		return _Obj;
	}
};

class HPWolfRayetStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	HPWolfRayetStarModel() : _Mybase() {}
	explicit HPWolfRayetStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_STL_ASSERT(IsWolfRayet(_Spec), ("\"" + _Spec.str() + "\" is not a Wolf-Rayet star type."));
	}

	explicit HPWolfRayetStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();

		// Locating params
		STPARS BaseParams;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.randint(_Par.param()[0][StarTableCoeffs::SpT], _Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]);
		}
		_STL_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");
		_STL_ASSERT(SType <= _Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");

		size_t i = 0;
		while
			(
				i < _Par.tablesize() - 1 &&
				!(SType >= _Par.param()[i][StarTableCoeffs::SpT] && SType < _Par.param()[i + 1][StarTableCoeffs::SpT])
				)
		{
			++i;
		}
		BaseParams = _Par.param()[i];

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun];
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * BaseMass;

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun];
		_Obj.Dimensions = vec3(BaseRadius * 2. * RadSol);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff];
		_Obj.Teff = BaseTEff;

		// Luminosities

		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol];
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv];
		_Obj.AbsMagn = BaseMBol - BaseBCorr;
		_Obj.LumBol = ToLuminosity3(BaseMBol);

		return _Obj;
	}
};

class BrownDwarfModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	BrownDwarfModel() : _Mybase() {}
	explicit BrownDwarfModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_STL_ASSERT(IsBrownDwarf(_Spec), ("\"" + _Spec.str() + "\" is not a substellar object."));
	}

	explicit BrownDwarfModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {} b", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();
		const STPARS* _NextTable;
		size_t _NextTableSize;
		if (_Par.spec().SClass() >= 20 && _Par.spec().SClass() < 22)
		{
			_NextTable = GetStarTable(static_cast<LSTARCLS::SpecClass>(_Par.spec().SClass() + 1), &_NextTableSize);
		}
		else { _NextTable = nullptr; }

		// Locating params
		STPARS BaseParams;
		STPARS NextParams;
		float64 Offset;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.uniform(_Par.param()[0][StarTableCoeffs::SpT], lround(_Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]) + 0.9);
		}
		_STL_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");

		size_t i = 0;
		while
		(
			i < _Par.tablesize() - 1 &&
			!(SType >= _Par.param()[i][StarTableCoeffs::SpT] && SType < _Par.param()[i + 1][StarTableCoeffs::SpT])
		){++i;}
		BaseParams = _Par.param()[i];
		if (i == _Par.tablesize() - 1 && _NextTable)
		{
			NextParams = _NextTable[0];
			NextParams[StarTableCoeffs::SpT] = 10.0;
		}
		else
		{
			NextParams = _Par.param()[i + 1];
			_STL_ASSERT(SType <= NextParams[StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");
		}

		Offset = (SType - BaseParams[StarTableCoeffs::SpT]) / (NextParams[StarTableCoeffs::SpT] - BaseParams[StarTableCoeffs::SpT]);

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun] +
			(NextParams[StarTableCoeffs::MSun] - BaseParams[StarTableCoeffs::MSun]) * Offset;
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * _Eng.normal(BaseMass, 0.005);

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun] +
			(NextParams[StarTableCoeffs::R_RSun] - BaseParams[StarTableCoeffs::R_RSun]) * Offset;
		_Obj.Dimensions = vec3(_Eng.normal(BaseRadius, 0.002) * 2. * RadSol);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff] +
			(NextParams[StarTableCoeffs::Teff] - BaseParams[StarTableCoeffs::Teff]) * Offset;
		_Obj.Teff = BaseTEff;

		// Luminosities

		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol] +
			(NextParams[StarTableCoeffs::Mbol] - BaseParams[StarTableCoeffs::Mbol]) * Offset;
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv] +
			(NextParams[StarTableCoeffs::BCv] - BaseParams[StarTableCoeffs::BCv]) * Offset;
		if (!isinf(BaseBCorr) && !isnan(BaseBCorr)) { _Obj.AbsMagn = BaseMBol - BaseBCorr; }
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);

		return _Obj;
	}
};

class HPBrownDwarfModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	HPBrownDwarfModel() : _Mybase() {}
	explicit HPBrownDwarfModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_STL_ASSERT(IsBrownDwarf(_Spec), ("\"" + _Spec.str() + "\" is not a substellar object."));
	}

	explicit HPBrownDwarfModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {} b", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();
		const STPARS* _NextTable;
		size_t _NextTableSize;
		if (_Par.spec().SClass() >= 20 && _Par.spec().SClass() < 22)
		{
			_NextTable = GetStarTable(static_cast<LSTARCLS::SpecClass>(_Par.spec().SClass() + 1), &_NextTableSize);
		}
		else { _NextTable = nullptr; }

		// Locating params
		STPARS BaseParams;
		STPARS NextParams;
		float64 Offset;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.uniform(_Par.param()[0][StarTableCoeffs::SpT], lround(_Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]) + 0.9);
		}
		_STL_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");

		size_t i = 0;
		while
		(
			i < _Par.tablesize() - 1 &&
			!(SType >= _Par.param()[i][StarTableCoeffs::SpT] && SType < _Par.param()[i + 1][StarTableCoeffs::SpT])
		){++i;}
		BaseParams = _Par.param()[i];
		if (i == _Par.tablesize() - 1 && _NextTable)
		{
			NextParams = _NextTable[0];
			NextParams[StarTableCoeffs::SpT] = 10.0;
		}
		else
		{
			NextParams = _Par.param()[i + 1];
			_STL_ASSERT(SType <= NextParams[StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");
		}

		Offset = (SType - BaseParams[StarTableCoeffs::SpT]) / (NextParams[StarTableCoeffs::SpT] - BaseParams[StarTableCoeffs::SpT]);

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun] +
			(NextParams[StarTableCoeffs::MSun] - BaseParams[StarTableCoeffs::MSun]) * Offset;
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * BaseMass;

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun] +
			(NextParams[StarTableCoeffs::R_RSun] - BaseParams[StarTableCoeffs::R_RSun]) * Offset;
		_Obj.Dimensions = vec3(BaseRadius * 2. * RadSol);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff] +
			(NextParams[StarTableCoeffs::Teff] - BaseParams[StarTableCoeffs::Teff]) * Offset;
		_Obj.Teff = BaseTEff;

		// Luminosities

		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol] +
			(NextParams[StarTableCoeffs::Mbol] - BaseParams[StarTableCoeffs::Mbol]) * Offset;
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv] +
			(NextParams[StarTableCoeffs::BCv] - BaseParams[StarTableCoeffs::BCv]) * Offset;
		if (!isinf(BaseBCorr) && !isnan(BaseBCorr)){ _Obj.AbsMagn = BaseMBol - BaseBCorr; }
		_Obj.LumBol = ToLuminosity3(BaseMBol);

		return _Obj;
	}
};

class SubgiantModelBase
{
public:
	using result_type = Object;

	struct param_type
	{
		using model_type = StarModelBase;
		using mass_range = vec2;

		mass_range _MassRange; // Subgiant models are based on mass instead of spectal-type.

		void _Init(mass_range _MRng)
		{
			// set internal state
			_STL_ASSERT(_MRng.x <= _MRng.y && 0 <= _MRng.x,
				"invalid min and max masses for SubGiant Model");
			_MassRange = _MRng;
		}

		_NODISCARD float64 a() const
		{
			return _MassRange.x;
		}

		_NODISCARD float64 b() const
		{
			return _MassRange.y;
		}

		param_type()
		{
			_Init(vec2(0.4, 0.9));
		}

		explicit param_type(float64 _Min0, float64 _Max0)
		{
			_Init(vec2(_Min0, _Max0));
		}
	}_Par;

	SubgiantModelBase() : _Par(float64{ 0.4 }, float64{ 0.9 }) {}

	explicit SubgiantModelBase(float64 _Min0, float64 _Max0) : _Par(_Min0, _Max0) {}

	explicit SubgiantModelBase(const param_type& _Par0) : _Par(_Par0) {}
};

class LowMassSubgiantModel : public SubgiantModelBase
{
public:
	using _Mybase = SubgiantModelBase;
	using mass_type = float64;

	mass_type _Mass = NO_DATA_FLOAT_INF;
	_STD vector<STPARS> MSPars; // Reference Lists

	LowMassSubgiantModel() : _Mybase(0.4, 0.9) { GetMSParams(); }

	LowMassSubgiantModel(mass_type _Mass0) : _Mass(_Mass0)
	{
		_STL_ASSERT(_Mass0 >= 0.4 && _Mass0 <= 0.9, "Mass is out of range.");
		GetMSParams();
	}

	LowMassSubgiantModel(mass_type _Min0, mass_type _Max0) : _Mybase(_Min0, _Max0)
	{
		_STL_ASSERT(_Min0 >= 0.4 && _Max0 <= 0.9, "Masses is out of range.");
		GetMSParams();
	}

	void GetMSParams()
	{
		size_t GTableSize, KTableSize, MTableSize;
		const STPARS* GTable = GetStarTable(LSTARCLS::G, &GTableSize);
		const STPARS* KTable = GetStarTable(LSTARCLS::K, &KTableSize);
		const STPARS* MTable = GetStarTable(LSTARCLS::M, &MTableSize);
		for (size_t i = 0; i < GTableSize; i++)
		{
			if (GTable[i][StarTableCoeffs::MSun] >= 0.4 && GTable[i][StarTableCoeffs::MSun] <= 0.9)
			{
				MSPars.push_back(GTable[i]);
			}
		}

		for (size_t i = 0; i < KTableSize; i++)
		{
			if (KTable[i][StarTableCoeffs::MSun] >= 0.4 && KTable[i][StarTableCoeffs::MSun] <= 0.9)
			{
				MSPars.push_back(KTable[i]);
			}
		}

		for (size_t i = 0; i < MTableSize; i++)
		{
			if (MTable[i][StarTableCoeffs::MSun] >= 0.4 && MTable[i][StarTableCoeffs::MSun] <= 0.9)
			{
				MSPars.push_back(MTable[i]);
			}
		}
	}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {}", _STD make_format_args(_Eng.seed()));

		float64 BaseMass;
		if (isinf(_Mass))
		{
			_Obj.Mass = _Eng.uniform(_Par.a() * MassSol, _Par.b() * MassSol);
		}
		else { _Obj.Mass = _Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;

		/////////////////// Citation needed ///////////////////
		//                                                   //
		// This model has no references to reliable soueces. //
		//                                                   //
		///////////////////////////////////////////////////////

		using ranges_type = _GL gl_vec2<STPARS>;
		_STD vector<ranges_type> Ranges;
		for (size_t i = 0; i < MSPars.size() - 1; i++)
		{
			if ((BaseMass <= MSPars[i][StarTableCoeffs::MSun] && BaseMass >= MSPars[i + 1][StarTableCoeffs::MSun]) ||
				(BaseMass >= MSPars[i][StarTableCoeffs::MSun] && BaseMass <= MSPars[i + 1][StarTableCoeffs::MSun]))
			{
				Ranges.push_back(ranges_type(MSPars[i], MSPars[i + 1]));
			}
		}
		ranges_type FinalRange = _Eng.choice<_STD vector<ranges_type>::iterator, ranges_type>(Ranges.begin(), Ranges.end());
		float64 Offset = (BaseMass - FinalRange.x[StarTableCoeffs::MSun]) / (FinalRange.y[StarTableCoeffs::MSun] - FinalRange.x[StarTableCoeffs::MSun]);

		// Generate base parameters as main-sequence stars
		// Radius
		float64 BaseRadius = FinalRange.x[StarTableCoeffs::R_RSun] +
			(FinalRange.y[StarTableCoeffs::R_RSun] - FinalRange.x[StarTableCoeffs::R_RSun]) * Offset;

		// Temperature
		float64 BaseTEff = FinalRange.x[StarTableCoeffs::Teff] +
			(FinalRange.y[StarTableCoeffs::Teff] - FinalRange.x[StarTableCoeffs::Teff]) * Offset;

		// As this takes place the fusing hydrogen shell gradually expands outward which 
		// increases the size of the outer shell of the star up to the subgiant size 
		// from two to ten times the original radius of the star when it was on the main sequence.
		_Obj.Dimensions = vec3(RadSol * 2. *
			_Eng.uniform(BaseRadius * 2., BaseRadius * 10.));

		// spectral class of the star to change very little in the lower end of this range of star mass.
		// (But how much is unknown)
		_Obj.Teff = BaseTEff - _Eng.uniform(10, 80);

		// Giving Luminosity
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);

		// Giving spectal type for this star
		int _SType = SPECSTR::G;
		size_t it = 0;
		while (it < MSPars.size() - 1 && !(
			(_Obj.Teff < MSPars[it][StarTableCoeffs::Teff] && _Obj.Teff >= MSPars[it + 1][StarTableCoeffs::Teff]) ||
			(_Obj.Teff > MSPars[it][StarTableCoeffs::Teff] && _Obj.Teff <= MSPars[it + 1][StarTableCoeffs::Teff])))
		{
			++it;
			if (MSPars[it][StarTableCoeffs::SpT] > MSPars[it + 1][StarTableCoeffs::SpT])
			{
				++_SType;
			}
		}
		_Obj.SpecClass = SPECSTR(static_cast<SPECSTR::SpecClass>(_SType),
			(SPECSTR::Type)MSPars[it][StarTableCoeffs::SpT], static_cast<SPECSTR::Type>(-1),
			SPECSTR::IV);

		return _Obj;
	}
};

class MidSizedSubgiantModel : public SubgiantModelBase
{
public:
	using _Mybase = SubgiantModelBase;
	using mass_type = float64;

	mass_type _Mass = NO_DATA_FLOAT_INF;
	_STD vector<STPARS> MSPars; // Reference Lists

	MidSizedSubgiantModel() : _Mybase(0.9, 8.0) { GetMSParams(); }

	MidSizedSubgiantModel(mass_type _Mass0) : _Mass(_Mass0)
	{
		_STL_ASSERT(_Mass0 > 0.9 && _Mass0 <= 8.0, "Mass is out of range.");
		GetMSParams();
	}

	MidSizedSubgiantModel(mass_type _Min0, mass_type _Max0) : _Mybase(_Min0, _Max0)
	{
		_STL_ASSERT(_Min0 > 0.9 && _Max0 <= 8.0, "Masses is out of range.");
		GetMSParams();
	}

	void GetMSParams()
	{
		size_t BTableSize, ATableSize, FTableSize, GTableSize;
		const STPARS* BTable = GetStarTable(LSTARCLS::B, &BTableSize);
		const STPARS* ATable = GetStarTable(LSTARCLS::A, &ATableSize);
		const STPARS* FTable = GetStarTable(LSTARCLS::F, &FTableSize);
		const STPARS* GTable = GetStarTable(LSTARCLS::G, &GTableSize);

		for (size_t i = 0; i < BTableSize; i++)
		{
			if ((BTable[i][StarTableCoeffs::MSun] >= 0.9 && BTable[i][StarTableCoeffs::MSun] <= 8.0) ||
				(BTable[i][StarTableCoeffs::MSun] > 8.0 && 
					(BTable[i + 1][StarTableCoeffs::MSun] >= 0.9 && BTable[i + 1][StarTableCoeffs::MSun] <= 8.0)))
			{
				MSPars.push_back(BTable[i]);
			}
		}

		for (size_t i = 0; i < ATableSize; i++)
		{
			if (ATable[i][StarTableCoeffs::MSun] >= 0.9 && ATable[i][StarTableCoeffs::MSun] <= 8.0)
			{
				MSPars.push_back(ATable[i]);
			}
		}

		for (size_t i = 0; i < FTableSize; i++)
		{
			if (FTable[i][StarTableCoeffs::MSun] >= 0.9 && FTable[i][StarTableCoeffs::MSun] <= 8.0)
			{
				MSPars.push_back(FTable[i]);
			}
		}

		for (size_t i = 0; i < GTableSize; i++)
		{
			if (GTable[i][StarTableCoeffs::MSun] >= 0.9 && GTable[i][StarTableCoeffs::MSun] <= 8.0)
			{
				MSPars.push_back(GTable[i]);
			}
		}
	}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {}", _STD make_format_args(_Eng.seed()));

		float64 BaseMass;
		if (isinf(_Mass))
		{
			_Obj.Mass = _Eng.uniform(_Par.a() * MassSol, _Par.b() * MassSol);
		}
		else { _Obj.Mass = _Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;

		// This model is based on HR Diagram and stellar evolution.

		STPARS _BsPars;
		for (size_t i = 0; i < MSPars.size() - 1; i++)
		{
			std::cout << MSPars[i][StarTableCoeffs::MSun] << '\n';
			if ((BaseMass < MSPars[i][StarTableCoeffs::MSun] && BaseMass >= MSPars[i + 1][StarTableCoeffs::MSun]) ||
				(BaseMass > MSPars[i][StarTableCoeffs::MSun] && BaseMass <= MSPars[i + 1][StarTableCoeffs::MSun]))
			{
				_BsPars = MSPars[i + 1];
				break;
			}
		}

		const float64 TemperatureLowLimit = _Eng.normal(5000, 80);
		const float64 MagnitudeLowLimit = _Eng.normal(2.4, 0.015);

		float64 BaseTEff;
		if ((_BsPars[StarTableCoeffs::Teff] - TemperatureLowLimit) <= 50)
		{
			BaseTEff = _BsPars[StarTableCoeffs::Teff] -
				50 * (_Eng.exponential() / 5.);
		}
		else
		{
			BaseTEff = _BsPars[StarTableCoeffs::Teff] -
				(_BsPars[StarTableCoeffs::Teff] - TemperatureLowLimit) *
				(_Eng.exponential() / 5.);
		}
		_Obj.Teff = BaseTEff;

		STPARS NextPars;
		int _SType = SPECSTR::B;
		size_t it = 0;
		while (it < MSPars.size() - 1 && !(
			(_Obj.Teff < MSPars[it][StarTableCoeffs::Teff] && _Obj.Teff >= MSPars[it + 1][StarTableCoeffs::Teff]) ||
			(_Obj.Teff > MSPars[it][StarTableCoeffs::Teff] && _Obj.Teff <= MSPars[it + 1][StarTableCoeffs::Teff])))
		{
			if (MSPars[it][StarTableCoeffs::SpT] > MSPars[it + 1][StarTableCoeffs::SpT])
			{
				++_SType;
			}
			++it;
		}
		_Obj.SpecClass = SPECSTR(static_cast<SPECSTR::SpecClass>(_SType),
			(SPECSTR::Type)MSPars[it][StarTableCoeffs::SpT], static_cast<SPECSTR::Type>(-1),
			SPECSTR::IV);
		NextPars = MSPars[it];

		float64 BaseMag = NextPars[StarTableCoeffs::Mbol] - NextPars[StarTableCoeffs::BCv] * (_Eng.normal(0, 0.003));
		if (BaseMag - 1 > MagnitudeLowLimit)
		{
			BaseMag = MagnitudeLowLimit;
		}
		else{BaseMag -= 1.;}

		_Obj.AbsMagn = BaseMag;
		_Obj.LumBol = ToLuminosity3(BaseMag + NextPars[StarTableCoeffs::BCv]);
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);

		return _Obj;
	}
};

class MassiveSubgiantModel : public SubgiantModelBase
{
	using _Mybase = SubgiantModelBase;
	using mass_type = float64;
};

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif