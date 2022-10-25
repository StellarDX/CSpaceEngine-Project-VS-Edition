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

class ObjectGenerationException : public _STD runtime_error
{
public:
	ObjectGenerationException(_STD string _Mesg) : _STD runtime_error(_Mesg) {};
};

#define _GENERATOR_ASSERT(cond, mesg) if (!(cond)) {throw ObjectGenerationException(mesg);}

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
//                       Evolution-based Star Model                          //
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

// ---------- Main-sequence stars ---------- //

class MainSequenceStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	MainSequenceStarModel() : _Mybase() {}
	explicit MainSequenceStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsMainSequence(_Spec), ("\"" + _Spec.str() + "\" is not a main-sequence star type."));
	}

	explicit MainSequenceStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
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
		_GENERATOR_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");

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
		_GENERATOR_ASSERT(IsMainSequence(_Spec), ("\"" + _Spec.str() + "\" is not a main-sequence star type."));
	}

	explicit HPMainSequenceStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
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
		_GENERATOR_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");

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

// ---------- Subgiants ---------- //

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
			_GENERATOR_ASSERT(_MRng.x <= _MRng.y && 0 <= _MRng.x,
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
		_GENERATOR_ASSERT(_Mass0 >= 0.4 && _Mass0 <= 0.9, "Mass is out of range.");
		GetMSParams();
	}

	LowMassSubgiantModel(mass_type _Min0, mass_type _Max0) : _Mybase(_Min0, _Max0)
	{
		_GENERATOR_ASSERT(_Min0 >= 0.4 && _Max0 <= 0.9 && _Min0 < _Max0, "Masses is out of range.");
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
			MSPars.push_back(GTable[i]);
		}

		for (size_t i = 0; i < KTableSize; i++)
		{
			MSPars.push_back(KTable[i]);
		}

		for (size_t i = 0; i < MTableSize; i++)
		{
			MSPars.push_back(MTable[i]);
		}
	}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

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
			if (MSPars[it][StarTableCoeffs::SpT] > MSPars[it + 1][StarTableCoeffs::SpT])
			{
				++_SType;
			}
			++it;
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
		_GENERATOR_ASSERT(_Mass0 > 0.9 && _Mass0 <= 8.0, "Mass is out of range.");
		GetMSParams();
	}

	MidSizedSubgiantModel(mass_type _Min0, mass_type _Max0) : _Mybase(_Min0, _Max0)
	{
		_GENERATOR_ASSERT(_Min0 > 0.9 && _Max0 <= 8.0 && _Min0 < _Max0, "Masses is out of range.");
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
			MSPars.push_back(BTable[i]);
		}

		for (size_t i = 0; i < ATableSize; i++)
		{
			MSPars.push_back(ATable[i]);
		}

		for (size_t i = 0; i < FTableSize; i++)
		{
			MSPars.push_back(FTable[i]);
		}

		for (size_t i = 0; i < GTableSize; i++)
		{
			MSPars.push_back(GTable[i]);
		}
	}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

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

		float64 BaseMag = (NextPars[StarTableCoeffs::Mbol] - NextPars[StarTableCoeffs::BCv]) + (_Eng.normal(0, 0.003));
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
public:
	using _Mybase = SubgiantModelBase;
	using mass_type = float64;

	mass_type _Mass = NO_DATA_FLOAT_INF;
	_STD vector<STPARS> MSPars; // Reference Lists

	MassiveSubgiantModel() : _Mybase(8.0, 12.0) { GetMSParams(); }

	MassiveSubgiantModel(mass_type _Mass0) : _Mass(_Mass0)
	{
		_GENERATOR_ASSERT(_Mass0 > 8.0 && _Mass0 <= 12.0, "Mass is out of range.");
		GetMSParams();
	}

	MassiveSubgiantModel(mass_type _Min0, mass_type _Max0) : _Mybase(_Min0, _Max0)
	{
		_GENERATOR_ASSERT(_Min0 > 8.0 && _Max0 <= 12.0 && _Min0 < _Max0, "Masses is out of range.");
		GetMSParams();
	}

	void GetMSParams()
	{
		size_t BTableSize;
		const STPARS* BTable = GetStarTable(LSTARCLS::B, &BTableSize);

		for (size_t i = 0; i < BTableSize; i++)
		{
			MSPars.push_back(BTable[i]);
		}
	}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		float64 BaseMass;
		if (isinf(_Mass))
		{
			_Obj.Mass = _Eng.uniform(_Par.a() * MassSol, _Par.b() * MassSol);
		}
		else { _Obj.Mass = _Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;

		STPARS _BsPars;
		for (size_t i = 0; i < MSPars.size() - 1; i++)
		{
			if ((BaseMass < MSPars[i][StarTableCoeffs::MSun] && BaseMass >= MSPars[i + 1][StarTableCoeffs::MSun]) ||
				(BaseMass > MSPars[i][StarTableCoeffs::MSun] && BaseMass <= MSPars[i + 1][StarTableCoeffs::MSun]))
			{
				_BsPars = MSPars[i + 1];
				break;
			}
		}

		_Obj.SpecClass = SPECSTR(static_cast<SPECSTR::SpecClass>(1),
			(SPECSTR::Type)_BsPars[StarTableCoeffs::SpT], static_cast<SPECSTR::Type>(-1),
			SPECSTR::IV);
		float64 BaseTEff = _BsPars[StarTableCoeffs::Teff] + _Eng.normal(0, _BsPars[StarTableCoeffs::Teff] * 0.05);
		_Obj.Teff = BaseTEff;

		float64 BaseMag = (_BsPars[StarTableCoeffs::Mbol] - _BsPars[StarTableCoeffs::BCv]) + (_Eng.normal(0, 0.003));
		std::cout << BaseMag << '\n';
		_Obj.AbsMagn = BaseMag - 0.1;
		_Obj.LumBol = ToLuminosity3(_Obj.AbsMagn + _BsPars[StarTableCoeffs::BCv]);
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);

		return _Obj;
	}
};

