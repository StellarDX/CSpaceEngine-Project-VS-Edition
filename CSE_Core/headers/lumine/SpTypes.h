#pragma once

#ifndef __CSE_SP_TYPES__
#define __CSE_SP_TYPES__

#include "../Core/CSECore.h"
#include <array>

_CSE_BEGIN

using STARPROB =
struct _Star_Type_Probabilities
{
	//      Star Types              Default Values from SE
	float64 ProbWolfRayetStar     = 0.05;       // probability of Wolf-Rayet star        among all O-class stars
	float64 ProbCarbonStar        = 0.08;       // probability of Carbon/S-Type star     among all red giant stars
	float64 ProbSolitaryWD        = 0.033;    	// probability of solitary  white dwarf  among all systems
	float64 ProbSolitaryNS        = 0.00025;	// probability of solitary  neutron star among all systems
	float64 ProbSolitaryBH        = 3e-05;		// probability of solitary  black hole   among all systems
	float64 ProbBrownDwarf        = 0.025;		// brown dwarf probability
};

class _Stellar_Classification
{
public:
	enum SpecClass
	{
		O, B, A, F, G, K, M, // Normal Classes
		WN, WNE, WNL, Ofpe, WNh, WNha, WNC, WC, WCE, WCL, WCd, WO, Of_p, // Hot blue emission star classes
		L, T, Y, // Cool red and brown dwarf classes
		C_R, C_N, C_J, C_H, C_Hd, S, MS, SC, // Late giant carbon-star classes
		DA, DB, DO, DQ, DZ, DC, DX, // White dwarf classifications
		Q, X, // Stellar remnants
		Exotic // Exotic stars
	};

	_STD array<_STD string, 42> _SpClassFmtStrings =
	{
		"O{:.1g}", "B{:.1g}", "A{:.1g}", "F{:.1g}", "G{:.1g}", "K{:.1g}", "M{:.1g}",
		"WN{:.0g}", "WN{:.0g}E", "WN{:.0g}L", "Ofpe", "WN{:.0g}h", "WN{:.0g}ha", "WNC{:.0g}", "WC{:.0g}", "WC{:.0g}E", "WC{:.0g}L", "WC{:.0g}d", "WO{:.0g}", "Of{:.0g}p",
		"L{:.1g}", "T{:.1g}", "Y{:.1g}",
		"C{:.1g}", "N{:.1g}", "J{:.1g}", "H{:.1g}", "Hd{:.1g}", "S{:.1g}", "MS{:.1g}", "SC{:.1g}",
		"DA{:.1g}", "DB{:.1g}", "DO{:.1g}", "DQ{:.1g}", "DZ{:.1g}", "DC{:.1g}", "DX{:.1g}", "WD",
		"Q", "X",
		"Exotic"
	};

	using Type = float;

	enum LumClass
	{I0, Ia, Iab, Ib, II, III, IV, V, sd, WD};

	_STD array<_STD string, 10> _LumClassStrings =
	{
		"0", "Ia", "Iab", "Ib", "II", "III", "IV", "V", "sd", "WD"
	};

	using ExtData = _STD string;

	_STD array<uint64, 7> _MAIN_SEQUENCE_WEIGHTS = // Calculated among 3000000 stars and converted to integers
   // O,   B,      A,       F,        G,        K,        M
	{ 143, 536250, 2681250, 13000000, 33000000, 53625000, 326157357 };
};

class spectum : public _Stellar_Classification
{
public:
	// Main Spectal Type
	SpecClass Cls = static_cast<SpecClass>(-1);
	Type TyMax = static_cast<Type>(-1);
	Type TyMin = static_cast<Type>(-1);
	LumClass LumMax = static_cast<LumClass>(-1);
	LumClass LumMin = static_cast<LumClass>(-1);
	ExtData Data;

	// Second Spectal Type
	SpecClass Cls2 = static_cast<SpecClass>(-1);
	Type Ty2Max = static_cast<Type>(-1);
	Type Ty2Min = static_cast<Type>(-1);
	LumClass Lum2Max = static_cast<LumClass>(-1);
	LumClass Lum2Min = static_cast<LumClass>(-1);
	ExtData Data2;

	// Third Spectal Type
	SpecClass Cls3 = static_cast<SpecClass>(-1);
	Type Ty3Max = static_cast<Type>(-1);
	Type Ty3Min = static_cast<Type>(-1);
	LumClass Lum3Max = static_cast<LumClass>(-1);
	LumClass Lum3Min = static_cast<LumClass>(-1);
	ExtData Data3;

public:
	spectum() = default;
	spectum(_STD string _Str);
	spectum
	(
		SpecClass _Cls1, 
		Type _MinTy1 = static_cast<Type>(-1), 
		Type _MaxTy1 = static_cast<Type>(-1),
		LumClass _Lum1Max = static_cast<LumClass>(-1),
		LumClass _Lum1Min = static_cast<LumClass>(-1),
		SpecClass _Cls2 = static_cast<SpecClass>(-1),
		Type _MinTy2 = static_cast<Type>(-1),
		Type _MaxTy2 = static_cast<Type>(-1),
		LumClass _Lum2Max = static_cast<LumClass>(-1),
		LumClass _Lum2Min = static_cast<LumClass>(-1),
		SpecClass _Cls3 = static_cast<SpecClass>(-1),
		Type _MinTy3 = static_cast<Type>(-1),
		Type _MaxTy3 = static_cast<Type>(-1),
		LumClass _Lum3Max = static_cast<LumClass>(-1),
		LumClass _Lum3Min = static_cast<LumClass>(-1)
	) : 
		Cls(_Cls1),
		TyMin(_MinTy1),
		TyMax(_MaxTy1),
		LumMax(_Lum1Max),
		LumMin(_Lum1Min),
		Cls2(_Cls2),
		Ty2Min(_MinTy2),
		Ty2Max(_MaxTy2),
		Lum2Max(_Lum2Max),
		Lum2Min(_Lum2Max),
		Cls3(_Cls3),
		Ty3Min(_MinTy3),
		Ty3Max(_MaxTy3),
		Lum3Max(_Lum3Max),
		Lum3Min(_Lum3Min)
	{};

	_STD string str();
	operator _STD string() { return this->str(); }

private:
	_CONSTEXPR20 void SingleSpectumParse(_STD string _SingleStr, SpecClass* _Cls, SpecClass* _Cls2, Type* _TyMax, Type* _TyMin, LumClass* _LumMax, LumClass* _LumMin, ExtData* _Data);
	void MultipleSpectumParse(_STD string _MultiStr);
};

// ----------RED GIANTS---------- //

bool IsGiant(spectum _Spec); // III, II, I, 0
bool IsNormalGiant(spectum _Spec); // III
bool IsBrightGiant(spectum _Spec); // II
bool IsSuperGiant(spectum _Spec); // I
bool IsHyperGiant(spectum _Spec); // 0

// ----------SUBGIANTS---------- //

bool IsSubGiant(spectum _Spec); // IV

// ----------MAIN SEQUENCES---------- //

bool IsMainSequence(spectum _Spec); // V

// ----------SUBDWARFS---------- //

bool IsSubDwarf(spectum _Spec); // VI

// ----------COLORS---------- //

bool IsOType(spectum _Spec); // O
bool IsBType(spectum _Spec); // B
bool IsAType(spectum _Spec); // A
bool IsFType(spectum _Spec); // F
bool IsGType(spectum _Spec); // G
bool IsKType(spectum _Spec); // K
bool IsMType(spectum _Spec); // M

// ----------WOLF RAYET---------- //

bool IsWolfRayet(spectum _Spec); // WR

// ----------BROWN DWARFS---------- //

bool IsBrownDwarf(spectum _Spec); // L, T, Y
bool IsLType(spectum _Spec); // L
bool IsTType(spectum _Spec); // T
bool IsYType(spectum _Spec); // Y

// ----------CARBON STARS---------- //

bool IsCarbonStar(spectum _Spec); // C, S, ...

// ----------STAR REMNANTS---------- //

bool IsWhiteDwarf(spectum _Spec);
bool IsNeutronStar(spectum _Spec);
bool IsBlackHole(spectum _Spec);
bool IsStarRemnant(spectum _Spec);

_CSE_END

#endif