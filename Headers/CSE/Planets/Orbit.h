#pragma once

#ifndef __ORBIT__
#define __ORBIT__

#include "CSE/Core.h"
#include "CSE/Object.h"
#include "CSE/Random.h"

// Orbit models and functions for CSE

_CSE_BEGIN

class OrbitCalculateException : public _STD runtime_error
{
public:
	OrbitCalculateException(_STD string e) : _STD runtime_error(e) {}
};

// ---------- Orbit Models ---------- //

// Reference:
// Wikipedia. Titius-Bode law
// https://en.wikipedia.org/wiki/Titius_Bode_law
// G. G. Lobban, A. E. Roy and J. C. Brown "A Review Of Blagg's Formula In The Light Of Recently Discovered Planetary Moons And Rings"
// http://articles.adsabs.harvard.edu//full/1982JBAA...92..260L/0000260.000.html
// Wikipedia. Dermott's law
// https://en.wikipedia.org/wiki/Dermott%27s_law
// Arcadio Poveda and Patricia Lara "The exo-planetary system of 55 Cancri and the Titus–Bode law"
// http://www.astroscu.unam.mx/rmaa/RMxAA..44-1/PDF/RMxAA..44-1_apoveda.pdf

struct Titius_Bode_Engine
{
	Titius_Bode_Engine(float64 _Bx0, float64 _Vx0) : Base(_Bx0), Variation(_Vx0) {}

	float64 operator[](int64 _Idx)
	{
		if (_Idx == 0) { return Base; }
		return Base + Variation * pow(2, _Idx - 1.);
	}

private:
	float64 Base;
	float64 Variation;
};

struct Mary_Blagg_Engine
{
	Mary_Blagg_Engine(float64 _Ax0, float64 _Bx0, float64 _Alf, float64 _Bet, int64 _Offset = -2)
		: Ax0(_Ax0), Bx0(_Bx0), Alf(_Alf), Bet(_Bet), Offset(_Offset) {}

	float64 operator[](int64 _Idx)
	{
		int64 n = _Idx + Offset;
		float64 Psi = Alf + n * Bet - 27.5;
		float64 f = 0.249 + 0.86 * ((cse::cos(Psi) / (3. - cse::cos(2. * Psi))) + (1. / (6. - 4. * cse::cos(2. * (Psi - 30.)))));
		return Ax0 * pow(1.7275, (float64)n) * (Bx0 + f);
	}

private:
	float64 Ax0;
	float64 Bx0;
	float64 Alf;
	float64 Bet;
	int64 Offset;
};

struct Stanley_Dermott_Engine
{
	Stanley_Dermott_Engine(float64 _Tx0, float64 _Cx0) : Tx0(_Tx0), Cx0(_Cx0) {}

	float64 operator[](int64 _Idx)
	{
		return Tx0 * pow(Cx0, (float64)_Idx);
	}

private:
	float64 Tx0;
	float64 Cx0;
};

struct Exponential_Fitting_Engine
{
	Exponential_Fitting_Engine(float64 _Cx0, float64 _Cx1) : Cx0(_Cx0), Cx1(_Cx1) {}

	float64 operator[](int64 _Idx)
	{
		return Cx0 * exp(Cx1 * _Idx);
	}

private:
	float64 Cx0;
	float64 Cx1;
};

extern Titius_Bode_Engine     SolarSys_TBL;

extern Mary_Blagg_Engine      SolarSys_BLF;
extern Mary_Blagg_Engine      Jupiter_BLF;
extern Mary_Blagg_Engine      Saturn_BLF;
extern Mary_Blagg_Engine      Uranus_BLF;

extern Stanley_Dermott_Engine Jovian_DML;
extern Stanley_Dermott_Engine Saturnian_DML;
extern Stanley_Dermott_Engine Uranian_DML;

extern Exponential_Fitting_Engine Rho1Cnc_Exp;

// ---------- Two-Line element set ---------- //

// Reference: 
// https://en.wikipedia.org/wiki/Two-line_element_set

class TLE
{
	typedef char TLE_Char[70]; // Add "\0" in the end of string.
	TLE_Char Name;
	TLE_Char Line1;
	TLE_Char Line2;

public:
	struct TLE_Spacecraft // Basic data of the spacecraft, in first line.
	{
		uint32_t    CatalogNumber;
		_STD string Classification;

		struct COSPAR_ID
		{
			int32_t     LaunchYear;
			uint32_t    LaunchNumber;
			_STD string LaunchPiece;
		}IntDesignator;

		float64     BallisticCoeff;
		float64     d2MeanMotion;
		float64     BSTAR;
		uint32_t    EphemerisType;
		uint32_t    ElementSet;
		uint32_t    RevolutionNum;
	};

	TLE() = default;
	TLE(_STD istream& _Input);

	static TLE ParseString(_STD string _Input);

	bool isValid()const;

	_STD string Title()const;
	TLE_Spacecraft BasicData()const;
	OrbitParam Orbit()const;

	_STD string COSPAR();
	void PrintSpacecraftData(_STD ostream& l1str);
};

// ---------- Functions ---------- //

void BinaryOrbitToKeplerianElems(OrbitParam* Par);

/// <summary>
/// Calculate roche limit of two objects
/// </summary>
/// <param name="Primary">Primary object</param>
/// <param name="Companion">Companion object</param>
/// <param name="Mode">Mode of function, 0 = Rigid, 1 = Fluid, default is 0</param>
float64 RocheLimit(const Object* Primary, const Object* Companion, int Mode = 0);

/// <summary>
/// Calculate Hill sphere of two objects
/// </summary>
/// <param name="Primary">Primary object</param>
/// <param name="Companion">Companion object</param>
/// <returns>Hill sphere of the companion</returns>
float64 HillSphere(const Object* Primary, const Object* Companion);

_Check_return_opt_ int __CRTDECL MakeOrbit(Object* Primary, Object* Companion, Object* ThirdGravSourse = nullptr, OrbitParam Args = { .RefPlane = "Extrasolar", .Epoch = J2000 });
_Check_return_ _STD shared_ptr<Object> __CRTDECL MakeBinary(Object* Primary, Object* Companion, Object* ThirdGravSourse = nullptr, OrbitParam Args = { .RefPlane = "Extrasolar", .Epoch = J2000 });

/// <summary>
/// Calculate orbital period using Kepler's 3rd law.
/// </summary>
float64 GetPeriod(float64 CenterObjMass, float64 PericenterDist, float64 Eccentricity);

/// <summary>
/// Calculate Semi-Major Axis using Kepler's 3rd law.
/// </summary>
float64 GetSemiMajorAxis(float64 CenterObjMass, float64 Period);

_CSE_END

#endif