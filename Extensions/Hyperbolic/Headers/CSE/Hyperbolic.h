#pragma once

#ifndef _HYPERBOLIC_
#define _HYPERBOLIC_

// Hyperbolic function and their inverse functions extension for CSE
// Reference: https://en.wikipedia.org/wiki/Hyperbolic_functions

#include "CSE/Core/AsMath.h"

// Namespaces
#define _HPB_BEGIN namespace hyperbolic {
#define _HPB_END   }
#define _HPB       hyperbolic::

_CSE_BEGIN
_HPB_BEGIN

_Check_return_ float64 __cdecl sinh(_In_ float64 _X);
_Check_return_ float64 __cdecl cosh(_In_ float64 _X);
_Check_return_ float64 __cdecl tanh(_In_ float64 _X);
_Check_return_ float64 __cdecl coth(_In_ float64 _X);
_Check_return_ float64 __cdecl sech(_In_ float64 _X);
_Check_return_ float64 __cdecl csch(_In_ float64 _X);

_Check_return_ complex64 __cdecl sinh(_In_ complex64 _X);
_Check_return_ complex64 __cdecl cosh(_In_ complex64 _X);
_Check_return_ complex64 __cdecl tanh(_In_ complex64 _X);
_Check_return_ complex64 __cdecl coth(_In_ complex64 _X);
_Check_return_ complex64 __cdecl sech(_In_ complex64 _X);
_Check_return_ complex64 __cdecl csch(_In_ complex64 _X);

_Check_return_ float64 __cdecl arsinh(_In_ float64 _X);
_Check_return_ float64 __cdecl arcosh(_In_ float64 _X);
_Check_return_ float64 __cdecl artanh(_In_ float64 _X);
_Check_return_ float64 __cdecl arcoth(_In_ float64 _X);
_Check_return_ float64 __cdecl arsech(_In_ float64 _X);
_Check_return_ float64 __cdecl arcsch(_In_ float64 _X);

_Check_return_ complex64 __cdecl arsinhc(_In_ complex64 _X, _In_ uint64 K1 = 0, _In_ int64 K2 = 0, _In_ int64 K3 = 0);
_Check_return_ complex64 __cdecl arcoshc(_In_ complex64 _X, _In_ uint64 K1 = 0, _In_ int64 K2 = 0, _In_ int64 K3 = 0);
_Check_return_ complex64 __cdecl artanhc(_In_ complex64 _X, _In_ int64 K = 0);
_Check_return_ complex64 __cdecl arcschc(_In_ complex64 _X, _In_ uint64 K1 = 0, _In_ int64 K2 = 0, _In_ int64 K3 = 0);
_Check_return_ complex64 __cdecl arsechc(_In_ complex64 _X, _In_ uint64 K1 = 0, _In_ int64 K2 = 0, _In_ int64 K3 = 0);
_Check_return_ complex64 __cdecl arcothc(_In_ complex64 _X, _In_ int64 K = 0);

// VECTOR FUNCTIONS

_VECGENTYPE_FUNCTION_TEMPLATE_S(sinh)
_VECGENTYPE_FUNCTION_TEMPLATE_S(cosh)
_VECGENTYPE_FUNCTION_TEMPLATE_S(tanh)
_VECGENTYPE_FUNCTION_TEMPLATE_S(sech)
_VECGENTYPE_FUNCTION_TEMPLATE_S(csch)
_VECGENTYPE_FUNCTION_TEMPLATE_S(coth)

_VECGENTYPE_FUNCTION_TEMPLATE_S(arsinh)
_VECGENTYPE_FUNCTION_TEMPLATE_S(arcosh)
_VECGENTYPE_FUNCTION_TEMPLATE_S(artanh)
_VECGENTYPE_FUNCTION_TEMPLATE_S(arsech)
_VECGENTYPE_FUNCTION_TEMPLATE_S(arcsch)
_VECGENTYPE_FUNCTION_TEMPLATE_S(arcoth)

_HPB_END
_CSE_END

#endif