#pragma once

#ifndef _CSE_PROC_STAR_BASE
#define _CSE_PROC_STAR_BASE

#include "CSE/Core.h"
#include "CSE/Lumine.h"
#include "CSE/Random.h"
#include "CSE/Object.h"

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

const _STD array<float64, 4>* GetOBGiantsTable(size_t* Tyc);

_Check_return_ uint64
__CRTDECL GetGiantParams(SPECSTR _Spec, vec2* _BC_Teff = nullptr);
void GetGiantParams(float64 _Teff, std::pair<SPECSTR, float64>* _Param);

_STD vector<_STD array<float64, 8>> GetSGTable();
_STD vector<_STD array<float64, 8>> GetSGTable(LSTARCLS::SpecClass _Cls);

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif