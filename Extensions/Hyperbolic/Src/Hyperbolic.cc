#include "CSE/Hyperbolic.h"

_CSE_BEGIN
_HPB_BEGIN

// BASE FUNCTIONS

_Check_return_ float64 __cdecl sinh(_In_ float64 _X)
{
	return (exp(_X) - exp(-_X)) / 2.;
}

_Check_return_ float64 __cdecl cosh(_In_ float64 _X)
{
	return (exp(_X) + exp(-_X)) / 2.;
}

_Check_return_ float64 __cdecl tanh(_In_ float64 _X)
{
	return sinh(_X) / cosh(_X);
}

_Check_return_ float64 __cdecl coth(_In_ float64 _X)
{
	return cosh(_X) / sinh(_X);
}

_Check_return_ float64 __cdecl sech(_In_ float64 _X)
{
	return 1. / cosh(_X);
}

_Check_return_ float64 __cdecl csch(_In_ float64 _X)
{
	return 1. / sinh(_X);
}

// COMPLEX FUNCTIONS

_Check_return_ complex64 __cdecl sinh(_In_ complex64 _X)
{
	return (exp(_X) - exp(-_X)) / 2.;
}

_Check_return_ complex64 __cdecl cosh(_In_ complex64 _X)
{
	return (exp(_X) + exp(-_X)) / 2.;
}

_Check_return_ complex64 __cdecl tanh(_In_ complex64 _X)
{
	return sinh(_X) / cosh(_X);
}

_Check_return_ complex64 __cdecl coth(_In_ complex64 _X)
{
	return cosh(_X) / sinh(_X);
}

_Check_return_ complex64 __cdecl sech(_In_ complex64 _X)
{
	return 1. / cosh(_X);
}

_Check_return_ complex64 __cdecl csch(_In_ complex64 _X)
{
	return 1. / sinh(_X);
}

// INVERSE FUNCTIONS

_Check_return_ float64 __cdecl arsinh(_In_ float64 _X)
{
	return ln(_X + sqrt(pow(_X, 2) + 1.));
}

_Check_return_ float64 __cdecl arcosh(_In_ float64 _X)
{
	return ln(_X + sqrt(pow(_X, 2) - 1.));
}

_Check_return_ float64 __cdecl artanh(_In_ float64 _X)
{
	return (1. / 2.) * ln((1. + _X) / (1. - _X));
}

_Check_return_ float64 __cdecl arcsch(_In_ float64 _X)
{
	return arsinh(1. / _X);
}

_Check_return_ float64 __cdecl arsech(_In_ float64 _X)
{
	return arcosh(1. / _X);
}

_Check_return_ float64 __cdecl arcoth(_In_ float64 _X)
{
	return artanh(1. / _X);
}

// INVERSE FUNCTIONS FOR COMPLEX

_Check_return_ complex64 __cdecl arsinh(_In_ complex64 _X, _In_ uint64 K1, _In_ int64 K2, _In_ int64 K3, _In_ int64 K4)
{
	return ln(_X + sqrt(pow(_X, 2, { .K_OFFSET = K4 }) + 1., { .K_OFFSET = K2 })[K1], { .K_OFFSET = K3 });
}

_Check_return_ complex64 __cdecl arcosh(_In_ complex64 _X, _In_ uint64 K1, _In_ int64 K2, _In_ int64 K3, _In_ int64 K4)
{
	return ln(_X + sqrt(pow(_X, 2, { .K_OFFSET = K4 }) - 1., { .K_OFFSET = K2 })[K1], { .K_OFFSET = K3 });
}

_Check_return_ complex64 __cdecl artanh(_In_ complex64 _X, _In_ int64 K)
{
	return (1. / 2.) * ln((1. + _X) / (1. - _X), { .K_OFFSET = K });
}

_Check_return_ complex64 __cdecl arcsch(_In_ complex64 _X, _In_ uint64 K1, _In_ int64 K2, _In_ int64 K3, _In_ int64 K4)
{
	return arsinh(1. / _X, K1, K2, K3, K4);
}

_Check_return_ complex64 __cdecl arsech(_In_ complex64 _X, _In_ uint64 K1, _In_ int64 K2, _In_ int64 K3, _In_ int64 K4)
{
	return arcosh(1. / _X, K1, K2, K3, K4);
}

_Check_return_ complex64 __cdecl arcoth(_In_ complex64 _X, _In_ int64 K)
{
	return artanh(1. / _X, K);
}

_HPB_END
_CSE_END