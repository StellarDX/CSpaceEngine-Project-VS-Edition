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
		_STL_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");
		if (SType == -1)
		{
			SType = (float)_Eng.uniform(_Par.param()[0][StarTableCoeffs::SpT], lround(_Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]) + 0.9);
		}

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

class HRMainSequenceStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	HRMainSequenceStarModel() : _Mybase() {}
	explicit HRMainSequenceStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_STL_ASSERT(IsMainSequence(_Spec), ("\"" + _Spec.str() + "\" is not a main-sequence star type."));
	}

	explicit HRMainSequenceStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

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

class HRWolfRayetStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	HRWolfRayetStarModel() : _Mybase() {}
	explicit HRWolfRayetStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_STL_ASSERT(IsWolfRayet(_Spec), ("\"" + _Spec.str() + "\" is not a Wolf-Rayet star type."));
	}

	explicit HRWolfRayetStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

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

Object RandomMainSequenceStar();

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif