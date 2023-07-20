#pragma once

#ifndef _CSE_PROC_STAR_CLS
#define _CSE_PROC_STAR_CLS

#include "CSE/Stars/Star_Models_Base.hpp"
#include "CSE/Stars/Star_Models_Evolution.hpp"

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

//===========================================================================//
//                                                                           //
//                       Classification-based Models                         //
//                                                                           //
//===========================================================================//

// ---------- Main-sequence stars ---------- //

// Main-Sequence star probabilities (O, B, A, F, G, K, M)
// Calculated among 3000000 stars and converted to integers
#define _MAIN_SEQUENCE_WEIGHTS { 143, 536250, 2681250, 13000000, 33000000, 53625000, 326157357 } 
Object RandomMainSequenceStar();

// ---------- Subswarfs ---------- //

#define _STAR_RADIUS_LOW_LIMIT 0.84 * RadJupiter

class CoolSubdwarfModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;
	_STD string ModelName = "Cool Subdwarf Model";

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
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
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
		_CSE_GEN_LOG("INFO", "Loading parameters...");
		STPARS BaseParams;
		STPARS NextParams;
		float64 Offset;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.uniform(_Par.param()[0][StarTableCoeffs::SpT], lround(_Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]) + 0.9);
			_CSE_GEN_LOG("WARNING", "Sub-type is unknown, suggesting \"" + SPECSTR(SClass, SType, -1, SPECSTR::sd).str() + "\" instead of \"" + _Par.spec().str() + "\"");

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
		else { NextParams = _Par.param()[i + 1]; }

		Offset = (SType - BaseParams[StarTableCoeffs::SpT]) / (NextParams[StarTableCoeffs::SpT] - BaseParams[StarTableCoeffs::SpT]);
		_CSE_GEN_LOG("INFO", "DONE");

		_CSE_GEN_LOG("INFO", "Generating data...");

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun] +
			(NextParams[StarTableCoeffs::MSun] - BaseParams[StarTableCoeffs::MSun]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of mass: {}", _STD make_format_args(BaseMass)));
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * _Eng.normal(BaseMass, BaseMass * 0.2 / 3.);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff] +
			(NextParams[StarTableCoeffs::Teff] - BaseParams[StarTableCoeffs::Teff]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of temperature: {}", _STD make_format_args(BaseTEff)));
		_Obj.Teff = _Eng.normal(BaseTEff, BaseTEff * 0.2 / 3.);

		// Luminosities
		_CSE_GEN_LOG("INFO", "Calculating luminosities...");
		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol] +
			(NextParams[StarTableCoeffs::Mbol] - BaseParams[StarTableCoeffs::Mbol]) * Offset;
		BaseMBol += _Eng.uniform(1.5, 2); // 1.5 to 2 magnitudes lower than that of main - sequence stars of the same spectral type.
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv] +
			(NextParams[StarTableCoeffs::BCv] - BaseParams[StarTableCoeffs::BCv]) * Offset;
		if (!isinf(BaseBCorr) && !isnan(BaseBCorr)) { _Obj.AbsMagn = _Eng.normal(BaseMBol, 0.01) - BaseBCorr; }
		_Obj.LumBol = ToLuminosity3(BaseMBol);

		// Radius
		_CSE_GEN_LOG("INFO", "Calculating radius...");
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
#if _STAR_RADIUS_PROTECTION
		if (_Obj.Radius() < _STAR_RADIUS_LOW_LIMIT)
		{
			_CSE_GEN_LOG("WARNING", "Calculated radius is too small!");
			_Obj.Dimensions = vec3(_Eng.uniform(_STAR_RADIUS_LOW_LIMIT, 1.2 * RadJupiter) * 2.);
		}
#endif
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

#undef _STAR_RADIUS_LOW_LIMIT
#undef _STAR_RADIUS_PROTECTION

using SubdwarfOStarModel =
class ExtendedHorizontalBranchO : public HorizontalBranch
{
public:
	using _Mybase = HorizontalBranch;
	_STD string ModelName = "Extended Horizontal Brunch Model";

	ExtendedHorizontalBranchO() : _Mybase
	(
		param_type::NORMAL, 0.5, 0.01,
		static_cast<param_type::param_mode>(-1), NO_DATA_FLOAT_INF, NO_DATA_FLOAT_INF,
		static_cast<param_type::param_mode>(-1), NO_DATA_FLOAT_INF, NO_DATA_FLOAT_INF,
		param_type::UNIFORM, 40000, 100000
	) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_CSE_GEN_LOG("INFO", "Generating data...");
		_Obj.SpecClass = "sdO";
		GenMass(_Eng, _Obj);
		GenTeff(_Eng, _Obj);

