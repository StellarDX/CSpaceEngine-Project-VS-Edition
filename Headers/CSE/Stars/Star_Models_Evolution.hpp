#pragma once

#ifndef _CSE_PROC_STAR_EVO
#define _CSE_PROC_STAR_EVO

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
//                       Evolution-based Star Models                         //
//                                                                           //
//===========================================================================//

// ---------- Main-sequence stars ---------- //

class MainSequenceStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;
	_STD string ModelName = "Main Sequence Star Model";

	MainSequenceStarModel() : _Mybase() {}
	explicit MainSequenceStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsMainSequence(_Spec), ("\"" + _Spec.str() + "\" is not a main-sequence star type."));
	}

	explicit MainSequenceStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

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
		const STPARS* _NextTable;
		size_t _NextTableSize;
		if (_Par.spec().SClass() < 6)
		{
			_NextTable = GetStarTable(static_cast<LSTARCLS::SpecClass>(_Par.spec().SClass() + 1), &_NextTableSize);
		}
		else { _NextTable = nullptr; }

		// Locating params
		_CSE_GEN_LOG("INFO", "Loading Parameters...");
		STPARS BaseParams;
		STPARS NextParams;
		float64 Offset;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();
		if (SType == -1)
		{
			SType = (float)_Eng.uniform(_Par.param()[0][StarTableCoeffs::SpT], lround(_Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]) + 0.9);
			_CSE_GEN_LOG("WARNING", "Sub-type is unknown, suggesting \"" + SPECSTR(SClass, SType, -1, SPECSTR::V).str() + "\" instead of \"" + _Par.spec().str() + "\"");
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
		_CSE_GEN_LOG("INFO", "DONE");

		_CSE_GEN_LOG("INFO", "Generating data...");

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun] +
			(NextParams[StarTableCoeffs::MSun] - BaseParams[StarTableCoeffs::MSun]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of mass: {}", _STD make_format_args(BaseMass)));
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * _Eng.normal(BaseMass, BaseMass * 0.2 / 3.);

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun] +
			(NextParams[StarTableCoeffs::R_RSun] - BaseParams[StarTableCoeffs::R_RSun]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of radius: {}", _STD make_format_args(BaseRadius)));
		_Obj.Dimensions = vec3(_Eng.normal(BaseRadius, BaseRadius * 0.15 / 3.) * 2. * RadSol);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff] +
			(NextParams[StarTableCoeffs::Teff] - BaseParams[StarTableCoeffs::Teff]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of temperature: {}", _STD make_format_args(BaseTEff)));
		_Obj.Teff = _Eng.normal(BaseTEff, BaseTEff * 0.2 / 3.);

		// Luminosities
		_CSE_GEN_LOG("INFO", "Calculating luminosities...");
		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol] +
			(NextParams[StarTableCoeffs::Mbol] - BaseParams[StarTableCoeffs::Mbol]) * Offset;
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv] +
			(NextParams[StarTableCoeffs::BCv] - BaseParams[StarTableCoeffs::BCv]) * Offset;
		_Obj.AbsMagn = _Eng.normal(BaseMBol, 0.01) - BaseBCorr;
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

class HPMainSequenceStarModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;
	_STD string ModelName = "High-Precision Main Sequence Star Model";

	HPMainSequenceStarModel() : _Mybase() {}
	explicit HPMainSequenceStarModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsMainSequence(_Spec), ("\"" + _Spec.str() + "\" is not a main-sequence star type."));
	}

	explicit HPMainSequenceStarModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

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
		const STPARS* _NextTable;
		size_t _NextTableSize;
		if (_Par.spec().SClass() < 6)
		{
			_NextTable = GetStarTable(static_cast<LSTARCLS::SpecClass>(_Par.spec().SClass() + 1), &_NextTableSize);
		}
		else { _NextTable = nullptr; }

		// Locating params
		_CSE_GEN_LOG("INFO", "Loading Parameters...");
		STPARS BaseParams;
		STPARS NextParams;
		float64 Offset;

		auto SClass = _Par.spec().SClass();
		auto SType = _Par.spec().MinType();

		if (SType == -1)
		{
			SType = (float)_Eng.uniform(_Par.param()[0][StarTableCoeffs::SpT], lround(_Par.param()[_Par.tablesize() - 1][StarTableCoeffs::SpT]) + 0.9);
			_CSE_GEN_LOG("WARNING", "Sub-type is unknown, suggesting \"" + SPECSTR(SClass, SType, -1, SPECSTR::V).str() + "\" instead of \"" + _Par.spec().str() + "\"");
		}
		_GENERATOR_ASSERT(SType >= _Par.param()[0][StarTableCoeffs::SpT], "Sub-spectal type is too early for this main type.");

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
		else { NextParams = _Par.param()[i + 1]; }

		Offset = (SType - BaseParams[StarTableCoeffs::SpT]) / (NextParams[StarTableCoeffs::SpT] - BaseParams[StarTableCoeffs::SpT]);
		_CSE_GEN_LOG("INFO", "DONE");

		_CSE_GEN_LOG("INFO", "Generating data...");

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun] +
			(NextParams[StarTableCoeffs::MSun] - BaseParams[StarTableCoeffs::MSun]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of mass: {}", _STD make_format_args(BaseMass)));
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * BaseMass;//_Eng.normal(BaseMass, BaseMass * 0.2 / 3.);

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun] +
			(NextParams[StarTableCoeffs::R_RSun] - BaseParams[StarTableCoeffs::R_RSun]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of radius: {}", _STD make_format_args(BaseRadius)));
		_Obj.Dimensions = vec3(BaseRadius * 2. * RadSol);

		// Temperature
		float64 BaseTEff = BaseParams[StarTableCoeffs::Teff] +
			(NextParams[StarTableCoeffs::Teff] - BaseParams[StarTableCoeffs::Teff]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of temperature: {}", _STD make_format_args(BaseTEff)));
		_Obj.Teff = BaseTEff;

		// Luminosities
		_CSE_GEN_LOG("INFO", "Calculating luminosities...");
		float64 BaseMBol = BaseParams[StarTableCoeffs::Mbol] +
			(NextParams[StarTableCoeffs::Mbol] - BaseParams[StarTableCoeffs::Mbol]) * Offset;
		float64 BaseBCorr = BaseParams[StarTableCoeffs::BCv] +
			(NextParams[StarTableCoeffs::BCv] - BaseParams[StarTableCoeffs::BCv]) * Offset;
		_Obj.AbsMagn = BaseMBol - BaseBCorr;
		_Obj.LumBol = ToLuminosity3(BaseMBol);
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

// ---------- Giant Branches ---------- //

#define _RGB_TABLE_TEFF_LOW_LIMIT 2918
#define _BRIGHT_GIANT_MAG_THRESHOLD -1.8

class RedGiantBranch
{
public:
	using result_type = Object;
	_STD string ModelName = "Red Giant Branch Model";

	struct param_type
	{
		using mass_range = vec2;
		using mass_type = float64;

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
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_CSE_GEN_LOG("INFO", "Loading parameters...");
		float64 BaseMass;
		if (isinf(_Par._Mass))
		{
			_CSE_GEN_LOG("WARNING", "Mass is unknown, generating...");
			_Obj.Mass = _Par.a() * MassSol + (_Par.b() * MassSol - _Par.a() * MassSol) *
				(_Eng.exponential() / 5.);
		}
		else { _Obj.Mass = _Par._Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;
		_CSE_GEN_LOG("INFO", _STD vformat("Base mass: {}", _STD make_format_args(BaseMass)));

		_CSE_GEN_LOG("INFO", "Generating temperature...");
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
		_CSE_GEN_LOG("INFO", _STD vformat("Base temperature: {}", _STD make_format_args(BaseTEff)));
		_Obj.Teff = BaseTEff;

		_CSE_GEN_LOG("INFO", "Matching spectal type and bolometric correction...");
		std::pair<SPECSTR, float64> _SP_BC_Base;
		GetGiantParams(BaseTEff, &_SP_BC_Base);

		// Stars with mass between 2MSun and 3Msun will become red clump giants.
		_CSE_GEN_LOG("INFO", "Calculating luminosities...");
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

		_CSE_GEN_LOG("INFO", "Applying spectal type...");
		if (_Obj.AbsMagn < _BRIGHT_GIANT_MAG_THRESHOLD)
		{
			_Obj.SpecClass = SPECSTR(_SP_BC_Base.first.SClass(), _SP_BC_Base.first.MinType(), -1.F, SPECSTR::II);
		}
		else
		{
			_Obj.SpecClass = SPECSTR(_SP_BC_Base.first.SClass(), _SP_BC_Base.first.MinType(), -1.F, SPECSTR::III);
		}

		_CSE_GEN_LOG("INFO", "Calculating radius...");
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

//#undef _RGB_TABLE_TEFF_LOW_LIMIT

class HorizontalBranch // A Hackable model
{
public:
	using result_type = Object;
	_STD string ModelName = "Horizontal Branch Model";

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
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_CSE_GEN_LOG("INFO", "Generating data...");
		GenMass(_Eng, _Obj);
		GenTeff(_Eng, _Obj);
		float64 _BC = GetSpecType(_Obj);
		_Obj.AbsMagn = GenMag(_Eng);
		_Obj.LumBol = ToLuminosity3(_Obj.AbsMagn + _BC);
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		_CSE_GEN_LOG("INFO", "DONE");
		return _Obj;
	}
};

class RedClumpGiantModel : public HorizontalBranch
{
public:
	using _Mybase = HorizontalBranch;
	_STD string ModelName = "Red Clump Giant Model";

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
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_CSE_GEN_LOG("INFO", "Generating data...");
		GenMass(_Eng, _Obj);
		GenTeff(_Eng, _Obj);
		float64 _BC = GetSpecType(_Obj);
		_Obj.AbsMagn = GenMag(_Eng);
		_Obj.LumBol = ToLuminosity3(_Obj.AbsMagn + _BC);
		_Obj.Dimensions = vec3(sqrt(_Obj.LumBol / (4. * CSE_PI * StBConstant * pow(_Obj.Teff, 4.))) * 2.);
		_Obj.FeH = _Eng.uniform(-0.6, +0.4);
		_CSE_GEN_LOG("INFO", "DONE");
		return _Obj;
	}
};

class AsymptoticGiantBranch
{
public:
	using result_type = Object;
	_STD string ModelName = "Asymptotic Giant Branch Model";

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
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_CSE_GEN_LOG("INFO", "Loading parameters...");
		float64 BaseMass;
		if (isinf(_Par._Mass))
		{
			_CSE_GEN_LOG("WARNING", "Mass is unknown, generating...");
			_Obj.Mass = _Par.a() * MassSol + (_Par.b() * MassSol - _Par.a() * MassSol) *
				(_Eng.exponential() / 5.);
		}
		else { _Obj.Mass = _Par._Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;
		_CSE_GEN_LOG("INFO", _STD vformat("Base mass: {}", _STD make_format_args(BaseMass)));

		_CSE_GEN_LOG("INFO", "Generating radius and temperature...");
		_Obj.Dimensions = vec3(_Eng.uniform(40. * RadSol, 500. * RadSol)); // Average is 215
		_Obj.Teff = _Eng.uniform(_RGB_TABLE_TEFF_LOW_LIMIT, 4200);

		_CSE_GEN_LOG("INFO", "Matching spectal type and bolometric correction...");
		std::pair<SPECSTR, float64> _SP_BC_Base;
		GetGiantParams(_Obj.Teff, &_SP_BC_Base);

		_CSE_GEN_LOG("INFO", "Caalculating luminosities...");
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
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

class SuperAsymptoticGiantBranch : public AsymptoticGiantBranch
{
public:
	using _Mybase = AsymptoticGiantBranch;
	_STD string ModelName = "Super Asymptotic Giant Branch Model";

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
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
		result_type _Obj;
		_Obj.Type = "Star";
		_Obj.Name.push_back(_STD vformat("CSE-RS {:X} A", _STD make_format_args(_Eng.seed())));
		_Obj.ParentBody = _STD vformat("CSE-RS {:X}", _STD make_format_args(_Eng.seed()));

		_CSE_GEN_LOG("INFO", "Loading parameters...");
		float64 BaseMass;
		if (isinf(_Par._Mass))
		{
			_CSE_GEN_LOG("WARNING", "Mass is unknown, generating...");
			_Obj.Mass = _Eng.uniform(_Par.a() * MassSol, _Par.b() * MassSol);
		}
		else { _Obj.Mass = _Par._Mass * MassSol; }
		BaseMass = _Obj.Mass / MassSol;
		_CSE_GEN_LOG("INFO", _STD vformat("Base mass: {}", _STD make_format_args(BaseMass)));

		_CSE_GEN_LOG("INFO", "Generating radius and temperature...");
		_Obj.Dimensions = vec3(_Eng.uniform(200. * RadSol, 750. * RadSol)); // [citation needed] maybe the upper limit is larger than 1000
		_Obj.Teff = _Eng.uniform(3000, 4000);

		_CSE_GEN_LOG("INFO", "Matching spectal type and bolometric correction...");
		std::pair<SPECSTR, float64> _SP_BC_Base;
		GetGiantParams(_Obj.Teff, &_SP_BC_Base);

		_CSE_GEN_LOG("INFO", "Caalculating luminosities...");
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
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

#undef _BRIGHT_GIANT_MAG_THRESHOLD

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif