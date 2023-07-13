// nth-root function by StellarDX, only for real roots

#include "CSE/Core/CSECore.h"
#include "CSE/Core/IEEE754/IEEE754.h"
#include "CSE/Core/IEEE754/MathFuncs.h"

#include <corecrt_math.h>

_CSE_BEGIN

_Check_return_ _FType __cdecl yroot(_In_ _FType _Expo, _In_ _FType _X)
{
	// Special returns
	if (_Expo == 1) { return _X; }
	if (_Expo == 2) { return _CSE sqrt(_X); }
	if (_Expo == 3) { return _CSE cbrt(_X); }

	#ifdef _QUAD_PRECISION
	#define _HALF_TYPE int64
	#define EXPO 0x7FFF000000000000
	#else
	#define _HALF_TYPE int32_t
	#define EXPO 0x7FF00000
	#endif

	using uint32 = unsigned;
	union half
	{
		float64    _Ref;    // Reference number
		_HALF_TYPE _Hfi[2]; // Half of number, 0 = High, 1 = Low
	}Src, Exp;
	enum Idx { LOW_HALF, HIGH_HALF };

	// Extract bytes
	Src._Ref = _X;
	Exp._Ref = _Expo;

	if ((Src._Hfi[HIGH_HALF] > 0 && Src._Hfi[HIGH_HALF] < EXPO) || // x>0 and not x->0
		(Src._Hfi[HIGH_HALF] == 0 && Src._Hfi[LOW_HALF] != 0))
	{
		return _CSE pow(_X, 1. / _Expo);
	}

	if (_X == 0)
	{
		if (Exp._Hfi[HIGH_HALF] > 0) { return 0; }
		else { return wrtval(Q_NAN_DOUBLE); } // I don't know if negative(th) root of 0 is exist...
	}

	if (Src._Hfi[HIGH_HALF] < 0)
	{
		return -yroot(_Expo, -_X);
	}

	return wrtval(Q_NAN_DOUBLE); // StellarDX: (maybe) unreachable, to make the compiler happy (*£þ3£þ)¨q
}

_CSE_END