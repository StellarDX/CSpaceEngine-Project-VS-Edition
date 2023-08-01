#include "CSE/Core/IEEE754/MathFuncs.h"
#include "CSE/Core/Algorithms.h"

_CSE_BEGIN

// Complex form of power functions

int CheckIntF64(uint64 iy);

_Check_return_ complex64 __cdecl powc(_In_ complex64 _X, _In_ complex64 _Power, int64 K_OFFSET)
{
	return _CSE expc(_Power * lnc(_X, K_OFFSET));
}

_Check_return_ std::array<complex64, 2> __cdecl sqrtc(_In_ complex64 _X)
{
	complex64 _Res;
	float64 Sign = sgn(_X.imag());
	if (Sign == 0)
	{
		if (_X.real() > 0) { return { sqrt(_X.real()).x, -sqrt(_X.real()).x }; }
		if (_X.real() < 0) { return { 1i * sqrt(-_X.real()).x, 1i * -sqrt(-_X.real()) }; }
	}
	_Res = _CSE sqrt((_CSE abs(_X) + _X.real()) / 2.).x + 1i * Sign * _CSE sqrt((_CSE abs(_X) - _X.real()) / 2.).x;
	return { _Res, -_Res };
}

_Check_return_ complex64 __cdecl inversesqrtc(_In_ complex64 _X)
{
	return 1. / _CSE sqrtc(_X)[0];
}

_Check_return_ std::array<complex64, 3> __cdecl cbrtc(_In_ complex64 _X)
{
	float64 r = _CSE abs(_X);
	float64 tet = std::atan2(_X.imag(), _X.real());
	float64 ri = _CSE cbrt(r);
	return
	{
		ri * _CSE expc((1i * float64(tet + 0. * 2. * CSE_PI)) / 3.),
		ri * _CSE expc((1i * float64(tet + 1. * 2. * CSE_PI)) / 3.),
		ri * _CSE expc((1i * float64(tet + 2. * 2. * CSE_PI)) / 3.)
	};
}

_Check_return_ std::vector<complex64> __cdecl yrootc(_In_ complex64 _Expo, _In_ complex64 _X, int64 K_OFFSET)
{
	std::vector<complex64> Buffer;

	float64 Real = _Expo.real();
	int _K = CheckIntF64(__Float64(_Expo.real()).Bytes);
	if (_Expo.imag() == 0 && _K != 0 && Real > 0)
	{
		float64 r = _CSE abs(_X);
		float64 tet = std::atan2(_X.imag(), _X.real());
		for (size_t i = 0; i < Real; ++i)
		{
			Buffer.push_back(yroot(Real, r).x * _CSE expc(1i * complex64(tet + (i + K_OFFSET) * 2. * CSE_PI) / Real));
		}
		return Buffer;
	}

	Buffer.push_back(_CSE powc(_X, 1. / _Expo));
	return Buffer;
}

_CSE_END