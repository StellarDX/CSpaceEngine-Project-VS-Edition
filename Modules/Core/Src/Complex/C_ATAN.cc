#include "CSE/Core/IEEE754/MathFuncs.h"

_CSE_BEGIN

// Complex Form of inverse trigonometric functions

_Check_return_ complex64 __cdecl arcsinc(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx, int64 K_OFFSET)
{
	return -1i * lnc(_CSE sqrtc(1. - _X * _X)[_SqrtIdx] + 1i * _X, K_OFFSET);
}

_Check_return_ complex64 __cdecl arccosc(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx, int64 K_OFFSET)
{
	return -1i * lnc(1i * _CSE sqrtc(1. - _X * _X)[_SqrtIdx] + _X, K_OFFSET);
}

_Check_return_ complex64 __cdecl arctanc(_In_ complex64 _X, int64 K_OFFSET)
{
	return -0.5i * lnc((1i - _X) / (1i + _X), K_OFFSET);
}

_Check_return_ complex64 __cdecl arcctgc(_In_ complex64 _X, int64 K_OFFSET)
{
	return -0.5i * lnc((_X + 1i) / (_X - 1i), K_OFFSET);
}

_Check_return_ complex64 __cdecl arcsecc(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx, int64 K_OFFSET)
{
	return -1i * lnc(1i * _CSE sqrtc(1. - 1. / (_X * _X))[_SqrtIdx] + 1. / _X, K_OFFSET);
}

_Check_return_ complex64 __cdecl arccscc(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx, int64 K_OFFSET)
{
	return -1i * lnc(_CSE sqrtc(1. - 1. / (_X * _X))[_SqrtIdx] + 1.i / _X, K_OFFSET);
}

_CSE_END