// Power function for double and quaduple-precision floating point
// Designed by StellarDX, both precisions using the same function.

#include "CSE/Core/CSECore.h"
#include "CSE/Core/IEEE754/IEEE754.h"
#include "CSE/Core/IEEE754/MathFuncs.h"

#include <corecrt_math.h>

_CSE_BEGIN

_Check_return_ _FType __cdecl pow(_In_ _FType _X, _In_ _FType _Power)
{
	return __pow(_X, _Power);
}

_EXTERN_C

/* __ieee754_pow(x,y) return x**y
 *
 * Method:  pow(a, b) = exp(b * ln(a))
 *
 * Special cases:
 *	1.  (anything) ** 0  is 1
 *	2.  (anything) ** 1  is itself
 *	3.  (anything) ** NAN is NAN
 *	4.  NAN ** (anything except 0) is NAN
 *	5.  +-(|x| > 1) **  +INF is +INF
 *	6.  +-(|x| > 1) **  -INF is +0
 *	7.  +-(|x| < 1) **  +INF is +0
 *	8.  +-(|x| < 1) **  -INF is +INF
 *	9.  +-1         ** +-INF is NAN
 *	10. +0 ** (+anything except 0, NAN)               is +0
 *	11. -0 ** (+anything except 0, NAN, odd integer)  is +0
 *	12. +0 ** (-anything except 0, NAN)               is +INF
 *	13. -0 ** (-anything except 0, NAN, odd integer)  is +INF
 *	14. -0 ** (odd integer) = -( +0 ** (odd integer) )
 *	15. +INF ** (+anything except 0,NAN) is +INF
 *	16. +INF ** (-anything except 0,NAN) is +0
 *	17. -INF ** (anything)  = -0 ** (-anything)
 *	18. (-anything) ** (integer) is (-1)**(integer)*(+anything**integer)
 *	19. (-anything except 0 and inf) ** (non-integer) is NAN
 *
 */

_END_EXTERN_C

/* Returns 0 if not int, 1 if odd int, 2 if even int.  The argument is
   the bit representation of a non-zero finite floating-point value.  */
// This function is from GLibC
int CheckIntF64(uint64 iy)
{
	int e = iy >> 52 & 0x7FF;
	if (e < 0x3FF) { return 0; }
	if (e > 0x3FF + 52) { return 2; }
	if (iy & ((1ULL << (0x3FF + 52 - e)) - 1)) { return 0; }
	if (iy & (1ULL << (0x3FF + 52 - e))) { return 1; }
	else { return 2; }
}

#ifdef _QUAD_PRECISION
// quaduple precision version
int CheckIntF128(unsigned __int128 iy)
{
	int e = iy >> 112 & 0x7FFF;
	if (e < 0x3FFF) { return 0; }
	if (e > 0x3FFF + 112) { return 2; }
	if (iy & ((1ULL << (0x3FFF + 112 - e)) - 1)) { return 0; }
	if (iy & (1ULL << (0x3FFF + 112 - e))) { return 1; }
	else { return 2; }
}
#endif

int __CheckInteger(_FType iy)
{
	#ifdef _QUAD_PRECISION
	return CheckIntF128(iy.Bytes);
	#else
	return CheckIntF64(iy.Bytes);
	#endif
}

