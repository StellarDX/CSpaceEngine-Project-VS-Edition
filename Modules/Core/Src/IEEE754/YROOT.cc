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
	if (_Expo == 2) { return _CSE __sqrt(_X); }
	if (_Expo == 3) { return _CSE __cbrt(_X); }
	return __pow(_X, 1. / _Expo);
}

_CSE_END