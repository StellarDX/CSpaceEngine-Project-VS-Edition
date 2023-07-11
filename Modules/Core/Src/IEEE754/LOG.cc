// Logarithm function with custom base by StellarDX
// Method: log(x, base) = log(x) / log(base)

#include "CSE/Core/CSECore.h"
#include "CSE/Core/IEEE754/IEEE754.h"
#include "CSE/Core/IEEE754/MathFuncs.h"

_CSE_BEGIN

_Check_return_ _FType __cdecl log(_In_ _FType _Base, _In_ _FType _X)
{
	return __log(_X) / __log(_Base);
}

_CSE_END