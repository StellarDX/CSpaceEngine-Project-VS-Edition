/////////////////////////////////////////////////////////////
//                                                         //
//              Math module for CSpaceEngine               //
//                                                         //
/////////////////////////////////////////////////////////////

#pragma once

#ifndef _CSE_MATH_MODULE
#define _CSE_MATH_MODULE

#include "../Core/CSECore.h"
#include "../Core/ConstLists.h"
#include "../gl/gltypes.h"
#include <cmath>   // Base on STD Math
#include <complex> // and add complex types
#include <array>
#include <vector>

/* ************************************************************************** *\
   genTypes in Templates can be multiple data types:
	   genType = float64, complex64, vec2, vec3, vec4
	   genIType = int64, ivec2, ivec3, ivec4
	   genUType = uint64, uvec2, uvec3, uvec4
	   genBType = bool, bvec2, bvec3, bvec4
   and no genDTypes because all the types are already in 64-bit.
\* ************************************************************************** */

_CSE_BEGIN

using std::complex;
using complex64 = complex<float64>;
using std::literals::complex_literals::operator""i;

using std::array;
using std::vector;

#if 0
template<typename genType>struct floatType;
template<>struct floatType<float64> { typedef float64 type; };
template<>struct floatType<complex64> { typedef complex64 type; };
#endif

template<typename genType> // Only used in C++20
concept floatType =
(
	is_same_v<genType, float64> ||
	is_same_v<genType, complex64>
);

class MathException : public std::runtime_error // output errors
{
public:
	MathException(const std::string& Error) : std::runtime_error{ Error }{}
};

extern CSELog MathLog;

float64 sgn(float64 x);

/****************************************************************************************\
*                                Trigonometric functions                                 *
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
float64 radians(float64 degrees); // base functions
/// <summary>
/// converts radians into degrees.
/// </summary>
float64 degrees(float64 radians);
/// <summary>
/// The standard trigonometric sine function(based on angle).
/// </summary>
float64 sin(float64 angle);
/// <summary>
/// The standard trigonometric cosine function(based on angle).
/// </summary>
float64 cos(float64 angle);
/// <summary>
/// The standard trigonometric tangent function(based on angle).
/// </summary>
float64 tan(float64 angle);

template<typename genType = complex64> requires floatType<genType> genType sin(genType radians); // expand to complex numbers
template<typename genType = complex64> requires floatType<genType> genType cos(genType radians);
template<typename genType = complex64> requires floatType<genType> genType tan(genType radians);

template<typename genType> requires vecType<genType> genType sin(genType angle); // for Arrays
template<typename genType> requires vecType<genType> genType cos(genType angle);
template<typename genType> requires vecType<genType> genType tan(genType angle);

///////////////////////////////////// EXPAND ////////////////////////////////////

float64 ctg(float64 angle); // cotangent
float64 sec(float64 angle); // secant
float64 csc(float64 angle); // cosecant

float64 crd(float64 angle, float64 radius = 1); // chord
float64 arc(float64 angle, float64 radius = 1); // arc

float64 siv(float64 angle); // versed sine
float64 vcs(float64 angle); // versed cosine
float64 cvs(float64 angle); // coversed sine
float64 cvc(float64 angle); // coversed cosine

float64 hvs(float64 angle); // haversed sine
float64 hvc(float64 angle); // haversed cosine
float64 hcv(float64 angle); // hacoversed sine
float64 hcc(float64 angle); // hacoversed cosine

float64 exs(float64 angle); // exsecant
float64 exc(float64 angle); // excosecant

template<typename genType = complex64> requires floatType<genType> genType ctg(genType angle); // expand to complex numbers
template<typename genType = complex64> requires floatType<genType> genType sec(genType angle);
template<typename genType = complex64> requires floatType<genType> genType csc(genType angle);

template<typename genType> requires vecType<genType> genType sec(genType angle); // for Arrays
template<typename genType> requires vecType<genType> genType csc(genType angle);
template<typename genType> requires vecType<genType> genType ctg(genType angle);

///////////////////////////////////// INVERSE ////////////////////////////////////

/// <summary>
/// Arc Sine. Returns the angle whose trigonometric sine is x. The range of values returned by asin is [-90, 90].
/// </summary>
float64 arcsin(float64 x);
/// <summary>
/// Arc cosine. Returns the angle whose trigonometric cosine is x. The range of values returned by acos is [0, 180].
/// </summary>
float64 arccos(float64 x);
/// <summary>
/// Arc tangent. Returns the angle whose tangent is y_over_x. The value returned in this case is in the range [-90, 90].
/// </summary>
float64 arctan(float64 x);
//float64 arctan(float64 y, float64 x); // SCRAPPED

float64 arcctg(float64 x);
float64 arcsec(float64 x);
float64 arccsc(float64 x);

float64 arccrd(float64 c, float64 r = 1);

float64 arcsiv(float64 y);
float64 arcvcs(float64 y);
float64 arccvs(float64 y);
float64 arccvc(float64 y);

float64 archvs(float64 y);
float64 archvc(float64 y);
float64 archcv(float64 y);
float64 archcc(float64 y);

float64 arcexs(float64 y);
float64 arcexc(float64 y);

template<typename genType = complex64> requires floatType<genType> genType arcsin(genType z); // Extension to complex plane
template<typename genType = complex64> requires floatType<genType> genType arccos(genType z);
template<typename genType = complex64> requires floatType<genType> genType arctan(genType z);
template<typename genType = complex64> requires floatType<genType> genType arcctg(genType z);
template<typename genType = complex64> requires floatType<genType> genType arcsec(genType z);
template<typename genType = complex64> requires floatType<genType> genType arccsc(genType z);

template<typename genType> requires vecType<genType> genType arcsin(genType x); // for Arrays
template<typename genType> requires vecType<genType> genType arccos(genType x);
template<typename genType> requires vecType<genType> genType arctan(genType x);
template<typename genType> requires vecType<genType> genType arcctg(genType x);
template<typename genType> requires vecType<genType> genType arcsec(genType x);
template<typename genType> requires vecType<genType> genType arccsc(genType x);

/****************************************************************************************\
*                                          E X P                                         *
\****************************************************************************************/

/// <summary>
/// Returns the natural exponentiation of x. i.e., e^x.
/// </summary>
float64 exp(float64 x); // base function

template<typename genType> requires floatType<genType> 
genType exp(genType x); // expand to complex numbers

template<typename genType> requires vecType<genType>
genType exp(genType x); // for Arrays

// Custom functions(SCRAPPED)
/*extern const float64 _EXPTAB[3][16];
float64 exptalor(float64 x, int64 NItems = 4);
float64 exp64f(float64 x);*/

/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

/// <summary>
/// Returns the logarithm of x, i.e. the value y which satisfies x=base^y.
/// </summary>
/// <param name="base">Specify the base value</param>
/// <param name="x">Specify the value of which to take the logarithm.</param>
float64 log(float64 base, float64 x);  // base functions
float64 log(float64 x);
float64 ln(float64 x);

template<typename genType> requires floatType<genType> genType log(genType base, genType x); // expand to complex numbers
template<typename genType> requires floatType<genType> genType log(genType x);
template<typename genType> requires floatType<genType> genType ln(genType x);

template<typename genType> requires vecType<genType> genType log(genType base, genType x); // for Arrays
template<typename genType> requires vecType<genType> genType log(genType x);
template<typename genType> requires vecType<genType> genType ln(genType x);

// Custom functions(SCRAPPED)
/*float64 lntalor(float64 x, int64 NItems = 100);
float64 ln64f(float64 x);*/

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

/// <summary>
/// Returns the value of x raised to the y power, i.e. x^y.
/// </summary>
/// <param name="x">Specify the value to raise to the power y.</param>
/// <param name="power">Specify the power to which to raise x.</param>
float64 pow(float64 x, float64 power); // base functions
/// <summary>
/// Returns the square root of x, i.e. the value ¡Ìx.
/// </summary>
float64 sqrt(float64 x);
/// <summary>
/// Returns the inverse of the square root of x, i.e. the value 1/¡Ìx
/// </summary>
float64 inversesqrt(float64 x);
/// <summary>
/// Returns the cube root of x, i.e. the value 3¡Ìx.
/// </summary>
float64 cbrt(float64 x);

template<typename genType> requires floatType<genType> genType pow(genType x, genType power); // expand to complex numbers
template<typename genType> requires floatType<genType> genType sqrt(genType x);
template<typename genType> requires floatType<genType> genType inversesqrt(genType x);
template<typename genType> requires floatType<genType> genType cbrt(genType x);

template<typename genType> requires vecType<genType> genType pow(genType x, genType power); // for Arrays
template<typename genType> requires vecType<genType> genType sqrt(genType x);
template<typename genType> requires vecType<genType> genType inversesqrt(genType x);
template<typename genType> requires vecType<genType> genType cbrt(genType x);

// Custom functions(SCRAPPED)
/*float64 iPow64f(float64 x, float64 power);
float64 CubeRoot(float64 x);*/

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
#if 0
template<typename InputArray>struct EquationsInputArray64f;
template<>struct EquationsInputArray64f<vec2> { typedef vec2 type; };
template<>struct EquationsInputArray64f<vec3> { typedef vec3 type; };
template<>struct EquationsInputArray64f<vec4> { typedef vec4 type; };
template<>struct EquationsInputArray64f<float64*> { typedef float64* type; };
template<>struct EquationsInputArray64f<vector<float64>> { typedef vector<float64> type; };

