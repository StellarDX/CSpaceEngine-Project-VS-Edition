#pragma once

#ifndef __CSE_SP_TYPES__
#define __CSE_SP_TYPES__

#include "../Core/CSECore.h"
#include <array>
#include <ctype.h>

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

using LSTARCLS =
class _Stellar_Classification
{
public:
	enum SpecClass
	{
		O, B, A, F, G, K, M, // Normal Classes
		WN, WNE, WNL, Ofpe, WNh, WNha, WNC, WC, WCE, WCL, WCd, WO, Of_p, // Hot blue emission star classes
		L, T, Y, // Cool red and brown dwarf classes
		C_R, C_N, C_J, C_H, C_Hd, S, MS, SC, // Late giant carbon-star classes
		DA, DB, DO, DQ, DZ, DC, DX, D,// White dwarf classifications
		Q, X, // Stellar remnants
		Exotic // Exotic stars
	};

	_STD array<_STD string, 42> _SpClassNoFmtStrings =
	{
		"O ", "B ", "A ", "F ", "G ", "K ", "M ",
		"WN", "WNE", "WNL", "Ofpe", "WNh", "WNha", "WNC", "WC", "WCE", "WCL", "WCd", "WO", "Of?p",
		"L ", "T ", "Y ",
		"C", "N", "J", "H", "Hd", "S", "MS", "SC",
		"A", "B", "O", "Q", "Z", "C", "X", "WD",
		"Q", "X",
		"Exotic"
	};

	_STD array<_STD string, 42> _SpClassFmtStrings =
	{
		"O{}", "B{}", "A{}", "F{}", "G{}", "K{}", "M{}",
		"WN{}", "WN{}E", "WN{}L", "Ofpe", "WN{}h", "WN{}ha", "WNC{}", "WC{}", "WC{}E", "WC{}L", "WC{}d", "WO{}", "Of{}p",
		"L{}", "T{}", "Y{}",
		"C{}", "N{}", "J{}", "H{}", "Hd{}", "S{}", "MS{}", "SC{}",
		"DA{}", "DB{}", "DO{}", "DQ{}", "DZ{}", "DC{}", "DX{}", "WD",
		"Q", "X",
		"Exotic"
	};

	using Type = float;

	enum LumClass
	{I0, Ia, Iab, Ib, II, III, IV, V, sd, WD};

	_STD array<_STD string, 10> _LumClassStrings =
	{
		"0", "Ia", "Iab", "Ib", "II", "III", "IV", "V", "VI", "WD"
	};

	using ExtData = _STD string;
};

using SPECSTR =
class spectum : public _Stellar_Classification
{
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
	spectum(const char* _Str);
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
	bool empty(uint8_t _Arg = 0);
	bool NoLumClass(uint8_t _Arg = 0);

	SpecClass SClass(size_t _N = 1)const
	{
		switch (_N)
		{
		case 1:
			return Cls;
			break;
		case 2:
			return Cls2;
			break;
		case 3:
			return Cls3;
			break;
		default:
			return Cls;
			break;
		}
	}

	Type MaxType(size_t _N = 1)const
	{
		switch (_N)
		{
		case 1:
			return TyMax;
			break;
		case 2:
			return Ty2Max;
			break;
		case 3:
			return Ty3Max;
			break;
		default:
			return TyMax;
			break;
		}
	}

	Type MinType(size_t _N = 1)const
	{
		switch (_N)
		{
		case 1:
			return TyMin;
			break;
		case 2:
			return Ty2Min;
			break;
		case 3:
			return Ty3Min;
			break;
		default:
			return TyMin;
			break;
		}
	}

	LumClass MaxLClass(size_t _N = 1)const
	{
		switch (_N)
		{
		case 1:
			return LumMax;
			break;
		case 2:
			return Lum2Max;
			break;
		case 3:
			return Lum3Max;
			break;
		default:
			return LumMax;
			break;
		}
	}

	LumClass MinLClass(size_t _N = 1)const
	{
		switch (_N)
		{
		case 1:
			return LumMin;
			break;
		case 2:
			return Lum2Min;
			break;
		case 3:
			return Lum3Min;
			break;
		default:
			return LumMin;
			break;
		}
	}

private:
	void Parser(_STD string _SpStr);
	_CONSTEXPR20 void SingleSpectumParse(_STD string _SingleStr, SpecClass* _Cls, SpecClass* _Cls2, Type* _TyMax, Type* _TyMin, LumClass* _LumMax, LumClass* _LumMin, ExtData* _Data);
	void MultipleSpectumParse(_STD string _MultiStr);
	_STD string to_str(const SpecClass* _Spec, const Type* _Ty1, const Type* _Ty2, const LumClass* _Lum1, const LumClass* _Lum2, const ExtData* _Data);
	_STD string to_WDstr(const SpecClass* _Spec, const SpecClass* _Spec2, const Type* _Ty1, const Type* _Ty2, const LumClass* _Lum1, const LumClass* _Lum2, const ExtData* _Data);
	_STD string to_sdstr(const SpecClass* _Spec, const Type* _Ty1, const Type* _Ty2, const ExtData* _Data);

public:
	// Friend functions
	friend bool IsGiant(spectum _Spec); // III, II, I, 0
	friend bool IsNormalGiant(spectum _Spec); // III
	friend bool IsBrightGiant(spectum _Spec); // II
	friend bool IsSuperGiant(spectum _Spec); // I
	friend bool IsHyperGiant(spectum _Spec); // 0
	friend bool IsSubGiant(spectum _Spec); // IV
	friend bool IsMainSequence(spectum _Spec); // V
	friend bool IsSubDwarf(spectum _Spec); // VI
	friend bool IsOType(spectum _Spec); // O
	friend bool IsBType(spectum _Spec); // B
	friend bool IsAType(spectum _Spec); // A
	friend bool IsFType(spectum _Spec); // F
	friend bool IsGType(spectum _Spec); // G
	friend bool IsKType(spectum _Spec); // K
	friend bool IsMType(spectum _Spec); // M
	friend bool IsWolfRayet(spectum _Spec); // WR
	friend bool IsBrownDwarf(spectum _Spec); // L, T, Y
	friend bool IsLType(spectum _Spec); // L
	friend bool IsTType(spectum _Spec); // T
	friend bool IsYType(spectum _Spec); // Y
	friend bool IsCarbonStar(spectum _Spec); // C, S, ...
	friend bool IsWhiteDwarf(spectum _Spec);
	friend bool IsNeutronStar(spectum _Spec);
	friend bool IsBlackHole(spectum _Spec);
	friend bool IsStarRemnant(spectum _Spec);

	// Comparing
	bool EarlierThan(const spectum& _Right)const
	{
		// Main-Sequence
		if
		(
			(IsWolfRayet(*this) && IsWolfRayet(_Right)) || 
			(IsCarbonStar(*this) && IsCarbonStar(_Right)) || 
			(IsWhiteDwarf(*this) && IsWhiteDwarf(_Right))
		)
		{
			if (this->SClass() == _Right.SClass())
			{
				return this->MinType() < _Right.MinType();
			}
			return false;
		}
		else if (this->MaxLClass() == _Right.MaxLClass())
		{
			if (this->SClass() == _Right.SClass())
			{
				return this->MinType() < _Right.MinType();
			}
			else
			{
				return this->SClass() < _Right.SClass();
			}
		}
		return false;
	}

	bool EarlierThanEqual(const spectum& _Right)const
	{
		// Main-Sequence
		if
		(
			(IsWolfRayet(*this) && IsWolfRayet(_Right)) ||
			(IsCarbonStar(*this) && IsCarbonStar(_Right)) ||
			(IsWhiteDwarf(*this) && IsWhiteDwarf(_Right))
		)
		{
			if (this->SClass() == _Right.SClass())
			{
				return this->MinType() <= _Right.MinType();
			}
			return false;
		}
		else if (this->MaxLClass() == _Right.MaxLClass())
		{
			if (this->SClass() == _Right.SClass())
			{
				return this->MinType() <= _Right.MinType();
			}
			else
			{
				return this->SClass() <= _Right.SClass();
			}
		}
		return false;
	}

	bool LaterThan(const spectum& _Right)const
	{
		// Main-Sequence
		if
		(
			(IsWolfRayet(*this) && IsWolfRayet(_Right)) || 
			(IsCarbonStar(*this) && IsCarbonStar(_Right)) || 
			(IsWhiteDwarf(*this) && IsWhiteDwarf(_Right))
		)
		{
			if (this->SClass() == _Right.SClass())
			{
				return this->MinType() > _Right.MinType();
			}
			return false;
		}
		else if (this->MaxLClass() == _Right.MaxLClass())
		{
			if (this->SClass() == _Right.SClass())
			{
				return this->MinType() > _Right.MinType();
			}
			else
			{
				return this->SClass() > _Right.SClass();
			}
		}
		return false;
	}

	bool LaterThanEqual(const spectum& _Right)const
	{
		// Main-Sequence
		if
		(
			(IsWolfRayet(*this) && IsWolfRayet(_Right)) ||
			(IsCarbonStar(*this) && IsCarbonStar(_Right)) ||
			(IsWhiteDwarf(*this) && IsWhiteDwarf(_Right))
		)
		{
			if (this->SClass() == _Right.SClass())
			{
				return this->MinType() >= _Right.MinType();
			}
			return false;
		}
		else if (this->MaxLClass() == _Right.MaxLClass())
		{
			if (this->SClass() == _Right.SClass())
			{
				return this->MinType() >= _Right.MinType();
			}
			else
			{
				return this->SClass() >= _Right.SClass();
			}
		}
		return false;
	}

	bool Equal(const spectum& _Right)const
	{
		return
			this->SClass() == _Right.SClass() &&
			this->MinType() == _Right.MinType() &&
			this->MaxLClass() == _Right.MaxLClass();
	}
};

// ----------RED GIANTS---------- //

bool IsGiant(SPECSTR _Spec); // III, II, I, 0
bool IsNormalGiant(SPECSTR _Spec); // III
bool IsBrightGiant(SPECSTR _Spec); // II
bool IsSuperGiant(SPECSTR _Spec); // I
bool IsHyperGiant(SPECSTR _Spec); // 0

// ----------SUBGIANTS---------- //

bool IsSubGiant(SPECSTR _Spec); // IV

// ----------MAIN SEQUENCES---------- //

bool IsMainSequence(SPECSTR _Spec); // V

// ----------SUBDWARFS---------- //

bool IsSubDwarf(SPECSTR _Spec); // VI

// ----------COLORS---------- //

bool IsOType(SPECSTR _Spec); // O
bool IsBType(SPECSTR _Spec); // B
bool IsAType(SPECSTR _Spec); // A
bool IsFType(SPECSTR _Spec); // F
bool IsGType(SPECSTR _Spec); // G
bool IsKType(SPECSTR _Spec); // K
bool IsMType(SPECSTR _Spec); // M

// ----------WOLF RAYET---------- //

bool IsWolfRayet(SPECSTR _Spec); // WR

// ----------BROWN DWARFS---------- //

bool IsBrownDwarf(SPECSTR _Spec); // L, T, Y
bool IsLType(SPECSTR _Spec); // L
bool IsTType(SPECSTR _Spec); // T
bool IsYType(SPECSTR _Spec); // Y

// ----------CARBON STARS---------- //

bool IsCarbonStar(SPECSTR _Spec); // C, S, ...

// ----------STAR REMNANTS---------- //

bool IsWhiteDwarf(SPECSTR _Spec);
bool IsNeutronStar(SPECSTR _Spec);
bool IsBlackHole(SPECSTR _Spec);
bool IsStarRemnant(SPECSTR _Spec);

_CSE_END

#endif