#include "CSE/Core/IEEE754/MathFuncs.h"

_CSE_BEGIN

// Complex form of trigonometric functions

_Check_return_ complex64 __cdecl sinc(_In_ complex64 _X)
{
	return (_CSE expc(1i * _X) - _CSE expc(-1i * _X)) / 2.i;
}

_Check_return_ complex64 __cdecl cosc(_In_ complex64 _X)
{
	return (_CSE expc(1i * _X) + _CSE expc(-1i * _X)) / 2.;
}

_Check_return_ complex64 __cdecl tanc(_In_ complex64 _X)
{
	return _CSE sinc(_X) / _CSE cosc(_X);
}

_Check_return_ complex64 __cdecl ctgc(_In_ complex64 _X)
{
	return complex64(1.) / _CSE tanc(_X);
}

_Check_return_ complex64 __cdecl secc(_In_ complex64 _X)
{
	return complex64(1.) / _CSE cosc(_X);
}

_Check_return_ complex64 __cdecl cscc(_In_ complex64 _X)
{
	return complex64(1.) / _CSE sinc(_X);
}

_CSE_END