template<typename OutputArray>struct EquationsOutputArray64f;
template<>struct EquationsOutputArray64f<complex64*> { typedef complex64* type; };
template<>struct EquationsOutputArray64f<vector<complex64>&> { typedef vector<complex64>& type; };
#endif

using std::convertible_to;

template<typename genType, int64 size> // Only used in C++20
concept EquationsInputArray64f =
(
	vecType<genType> || 
	convertible_to<genType, float64*> ||
	convertible_to<genType, array<float64, size>> ||
	convertible_to<genType, vector<float64>>
);

template<typename genType, int64 size> // Only used in C++20
concept EquationsOutputArray64f =
(
	convertible_to<genType, complex64*> ||
	convertible_to<genType, array<complex64, size>> ||
	convertible_to<genType, vector<complex64>>
);

/// <summary>
/// Finds the real root of a linear equation.
/// </summary>
/// <param name="Coeffs">A 2-element vector to store coefficients(Sorted by descending powers)</param>
/// <param name="Roots">The roots are stored in this array</param>
template<typename InputArray = float64*, typename OutputArray = float64*>
requires EquationsInputArray64f<InputArray, 2>
int SolveLinear(InputArray Coeffs, OutputArray& Roots);

/// <summary>
/// Finds the complex roots of a quadratic equation.
/// </summary>
/// <param name="Coeffs">A 3-element vector to store coefficients(Sorted by descending powers)</param>
/// <param name="Roots">The roots are stored in this array</param>
template<typename InputArray = float64*, typename OutputArray = complex64*>
requires EquationsInputArray64f<InputArray, 3> && EquationsOutputArray64f<OutputArray, 2>
int SolveQuadratic(InputArray Coeffs, OutputArray& Roots);

/// <summary>
/// Finds the complex roots of a cubic equation.
/// </summary>
/// <param name="Coeffs">A 4-element vector to store coefficients(Sorted by descending powers)</param>
/// <param name="Roots">The roots are stored in this array</param>
/// <param name="p_Error">Negative logarithm of errors, default is 10, means 10^-10</param>
/// <returns>Formula number used</returns>
template<typename InputArray = float64*, typename OutputArray = complex64*>
requires EquationsInputArray64f<InputArray, 4> && EquationsOutputArray64f<OutputArray, 3>
int SolveCubic(InputArray Coeffs, OutputArray& Roots, int64 p_Error = 10);

/// <summary>
/// Finds the complex roots of a quartic equation.
/// </summary>
/// <param name="Coeffs">A 5-element vector to store coefficients(Sorted by descending powers)</param>
/// <param name="Roots">The roots are stored in this array</param>
/// <param name="p_Error">Negative logarithm of errors, default is 10, means 10^-10</param>
/// <returns>Formula number used</returns>
template<typename InputArray = float64*, typename OutputArray = complex64*>
requires EquationsInputArray64f<InputArray, 5> && EquationsOutputArray64f<OutputArray, 4>
int SolveQuartic(InputArray Coeffs, OutputArray& Roots, int64 p_Error = 10);

/* ************************************************************************** *\
   StellarDX: Finding the solving algorithms to a quintic or higher equation 
   had troubled mathematicians for more than 300 years.
   Some people in history were tried to derivate the common solving algorithms,
   but failed. In 1926, Su Jiaju published an article which introduced his 
   solution and derivation, and discoverd a quintic equation soving formula.
   But soon, his theory has been disproved by Hua Luogeng.
   Now, it has been confirmed that solutions of a quintic or higher equation
   can't be expressed as nth roots. In other words, there is no common solving 
   formulas for these equations. However, this doesn't mean that they are 
   completely unsolvable. Iterations maybe the best method to find numerical 
   roots. Here use the Durand¨CKerner method to solve this problem.
\* ************************************************************************** */

/// <summary>
/// finds complex roots of a polynomial using Durand-Kerner method:
/// http://en.wikipedia.org/wiki/Durand%E2%80%93Kerner_method
/// </summary>
/// <param name="Coeffs">A array to store coefficients</param>
/// <param name="Roots">The roots are stored in this array</param>
/// <param name="p_Error">Negative logarithm of errors, default is 15, means 10^-15</param>
/// <param name="MaxIterLog">Logarithm of Max number of iterations, defult is 3</param>
/// <param name="Base">Base number use to solve.</param>
/// <returns>Number of iteration</returns>
uint64 SolvePoly(vector<float64> Coeffs, vector<complex64>& Roots, int64 p_Error = 15, int64 MaxIterLog = 3, complex64 Base = 0.4 + 0.9i);

_CSE_END

#include "..\..\astromath\MathFuncs.Complex.inl" // Inline file
#include "..\..\astromath\MathFuncs.Template.inl" // Inline file

// Reference:
// OpenGL 4.x reference page - https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/

#endif