// ---------- Giants ---------- //

#define _RGB_TABLE_TEFF_LOW_LIMIT 2918
#define _BRIGHT_GIANT_MAG_THRESHOLD -1.8

_Check_return_ uint64
__CRTDECL GetGiantParams(SPECSTR _Spec, vec2* _BC_Teff = nullptr);
void GetGiantParams(float64 _Teff, std::pair<SPECSTR, float64>* _Param);

class RedGiantBranch
{
public:
	using result_type = Object;

	struct param_type
	{
		using mass_range = vec2;
		using mass_type  = float64;

		mass_range _MassRange;
		mass_type _Mass = NO_DATA_FLOAT_INF;

		void _Init(mass_type _Ms)
		{
			// set internal state
			_GENERATOR_ASSERT(_Ms >= 0.4 && _Ms <= 12,
				"invalid mass for RGB Model");
			_Mass = _Ms;
		}

		void _Init(mass_range _MRng)
		{
			// set internal state
			_GENERATOR_ASSERT(_MRng.x <= _MRng.y && 0.4 <= _MRng.x && 12 >= _MRng.y,
				"invalid min and max masses for RGB Model");
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
			_Init(vec2(0.85, 8.0)); // max is 12
		}

		explicit param_type(float64 _Mx0)
		{
			_Init(_Mx0);
		}

		explicit param_type(float64 _Min0, float64 _Max0)
		{
			_Init(vec2(_Min0, _Max0));
		}
	}_Par;

	RedGiantBranch() : _Par() {}

	RedGiantBranch(float64 _Mx0) : _Par(_Mx0) {}

	RedGiantBranch(float64 _Min0, float64 _Max0) : _Par(_Min0, _Max0) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		float64 BaseMass;
		if (isinf(_Par._Mass))
		{
			_Obj.Mass = _Par.a() * MassSol + (_Par.b() * MassSol - _Par.a() * MassSol) * 
				(_Eng.exponential() / 5.);
		}
		else { _Obj.Mass = _Par._Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;

		float64 _TemperatureHighLimit = _Eng.normal(5000, 120);
		float64 _TemperatureLowLimit;
		if (BaseMass > 2)
		{
			_TemperatureLowLimit = _Eng.normal(4000, 150);
		}
		else
		{
			_TemperatureLowLimit = _Eng.normal(3000, 50);
		}
		if (_RGB_TABLE_TEFF_LOW_LIMIT > _TemperatureLowLimit)
		{
			_TemperatureLowLimit = _RGB_TABLE_TEFF_LOW_LIMIT;
		}
		float64 BaseTEff = _Eng.uniform(_TemperatureLowLimit, _TemperatureHighLimit);
		_Obj.Teff = BaseTEff;

		std::pair<SPECSTR, float64> _SP_BC_Base;
		GetGiantParams(BaseTEff, &_SP_BC_Base);

		// Stars with mass between 2MSun and 3Msun will become red clump giants.
		if (BaseMass >= 2 && BaseMass <= 3)
		{
			_Obj.LumBol = _Eng.uniform(SolarLum * 15, SolarLum * 200);
			_Obj.AbsMagn = ToAbsMagn4(_Obj.LumBol) - _SP_BC_Base.second;
		}
		else
		{
			_Obj.AbsMagn = _Eng.uniform(-3, 1);
			_Obj.LumBol = ToLuminosity3(_Obj.AbsMagn + _SP_BC_Base.second);
		}

		if (_Obj.AbsMagn < _BRIGHT_GIANT_MAG_THRESHOLD)
		{
			_Obj.SpecClass = SPECSTR(_SP_BC_Base.first.SClass(), _SP_BC_Base.first.MinType(), -1.F, SPECSTR::II);
		}
		else
		{
			_Obj.SpecClass = SPECSTR(_SP_BC_Base.first.SClass(), _SP_BC_Base.first.MinType(), -1.F, SPECSTR::III);
		}

		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);

		return _Obj;
	}
};

//#undef _RGB_TABLE_TEFF_LOW_LIMIT

class HorizontalBranch // A Hackable model
{
public:
	using result_type = Object;

	struct param_type
	{
		enum param_mode
		{
			UNIFORM,
			NORMAL
		};
		using param_args = vec2;

		param_mode _MsMode;
		param_mode _RadMode;
		param_mode _MagMode;
		param_mode _TeMode;

		param_args _Masses = vec2(NO_DATA_FLOAT_INF);
		param_args _Radiuses = vec2(NO_DATA_FLOAT_INF);
		param_args _Mags = vec2(NO_DATA_FLOAT_INF);
		param_args _Teffs = vec2(NO_DATA_FLOAT_INF);
		
		void _Init() // Default profile
		{
			MassMode(UNIFORM);
			MassRange(vec2(1.5, 4)); // 0.5 - 8 (Possibly ~12)
			MagMode(UNIFORM);
			MagRange(vec2(-0.26, 0.45));
			TeffMode(UNIFORM);
			TeffRange(vec2(4500, 9500));
		}

		param_mode MassMode(param_mode _New = static_cast<param_mode>(-1))
		{
			param_mode _Old = _MsMode;
			if (_New >= 0) { _MsMode = _New; }
			return _Old;
		}

		param_mode RadMode(param_mode _New = static_cast<param_mode>(-1))
		{
			param_mode _Old = _RadMode;
			if (_New >= 0) { _RadMode = _New; }
			return _Old;
		}

		param_mode MagMode(param_mode _New = static_cast<param_mode>(-1))
		{
			param_mode _Old = _MagMode;
			if (_New >= 0) { _MagMode = _New; }
			return _Old;
		}

		param_mode TeffMode(param_mode _New = static_cast<param_mode>(-1))
		{
			param_mode _Old = _TeMode;
			if (_New >= 0) { _TeMode = _New; }
			return _Old;
		}

		param_args MassRange(param_args _New = vec2(NO_DATA_FLOAT_INF))
		{
			param_args _Old = _Masses;
			if (!any(isinf(_New))) { _Masses = _New; }
			return _Old;
		}

		param_args RadRange(param_args _New = vec2(NO_DATA_FLOAT_INF))
		{
			param_args _Old = _Radiuses;
			if (!any(isinf(_New))) { _Radiuses = _New; }
			return _Old;
		}

		param_args MagRange(param_args _New = vec2(NO_DATA_FLOAT_INF))
		{
			param_args _Old = _Mags;
			if (!any(isinf(_New))) { _Mags = _New; }
			return _Old;
		}

		param_args TeffRange(param_args _New = vec2(NO_DATA_FLOAT_INF))
		{
			param_args _Old = _Teffs;
			if (!any(isinf(_New))) { _Teffs = _New; }
			return _Old;
		}

		param_type() { _Init(); }

		param_type
		(
			param_type::param_mode _Mm0, float64 _Mx0, float64 _Mx1,
			param_type::param_mode _Rm0, float64 _Rx0, float64 _Rx1,
			param_type::param_mode _Lm0, float64 _Lx0, float64 _Lx1,
			param_type::param_mode _Tm0, float64 _Tx0, float64 _Tx1
		)
		{
			MassMode(_Mm0);
			MassRange(vec2(_Mx0, _Mx1));
			RadMode(_Rm0);
			RadRange(vec2(_Rx0, _Rx1));
			MagMode(_Lm0);
			MagRange(vec2(_Lx0, _Lx1));
			TeffMode(_Tm0);
			TeffRange(vec2(_Tx0, _Tx1));
		}
	}_Par;

	HorizontalBranch() : _Par() {}

	HorizontalBranch
	(
		param_type::param_mode _Mm0, float64 _Mx0, float64 _Mx1,
		param_type::param_mode _Rm0, float64 _Rx0, float64 _Rx1,
		param_type::param_mode _Lm0, float64 _Lx0, float64 _Lx1,
		param_type::param_mode _Tm0, float64 _Tx0, float64 _Tx1
	)
		: _Par
		(
			_Mm0, _Mx0, _Mx1,
			_Rm0, _Rx0, _Rx1,
			_Lm0, _Lx0, _Lx1,
			_Tm0, _Tx0, _Tx1
		) {}

	template <class _Engine>
	_Check_return_ float64 __CRTDECL GenMass(_CSE_Random_Engine<_Engine> _Eng, result_type& _Obj)
	{
		if (_Par.MassMode() == param_type::UNIFORM)
		{
			_Obj.Mass = _Eng.uniform(MassSol * _Par.MassRange().x, MassSol * _Par.MassRange().y);
		}
		else if (_Par.MassMode() == param_type::NORMAL)
		{
			_Obj.Mass = _Eng.normal(MassSol * _Par.MassRange().x, MassSol * _Par.MassRange().y);
		}
		return _Obj.Mass / MassSol;
	}

	template <class _Engine>
	_Check_return_ float64 __CRTDECL GenRadius(_CSE_Random_Engine<_Engine> _Eng, result_type& _Obj)
	{
		if (_Par.RadMode() == param_type::UNIFORM)
		{
			_Obj.Dimensions = vec3(_Eng.uniform(RadSol * _Par.RadRange().x, RadSol * _Par.RadRange().y) * 2.);
		}
		else if (_Par.RadMode() == param_type::NORMAL)
		{
			_Obj.Dimensions = vec3(_Eng.normal(RadSol * _Par.RadRange().x, RadSol * _Par.RadRange().y) * 2.);
		}
		return _Obj.Radius() / RadSol;
	}

	template <class _Engine>
	_Check_return_ float64 __CRTDECL GenMag(_CSE_Random_Engine<_Engine> _Eng)
	{
		float64 _Mag;
		if (_Par.MagMode() == param_type::UNIFORM)
		{
			_Mag = _Eng.uniform(_Par.MagRange().x, _Par.MagRange().y);
		}
		else if (_Par.MagMode() == param_type::NORMAL)
		{
			_Mag = _Eng.normal(_Par.MagRange().x, _Par.MagRange().y);
		}
		return _Mag;
	}

	template <class _Engine>
	_Check_return_ float64 __CRTDECL GenTeff(_CSE_Random_Engine<_Engine> _Eng, result_type& _Obj)
	{
		if (_Par.TeffMode() == param_type::UNIFORM)
		{
			_Obj.Teff = _Eng.uniform(_Par.TeffRange().x, _Par.TeffRange().y);
		}
		else if (_Par.TeffMode() == param_type::NORMAL)
		{
			_Obj.Teff = _Eng.normal(_Par.TeffRange().x, _Par.TeffRange().y);
		}
		return _Obj.Teff;
	}

	_Check_return_ float64 __CRTDECL GetSpecType(result_type& _Obj)
	{
		float64 _Teff = _Obj.Teff;
		_STD pair<SPECSTR, float64> _Params;
		GetGiantParams(_Teff, &_Params);
		int _SCls = _Params.first.SClass();
		float _STy = _Params.first.MinType();
		if (_Obj.AbsMagn < _BRIGHT_GIANT_MAG_THRESHOLD)
		{
			_Obj.SpecClass = SPECSTR(static_cast<SPECSTR::SpecClass>(_SCls), _STy, -1.F, SPECSTR::II);
		}
		else
		{
			_Obj.SpecClass = SPECSTR(static_cast<SPECSTR::SpecClass>(_SCls), _STy, -1.F, SPECSTR::III);
		}
		return _Params.second;
	}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		GenMass(_Eng, _Obj);
		GenTeff(_Eng, _Obj);
		float64 _BC = GetSpecType(_Obj);
		_Obj.AbsMagn = GenMag(_Eng);
		_Obj.LumBol = ToLuminosity3(_Obj.AbsMagn + _BC);
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		return _Obj;
	}
};

class RedClumpGiantModel : public HorizontalBranch
{
public:
	using _Mybase = HorizontalBranch;

	RedClumpGiantModel() : _Mybase
	(
		param_type::UNIFORM, 2, 3,
		static_cast<param_type::param_mode>(-1), NO_DATA_FLOAT_INF, NO_DATA_FLOAT_INF, // Radiuses will be automatically calculated
		param_type::NORMAL, 0.81, 0.05,
		param_type::NORMAL, 5000, 80
	) {}

	_Check_return_ float64 __CRTDECL GetSpecType(result_type& _Obj)
	{
		float64 _Teff = _Obj.Teff;
		_STD pair<SPECSTR, float64> _Params;
		GetGiantParams(_Teff, &_Params);
		int _SCls = _Params.first.SClass();
		float _STy = _Params.first.MinType() + 6;
		if (_STy >= 10)
		{
			_STy -= 10;
			++_SCls;
		}

		if (_Obj.AbsMagn < -1.8)
		{
			_Obj.SpecClass = SPECSTR(static_cast<SPECSTR::SpecClass>(_SCls), _STy, -1.F, SPECSTR::II);
		}
		else
		{
			_Obj.SpecClass = SPECSTR(static_cast<SPECSTR::SpecClass>(_SCls), _STy, -1.F, SPECSTR::III);
		}
		return _Params.second;
	}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		GenMass(_Eng, _Obj);
		GenTeff(_Eng, _Obj);
		float64 _BC = GetSpecType(_Obj);
		_Obj.AbsMagn = GenMag(_Eng);
		_Obj.LumBol = ToLuminosity3(_Obj.AbsMagn + _BC);
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		_Obj.FeH = _Eng.uniform(-0.6, +0.4);
		return _Obj;
	}
};

class AsymptoticGiantBranch
{
public:
	using result_type = Object;

	struct param_type
	{
		using mass_range = vec2;
		using mass_type = float64;

		mass_range _MassRange;
		mass_type _Mass = NO_DATA_FLOAT_INF;

