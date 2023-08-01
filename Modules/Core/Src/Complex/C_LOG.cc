#include "CSE/Core/IEEE754/MathFuncs.h"

_CSE_BEGIN

// Complex Form of logarithm functions

_Check_return_ complex64 __cdecl lnc(_In_ complex64 _X, int64 K_OFFSET)
{
	// Convert to polar form (r, tet)
	float64 r = _CSE abs(_X);
	float64 tet = std::atan2(_X.imag(), _X.real());
	return ln(r).x + 1i * float64(tet + 2. * CSE_PI * K_OFFSET);
}

_Check_return_ complex64 __cdecl logc(_In_ complex64 _X, int64 K_OFFSET)
{
	return lnc(_X, K_OFFSET) / complex64(CSE_LN10);
}

_Check_return_ complex64 __cdecl logc(_In_ complex64 _Base, _In_ complex64 _X, int64 K_OFFSET)
{
	return lnc(_X, K_OFFSET) / lnc(_Base, K_OFFSET);
}

_CSE_END