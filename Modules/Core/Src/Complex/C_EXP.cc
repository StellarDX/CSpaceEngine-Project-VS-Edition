#include "CSE/Core/IEEE754/MathFuncs.h"

// Exponential function in complex form

_CSE_BEGIN

_Check_return_ complex64 __cdecl expc(_In_ complex64 _X)
{
	return _CSE exp(_X.real()).Val * (cos(degrees(_X.imag())).Val + 1i * sin(degrees(_X.imag())).Val);
}

_CSE_END