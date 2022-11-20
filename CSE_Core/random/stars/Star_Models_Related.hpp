#pragma once

#ifndef _CSE_PROC_STAR_RLT
#define _CSE_PROC_STAR_RLT

#include "Star_Models_Base.hpp"

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

//===========================================================================//
//                                                                           //
//                              Other Models                                 //
//                                                                           //
//===========================================================================//

class BrownDwarfModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;
	_STD string ModelName = "Brown Dwarf Model";

	BrownDwarfModel() : _Mybase("L2.5V") {}
	explicit BrownDwarfModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsBrownDwarf(_Spec), ("\"" + _Spec.str() + "\" is not a substellar object."));
	}

	explicit BrownDwarfModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
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
		) {++i;}
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
		_CSE_GEN_LOG("INFO", "DONE");

		_CSE_GEN_LOG("INFO", "Generating data...");

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun] +
			(NextParams[StarTableCoeffs::MSun] - BaseParams[StarTableCoeffs::MSun]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of mass: {}", _STD make_format_args(BaseMass)));
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * _Eng.normal(BaseMass, 0.005);

		// Radius
		float64 BaseRadius = BaseParams[StarTableCoeffs::R_RSun] +
			(NextParams[StarTableCoeffs::R_RSun] - BaseParams[StarTableCoeffs::R_RSun]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of radius: {}", _STD make_format_args(BaseRadius)));
		_Obj.Dimensions = vec3(_Eng.normal(BaseRadius, 0.002) * 2. * RadSol);

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
		if (!isinf(BaseBCorr) && !isnan(BaseBCorr)) { _Obj.AbsMagn = BaseMBol - BaseBCorr; }
		_Obj.LumBol = ToLuminosity1(_Obj.Radius(), _Obj.Teff);
		_CSE_GEN_LOG("INFO", "DONE");

		return _Obj;
	}
};

class HPBrownDwarfModel : public StarModelBase
{
public:
	using _Mybase = StarModelBase;
	_STD string ModelName = "High-presision Brown Dwarf Model";

	HPBrownDwarfModel() : _Mybase("L2.5V") {}
	explicit HPBrownDwarfModel(SPECSTR _Spec) : _Mybase(_Spec)
	{
		_GENERATOR_ASSERT(IsBrownDwarf(_Spec), ("\"" + _Spec.str() + "\" is not a substellar object."));
	}

	explicit HPBrownDwarfModel(_Mybase::param_type _Par2) : _Mybase(_Par2) {}

	template <class _Engine> // Procedural star generator
	result_type operator()(_CSE_Random_Engine<_Engine> _Eng)
	{
		_CSE_GEN_LOG("INFO", "Starting generation...");
		_CSE_GEN_LOG("INFO", _STD vformat("Generator Seed: 0x{:X}", _STD make_format_args(_Eng.seed())));
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
		else
		{
			NextParams = _Par.param()[i + 1];
			_GENERATOR_ASSERT(SType <= NextParams[StarTableCoeffs::SpT], "Sub-spectal type is too late for this main type.");
		}

		Offset = (SType - BaseParams[StarTableCoeffs::SpT]) / (NextParams[StarTableCoeffs::SpT] - BaseParams[StarTableCoeffs::SpT]);
		_CSE_GEN_LOG("INFO", "DONE");

		_CSE_GEN_LOG("INFO", "Generating data...");

		// Masses
		float64 BaseMass = BaseParams[StarTableCoeffs::MSun] +
			(NextParams[StarTableCoeffs::MSun] - BaseParams[StarTableCoeffs::MSun]) * Offset;
		_CSE_GEN_LOG("INFO", _STD vformat("Base value of mass: {}", _STD make_format_args(BaseMass)));
		// The actual values for a star may vary by as much as 20-30% from the values listed in tables
		_Obj.Mass = MassSol * BaseMass;

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
		if (!isinf(BaseBCorr) && !isnan(BaseBCorr)) { _Obj.AbsMagn = BaseMBol - BaseBCorr; }
		_Obj.LumBol = ToLuminosity3(BaseMBol);
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
