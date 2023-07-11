#pragma once

#ifndef _CSE_MATH_BASE
#define _CSE_MATH_BASE

#include "CSE/Core/CSECore.h"
#include "CSE/Core/ConstLists.h"

#if 0//(defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__)
#include <cmath>   // Base on STD Math
#endif

#include "CSE/Core/CSEM_Algo.h"
#include <array>
#include <vector>
//#include "Complex.h"
#include <complex> // and add complex types

#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS 4190)

/* ************************************************************************** *\
   genTypes in Templates can be multiple data types:
	   genType = float64, complex64, vec2, vec3, vec4
	   genIType = int64, ivec2, ivec3, ivec4
	   genUType = uint64, uvec2, uvec3, uvec4
	   genBType = bool, bvec2, bvec3, bvec4
   and no genDTypes because all the types are already in 64-bit.
\* ************************************************************************** */

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

#define _USE_CUSTOM_FUNCTIONS
using __Float64 = IEEE754_Dbl64;

// INFs and NANs
#ifdef _QUAD_PRECISION // Unused
#define _FType            __Float128
#define CSE_POSITIVE_ZERO POS_ZERO_QUADUPLE
#define CSE_NEGATIVE_ZERO NEG_ZERO_QUADUPLE
#define CSE_POSITIVE_INF  POS_INF_QUADUPLE
#define CSE_NEGATIVE_INF  NEG_INF_QUADUPLE
#define CSE_NAN           BIG_NAN_QUADUPLE
#else
#define _FType            __Float64
#define CSE_POSITIVE_ZERO POS_ZERO_DOUBLE
#define CSE_NEGATIVE_ZERO NEG_ZERO_DOUBLE
#define CSE_POSITIVE_INF  POS_INF_DOUBLE
#define CSE_NEGATIVE_INF  NEG_INF_DOUBLE
#define CSE_NAN           BIG_NAN_DOUBLE
#endif

// Temporary definations
using InputArray = const std::vector<float64>&;
using OutputArray = std::vector<complex64>&;

class MathException : public std::runtime_error
{
public:
	MathException(std::string _Msg) : std::runtime_error(_Msg) {}
};

//#define _USE_CV_FUNCTIONS

/****************************************************************************************\
*                                          E X P                                         *
\****************************************************************************************/

// Configs for quaduple-precision algorithm
#ifdef _QUAD_PRECISION // Unused
#define EXP_himark 11356.523406294143949491931077970765L
#define EXP_lomark -11433.4627433362978788372438434526231L
#define EXP_TINY   1.0E-4900L
#define EXP_EMAX   5.94865747678615882542879663314003565E+4931L
#define EXP_UNSAFE 15000
#define EXP_ROUND  roundl
#define __exp      __IEEE754_EXPF128
#else
#define EXP_himark 709.78271289338399678773454114191496482
#define EXP_lomark -744.44007192138126231410729844608163411
#define EXP_TINY   1.0E-308
#define EXP_EMAX   8.988465674311579538646525953945123668E+307
#define EXP_UNSAFE 1020
#define EXP_ROUND  round
#define __exp      __IEEE754_EXPF64
#endif

_EXTERN_C
// TABLES
extern const unsigned long long __Expf64_table[]; // For Double
//extern const _FType __Expf128_table[]; // For Quaduple
// BASE FUNCTIONS
_Check_return_ __Float64 __cdecl __IEEE754_EXPF64(_In_ __Float64 _X);
//_Check_return_ _FType __cdecl __IEEE754_EXPF128(_In_ _FType _X);
_END_EXTERN_C

/// <summary>
/// Returns the natural exponentiation of x. i.e., e^x.
/// </summary>
_Check_return_ _FType __cdecl exp(_In_ _FType _X);
_Check_return_ complex64 __cdecl expc(_In_ complex64 _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl exp(_In_ genType _X);

/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

#ifdef _QUAD_PRECISION // Unused
#define LOG_FREXP frexpl
#define LN_FREXP  frexpl
#define __log     __IEEE754_LOGF128_CF64
#define __ln      __IEEE754_LNF128
#else
#define LOG_FREXP frexp
#define LN_FREXP  frexp
#define __log     __IEEE754_LOGF128_CF64
#define __ln      __IEEE754_LNF64
#endif

_EXTERN_C
// TABLES
extern const double __Lnf64_table[]; // For Double
//extern const _FType __Lnf128_table[]; // For Quaduple
// BASE FUNCTIONS
_Check_return_ _FType __cdecl __IEEE754_LOGF128_CF64(_In_ _FType _X); // quaduple precision function, but can be used for double precision.
_Check_return_ __Float64 __cdecl __IEEE754_LNF64(_In_ __Float64 _X); // natural logarithm for double precision
//_Check_return_ _FType __cdecl __IEEE754_LNF128(_In_ _FType _X); // natural logarithm for quaduple precision
_END_EXTERN_C

/// <summary>
/// Returns the logarithm of x, i.e. the value y which satisfies x=base^y.
/// Overloads:
/// log(x) returns the base 10 logarithm of x, i.e. the value y which satisfies x=10^y.
/// ln(x) returns the natural logarithm of x.
/// </summary>
/// <param name="_Base">Specify the base value</param>
/// <param name="_X">Specify the value of which to take the logarithm.</param>
_Check_return_ _FType __cdecl log(_In_ _FType _Base, _In_ _FType _X);
_Check_return_ _FType __cdecl log(_In_ _FType _X);
_Check_return_ _FType __cdecl ln(_In_ _FType _X);
_Check_return_ complex64 __cdecl logc(_In_ complex64 _Base, _In_ complex64 _X, int64 K_OFFSET = 0);
_Check_return_ complex64 __cdecl logc(_In_ complex64 _X, int64 K_OFFSET = 0);
_Check_return_ complex64 __cdecl lnc(_In_ complex64 _X, int64 K_OFFSET = 0);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl log(_In_ float64 _Base, _In_ genType _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl log(_In_ genType _Base, _In_ genType _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl log(_In_ genType _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl ln(_In_ genType _X);

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

#ifdef _QUAD_PRECISION // Unused
#define CBRT_FREXP frexpl
#define CBRT_LDEXP ldexpl
#define __pow      __CosmoCXX_Fast_Power
#define __sqrt     __CosmoCXX_Fast_SquareRoot
#define __cbrt     __IEEE754_CBRTF128
#else
#define CBRT_FREXP frexp
#define CBRT_LDEXP ldexp
#define __pow      __IEEE754_POWF
#define __sqrt     __IBM_SQRTF64
#define __cbrt     __IEEE754_CBRTF64
#endif

_EXTERN_C
// TABLES
extern const double __IBM_inroot_table[128];
// BASE FUNCTIONS
_Check_return_ __Float64 __cdecl __IEEE754_POWF(_In_ __Float64 _X, _In_ __Float64 _Power); // Power function in older version, only used for double precision.
_Check_return_ _FType __cdecl __CosmoCXX_Fast_Power(_In_ _FType _X, _In_ _FType _Power); // Power function use for double and quaduple precision
_Check_return_ __Float64 __cdecl __IBM_SQRTF64(_In_ __Float64 _X); // IBM's implemention of square root (Extremely high precision in double precision.)
_Check_return_ _FType __cdecl __CosmoCXX_Fast_SquareRoot(_In_ _FType _X); // solving square root in common routine.
_Check_return_ __Float64 __cdecl __IEEE754_CBRTF64(_In_ __Float64 _X);
//_Check_return_ _FType __cdecl __IEEE754_CBRTF128(_In_ _FType _X); // Cube root for quaduple precision
_END_EXTERN_C

/// <summary>
/// Returns the value of x raised to the y power, i.e. x^y.
/// </summary>
/// <param name="_X">Specify the value to raise to the power y.</param>
/// <param name="_Power">Specify the power to which to raise x.</param>
_Check_return_ _FType __cdecl pow(_In_ _FType _X, _In_ _FType _Power); // base functions
_Check_return_ complex64 __cdecl powc(_In_ complex64 _X, _In_ complex64 _Power, int64 K_OFFSET = 0);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl pow(_In_ genType _X, _In_ float64 _Power);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl pow(_In_ genType _X, _In_ genType _Power);

template<typename _Ty, size_t _Size>
_Check_return_ _GL basic_matrix<_Ty, _Size, _Size> __cdecl pow(_In_ _GL basic_matrix<_Ty, _Size, _Size> _A, _In_ uint64 _Power);

/// <summary>
/// Returns the square root of x, i.e. the value √x.
/// </summary>
_Check_return_ _FType __cdecl sqrt(_In_ _FType _X);
_Check_return_ std::array<complex64, 2> __cdecl sqrtc(_In_ complex64 _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl sqrt(_In_ genType _X);

/// <summary>
/// Returns the inverse of the square root of x, i.e. the value 1/√x
/// </summary>
_Check_return_ _FType __cdecl inversesqrt(_In_ _FType _X);
_Check_return_ complex64 __cdecl inversesqrtc(_In_ complex64 _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl inversesqrt(_In_ genType _X);

/// <summary>
/// Returns the cube root of x, i.e. the value 3√x.
/// </summary>
_Check_return_ _FType __cdecl cbrt(_In_ _FType _X);
_Check_return_ std::array<complex64, 3> __cdecl cbrtc(_In_ complex64 _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl cbrt(_In_ genType _X);

/// <summary>
/// Returns the nth-root of x, i.e. the value n√x.
/// </summary>
/// <param name="_Expo">Specify the value of exponential of root.</param>
/// <param name="_X">Specify the value to find root.</param>
_Check_return_ _FType __cdecl yroot(_In_ _FType _Expo, _In_ _FType _X);
_Check_return_ std::vector<complex64> __cdecl yrootc(_In_ complex64 _Expo, _In_ complex64 _X, int64 K_OFFSET = 0);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl yroot(_In_ float64 _Expo, _In_ genType _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl yroot(_In_ genType _Expo, _In_ genType _X);

/****************************************************************************************\
*                                      TRIGONOMETRY                                      *
\****************************************************************************************/

// Reference: 
// https://en.wikipedia.org/wiki/Trigonometric_functions
// https://en.wikipedia.org/wiki/Chord_(geometry)
// https://en.wikipedia.org/wiki/Versine
// https://en.wikipedia.org/wiki/Exsecant
// image of the returns of these function: https://en.wikipedia.org/wiki/File:Circle-trig6.svg

// Use degrees for real number functions, radians for complex functions.

/// <summary>
/// Converts degrees into radians.
/// </summary>
_Check_return_ _FType __cdecl radians(_In_ _FType _Deg);

/// <summary>
/// converts radians into degrees.
/// </summary>
_Check_return_ _FType __cdecl degrees(_In_ _FType _Rad);

// define "_USE_RADIANS_FOR_SINCOS" to force using radians.
#ifdef _QUAD_PRECISION // Unused
#define __sin // Not implemented.
#define __cos
#define __tan
#else
#ifdef _USE_RADIANS_FOR_SINCOS
#define __sin __IBM_SINF64
#define __cos __IBM_COSF64
#define __tan __IBM_TANF64
#else
#define __sin __CV_SIN_ANGLES
#define __cos __CV_COS_ANGLES
#define __tan __CV_TAN_ANGLES
#endif
#endif

// Safe mode for angles function, recommand to open when input number is very large.
#define __CV_SNCS_SAFE_MODE

// Trigonometric functions for angles
// TABLES
extern const float64 __CV_SinCos_Tab[];
// BASE FUNCTIONS
_Check_return_ float64 __cdecl __CV_SIN_ANGLES(_In_ float64 _X);
_Check_return_ float64 __cdecl __CV_COS_ANGLES(_In_ float64 _X);
_Check_return_ float64 __cdecl __CV_TAN_ANGLES(_In_ float64 _X);
// Trigonometric functions for radians
_EXTERN_C
// TABLES
extern const union __IBM_Table_Type440 __IBM_sincostab;
extern const __Float64 __IBM_TANF64_XFGTBL[186][4];
// BASE FUNCTIONS
_Check_return_ __Float64 __cdecl __IBM_SINF64(_In_ __Float64 x);
_Check_return_ __Float64 __cdecl __IBM_COSF64(_In_ __Float64 x);
_Check_return_ __Float64 __cdecl __IBM_TANF64(_In_ __Float64 x);
_END_EXTERN_C


/// <summary>
/// The standard trigonometric sine function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ _FType __cdecl sin(_In_ _FType _X);
_Check_return_ complex64 __cdecl sinc(_In_ complex64 _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl sin(_In_ genType _X);

/// <summary>
/// The standard trigonometric cosine function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ _FType __cdecl cos(_In_ _FType _X);
_Check_return_ complex64 __cdecl cosc(_In_ complex64 _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl cos(_In_ genType _X);

/// <summary>
/// The standard trigonometric tangent function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ _FType __cdecl tan(_In_ _FType _X);
_Check_return_ complex64 __cdecl tanc(_In_ complex64 _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl tan(_In_ genType _X);

/// <summary>
/// The standard trigonometric cotangent function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ _FType __cdecl ctg(_In_ _FType _X);
_Check_return_ complex64 __cdecl ctgc(_In_ complex64 _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl ctg(_In_ genType _X);

/// <summary>
/// The standard trigonometric secant function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ _FType __cdecl sec(_In_ _FType _X);
_Check_return_ complex64 __cdecl secc(_In_ complex64 _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl sec(_In_ genType _X);

/// <summary>
/// The standard trigonometric cosecant function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ _FType __cdecl csc(_In_ _FType _X);
_Check_return_ complex64 __cdecl cscc(_In_ complex64 _X);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl csc(_In_ genType _X);

///////////////////////////////////// INVERSE ////////////////////////////////////

// Use degrees for real number functions, radians for complex functions.
// define "_USE_RADIANS_FOR_SINCOS" to force using radians.
#ifdef _QUAD_PRECISION // Unused
#define __asin // Not implemented.
#define __acos
#define __atan
#else
#ifdef _USE_RADIANS_FOR_SINCOS
#define __asin __IBM_ASINF64
#define __acos __IBM_ACOSF64
#define __atan __IBM_ATANF64
#else
#define __asin __IBM_ASINF64
#define __acos __IBM_ACOSF64
#define __atan __IEEE754_ATANF128_CF64
#endif
#endif

_EXTERN_C
// TABLES
extern const union __IBM_Table_Type2568 __IBM_asncs_table;
extern const _FType __ArctanF128_table_deg[];
extern const _FType __ArctanF128_table_rad[];
// BASE FUNCTIONS
_Check_return_ __Float64 __cdecl __IBM_ASINF64(_In_ __Float64 x);
_Check_return_ __Float64 __cdecl __IBM_ACOSF64(_In_ __Float64 x);
// _Check_return_ __Float64 __cdecl __IBM_ATANF64(_In_ __Float64 x);
_Check_return_ _FType __cdecl __IEEE754_ATANF128_CF64(_In_ _FType x);
_END_EXTERN_C

/// <summary>
/// Arc Sine. Returns the angle whose trigonometric sine is x. The range of values returned by asin is [-90, 90].
/// </summary>
_Check_return_ _FType __cdecl arcsin(_In_ _FType _X);
_Check_return_ complex64 __cdecl arcsinc(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx = 0, int64 K_OFFSET = 0);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arcsin(_In_ genType _X);

/// <summary>
/// Arc cosine. Returns the angle whose trigonometric cosine is x. The range of values returned by acos is [0, 180].
/// </summary>
_Check_return_ _FType __cdecl arccos(_In_ _FType _X);
_Check_return_ complex64 __cdecl arccosc(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx = 0, int64 K_OFFSET = 0);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arccos(_In_ genType _X);

/// <summary>
/// Arc tangent. Returns the angle whose tangent is x. The value returned in this case is in the range [-90, 90].
/// </summary>
_Check_return_ _FType __cdecl arctan(_In_ _FType _X);
_Check_return_ complex64 __cdecl arctanc(_In_ complex64 _X, int64 K_OFFSET = 0);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arctan(_In_ genType _X);

_Check_return_ _FType __cdecl arcctg(_In_ _FType _X);
_Check_return_ complex64 __cdecl arcctgc(_In_ complex64 _X, int64 K_OFFSET = 0);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arcctg(_In_ genType _X);

_Check_return_ _FType __cdecl arcsec(_In_ _FType _X);
_Check_return_ complex64 __cdecl arcsecc(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx = 0, int64 K_OFFSET = 0);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arcsec(_In_ genType _X);

_Check_return_ _FType __cdecl arccsc(_In_ _FType _X);
_Check_return_ complex64 __cdecl arccscc(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx = 0, int64 K_OFFSET = 0);

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arccsc(_In_ genType _X);

///////////////////////////////////// EXPAND ////////////////////////////////////

// A few functions were common historically, but are now seldom used, 
// such as: 
// the chord
// the versine (which appeared in the earliest tables), coversine, the haversine
// the exsecant and the excosecant.
// Define "_USE_FULL_TRIGONOMETRY_SYSTEM" to release them.

#ifdef _USE_FULL_TRIGONOMETRY_SYSTEM

_Check_return_ _FType __cdecl crd(_In_ _FType _X, _In_ _FType _Radius = 1); // chord
_Check_return_ _FType __cdecl arc(_In_ _FType _X, _In_ _FType _Radius = 1); // arc

_Check_return_ _FType __cdecl siv(_In_ _FType _X); // versed sine
_Check_return_ _FType __cdecl vcs(_In_ _FType _X); // versed cosine
_Check_return_ _FType __cdecl cvs(_In_ _FType _X); // coversed sine
_Check_return_ _FType __cdecl cvc(_In_ _FType _X); // coversed cosine

_Check_return_ _FType __cdecl hvs(_In_ _FType _X); // haversed sine
_Check_return_ _FType __cdecl hvc(_In_ _FType _X); // haversed cosine
_Check_return_ _FType __cdecl hcv(_In_ _FType _X); // hacoversed sine
_Check_return_ _FType __cdecl hcc(_In_ _FType _X); // hacoversed cosine

_Check_return_ _FType __cdecl exs(_In_ _FType _X); // exsecant
_Check_return_ _FType __cdecl exc(_In_ _FType _X); // excosecant

// Inverse functions

_Check_return_ _FType __cdecl arccrd(_In_ _FType _X, _In_ _FType _Radius = 1);

_Check_return_ _FType __cdecl arcsiv(_In_ _FType _X);
_Check_return_ _FType __cdecl arcvcs(_In_ _FType _X);
_Check_return_ _FType __cdecl arccvs(_In_ _FType _X);
_Check_return_ _FType __cdecl arccvc(_In_ _FType _X);

_Check_return_ _FType __cdecl archvs(_In_ _FType _X);
_Check_return_ _FType __cdecl archvc(_In_ _FType _X);
_Check_return_ _FType __cdecl archcv(_In_ _FType _X);
_Check_return_ _FType __cdecl archcc(_In_ _FType _X);

_Check_return_ _FType __cdecl arcexs(_In_ _FType _X);
_Check_return_ _FType __cdecl arcexc(_In_ _FType _X);

#endif

/////////////////////////////////////////////////////////////////
//                      Solving Equations                      //
//                                                             //
// Finds roots of Polynomial equations.                        //
// Linear and Quadratic function use common algorithm.         //
// Cubic function using Fan Shengjin's algorithm.              //
// polynomials using Durand-Kerner method.                     //
//                                                             //
// Port by StellarDX, 2022.                                    //
/////////////////////////////////////////////////////////////////

/// <summary>
/// Finds the real root of a linear equation.
/// </summary>
/// <param name="Coeffs">A 2-element vector to store coefficients(Sorted by descending powers)</param>
/// <param name="Roots">The roots are stored in this array</param>
int SolveLinear(InputArray Coeffs, OutputArray Roots);

/// <summary>
/// Finds the complex roots of a quadratic equation.
/// </summary>
/// <param name="Coeffs">A 3-element vector to store coefficients(Sorted by descending powers)</param>
/// <param name="Roots">The roots are stored in this array</param>
int SolveQuadratic(InputArray Coeffs, OutputArray Roots);

/// <summary>
/// Finds the complex roots of a cubic equation.
/// </summary>
/// <param name="Coeffs">A 4-element vector to store coefficients(Sorted by descending powers)</param>
/// <param name="Roots">The roots are stored in this array</param>
/// <param name="p_Error">Negative logarithm of errors, default is 10, means 10^-10</param>
/// <returns>Formula number used</returns>
int SolveCubic(InputArray Coeffs, OutputArray Roots, int64 p_Error = 10);

/// <summary>
/// Finds the complex roots of a quartic equation.
/// </summary>
/// <param name="Coeffs">A 5-element vector to store coefficients(Sorted by descending powers)</param>
/// <param name="Roots">The roots are stored in this array</param>
/// <param name="p_Error">Negative logarithm of errors, default is 10, means 10^-10</param>
/// <returns>Formula number used</returns>
int SolveQuartic(InputArray Coeffs, OutputArray Roots, int64 p_Error = 10);

/* ************************************************************************** *\
   StellarDX: Finding the solving algorithms to a quintic or higher equation
   had troubled mathematicians for more than 300 years.
   Some people in history were tried to derivate the common solving algorithms,
   but failed. In 1926, Su Jiaju published an article which introduced his
   solution and derivation, and discovered a quintic equation solving formula.
   But soon, his theory has been disproved by Hua Luogeng.
   Now, it has been confirmed that solutions of a quintic or higher equation
   can't be expressed as nth roots. In other words, there is no common solving
   formulas for these equations. However, this doesn't mean that they are
   completely unsolvable. Iterations maybe the best method to find numerical
   roots. Here use the Durand–Kerner method to solve this problem.
\* ************************************************************************** */

// Addition:
// In fact, after reading Su Jiaju's paper, Hua Luogeng also made mistakes.
// He extends Su's work and published a root-finding algorithm to solve sextic equation.
// Subsequently, Hua Luogeng tried to disprove Abel's theory (Abel has proved that 
// quintic or higher equations has no root-finding formula), but soon he had to admit
// that Abel's theory is "well-regulated and impeccable"
// So, he began to turn to find errors in Su Jiaju's theory, and published his second
// paper that disproved Su Jiaju's theory and his previous theory.

// (If they are correct, this library would contain their algorithms as functions 
// to solving quintic and sextic equations, and would name them as "Su Jiaju's algorithm"
// and "Hua Luogeng's algorithm", because the formula for cubic functions is named
// "Fan Shengjin's algorithm" and for quartic function is "Shen Tianheng's algorithm", 
// but both of Su and Hua are wrong...)

struct _SOLVEPOLY_CONFIG
{
	float64          P_ERROR     = 15;
	float64          MAXITER_LOG = 3;
	complex64        BASE        = 0.4 + 0.9i;
};

/// <summary>
/// finds complex roots of a polynomial using Durand-Kerner method:
/// http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method
/// </summary>
/// <param name="Coeffs">A array to store coefficients</param>
/// <param name="Roots">The roots are stored in this array</param>
/// <param name="Conf">Function configs</param>
/// <returns>Number of iteration</returns>
uint64 SolvePoly(InputArray Coeffs, OutputArray Roots, _SOLVEPOLY_CONFIG Conf = _SOLVEPOLY_CONFIG());

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#include "CSE/Core/Inline/MathFuncs.Template.inl"

#pragma warning(pop)

#endif