		void _Init(mass_type _Ms)
		{
			// set internal state
			_GENERATOR_ASSERT(_Ms >= 0.5 && _Ms <= 8,
				"invalid mass for AGB Model");
			_Mass = _Ms;
		}

		void _Init(mass_range _MRng)
		{
			// set internal state
			_GENERATOR_ASSERT(_MRng.x <= _MRng.y && 0.5 <= _MRng.x && 8 >= _MRng.y,
				"invalid min and max masses for AGB Model");
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
			_Init(vec2(0.85, 8.0));
		}

		explicit param_type(float64 _Mx0)
		{
			_Init(_Mx0);
		}

		explicit param_type(float64 _Min0, float64 _Max0)
		{
			_Init(vec2(_Min0, _Max0));
		}
	}_Par;

	AsymptoticGiantBranch() : _Par() {}

	AsymptoticGiantBranch(float64 _Mx0) : _Par(_Mx0) {}

	AsymptoticGiantBranch(float64 _Min0, float64 _Max0) : _Par(_Min0, _Max0) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		float64 BaseMass;
		if (isinf(_Par._Mass))
		{
			_Obj.Mass = _Par.a() * MassSol + (_Par.b() * MassSol - _Par.a() * MassSol) *
				(_Eng.exponential() / 5.);
		}
		else { _Obj.Mass = _Par._Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;

		_Obj.Dimensions = vec3(_Eng.uniform(40. * RadSol, 500. * RadSol)); // Average is 215
		_Obj.Teff = _Eng.uniform(_RGB_TABLE_TEFF_LOW_LIMIT, 4200);

		std::pair<SPECSTR, float64> _SP_BC_Base;
		GetGiantParams(_Obj.Teff, &_SP_BC_Base);

		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);
		_Obj.AbsMagn = ToAbsMagn4(_Obj.LumBol) - _SP_BC_Base.second;
		if (_Obj.AbsMagn < _BRIGHT_GIANT_MAG_THRESHOLD)
		{
			_Obj.SpecClass = SPECSTR(_SP_BC_Base.first.SClass(), _SP_BC_Base.first.MinType(), -1.F, SPECSTR::II);
		}
		else
		{
			_Obj.SpecClass = SPECSTR(_SP_BC_Base.first.SClass(), _SP_BC_Base.first.MinType(), -1.F, SPECSTR::III);
		}

		return _Obj;
	}
};

class SuperAsymptoticGiantBranch : public AsymptoticGiantBranch
{
public:
	using _Mybase = AsymptoticGiantBranch;

	struct param_type : _Mybase::param_type
	{
		void _Init(mass_type _Ms)
		{
			// set internal state
			_GENERATOR_ASSERT(_Ms >= 7.5 && _Ms <= 9.25,
				"invalid mass for SAGB Model");
			_Mass = _Ms;
		}

		void _Init(mass_range _MRng)
		{
			// set internal state
			_GENERATOR_ASSERT(_MRng.x <= _MRng.y && 7.5 <= _MRng.x && 9.25 >= _MRng.y,
				"invalid min and max masses for AGB Model");
			_MassRange = _MRng;
		}

		param_type()
		{
			_Init(vec2(7.5, 9.25));
		}

		explicit param_type(float64 _Mx0)
		{
			_Init(_Mx0);
		}

		explicit param_type(float64 _Min0, float64 _Max0)
		{
			_Init(vec2(_Min0, _Max0));
		}
	}_Par;

	SuperAsymptoticGiantBranch() : _Par() {}

	SuperAsymptoticGiantBranch(float64 _Mx0) : _Par(_Mx0) {}

	SuperAsymptoticGiantBranch(float64 _Min0, float64 _Max0) : _Par(_Min0, _Max0) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		float64 BaseMass;
		if (isinf(_Par._Mass))
		{
			_Obj.Mass = _Eng.uniform(_Par.a() * MassSol, _Par.b() * MassSol);
		}
		else { _Obj.Mass = _Par._Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;

		_Obj.Dimensions = vec3(_Eng.uniform(200. * RadSol, 750. * RadSol)); // [citation needed] maybe the upper limit is larger than 1000
		_Obj.Teff = _Eng.uniform(3000, 4000);

		std::pair<SPECSTR, float64> _SP_BC_Base;
		GetGiantParams(_Obj.Teff, &_SP_BC_Base);

		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);
		_Obj.AbsMagn = ToAbsMagn4(_Obj.LumBol) - _SP_BC_Base.second;
		if (_Obj.AbsMagn < _BRIGHT_GIANT_MAG_THRESHOLD)
		{
			_Obj.SpecClass = SPECSTR(_SP_BC_Base.first.SClass(), _SP_BC_Base.first.MinType(), -1.F, SPECSTR::II);
		}
		else
		{
			_Obj.SpecClass = SPECSTR(_SP_BC_Base.first.SClass(), _SP_BC_Base.first.MinType(), -1.F, SPECSTR::III);
		}

		return _Obj;
	}
};

#undef _BRIGHT_GIANT_MAG_THRESHOLD

//===========================================================================//
//                                                                           //
//                       Classification-based Models                         //
//                                                                           //
//===========================================================================//

// ---------- Subswarfs ---------- //

#define _STAR_RADIUS_LOW_LIMIT 0.84 * RadJupiter

class CoolSubdwarfModel: public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	// Radius - protection:
	// If on, the star radius lower than the low limit of star radius will be force limited.
	// Default is ttrue
	#define _STAR_RADIUS_PROTECTION true

	CoolSubdwarfModel() : _Mybase("sdM") {}
	explicit CoolSubdwarfModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsSubDwarf(_Spec) && ((_Spec.SClass() >= 4 || _Spec.SClass() <= 6) || (_Spec.SClass() >= 20 || _Spec.SClass() <= 22)),
			("Invalid type for subdwarf model \"" + _Spec.str() + "\""));
	}

	explicit CoolSubdwarfModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		// Generate a main-sequence star first
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();
		const STPARS* _NextTable;
		size_t _NextTableSize;
		if (_Par.spec().SClass() < 6)
		{
			_NextTable = GetStarTable(static_cast<LSTARCLS::SpecClass>(_Par.spec().SClass() + 1), &_NextTableSize);
		}
		else if (_Par.spec().SClass() >= 20 && _Par.spec().SClass() < 22)
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
		_GENERATOR_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");

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

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff] +
			(NextParams[StarTableCoeffs::Teff] - BaseParams[StarTableCoeffs::Teff]) * Offset;
		_Obj.Teff = _Eng.normal(BaseTEff, BaseTEff * 0.2 / 3.);

		// Luminosities

		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol] +
			(NextParams[StarTableCoeffs::Mbol] - BaseParams[StarTableCoeffs::Mbol]) * Offset;
		BaseMBol += _Eng.uniform(1.5, 2); // 1.5 to 2 magnitudes lower than that of main - sequence stars of the same spectral type.
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv] +
			(NextParams[StarTableCoeffs::BCv] - BaseParams[StarTableCoeffs::BCv]) * Offset;
		if (!isinf(BaseBCorr) && !isnan(BaseBCorr)) { _Obj.AbsMagn = _Eng.normal(BaseMBol, 0.01) - BaseBCorr; }
		_Obj.LumBol = ToLuminosity3(BaseMBol);

		// Radius
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		#if _STAR_RADIUS_PROTECTION
		if (_Obj.Radius() < _STAR_RADIUS_LOW_LIMIT)
		{
			_Obj.Dimensions = vec3(_Eng.uniform(_STAR_RADIUS_LOW_LIMIT, 1.2 * RadJupiter) * 2.);
		}
		#endif

		return _Obj;
	}
};

#undef _STAR_RADIUS_LOW_LIMIT
#undef _STAR_RADIUS_PROTECTION

using SubdwarfBStarModel =
class ExtremeHorizontalBranch : public HorizontalBranch
{
public:
	using _Mybase = HorizontalBranch;

	ExtremeHorizontalBranch() : _Mybase
	(
		param_type::NORMAL, 0.5, 0.01,
		param_type::UNIFORM, 0.15, 0.25,
		static_cast<param_type::param_mode>(-1), NO_DATA_FLOAT_INF, NO_DATA_FLOAT_INF,
		param_type::UNIFORM, 20000, 40000
	) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_Obj.SpecClass = "sdB";
		GenMass(_Eng, _Obj);
		GenRadius(_Eng, _Obj);
		GenTeff(_Eng, _Obj);
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);
		return _Obj;
	}
};

// ---------- Wolf-Rayet stars ---------- //

class WolfRayetStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	WolfRayetStarModel() : _Mybase("WN5h") {}
	explicit WolfRayetStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsWolfRayet(_Spec), ("\"" + _Spec.str() + "\" is not a Wolf-Rayet star type."));
	}

	explicit WolfRayetStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();

		// Locating params
		STPARS BaseParams;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.randint(_Par.param()[0][StarTableCoeffs::SpT], _Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]);
		}
		_GENERATOR_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");
		_GENERATOR_ASSERT(SType <= _Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");

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

	HPWolfRayetStarModel() : _Mybase("WN5h") {}
	explicit HPWolfRayetStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsWolfRayet(_Spec), ("\"" + _Spec.str() + "\" is not a Wolf-Rayet star type."));
	}

	explicit HPWolfRayetStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();

		// Locating params
		STPARS BaseParams;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.randint(_Par.param()[0][StarTableCoeffs::SpT], _Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]);
		}
		_GENERATOR_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");
		_GENERATOR_ASSERT(SType <= _Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");

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

//===========================================================================//
//                                                                           //
//                              Other Models                                 //
//                                                                           //
//===========================================================================//

class BrownDwarfModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;

	BrownDwarfModel() : _Mybase("L2.5V") {}
	explicit BrownDwarfModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsBrownDwarf(_Spec), ("\"" + _Spec.str() + "\" is not a substellar object."));
	}

	explicit BrownDwarfModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} b", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
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
		_GENERATOR_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");

		size_t i = 0;
		while
			(
				i < _Par.tablesize() - 1 &&
				!(SType >= _Par.param()[i][StarTableCoeffs::SpT] && SType < _Par.param()[i + 1][StarTableCoeffs::SpT])
				) {
			++i;
		}
		BaseParams = _Par.param()[i];
		if (i == _Par.tablesize() - 1 && _NextTable)
		{
			NextParams = _NextTable[0];
			NextParams[StarTableCoeffs::SpT] = 10.0;
		}
		else
		{
			NextParams = _Par.param()[i + 1];
			_GENERATOR_ASSERT(SType <= NextParams[StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");
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

	HPBrownDwarfModel() : _Mybase("L2.5V") {}
	explicit HPBrownDwarfModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsBrownDwarf(_Spec), ("\"" + _Spec.str() + "\" is not a substellar object."));
	}

	explicit HPBrownDwarfModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} b", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
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
		_GENERATOR_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");

		size_t i = 0;
		while
			(
				i < _Par.tablesize() - 1 &&
				!(SType >= _Par.param()[i][StarTableCoeffs::SpT] && SType < _Par.param()[i + 1][StarTableCoeffs::SpT])
				) {
			++i;
		}
		BaseParams = _Par.param()[i];
		if (i == _Par.tablesize() - 1 && _NextTable)
		{
			NextParams = _NextTable[0];
			NextParams[StarTableCoeffs::SpT] = 10.0;
		}
		else
		{
			NextParams = _Par.param()[i + 1];
			_GENERATOR_ASSERT(SType <= NextParams[StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");
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
		if (!isinf(BaseBCorr) && !isnan(BaseBCorr)) { _Obj.AbsMagn = BaseMBol - BaseBCorr; }
		_Obj.LumBol = ToLuminosity3(BaseMBol);

		return _Obj;
	}
};

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif