#include "CSE/Core/IEEE754/MathFuncs.h"

// Exponential function in complex form

_CSE_BEGIN

_Check_return_ complex64 __cdecl expc(_In_ complex64 _X)
{
	return _CSE exp(_X.real()).Val * (std::cos(_X.imag()) + 1i * std::sin(_X.imag()));
}

_CSE_END