		float64 GravAccel = pow(10, _Eng.uniform(4, 6.5)) / 100; // convert to m/s^2
		_Obj.Dimensions = vec3(sqrt((GravConstant * _Obj.Mass) / GravAccel) * 2.);
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);
		_CSE_GEN_LOG("INFO", "DONE");
		return _Obj;
	}
};

using SubdwarfBStarModel =
class ExtremeHorizontalBranch : public HorizontalBranch
{
public:
	using _Mybase = HorizontalBranch;
	_STD string ModelName = "Extreme Horizontal Brunch Model";

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
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_CSE_GEN_LOG("INFO", "Generating data...");
		_Obj.SpecClass = "sdB";
		GenMass(_Eng, _Obj);
		GenRadius(_Eng, _Obj);
		GenTeff(_Eng, _Obj);
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);
		_CSE_GEN_LOG("INFO", "DONE");
		return _Obj;
	}
};

// ---------- Wolf-Rayet stars ---------- //

class WolfRayetStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;
	_STD string ModelName = "Wolf-Rayet Star Model";

	WolfRayetStarModel() : _Mybase("WN5h") {}
	explicit WolfRayetStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsWolfRayet(_Spec), ("\"" + _Spec.str() + "\" is not a Wolf-Rayet star type."));
	}

	explicit WolfRayetStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

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

		// Locating params
		_CSE_GEN_LOG("INFO", "Loading parameters...");
		STPARS BaseParams;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.randint((int64)_Par.param()[0][StarTableCoeffs::SpT], (int64)_Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]);
			_CSE_GEN_LOG("WARNING", "Sub-type is unknown, suggesting \"" + SPECSTR(SClass, SType).str() + "\" instead of \"" + _Par.spec().str() + "\"");
		}
		_GENERATOR_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");
		_GENERATOR_ASSERT(SType <= _Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");

		size_t i = 0;
		while
		(
			i < _Par.tablesize() - 1 &&
			!(SType >= _Par.param()[i][StarTableCoeffs::SpT] && SType < _Par.param()[i + 1][StarTableCoeffs::SpT])
		){++i;}
		BaseParams = _Par.param()[i];
		_CSE_GEN_LOG("INFO", "DONE");

		_CSE_GEN_LOG("INFO", "Generating data...");

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun];
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of mass: {}", _STD make_format_args(BaseMass)));
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * _Eng.normal(BaseMass, BaseMass * 0.2 / 3.);

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun];
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of radius: {}", _STD make_format_args(BaseRadius)));
		_Obj.Dimensions = vec3(_Eng.normal(BaseRadius, BaseRadius * 0.15 / 3.) * 2. * RadSol);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff];
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of temperature: {}", _STD make_format_args(BaseTEff)));
		_Obj.Teff = _Eng.normal(BaseTEff, BaseTEff * 0.2 / 3.);

		// Luminosities
		_CSE_GEN_LOG("INFO", "Calculating luminosities...");
		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol];
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv];
		_Obj.AbsMagn = _Eng.normal(BaseMBol, 0.01) - BaseBCorr;
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

class HPWolfRayetStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;
	_STD string ModelName = "High-Precision Wolf-Rayet Star Model";

	HPWolfRayetStarModel() : _Mybase("WN5h") {}
	explicit HPWolfRayetStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsWolfRayet(_Spec), ("\"" + _Spec.str() + "\" is not a Wolf-Rayet star type."));
	}

	explicit HPWolfRayetStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

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

		// Locating params
		_CSE_GEN_LOG("INFO", "Loading parameters...");
		STPARS BaseParams;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.randint((int64)_Par.param()[0][StarTableCoeffs::SpT], (int64)_Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]);
			_CSE_GEN_LOG("WARNING", "Sub-type is unknown, suggesting \"" + SPECSTR(SClass, SType).str() + "\" instead of \"" + _Par.spec().str() + "\"");
		}
		_GENERATOR_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");
		_GENERATOR_ASSERT(SType <= _Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");

		size_t i = 0;
		while
		(
			i < _Par.tablesize() - 1 &&
			!(SType >= _Par.param()[i][StarTableCoeffs::SpT] && SType < _Par.param()[i + 1][StarTableCoeffs::SpT])
		){++i;}
		BaseParams = _Par.param()[i];
		_CSE_GEN_LOG("INFO", "DONE");

		_CSE_GEN_LOG("INFO", "Generating data...");

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun];
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of mass: {}", _STD make_format_args(BaseMass)));
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * BaseMass;

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun];
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of radius: {}", _STD make_format_args(BaseRadius)));
		_Obj.Dimensions = vec3(BaseRadius * 2. * RadSol);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff];
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of temperature: {}", _STD make_format_args(BaseTEff)));
		_Obj.Teff = BaseTEff;

		// Luminosities
		_CSE_GEN_LOG("INFO", "Calculating luminosities...");
		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol];
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv];
		_Obj.AbsMagn = BaseMBol - BaseBCorr;
		_Obj.LumBol = ToLuminosity3(BaseMBol);
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

// ---------- OB Stars ---------- //

Object RandomOBStar();

// ---------- Subgiants ---------- //

class LowMassSubgiantModel : public SubgiantModelBase
{
public:
	using _Mybase = SubgiantModelBase;
	using mass_type = float64;
	_STD string ModelName = "Low-mass Subgiant Model";

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
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_CSE_GEN_LOG("INFO", "Loading parameters...");
		float64 BaseMass;
		if (isinf(_Mass))
		{
			_CSE_GEN_LOG("WARNING", "Mass is unknown, generating...");
			_Obj.Mass = _Eng.uniform(_Par.a() * MassSol, _Par.b() * MassSol);
		}
		else { _Obj.Mass = _Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;
		_CSE_GEN_LOG("INFO", _STD vformat("Base mass: {}", _STD make_format_args(BaseMass)));

		/////////////////// Citation needed ///////////////////
		//                                                   //
		// This model has no references to reliable soueces. //
		//                                                   //
		///////////////////////////////////////////////////////

		_CSE_GEN_LOG("INFO", "Matching main-sequence parameters...");
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
		_CSE_GEN_LOG("INFO", "DONE");

		// Generate base parameters as main-sequence stars
		_CSE_GEN_LOG("INFO", "Generating data...");
		// Radius
		float64 BaseRadius = FinalRange.x[StarTableCoeffs::R_RSun] +
			(FinalRange.y[StarTableCoeffs::R_RSun] - FinalRange.x[StarTableCoeffs::R_RSun]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base radius: {}", _STD make_format_args(BaseRadius)));
		// Temperature
		float64 BaseTEff = FinalRange.x[StarTableCoeffs::Teff] +
			(FinalRange.y[StarTableCoeffs::Teff] - FinalRange.x[StarTableCoeffs::Teff]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base temperature: {}", _STD make_format_args(BaseTEff)));

		// As this takes place the fusing hydrogen shell gradually expands outward which 
		// increases the size of the outer shell of the star up to the subgiant size 
		// from two to ten times the original radius of the star when it was on the main sequence.
		_CSE_GEN_LOG("INFO", "Calculating radius...");
		_Obj.Dimensions = vec3(RadSol * 2. *
			_Eng.uniform(BaseRadius * 2., BaseRadius * 10.));

		// spectral class of the star to change very little in the lower end of this range of star mass.
		// (But how much is unknown)
		_CSE_GEN_LOG("INFO", "Calculating temperature...");
		_Obj.Teff = BaseTEff - _Eng.uniform(10, 80);

		// Giving Luminosity
		_CSE_GEN_LOG("INFO", "Calculating luminosity...");
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);

		// Giving spectal type for this star
		_CSE_GEN_LOG("INFO", "Matching spectal type...");
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
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

class MidSizedSubgiantModel : public SubgiantModelBase
{
public:
	using _Mybase = SubgiantModelBase;
	using mass_type = float64;
	_STD string ModelName = "Mid-sized Subgiant Model";

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
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_CSE_GEN_LOG("INFO", "Loading parameters...");
		float64 BaseMass;
		if (isinf(_Mass))
		{
			_CSE_GEN_LOG("WARNING", "Mass is unknown, generating...");
			_Obj.Mass = _Eng.uniform(_Par.a() * MassSol, _Par.b() * MassSol);
		}
		else { _Obj.Mass = _Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;
		_CSE_GEN_LOG("INFO", _STD vformat("Base mass: {}", _STD make_format_args(BaseMass)));

		// This model is based on HR Diagram and stellar evolution.

		_CSE_GEN_LOG("INFO", "Matching main-sequence parameters...");
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
		_CSE_GEN_LOG("INFO", "DONE");

		const float64 TemperatureLowLimit = _Eng.normal(5000, 80);
		const float64 MagnitudeLowLimit = _Eng.normal(2.4, 0.015);
		_CSE_GEN_LOG("INFO", "Generating data...");

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
		_CSE_GEN_LOG("INFO", _STD vformat("Base temperature: {}", _STD make_format_args(BaseTEff)));

		_CSE_GEN_LOG("INFO", "Matching spectal type...");
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
		_CSE_GEN_LOG("INFO", "DONE");

		_CSE_GEN_LOG("INFO", "Calculating...");
		float64 BaseMag = (NextPars[StarTableCoeffs::Mbol] - NextPars[StarTableCoeffs::BCv]) + (_Eng.normal(0, 0.003));
		if (BaseMag - 1 > MagnitudeLowLimit)
		{
			BaseMag = MagnitudeLowLimit;
		}
		else { BaseMag -= 1.; }

		_Obj.AbsMagn = BaseMag;
		_Obj.LumBol = ToLuminosity3(BaseMag + NextPars[StarTableCoeffs::BCv]);
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

class MassiveSubgiantModel : public SubgiantModelBase
{
public:
	using _Mybase = SubgiantModelBase;
	using mass_type = float64;
	_STD string ModelName = "Massive Subgiant Model";

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
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_CSE_GEN_LOG("INFO", "Loading parameters...");
		float64 BaseMass;
		if (isinf(_Mass))
		{
			_CSE_GEN_LOG("WARNING", "Mass is unknown, generating...");
			_Obj.Mass = _Eng.uniform(_Par.a() * MassSol, _Par.b() * MassSol);
		}
		else { _Obj.Mass = _Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;
		_CSE_GEN_LOG("INFO", _STD vformat("Base mass: {}", _STD make_format_args(BaseMass)));

		_CSE_GEN_LOG("INFO", "Matching main-sequence parameters...");
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
		_CSE_GEN_LOG("INFO", "DONE");

		_CSE_GEN_LOG("INFO", "Matching spectal type...");
		_Obj.SpecClass = SPECSTR(static_cast<SPECSTR::SpecClass>(1),
			(SPECSTR::Type)_BsPars[StarTableCoeffs::SpT], static_cast<SPECSTR::Type>(-1),
			SPECSTR::IV);
		float64 BaseTEff = _BsPars[StarTableCoeffs::Teff] + _Eng.normal(0, _BsPars[StarTableCoeffs::Teff] * 0.05);
		_Obj.Teff = BaseTEff;
		_CSE_GEN_LOG("INFO", _STD vformat("Base temperature: {}", _STD make_format_args(BaseTEff)));

		_CSE_GEN_LOG("INFO", "Calculating...");
		float64 BaseMag = (_BsPars[StarTableCoeffs::Mbol] - _BsPars[StarTableCoeffs::BCv]) + (_Eng.normal(0, 0.003));
		_Obj.AbsMagn = BaseMag - 0.1;
		_Obj.LumBol = ToLuminosity3(_Obj.AbsMagn + _BsPars[StarTableCoeffs::BCv]);
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

// ---------- Giants ---------- //

class BlueGiantModel
{
public:
	using result_type = Object;
	_STD string ModelName = "Blue Giant Model";

	struct param_type
	{
		SPECSTR _Spectum;
		const _STD array<float64, 4>* _TableParams;
		size_t _TableSize;

		void _Init(SPECSTR _Spec)
		{
			_Spectum = _Spec;
			_TableParams = GetOBGiantsTable(&_TableSize);
		}

		param_type()
		{
			_Init("B2III");
		}

		param_type(SPECSTR _Spec)
		{
			_Init(_Spec);
		}

		SPECSTR spec()const { return _Spectum; }
		const _STD array<float64, 4>* param()const { return _TableParams; }
		size_t tablesize()const { return _TableSize; }
	}_Par;

	BlueGiantModel() : _Par() {}

	explicit BlueGiantModel(SPECSTR _Spec) : _Par(_Spec)
	{
		_GENERATOR_ASSERT((IsNormalGiant(_Spec) || IsBrightGiant(_Spec)) &&
			(_Spec.SClass() == SPECSTR::O || _Spec.SClass() == SPECSTR::B),
			("\"" + _Spec.str() + "\" is not a blue-giant star type."));
	}

	explicit BlueGiantModel(param_type _Par2) : _Par(_Par2) {}

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

		_CSE_GEN_LOG("INFO", "Loading Parameters...");
		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		float64 MaxTy = 10;
		float64 MinTy = 0;
		for (size_t i = 0; i < _Par.tablesize(); i++)
		{
			if ((int64)_Par.param()[i][0] == SClass)
			{
				MaxTy = min(MaxTy, _Par.param()[i][1]);
				MinTy = max(MinTy, _Par.param()[i][1]);
			}
		}
		if (SType == -1)
		{
			SType = (float)_Eng.uniform(MaxTy, MinTy);
			_CSE_GEN_LOG("WARNING", "Sub-type is unknown, suggesting \"" + SPECSTR(SClass, SType, -1, SPECSTR::III).str() + "\" instead of \"" + _Par.spec().str() + "\"");
		}
		_GENERATOR_ASSERT(SType >= MaxTy, "Sub-spectal type is too early for this main type.");
		_CSE_GEN_LOG("INFO", "DONE");

		_CSE_GEN_LOG("INFO", "Generating data...");
		float64 BaseMass;
		float64 BaseAbsMagn;
		float64 Offset;
		_STD array<float64, 4> This;
		_STD array<float64, 4> Next;
		bool _Found = false;
		for (size_t i = 0; i < _Par.tablesize() - 1 && !_Found; i++)
		{
			This = _Par.param()[i];
			Next = _Par.param()[i + 1];
			if (SType <= Next[1] && SType >= This[1])
			{
				if ((int64)This[0] == SClass && (int64)Next[0] == SClass)
				{
					Offset = (SType - This[1]) / (Next[1] - This[1]);
					_Found = true;
				}
			}
			else if (Next[1] < This[1])
			{
				if ((int64)This[0] == SClass && (int64)Next[0] == SClass + 1)
				{
					float64 _MTy = SType;
					if (_MTy == Next[1]) { _MTy = 10; }
					Offset = (_MTy - This[1]) / (10 - This[1]);
					_Found = true;
				}
			}
		}

		BaseMass = This[3] + (Next[3] - This[3]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base mass: {}", _STD make_format_args(BaseMass)));
		BaseAbsMagn = This[2] + (Next[2] - This[2]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base absolute magnitude: {}", _STD make_format_args(BaseAbsMagn)));

		_CSE_GEN_LOG("INFO", "Matching temperature and bolometric correction...");
		vec2 _BC_Teff;
		GetGiantParams(SPECSTR(SClass, SType), &_BC_Teff);

		if (SClass == 1)
		{
			BaseMass += _Eng.normal(0, 0.8);
			if (BaseMass < 2.8) { BaseMass = _Eng.normal(2.8, 0.1); }
		}
		else if (SClass == 0)
		{
			BaseMass += _Eng.uniform(-6, 50);
		}

		BaseAbsMagn += _Eng.normal(0, 0.25);

		_CSE_GEN_LOG("INFO", "Applyig...");
		_Obj.Mass = BaseMass * MassSol;
		_Obj.AbsMagn = BaseAbsMagn;
		_Obj.Teff = _BC_Teff.y + _Eng.normal(0, 50);
		_Obj.LumBol = ToLuminosity3(BaseAbsMagn + _BC_Teff.x);
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

// ---------- Supergiants ---------- //

class SupergiantModel
{
public:
	using result_type = Object;
	_STD string ModelName = "Supergiant Model";

	struct param_type
	{
		SPECSTR _Spectum;
		_STD vector<_STD array<float64, 8>> _TableParams;

		void _Init(SPECSTR _Spec)
		{
			_Spectum = _Spec;
			_TableParams = GetSGTable(_Spec.SClass());
		}

		param_type()
		{
			_Init("F5Ib");
		}

		param_type(SPECSTR _Spec)
		{
			_Init(_Spec);
		}

		SPECSTR spec()const { return _Spectum; }
		const _STD vector<_STD array<float64, 8>> param()const { return _TableParams; }
	}_Par;

	SupergiantModel() : _Par() {}

	explicit SupergiantModel(SPECSTR _Spec) : _Par(_Spec)
	{
		_GENERATOR_ASSERT(IsSuperGiant(_Spec), _Spec.str() + " is not a supergiant spectum.");
	}

	explicit SupergiantModel(param_type _Par2) : _Par(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		// Supergiants are evolved from massive O/B-type stars
		// The list below is Schematic evolution of stars by initial mass (at solar metallicity)
		// 8 - 15 MSun : B -> RSG -> SN II-P
		// 15 - 20 MSun : O -> RSG -> YHG -> BSG(Blue-loop) -> YHG -> RSG -> SN II-L/IIb
		// 20 - 45 MSun : O -> RSG -> WNE -> WC -> SN Ib
		// 45 - 60 MSun : O -> WNh -> LBV/WNE -> WO -> SN Ib/Ic
		// 60 - 120 MSun : O -> Of -> WNh -> LBV -> WNL -> SN IIn
		// 120 - 140 MSun : WNh(O0-O1 type stars) -> WN -> WC -> WO -> SN Ic
		// 140 - 250 MSun : WNh(O0-O1 type stars) -> WNE -> WO -> SN Ic
		// 250 MSun or higher : Uncertained

		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));
		_Obj.SpecClass = _Par.spec();

		_CSE_GEN_LOG("INFO", "Loading Parameters...");
		float64 MinTy = _Par.spec().MinType();
		if (MinTy == -1)
		{
			size_t MaxIndex;
			if (_Par.spec().SClass() == 6) { MaxIndex = _Par.param().size() - 1; }
			else { MaxIndex = _Par.param().size() - 2; }
			MinTy = _Eng.uniform(_Par.param()[0][1], _Par.param()[MaxIndex][1]);
			_CSE_GEN_LOG("WARNING", std::format("suggesting spectral class {} instead of {}", SPECSTR(_Par.spec().SClass(), (float)MinTy, -1, _Par.spec().MaxLClass()).str(), _Obj.SpecClass));
		}

		_STD array<float64, 8> BaseParams;
		_STD array<float64, 8> NextParams;
		for (size_t i = 0; i < _Par.param().size() - 1; i++)
		{
			if (MinTy >= _Par.param()[i][1])
			{
				BaseParams = _Par.param()[i];
				NextParams = _Par.param()[i + 1];
			}
		}
		float64 Offset = (NextParams[1] - MinTy) / (NextParams[1] - BaseParams[1]);

		_CSE_GEN_LOG("INFO", "Generating data...");

		float64 BaseMass;
		if (_Par.spec().SClass() > SPECSTR::G)
		{
			BaseMass = BaseParams[7] + (BaseParams[7] - NextParams[7]) * Offset;
			BaseMass += (45 - BaseMass) * (_Eng.exponential() / 7.5); // Stars massive than 45 MSun will not produce red-supergiants
		}
		else { BaseMass = BaseParams[7] + (BaseParams[7] - NextParams[7]) * Offset + 75 * (_Eng.exponential() / 7.5); }
		_CSE_GEN_LOG("INFO", _STD vformat("Base mass: {}", _STD make_format_args(BaseParams[7])));
		_Obj.Mass = BaseMass * MassSol;

		float64 BaseTeff = BaseParams[2] + (BaseParams[2] - NextParams[2]) * Offset + _Eng.normal(0, 160);
		_CSE_GEN_LOG("INFO", _STD vformat("Base temperature: {}", _STD make_format_args(BaseParams[2])));
		_Obj.Teff = BaseTeff;

		float64 BaseMag;
		switch (_Par.spec().MaxLClass())
		{
		case LSTARCLS::Ia:
			BaseMag = BaseParams[6] + (BaseParams[6] - NextParams[6]) * Offset;
			break;
		case LSTARCLS::Iab:
			BaseMag = BaseParams[5] + (BaseParams[5] - NextParams[5]) * Offset;
			break;
		case LSTARCLS::Ib:
			BaseMag = BaseParams[4] + (BaseParams[4] - NextParams[4]) * Offset;
			break;
		default:
			break;
		}
		_CSE_GEN_LOG("INFO", _STD vformat("Base magnitude: {}", _STD make_format_args(BaseMag)));
		BaseMag += _Eng.uniform(0, 1);
		_Obj.AbsMagn = BaseMag;
		_Obj.LumBol = ToLuminosity3(BaseMag + BaseParams[3]);
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		if (_Obj.Radius() / RadSol > 2500)
		{
			_Obj.Dimensions = vec3(_Eng.normal(2500, 50) * RadSol * 2.);
		}
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

Object RandomBlueSupergiant();

Object RandomRedSupergiant();

Object RandomYellowSupergiant();

class HypergiantModel
{
public:
	using result_type = Object;
	_STD string ModelName = "Hypergiant Model";

	struct param_type
	{
		SPECSTR _Spectum;
		_STD vector<_STD array<float64, 8>> _TableParams;

		void _Init(SPECSTR _Spec)
		{
			_Spectum = _Spec;
			_TableParams = GetSGTable(_Spec.SClass());
		}

		param_type()
		{
			_Init("A0 0");
		}

		param_type(SPECSTR _Spec)
		{
			_Init(_Spec);
		}

		SPECSTR spec()const { return _Spectum; }
		const _STD vector<_STD array<float64, 8>> param()const { return _TableParams; }
	}_Par;

	HypergiantModel() : _Par() {}

	explicit HypergiantModel(SPECSTR _Spec) : _Par(_Spec)
	{
		_GENERATOR_ASSERT(IsHyperGiant(_Spec), _Spec.str() + " is not a hypergiant spectum.");
	}

	explicit HypergiantModel(param_type _Par2) : _Par(_Par2) {}

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

		_CSE_GEN_LOG("INFO", "Loading Parameters...");
		float64 MinTy = _Par.spec().MinType();
		if (MinTy == -1)
		{
			size_t MaxIndex;
			if (_Par.spec().SClass() == 6) { MaxIndex = _Par.param().size() - 1; }
			else { MaxIndex = _Par.param().size() - 2; }
			MinTy = _Eng.uniform(_Par.param()[0][1], _Par.param()[MaxIndex][1]);
			_CSE_GEN_LOG("WARNING", std::format("suggesting spectral class {} instead of {}", SPECSTR(_Par.spec().SClass(), (float)MinTy, -1, _Par.spec().MaxLClass()).str(), _Obj.SpecClass));
		}

		_STD array<float64, 8> BaseParams;
		_STD array<float64, 8> NextParams;
		for (size_t i = 0; i < _Par.param().size() - 1; i++)
		{
			if (MinTy >= _Par.param()[i][1])
			{
				BaseParams = _Par.param()[i];
				NextParams = _Par.param()[i + 1];
			}
		}
		float64 Offset = (NextParams[1] - MinTy) / (NextParams[1] - BaseParams[1]);

		_CSE_GEN_LOG("INFO", "Generating data...");

		float64 BaseMass;
		if (_Par.spec().SClass() > SPECSTR::G)
		{
			BaseMass = BaseParams[7] + (BaseParams[7] - NextParams[7]) * Offset;
			BaseMass += (45 - BaseMass) * (_Eng.exponential() / 7.5); // Stars massive than 45 MSun will not produce red-supergiants
		}
		else { BaseMass = BaseParams[7] + (BaseParams[7] - NextParams[7]) * Offset + 75 * (_Eng.exponential() / 7.5); }
		_CSE_GEN_LOG("INFO", _STD vformat("Base mass: {}", _STD make_format_args(BaseParams[7])));
		_Obj.Mass = BaseMass * MassSol;

		float64 BaseTeff = BaseParams[2] + (BaseParams[2] - NextParams[2]) * Offset + _Eng.normal(0, 160);
		_CSE_GEN_LOG("INFO", _STD vformat("Base temperature: {}", _STD make_format_args(BaseParams[2])));
		_Obj.Teff = BaseTeff;

		float64 BaseMag = random.uniform(-10, -7);
		_CSE_GEN_LOG("INFO", _STD vformat("Base magnitude: {}", _STD make_format_args(BaseMag)));
		
		_Obj.AbsMagn = BaseMag;
		_Obj.LumBol = ToLuminosity3(BaseMag + BaseParams[3]);

		_CSE_GEN_LOG("INFO", "Calculating Radius");
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		if (_Obj.Radius() / RadSol > 2500)
		{
			_Obj.Dimensions = vec3(_Eng.normal(2500, 50) * RadSol * 2.);
		}
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

class CarbonStarModel
{
public:
	using result_type = Object;
	_STD string ModelName = "Carbon Star Model";

	struct param_type
	{
		SPECSTR _Spectum;

		void _Init(SPECSTR _Spec)
		{
			_Spectum = _Spec;
		}

		param_type(){_Init("C");}
		param_type(SPECSTR _Spec){_Init(_Spec);}

		SPECSTR spec()const { return _Spectum; }
	}_Par;

	CarbonStarModel() : _Par() {}

	explicit CarbonStarModel(SPECSTR _Spec) : _Par(_Spec)
	{
		_GENERATOR_ASSERT(IsCarbonStar(_Spec), _Spec.str() + " is not a Carbon-Star spectum.");
	}

	explicit CarbonStarModel(param_type _Par2) : _Par(_Par2) {}

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

		float64 BaseMass;
		if (_Par.spec().SClass() >= 28 && _Par.spec().SClass() <= 30)
		{
			BaseMass = _Eng.uniform(1.5, 5);
		}
		else{BaseMass = _Eng.uniform(0.6, 4.5);}
		_CSE_GEN_LOG("INFO", _STD vformat("Base mass: {}", _STD make_format_args(BaseMass)));
		_Obj.Mass = BaseMass * MassSol;

		float64 BaseAbsMagn;
		if (_Par.spec().SClass() >= 28 && _Par.spec().SClass() <= 30)
		{
			_Obj.LumBol = _Eng.uniform(2000 * SolarLum, 10000 * SolarLum);
		}
		else
		{
			switch (_Par.spec().SClass())
			{
			default:
			case SPECSTR::C_R:
				BaseAbsMagn = _Eng.normal(0, 0.3);
				break;
			case SPECSTR::C_N:
				BaseAbsMagn = _Eng.normal(-2.2, 0.5);
				break;
			case SPECSTR::C_J:
				BaseAbsMagn = _Eng.normal(-1.2, 0.35);
				break;
			case SPECSTR::C_H:
				BaseAbsMagn = _Eng.normal(-1.8, 0.38);
				break;
			case SPECSTR::C_Hd:
				BaseAbsMagn = _Eng.normal(-3.5, 0.8);
				break;
			}
			_CSE_GEN_LOG("INFO", _STD vformat("Base magnitude: {}", _STD make_format_args(BaseAbsMagn)));
			_Obj.AbsMagn = BaseAbsMagn;
		}
	
		float64 TEffLowlimit;
		if (_Par.spec().SClass() >= 28 && _Par.spec().SClass() <= 30) {}
		else
		{
			#ifdef _UNLOCK_C_STAR_TEMP_LOW_LIMIT
			TEffLowlimit = 1670 // The coolest known carbon star is V688 Mon, which temperature is 1670
			#else
			switch (_Par.spec().SClass())
			{
			default:
			case SPECSTR::C_R:
				TEffLowlimit = 2800;
				break;
			case SPECSTR::C_N:
				TEffLowlimit = 2600;
				break;
			case SPECSTR::C_J:
				TEffLowlimit = 2800;
				break;
			case SPECSTR::C_H:
				TEffLowlimit = 4100;
				break;
			case SPECSTR::C_Hd:
				TEffLowlimit = 4700; // Uncertained, only 7 known stars is classified as this type and most of them are R CrB variables
				break;
			}
			#endif
		}

		float64 BaseTEff;
		switch (_Par.spec().SClass())
		{
		default:
		case SPECSTR::C_R:
			BaseTEff = _Eng.uniform(TEffLowlimit, 5100);
			break;
		case SPECSTR::C_N:
			BaseTEff = _Eng.uniform(TEffLowlimit, 3100);
			break;
		case SPECSTR::C_J:
			BaseTEff = _Eng.uniform(TEffLowlimit, 3900);
			break;
		case SPECSTR::C_H:
			BaseTEff = _Eng.uniform(TEffLowlimit, 5000);
			break;
		case SPECSTR::C_Hd:
			BaseTEff = _Eng.uniform(TEffLowlimit, 25000); // Uncertained, only 7 known stars is classified as this type and most of them are R CrB variables
			break;
		case SPECSTR::MS:
		case SPECSTR::S:
		case SPECSTR::SC:
			BaseTEff = _Eng.normal(3100, 450);
			break;
		}
		_CSE_GEN_LOG("INFO", _STD vformat("Base temperature: {}", _STD make_format_args(BaseTEff)));
		_Obj.Teff = BaseTEff;

		// Calculate CtoO Rate for S-type stars
		if (_Par.spec().SClass() >= 28 && _Par.spec().SClass() <= 30)
		{
			const float64 _S_STARS_C_O_TABLE[]
			(0.90, 0.95, 0.95, 0.96, 0.97, 0.97, 0.98, 0.99, 1.00, 1.02, 1.10);
			if (_Par.spec().MinType() != -1) { _Obj.CtoO = _S_STARS_C_O_TABLE[int(_Par.spec().MinType())]; }
			else 
			{
				switch (_Par.spec().SClass())
				{
				case SPECSTR::MS:
					_Obj.CtoO = _S_STARS_C_O_TABLE[0];
					break;
				case SPECSTR::S:
				default:
					_Obj.CtoO = _S_STARS_C_O_TABLE[_Eng.randint(1, 6)];
					break;
				case SPECSTR::SC:
					_Obj.CtoO = _S_STARS_C_O_TABLE[_Eng.randint(7, 10)];
					break;
				}
			}
		}

		_CSE_GEN_LOG("INFO", "Calculating Luminosity");
		std::pair<SPECSTR, float64> BC;
		GetGiantParams(BaseTEff, &BC);
		_CSE_GEN_LOG("INFO", _STD vformat("Matching Spectal type: {}", _STD make_format_args(BC.first.str())));
		if (_Par.spec().SClass() >= 28 && _Par.spec().SClass() <= 30)
		{
			_Obj.AbsMagn = ToAbsMagn4(_Obj.LumBol) - BC.second;
		}
		else {_Obj.LumBol = ToLuminosity3(BaseAbsMagn + BC.second);}
		
		_CSE_GEN_LOG("INFO", "Calculating Radius");
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		if (_Obj.Radius() / RadSol > 2500)
		{
			_Obj.Dimensions = vec3(_Eng.normal(2500, 50) * RadSol * 2.);
		}
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif