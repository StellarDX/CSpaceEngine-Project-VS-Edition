#pragma once

#ifndef _CSE_MATH_BASE
#define _CSE_MATH_BASE

#include "../Core/CSECore.h"
#include "../Core/ConstLists.h"

#if 0//(defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__)
#include <cmath>   // Base on STD Math
#endif

#include "CSEM_Algo.h"
#include <array>
#include <vector>
//#include "Complex.h"
#include <complex> // and add complex types

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

extern const float96 _Exponential_Tab[]; // the default table used to calculate exp(x)

struct EXP_CONFIG // Default settings
{
	uint64           BITSEXPO   = 11;
	uint64           BITSFRAC   = 52;
	const float96*   TABLE      = _Exponential_Tab;
	uint64           SCALE      = 6;
	uint64           MASK       = (1 << SCALE) - 1;
};

/// <summary>
/// Returns the natural exponentiation of x. i.e., e^x.
/// </summary>
_Check_return_ float64 __cdecl exp(_In_ float64 _X, EXP_CONFIG _Conf = EXP_CONFIG());
_Check_return_ complex64 __cdecl exp(_In_ complex64 _X, EXP_CONFIG _Conf = EXP_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl exp(_In_ genType _X, EXP_CONFIG _Conf = EXP_CONFIG());

/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

extern const float96 _Logarithm_Tab[]; // the default table used to calculate ln(x)

struct LOG_CONFIG // used for ln(x) function
{
	uint64           BITSEXPO   = 11;
	uint64           BITSFRAC   = 52;
	uint64           SCALE      = 8;
	uint64           MASK       = (1ui64 << SCALE) - 1;
	const float96*   TABLE      = _Logarithm_Tab;
	uint64           TABSIZE    = (MASK + 1) * 2;

	int64            K_OFFSET   = 0; // for complex
};

/// <summary>
/// Returns the logarithm of x, i.e. the value y which satisfies x=base^y.
/// Overloads:
/// log(x) returns the base 10 logarithm of x, i.e. the value y which satisfies x=10^y.
/// ln(x) returns the natural logarithm of x.
/// </summary>
/// <param name="_Base">Specify the base value</param>
/// <param name="_X">Specify the value of which to take the logarithm.</param>
_Check_return_ float64 __cdecl log(_In_ float64 _Base, _In_ float64 _X, LOG_CONFIG _Conf = LOG_CONFIG());
_Check_return_ float64 __cdecl log(_In_ float64 _X, LOG_CONFIG _Conf = LOG_CONFIG());
_Check_return_ float64 __cdecl ln(_In_ float64 _X, LOG_CONFIG _Conf = LOG_CONFIG());
_Check_return_ complex64 __cdecl log(_In_ complex64 _Base, _In_ complex64 _X, LOG_CONFIG _Conf = LOG_CONFIG());
_Check_return_ complex64 __cdecl log(_In_ complex64 _X, LOG_CONFIG _Conf = LOG_CONFIG());
_Check_return_ complex64 __cdecl ln(_In_ complex64 _X, LOG_CONFIG _Conf = LOG_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl log(_In_ float64 _Base, _In_ genType _X, LOG_CONFIG _Conf = LOG_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl log(_In_ genType _Base, _In_ genType _X, LOG_CONFIG _Conf = LOG_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl log(_In_ genType _X, LOG_CONFIG _Conf = LOG_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl ln(_In_ genType _X, LOG_CONFIG _Conf = LOG_CONFIG());

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

struct POWER_CONFIG
{
	int64            K_OFFSET   = 0; // for complex
};

/// <summary>
/// Returns the value of x raised to the y power, i.e. x^y.
/// </summary>
/// <param name="_X">Specify the value to raise to the power y.</param>
/// <param name="_Power">Specify the power to which to raise x.</param>
_Check_return_ float64 __cdecl pow(_In_ float64 _X, _In_ float64 _Power, POWER_CONFIG _Conf = POWER_CONFIG()); // base functions
_Check_return_ complex64 __cdecl pow(_In_ complex64 _X, _In_ complex64 _Power, POWER_CONFIG _Conf = POWER_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl pow(_In_ genType _X, _In_ float64 _Power, POWER_CONFIG _Conf = POWER_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl pow(_In_ genType _X, _In_ genType _Power, POWER_CONFIG _Conf = POWER_CONFIG());

template<typename _Ty, size_t _Size>
_Check_return_ _GL basic_matrix<_Ty, _Size, _Size> __cdecl pow(_In_ _GL basic_matrix<_Ty, _Size, _Size> _A, _In_ uint64 _Power);

/// <summary>
/// Returns the square root of x, i.e. the value √x.
/// </summary>
_Check_return_ float64 __cdecl sqrt(_In_ float64 _X, POWER_CONFIG _Conf = POWER_CONFIG());
_Check_return_ std::array<complex64, 2> __cdecl sqrt(_In_ complex64 _X, POWER_CONFIG _Conf = POWER_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl sqrt(_In_ genType _X, POWER_CONFIG _Conf = POWER_CONFIG());

/// <summary>
/// Returns the inverse of the square root of x, i.e. the value 1/√x
/// </summary>
_Check_return_ float64 __cdecl inversesqrt(_In_ float64 _X, POWER_CONFIG _Conf = POWER_CONFIG());
_Check_return_ complex64 __cdecl inversesqrt(_In_ complex64 _X, POWER_CONFIG _Conf = POWER_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl inversesqrt(_In_ genType _X, POWER_CONFIG _Conf = POWER_CONFIG());

struct CBRT_CONFIG
{
	uint64           BITSEXPO   = 11;
	uint64           BITSFRAC   = 52;
	uint64           NUMITER    = 6;
};

/// <summary>
/// Returns the cube root of x, i.e. the value 3√x.
/// </summary>
_Check_return_ float64 __cdecl cbrt(_In_ float64 _X, CBRT_CONFIG _Conf = CBRT_CONFIG());
_Check_return_ std::array<complex64, 3> __cdecl cbrt(_In_ complex64 _X, CBRT_CONFIG _Conf = CBRT_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl cbrt(_In_ genType _X, CBRT_CONFIG _Conf = CBRT_CONFIG());

/// <summary>
/// Returns the nth-root of x, i.e. the value n√x.
/// </summary>
/// <param name="_Expo">Specify the value of exponential of root.</param>
/// <param name="_X">Specify the value to find root.</param>
_Check_return_ float64 __cdecl yroot(_In_ float64 _Expo, _In_ float64 _X);
_Check_return_ std::vector<complex64> __cdecl yroot(_In_ complex64 _Expo, _In_ complex64 _X, POWER_CONFIG _Conf = POWER_CONFIG());

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
_Check_return_ float64 __cdecl radians(_In_ float64 _Deg);

/// <summary>
/// converts radians into degrees.
/// </summary>
_Check_return_ float64 __cdecl degrees(_In_ float64 _Rad);

extern const float96 _SinCos_Tab[];

struct SINCOS_CONFIG
{
	const float96*   TABLE      = _SinCos_Tab;
	uint64           TABSIZE    = 64;
};

/// <summary>
/// The standard trigonometric sine function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ float64 __cdecl sin(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());
_Check_return_ complex64 __cdecl sin(_In_ complex64 _X, EXP_CONFIG _Conf = EXP_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl sin(_In_ genType _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());

/// <summary>
/// The standard trigonometric cosine function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ float64 __cdecl cos(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());
_Check_return_ complex64 __cdecl cos(_In_ complex64 _X, EXP_CONFIG _Conf = EXP_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl cos(_In_ genType _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());

/// <summary>
/// The standard trigonometric tangent function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ float64 __cdecl tan(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());
_Check_return_ complex64 __cdecl tan(_In_ complex64 _X, EXP_CONFIG _Conf = EXP_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl tan(_In_ genType _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());

/// <summary>
/// The standard trigonometric cotangent function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ float64 __cdecl ctg(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());
_Check_return_ complex64 __cdecl ctg(_In_ complex64 _X, EXP_CONFIG _Conf = EXP_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl ctg(_In_ genType _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());

/// <summary>
/// The standard trigonometric secant function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ float64 __cdecl sec(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());
_Check_return_ complex64 __cdecl sec(_In_ complex64 _X, EXP_CONFIG _Conf = EXP_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl sec(_In_ genType _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());

/// <summary>
/// The standard trigonometric cosecant function(Real number based on degrees, Complex based on radians).
/// </summary>
_Check_return_ float64 __cdecl csc(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());
_Check_return_ complex64 __cdecl csc(_In_ complex64 _X, EXP_CONFIG _Conf = EXP_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl csc(_In_ genType _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG());

///////////////////////////////////// INVERSE ////////////////////////////////////

struct ATAN_CONFIG
{
	// Nothing...
};

/// <summary>
/// Arc Sine. Returns the angle whose trigonometric sine is x. The range of values returned by asin is [-90, 90].
/// </summary>
_Check_return_ float64 __cdecl arcsin(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ complex64 __cdecl arcsin(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx = 0, LOG_CONFIG _Conf = LOG_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arcsin(_In_ genType _X, ATAN_CONFIG _Conf = ATAN_CONFIG());

/// <summary>
/// Arc cosine. Returns the angle whose trigonometric cosine is x. The range of values returned by acos is [0, 180].
/// </summary>
_Check_return_ float64 __cdecl arccos(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ complex64 __cdecl arccos(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx = 0, LOG_CONFIG _Conf = LOG_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arccos(_In_ genType _X, ATAN_CONFIG _Conf = ATAN_CONFIG());

/// <summary>
/// Arc tangent. Returns the angle whose tangent is x. The value returned in this case is in the range [-90, 90].
/// </summary>
_Check_return_ float64 __cdecl arctan(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ complex64 __cdecl arctan(_In_ complex64 _X, LOG_CONFIG _Conf = LOG_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arctan(_In_ genType _X, ATAN_CONFIG _Conf = ATAN_CONFIG());

_Check_return_ float64 __cdecl arcctg(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ complex64 __cdecl arcctg(_In_ complex64 _X, LOG_CONFIG _Conf = LOG_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arcctg(_In_ genType _X, ATAN_CONFIG _Conf = ATAN_CONFIG());

_Check_return_ float64 __cdecl arcsec(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ complex64 __cdecl arcsec(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx = 0, LOG_CONFIG _Conf = LOG_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arcsec(_In_ genType _X, ATAN_CONFIG _Conf = ATAN_CONFIG());

_Check_return_ float64 __cdecl arccsc(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ complex64 __cdecl arccsc(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx = 0, LOG_CONFIG _Conf = LOG_CONFIG());

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arccsc(_In_ genType _X, ATAN_CONFIG _Conf = ATAN_CONFIG());

///////////////////////////////////// EXPAND ////////////////////////////////////

// A few functions were common historically, but are now seldom used, 
// such as: 
// the chord
// the versine (which appeared in the earliest tables), coversine, the haversine
// the exsecant and the excosecant.
// Define "_USE_FULL_TRIGONOMETRY_SYSTEM" to release them.

#ifdef _USE_FULL_TRIGONOMETRY_SYSTEM

_Check_return_ float64 __cdecl crd(_In_ float64 _X, _In_ float64 _Radius = 1, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // chord
_Check_return_ float64 __cdecl arc(_In_ float64 _X, _In_ float64 _Radius = 1, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // arc

_Check_return_ float64 __cdecl siv(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // versed sine
_Check_return_ float64 __cdecl vcs(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // versed cosine
_Check_return_ float64 __cdecl cvs(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // coversed sine
_Check_return_ float64 __cdecl cvc(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // coversed cosine

_Check_return_ float64 __cdecl hvs(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // haversed sine
_Check_return_ float64 __cdecl hvc(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // haversed cosine
_Check_return_ float64 __cdecl hcv(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // hacoversed sine
_Check_return_ float64 __cdecl hcc(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // hacoversed cosine

_Check_return_ float64 __cdecl exs(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // exsecant
_Check_return_ float64 __cdecl exc(_In_ float64 _X, SINCOS_CONFIG _Conf = SINCOS_CONFIG()); // excosecant

// Inverse functions

_Check_return_ float64 __cdecl arccrd(_In_ float64 _X, _In_ float64 _Radius = 1, ATAN_CONFIG _Conf = ATAN_CONFIG());

_Check_return_ float64 __cdecl arcsiv(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ float64 __cdecl arcvcs(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ float64 __cdecl arccvs(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ float64 __cdecl arccvc(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());

_Check_return_ float64 __cdecl archvs(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ float64 __cdecl archvc(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ float64 __cdecl archcv(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ float64 __cdecl archcc(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());

_Check_return_ float64 __cdecl arcexs(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());
_Check_return_ float64 __cdecl arcexc(_In_ float64 _X, ATAN_CONFIG _Conf = ATAN_CONFIG());

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

#include "../../math/MathFuncs.Template.inl"

#endif