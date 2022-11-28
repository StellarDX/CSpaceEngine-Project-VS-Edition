#include "CSE/Core/CSEM_Func.h"
#include "CSE/Core/CSEM_Algo.h"

// pow(a, b) can be expressed as exp(b * ln(a))
// but the domain becomes smaller (only positive) because ln(x) is used
// so we need to use multiple method to find the solution.

// All the functions are based on 64-bit float.

// Power function has lots of special cases:
// 1.  pow(anything, 0)            = 1 (even pow(0, 0) = 1? see https://en.wikipedia.org/wiki/Zero_to_the_power_of_zero)
// 2.  pow(anything, 1)            = itself
// 3.  pow(anything, nan)          = nan
// 4.  pow(nan, anything except 0) = nan
// 5.  pow({x | abs(x) > 1}, +inf) = +inf
// 6.  pow({x | abs(x) > 1}, -inf) = +0
// 7.  pow({x | abs(x) < 1}, +inf) = +0
// 8.  pow({x | abs(x) < 1}, -inf) = +inf
// 9.  pow(1, inf)                 = nan (Why? see https://math.stackexchange.com/questions/319764/1-to-the-power-of-infinity-why-is-it-indeterminate)
// 10. pow(+0, positive anything except 0, nan)                = +0
// 11. pow(-0, positive anything except 0, nan, odd integer)   = +0
// 12. pow(+0, negative anything except 0, nan)                = +inf
// 13. pow(-0, negative anything except 0, nan, odd integer)   = +inf
// 14. pow(-0, odd integer)                                    = -pow(+0, odd integer)
// 15. pow(+inf, positive anything except 0, nan)              = +inf
// 16. pow(+inf, negative anything except 0, nan)              = +0
// 17. pow(-inf, anything)         = pow(-0, negative anything)
// 18. pow(negative, integer)      = pow(-1, integer) * pow(anything, integer)
// 19. pow(negative, non-integer)  = nan (negative number has no real even(th)-root)

// Reference: https://www.codenong.com/40824677/

_CSE_BEGIN

/**********************************************************************/
/* Routine receives a double x and checks if it is an integer. If not */
/* it returns 0, else it returns 1 if even or -1 if odd.              */
/**********************************************************************/
static int checkint(float64 x) // function by IBM, taken from glibc
{
	union { int i[2]; float64 x; } u;
	int k, m, n;
	enum Idx { LOW_HALF, HIGH_HALF };

	u.x = x;
	m = u.i[HIGH_HALF] & 0x7FFFFFFF;    // no sign
	if (m >= 0x7FF00000) { return 0; }    // x is +/-inf or NaN
	if (m >= 0x43400000) { return 1; }    // |x| >= 2**53
	if (m < 0x40000000) { return 0; }     // |x| < 2,  can not be 0 or 1
	n = u.i[LOW_HALF];
	k = (m >> 20) - 1023;                 // 1 <= k <= 52
	if (k == 52) { return (n & 1) ? -1 : 1; }  // odd or even
	if (k > 20) 
	{
		if (n << (k - 20)) { return 0; }        // if not integer
		return (n << (k - 21)) ? -1 : 1;
	}
	if (n) { return 0; }                  // if  not integer
	if (k == 20) { return (m & 1) ? -1 : 1; }
	if (m << (k + 12)) { return 0; }
	return (m << (k + 11)) ? -1 : 1;
}

_Check_return_ float64 __cdecl pow(_In_ float64 _X, _In_ float64 _Power, POWER_CONFIG _Conf)
{
	#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return std::pow(_X, _Power);
	#else

	// IEEE754-Power function by IBM (LGPL Lic.)
	// Original function: https://azrael.digipen.edu/~mmead/www/Courses/CS120/e_pow.c.html
	// Port by StellarDX
	using uint32 = unsigned;
	union half
	{
		float64 _Ref;    // Reference number
		int     _Hfi[2]; // Half of number, 0 = High, 1 = Low
	}Src, Pwr;
	enum Idx {LOW_HALF, HIGH_HALF};

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
			return wrtval(Q_NAN_DOUBLE);
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
		float64 _Ln = ln(_X);
		_Ln *= _Power;
		return exp(_Ln);
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
			return _Power > 0 ? 0 : wrtval(POS_INF_DOUBLE); // 0 to the power of +-inf
		}

		int _K = checkint(_Power);
		if (_K == -1) { return _Power < 0 ? 1. / _X : _X; }
		else { return _Power < 0 ? 1. / abs(_X) : 0.; }
	}

	qx = Src._Hfi[HIGH_HALF] & 0x7FFFFFFF; // remove sign bit
	qy = Pwr._Hfi[HIGH_HALF] & 0x7FFFFFFF;

	if (qx >= 0x7FF00000 && (qx > 0x7FF00000 || Src._Hfi[LOW_HALF] != 0)) { return wrtval(Q_NAN_DOUBLE); }
	if (qy >= 0x7FF00000 && (qy > 0x7FF00000 || Pwr._Hfi[LOW_HALF] != 0)) { return _X == 1. ? 1. : wrtval(Q_NAN_DOUBLE); }

	// If base is negative...
	if (Src._Hfi[HIGH_HALF] < 0)
	{
		int _K = checkint(_Power);
		if (_K == 0)
		{
			if (qy == 0x7FF00000)
			{
				if (_X == -1) { return 1; }
				else if (_X > -1) { return Pwr._Hfi[HIGH_HALF] < 0 ? wrtval(POS_INF_DOUBLE) : 0; }
				else { return Pwr._Hfi[HIGH_HALF] < 0 ? 0 : wrtval(POS_INF_DOUBLE); }
			}
			else if (qx == 0x7FF00000)
			{
				return _Power < 0 ? 0.0 : wrtval(POS_INF_DOUBLE);
			}
			return wrtval(Q_NAN_DOUBLE); // y not integer and x < 0
		}
		else if (qx == 0x7FF00000)
		{
			if (_K < 0) { return _Power < 0 ? wrtval(NEG_ZERO_DOUBLE) : wrtval(NEG_INF_DOUBLE); }
			else { return _Power < 0 ? 0.0 : wrtval(POS_ZERO_DOUBLE); }
		}
		return _K == 1 ? _CSE pow(-_X, _Power) : -_CSE pow(-_X, _Power); // if y even or odd
	}

	if (qx == 0x7FF00000) // x= 2^-0x3FF
	{
		if (_Power == 0) { return wrtval(Q_NAN_DOUBLE); } // pow(inf, 0)
		return (_Power > 0) ? _X : 0;
	}

	if (qy > 0x45F00000 && qy < 0x7FF00000)
	{
		if (_X == 1) { return 1; }
		if (_Power > 0) { return (_X > 1) ? wrtval(POS_INF_DOUBLE) : 0; }
		if (_Power < 0) { return (_X < 1) ? wrtval(POS_INF_DOUBLE) : 0; }
	}

	if (_X == 1) { return 1; }
	if (_Power > 0) { return (_X > 1) ? wrtval(POS_INF_DOUBLE) : 0; }
	if (_Power < 0) { return (_X < 1) ? wrtval(POS_INF_DOUBLE) : 0; }
	return wrtval(Q_NAN_DOUBLE); // IBM: unreachable, to make the compiler happy (*￣3￣)╭

	#endif
}

_Check_return_ complex64 __cdecl pow(_In_ complex64 _X, _In_ complex64 _Power, POWER_CONFIG _Conf)
{
	return _CSE exp(_Power * ln(_X, { .K_OFFSET = _Conf.K_OFFSET }));
}

_Check_return_ float64 __cdecl sqrt(_In_ float64 _X, POWER_CONFIG _Conf)
{
	#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return std::sqrt(_X);
	#else
	return _CSE pow(_X, 0.5, _Conf);
	#endif
}

_Check_return_ std::array<complex64, 2> __cdecl sqrt(_In_ complex64 _X, POWER_CONFIG _Conf)
{
	complex64 _Res;
	float64 Sign = sgn(_X.imag());
	if (Sign == 0)
	{
		if (_X.real() > 0) { return { sqrt(_X.real()), -sqrt(_X.real()) }; }
		if (_X.real() < 0) { return { 1i * sqrt(-_X.real()), 1i * -sqrt(-_X.real()) }; }
	}
	_Res = _CSE sqrt((_CSE abs(_X) + _X.real()) / 2.) + 1i * Sign * _CSE sqrt((_CSE abs(_X) - _X.real()) / 2.);
	return { _Res, -_Res };
}

_Check_return_ float64 __cdecl inversesqrt(_In_ float64 _X, POWER_CONFIG _Conf)
{
	return 1. / _CSE sqrt(_X, _Conf);
}

_Check_return_ complex64 __cdecl inversesqrt(_In_ complex64 _X, POWER_CONFIG _Conf)
{
	return 1. / _CSE sqrt(_X, _Conf)[0];
}

