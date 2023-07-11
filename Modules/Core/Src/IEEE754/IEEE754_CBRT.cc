// Fast cube root function for double and quaduple precision.
/*
   Copyright (C) 2018-2023 Free Software Foundation, Inc.
   This file is part of the GNU C Library. Adapted for CSpaceEngine

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.
*/

#include "CSE/Core/CSECore.h"
#include "CSE/Core/IEEE754/IEEE754.h"
#include "CSE/Core/IEEE754/MathFuncs.h"

#include <corecrt_math.h>

#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS 4190)

_CSE_BEGIN

_Check_return_ _FType __cdecl cbrt(_In_ _FType _X)
{
    return __cbrt(_X);
}

_EXTERN_C

/*
 *	Cube root for double precision
 */

_Check_return_ __Float64 __cdecl __IEEE754_CBRTF64(_In_ __Float64 _X)
{
    #if !defined(_USE_CUSTOM_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__)) && !defined(_QUAD_PRECISION)
    return _CSTD cbrt(_X);
    #else

    __Float64 x = _X;
    __Float64 xm, ym, u, t2;
    int xe;

    static const __Float64 factor[5] =
    {
        0.62996052494743658238360530363912, // 1 / cbrt(2)^2
        0.79370052598409973737585281963615, // 1 / cbrt(2)
        1.00000000000000000000000000000000,
        1.25992104989487316476721060727820, // cbrt(2)
        1.58740105196819947475170563927230, // cbrt(2)^2
    };

    /* Reduce X.  XM now is an range 1.0 to 0.5.  */
    xm = frexp(fabs(x), &xe);

    /* If X is not finite or is null return it (with raising exceptions
       if necessary.
       Note: *Our* version of `frexp' sets XE to zero if the argument is
       Inf or NaN.  This is not portable but faster.  */
    if (xe == 0 && fpclassify(x) <= FP_ZERO) { return x; }

    u = (0.354895765043919860 + ((1.50819193781584896 + ((-2.11499494167371287 + ((2.44693122563534430 + ((-1.83469277483613086 + (0.784932344976639262 - 0.145263899385486377 * xm) * xm) * xm)) * xm)) * xm)) * xm));
    t2 = u * u * u;
    ym = u * (t2 + 2.0 * xm) / (2.0 * t2 + xm) * factor[2 + xe % 3].Val;

    return ldexp(x > 0.0 ? ym.Val : -ym.Val, xe / 3);

    #endif
}

_END_EXTERN_C
_CSE_END

#pragma warning(pop)