_Check_return_ __Float64 __cdecl __IEEE754_POWF(_In_ __Float64 _X, _In_ __Float64 _Power)
{
	#if !defined(_USE_CUSTOM_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__)) && !defined(_QUAD_PRECISION)
	return _CSTD pow(_X, _Power);
	#else

	// IEEE754-Power function by IBM (LGPL Lic.)
	// Original function: https://azrael.digipen.edu/~mmead/www/Courses/CS120/e_pow.c.html
	// Port by StellarDX
	using uint32 = unsigned;
	union half
	{
		__Float64 _Ref;    // Reference number
		int     _Hfi[2]; // Half of number, 0 = High, 1 = Low
		half() {}
	}Src, Pwr;
	enum Idx { LOW_HALF, HIGH_HALF };

	// Extract bytes
	Src._Ref = _X;
	Pwr._Ref = _Power;
	uint32 qx, qy;

	if (0 == Pwr._Hfi[LOW_HALF])
	{
		qx = Src._Hfi[HIGH_HALF] & 0x7FFFFFFF; // remove sign
		// Checking if x is not too small to compute
		if ((qx == 0x7FF00000 && Src._Hfi[LOW_HALF] != 0) || qx > 0x7FF00000)
		{
			return __Float64::FromBytes(Q_NAN_DOUBLE);
		}
		if (_Power == 1) { return _X; }
		if (_Power == 2) { return _X * _X; }
		if (_Power == -1) { return 1. / _X; }
		if (_Power == 0) { return 1; }
	}

	if (((Src._Hfi[HIGH_HALF] > 0 && Src._Hfi[HIGH_HALF] < 0x7FF00000) || // x>0 and not x->0
		(Src._Hfi[HIGH_HALF] == 0 && Src._Hfi[LOW_HALF] != 0)) &&
		// 2^-1023 < x <= 2^-1023 * 0x1.0000FFFFFFFF
		(Pwr._Hfi[HIGH_HALF] & 0x7FFFFFFF) < 0x4FF00000)
	{
		__Float64 _Ln = __IEEE754_LNF64(_X);
		_Ln *= _Power;
		return __IEEE754_EXPF64(_Ln);
	}

	if (_X == 0)
	{
		if (((Pwr._Hfi[HIGH_HALF] & 0x7FFFFFFF) == 0x7FF00000 && Pwr._Hfi[LOW_HALF] != 0)
			|| (Pwr._Hfi[HIGH_HALF] & 0x7FFFFFFF) > 0x7FF00000) // pow(0, nan)
		{
			return _Power;
		}
		if (abs(_Power) > 1E+20)
		{
			return _Power > 0 ? __Float64(0) : __Float64::FromBytes(POS_INF_DOUBLE); // 0 to the power of +-inf
		}

		int _K = CheckIntF64(_Power.Bytes);
		if (_K == -1) { return _Power < 0 ? __Float64(1. / _X) : _X; }
		else { return _Power < 0 ? 1. / abs(_X) : 0.; }
	}

	qx = Src._Hfi[HIGH_HALF] & 0x7FFFFFFF; // remove sign bit
	qy = Pwr._Hfi[HIGH_HALF] & 0x7FFFFFFF;

	if (qx >= 0x7FF00000 && (qx > 0x7FF00000 || Src._Hfi[LOW_HALF] != 0)) { return __Float64::FromBytes(Q_NAN_DOUBLE); }
	if (qy >= 0x7FF00000 && (qy > 0x7FF00000 || Pwr._Hfi[LOW_HALF] != 0)) { return _X == 1. ? __Float64(1) : __Float64::FromBytes(Q_NAN_DOUBLE); }

	// If base is negative...
	if (Src._Hfi[HIGH_HALF] < 0)
	{
		int _K = CheckIntF64(_Power.Bytes);
		if (_K == 0)
		{
			if (qy == 0x7FF00000)
			{
				if (_X == -1) { return 1; }
				else if (_X > -1) { return Pwr._Hfi[HIGH_HALF] < 0 ? __Float64::FromBytes(POS_INF_DOUBLE) : __Float64(0); }
				else { return Pwr._Hfi[HIGH_HALF] < 0 ? __Float64(0) : __Float64::FromBytes(POS_INF_DOUBLE); }
			}
			else if (qx == 0x7FF00000)
			{
				return _Power < 0 ? __Float64(0) : __Float64::FromBytes(POS_INF_DOUBLE);
			}
			return __Float64::FromBytes(Q_NAN_DOUBLE); // y not integer and x < 0
		}
		else if (qx == 0x7FF00000)
		{
			if (_K < 0) { return _Power < 0 ? __Float64::FromBytes(NEG_ZERO_DOUBLE) : __Float64::FromBytes(NEG_INF_DOUBLE); }
			else { return _Power < 0 ? __Float64(0) : __Float64::FromBytes(POS_ZERO_DOUBLE); }
		}
		return _K == 1 ? __IEEE754_POWF(-_X, _Power) : __IEEE754_POWF(-_X, _Power); // if y even or odd
	}

	if (qx == 0x7FF00000) // x= 2^-0x3FF
	{
		if (_Power == 0) { return __Float64::FromBytes(Q_NAN_DOUBLE); } // pow(inf, 0)
		return (_Power > 0) ? _X : __Float64(0);
	}

	if (qy > 0x45F00000 && qy < 0x7FF00000)
	{
		if (_X == 1) { return 1; }
		if (_Power > 0) { return (_X > 1) ? __Float64::FromBytes(POS_INF_DOUBLE) : __Float64(0); }
		if (_Power < 0) { return (_X < 1) ? __Float64::FromBytes(POS_INF_DOUBLE) : __Float64(0); }
	}

	if (_X == 1) { return 1; }
	if (_Power > 0) { return (_X > 1) ? __Float64::FromBytes(POS_INF_DOUBLE) : __Float64(0); }
	if (_Power < 0) { return (_X < 1) ? __Float64::FromBytes(POS_INF_DOUBLE) : __Float64(0); }
	return __Float64::FromBytes(Q_NAN_DOUBLE); // IBM: unreachable, to make the compiler happy (*￣3￣)╭
}

// A megafunction designed for higher precision types
_Check_return_ _FType __cdecl __CosmoCXX_Fast_Power(_In_ _FType _X, _In_ _FType _Power)
{
	// Check the special cases
	if (_Power == 0) { return 1; }
	if (_Power == 1) { return _X; }
	if (isnan(_X) || isnan(_Power)) { return _FType::FromBytes(CSE_NAN); }
	_FType ax = abs(_X);
	int ChkInt = __CheckInteger(_X);
	if (ax == 0)
	{
		if (ChkInt == 1)
		{
			_FType res = __CosmoCXX_Fast_Power(0, _Power);
			res.Negative = ~res.Negative;
			return res;
		}
		if (_Power.Negative) { return _FType::FromBytes(CSE_POSITIVE_INF); }
		else { return 0; }
	}
	if (isinf(_Power))
	{
		if (ax > 1.) {return _Power.Negative ? _FType(0) : _FType::FromBytes(CSE_POSITIVE_INF);}
		if (ax < 1.) { return _Power.Negative ? _FType::FromBytes(CSE_POSITIVE_INF) : _FType(0); }
		return _FType::FromBytes(CSE_NAN); // 1 ** inf is undefined.
	}
	if (isinf(_X))
	{
		if (_X.Negative) { return __CosmoCXX_Fast_Power(_FType::FromBytes(NEG_ZERO_DOUBLE), _Power); }
		if (_Power.Negative) {return _FType::FromBytes(CSE_POSITIVE_INF);}
		else { return 0; }
	}

	if (_X.Negative)
	{
		if (ChkInt == 1) { return -__CosmoCXX_Fast_Power(ax, _Power); }
		if (ChkInt == 2) { return +__CosmoCXX_Fast_Power(ax, _Power); }
		else {return _FType::FromBytes(CSE_NAN);}
	}

	// Common case
	return __exp(_Power * __ln(_X));

	#endif
}

_CSE_END