/* ************************************************************************** *\
   Fast cube root by Ken Turkowski (taken from OpenCV)
   (https://people.freebsd.org/~lstewart/references/apple_tr_kt32_cuberoot.pdf)
   and expand to 64-bit floating point.
\* ************************************************************************** */
_Check_return_ float64 __cdecl cbrt(_In_ float64 _X, CBRT_CONFIG _Conf)
{
#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return std::cbrt(_X);
#else

	if (_X == 0) { return 0; }

	float64 fr;
	int64 ix, s;
	int64 ex, shx;

	int Offset = (1 << (_Conf.BITSEXPO - 1)) - 1;

	// separate into mantissa and exponent
	int64 v = wrtval<float64, int64>(_X);
	ix = v & ((1ll << (_Conf.BITSEXPO + _Conf.BITSFRAC)) - 1);
	s = v & (1ll << (_Conf.BITSEXPO + _Conf.BITSFRAC));
	ex = (ix >> _Conf.BITSFRAC) - Offset;

	// compute shx such that (ex - shx) is divisible by 3
	shx = ex % 3;
	shx -= shx > 0 ? 3 : 0;
	ex = (ex - shx) / 3; // this is the exponent of cube root
	fr = wrtval((ix & ((1ll << _Conf.BITSFRAC) - 1)) | ((shx + Offset) << _Conf.BITSFRAC)); // 0.125 <= fr < 1.0

	// Compute seed with a quadratic approximation
	fr = (-0.46946116L * fr + 1.072302L) * fr + 0.3812513L; // x6
	float64 r = wrtval(wrtval<float64, int64>(fr) + (ex << _Conf.BITSFRAC) + s);

	// Newton-Raphson iterations
	for (size_t i = 0; i < _Conf.NUMITER; i++)
	{
		r = (2.0 / 3.0) * r + (1.0 / 3.0) * _X / (r * r);
	}

	return wrtval(r);

#endif
}

_Check_return_ std::array<complex64, 3> __cdecl cbrt(_In_ complex64 _X, CBRT_CONFIG _Conf)
{
	float64 r = _CSE abs(_X);
	float64 tet = std::atan2(_X.imag(), _X.real());
	float64 ri = _CSE cbrt(r);
	return
	{
		ri * _CSE exp((1i * float64(tet + 0. * 2. * CSE_PI)) / 3.),
		ri * _CSE exp((1i * float64(tet + 1. * 2. * CSE_PI)) / 3.),
		ri * _CSE exp((1i * float64(tet + 2. * 2. * CSE_PI)) / 3.)
	};
}

// nth-root function by StellarDX, only for real roots
_Check_return_ float64 __cdecl yroot(_In_ float64 _Expo, _In_ float64 _X)
{
	// Special returns
	if (_Expo == 1) { return _X; }
	if (_Expo == 2) { return _CSE sqrt(_X); }
	if (_Expo == 3) { return _CSE cbrt(_X); }

	using uint32 = unsigned;
	union half
	{
		float64 _Ref;    // Reference number
		int     _Hfi[2]; // Half of number, 0 = High, 1 = Low
	}Src, Exp;
	enum Idx { LOW_HALF, HIGH_HALF };

	// Extract bytes
	Src._Ref = _X;
	Exp._Ref = _Expo;

	if ((Src._Hfi[HIGH_HALF] > 0 && Src._Hfi[HIGH_HALF] < 0x7FF00000) || // x>0 and not x->0
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

	return wrtval(Q_NAN_DOUBLE); // StellarDX: (maybe) unreachable, to make the compiler happy (*￣3￣)╭
}

_Check_return_ std::vector<complex64> __cdecl yroot(_In_ complex64 _Expo, _In_ complex64 _X, POWER_CONFIG _Conf)
{
	std::vector<complex64> Buffer;

	float64 Real = _Expo.real();
	int _K = checkint(_Expo.real());
	if (_Expo.imag() == 0 && _K != 0 && Real > 0)
	{
		float64 r = _CSE abs(_X);
		float64 tet = std::atan2(_X.imag(), _X.real());
		for (size_t i = 0; i < Real; ++i)
		{
			Buffer.push_back(yroot(Real, r) * _CSE exp(1i * complex64(tet + (i + _Conf.K_OFFSET) * 2. * CSE_PI) / Real));
		}
		return Buffer;
	}

	Buffer.push_back(_CSE pow(_X, 1. / _Expo, _Conf));
	return Buffer;
}

_CSE_END