#ifndef __MATHFUNCS_COMP__
#define __MATHFUNCS_COMP__

#include "..\headers\math\AsMath.h"

#include <string>

using std::complex;
using std::literals::complex_literals::operator""i;

_CSE_BEGIN

/****************************************************************************************\
*                                Trigonometric functions                                 *
\****************************************************************************************/

template<typename genType> requires floatType<genType>
genType sin(genType radians)
{
	return (exp<complex64>(1i * radians) - exp<complex64>(-1i * radians)) / 2.0i; // Defined by Eular's Formula
}

template<typename genType> requires floatType<genType>
genType cos(genType radians)
{
	return (exp<complex64>(1i * radians) + exp<complex64>(-1i * radians)) / 2.0; // Defined by Eular's Formula
}

template<typename genType> requires floatType<genType>
genType tan(genType radians)
{
	return sin<complex64>(radians) / cos<complex64>(radians);
}

template<typename genType> requires floatType<genType>
genType ctg(genType radians)
{
	return 1.0 / tan<complex64>(radians);
}

template<typename genType> requires floatType<genType>
genType sec(genType radians)
{
	return 1.0 / cos<complex64>(radians);
}

template<typename genType> requires floatType<genType>
genType csc(genType radians)
{
	return 1.0 / sin<complex64>(radians);
}

///////////////////////////////////// INVERSE ////////////////////////////////////

template<typename genType> requires floatType<genType>
genType arcsin(genType z)
{
	return 1i * ln<complex64>(sqrt<complex64>(1.0 - cse::pow<complex64>(z, 2.0)) - 1i * z); // Defined by complex logarithms
}

template<typename genType> requires floatType<genType>
genType arccos(genType z)
{
	return -1i * ln<complex64>(1i * sqrt<complex64>(1.0 - cse::pow<complex64>(z, 2.0)) + z);
}

template<typename genType> requires floatType<genType>
genType arctan(genType z)
{
	return -(1i / 2.0) * ln<complex64>((1.0 + 1i * z) / (1.0 - 1i * z));
}

template<typename genType> requires floatType<genType>
genType arcctg(genType z)
{
	return -(1i / 2.0) * ln<complex64>((1i * z - 1.0) / (1i * z + 1.0));
}

template<typename genType> requires floatType<genType>
genType arcsec(genType z)
{
	return -1i * ln<complex64>(1i * sqrt<complex64>(1.0 - 1.0 / cse::pow<complex64>(z, 2.0)) + 1.0 / z);
}

template<typename genType> requires floatType<genType>
genType arccsc(genType z)
{
	return 1i * ln<complex64>(sqrt<complex64>(1.0 - 1.0 / cse::pow<complex64>(z, 2.0)) - 1i / z);
}

/****************************************************************************************\
*                                          E X P                                         *
\****************************************************************************************/

template<typename genType> requires floatType<genType>
genType exp(genType x)
{
	return std::exp(x);
}

/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

template<typename genType> requires floatType<genType>
genType log(genType base, genType x)
{
	return std::log(x) / std::log(base);
}

template<typename genType> requires floatType<genType>
genType log(genType x)
{
	return std::log10(x);
}

template<typename genType> requires floatType<genType>
genType ln(genType x)
{
	return std::log(x);
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template<typename genType> requires floatType<genType>
genType pow(genType x, genType power)
{
	return std::pow(x, power);
}

template<typename genType> requires floatType<genType>
genType sqrt(genType x)
{
	return std::sqrt(x);
}

template<typename genType> requires floatType<genType>
genType inversesqrt(genType x)
{
	return 1.0 / std::sqrt(x);
}

template<typename genType> requires floatType<genType>
genType cbrt(genType x)
{
	return std::cbrt(x);
}

// Because floating-points have a very small error so it maybe unstable when comparing with 0
// so use this function to compare a float with zero.
inline bool IsZero(float64 _src, float64 _log_eps)
{
	if (-pow(10, _log_eps) <= _src &&
		_src <= pow(10, _log_eps))
	{
		MathLog.Out("MathFuncs-Complex", "WARNING", "Number : " + std::to_string(_src) + " is been considered to be zero, may cause accuracy loss.", SysLogLevel);
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////
//                      Solving Equations                      //
/////////////////////////////////////////////////////////////////

// Algorithms must be Orderly, symmetrical, harmonious and simple.

template<typename InputArray, typename OutputArray>
requires EquationsInputArray64f<InputArray, 2>
int SolveLinear(InputArray Coeffs, OutputArray& Roots)
{
	//if (Coeffs.size() != 2) { throw MathException("Number of Coefficients is not match."); }
	float64 a = Coeffs[0];
	float64 b = Coeffs[1];

	if (a == 0) { throw MathException("Highest power of polynomial can't be zero."); }

	Roots[0] = -b / a;

	return 0;
}

template<typename InputArray, typename OutputArray>
requires EquationsInputArray64f<InputArray, 3> && EquationsOutputArray64f<OutputArray, 2>
int SolveQuadratic(InputArray Coeffs, OutputArray& Roots)
{
	//if (Coeffs.size() != 3) { throw MathException("Number of Coefficients is not match."); }
	float64 a = Coeffs[0];
	float64 b = Coeffs[1];
	float64 c = Coeffs[2];

	if (a == 0){throw MathException("Highest power of polynomial can't be zero.");}

	float64 del = pow(b, 2) - 4.0 * a * c;

	Roots[0] = (-b + sqrt<complex64>(del)) / (2.0 * a);
	Roots[1] = (-b - sqrt<complex64>(del)) / (2.0 * a);

	return 0;
}

template<typename InputArray, typename OutputArray>
requires EquationsInputArray64f<InputArray, 4> && EquationsOutputArray64f<OutputArray, 3>
int SolveCubic(InputArray Coeffs, OutputArray& Roots, int64 p_Error)
{
	// Reference:
	// NATURAL SCIENCE JOURNAL OF HAINAN TEACHERES COLLEGE,Hainan Province,China. Vol. 2,No. 2; Dec, 1989,
	// A new extracting formula and a new distinguishing means on the one variable cubic equation., Fan Shengjin. PP.91-98 .
	float64 a = Coeffs[0];
	float64 b = Coeffs[1];
	float64 c = Coeffs[2];
	float64 d = Coeffs[3];

	if (a == 0) { throw MathException("Highest power of polynomial can't be zero."); }

	float64 A = pow(b, 2) - 3.0 * a * c;
	float64 B = b * c - 9.0 * a * d;
	float64 C = pow(c, 2) - 3.0 * b * d;

	float64 del = pow(B, 2) - 4.0 * A * C;
	//std::cout << del << '\n';

	if (IsZero(A, -p_Error) && IsZero(B, -p_Error))
	{
		Roots[0] = -b / (3.0 * a);
		Roots[1] = Roots[0]; // -c / b
		Roots[2] = Roots[0]; // -3 * d / c

		return 1;
	}

	if (IsZero(del, -p_Error))
	{
		float64 K = B / A;

		Roots[0] = -b / a + K;
		Roots[1] = -K / 2;
		Roots[2] = Roots[1];

		return 3;
	}

	if (del > 0)
	{
		float64 Y1 = A * b + (3. * a * (-B - sqrt(del)) / 2.);
		float64 Y2 = A * b + (3. * a * (-B + sqrt(del)) / 2.);

		Roots[0] = (-b - (cbrt(Y1) + cbrt(Y2))) / (3. * a);
		Roots[1] = (-2. * b + (cbrt(Y1) + cbrt(Y2)) + sqrt(3.) * (cbrt(Y1) - cbrt(Y2)) * 1i) / (6. * a);
		Roots[2] = (-2. * b + (cbrt(Y1) + cbrt(Y2)) - sqrt(3.) * (cbrt(Y1) - cbrt(Y2)) * 1i) / (6. * a);

		return 2;
	}

	if (del < 0)
	{
		float64 tet = arccos((2 * A * b - 3 * a * B) / (2. * sqrt(pow(A, 3)))); // (+2*k*PI)

		Roots[0] = (-b - 2. * sqrt(A) * cos(tet / 3.)) / (3. * a);
		Roots[1] = (-b + sqrt(A) * (cos(tet / 3) + sqrt(3) * sin(tet / 3))) / (3. * a);
		Roots[2] = (-b + sqrt(A) * (cos(tet / 3) - sqrt(3) * sin(tet / 3))) / (3. * a);

		return 4;
	}

	return 0;
}

template<typename InputArray, typename OutputArray>
requires EquationsInputArray64f<InputArray, 5> && EquationsOutputArray64f<OutputArray, 4>
int SolveQuartic(InputArray Coeffs, OutputArray& Roots, int64 p_Error)
{
	float64 a = Coeffs[0];
	float64 b = Coeffs[1];
	float64 c = Coeffs[2];
	float64 d = Coeffs[3];
	float64 e = Coeffs[4];

	if (a == 0) { throw MathException("Highest power of polynomial can't be zero."); }

	float64 D = 3.0 * pow(b, 2.0) - 8.0 * a * c;
	float64 E = -pow(b, 3.0) + 4 * a * b * c - 8 * pow(a, 2.0) * d;
	float64 F = 3.0 * pow(b, 4.0) + 16.0 * pow(a, 2.0) * pow(c, 2.0) - 16.0 * a * pow(b, 2.0) * c + 16.0 * pow(a, 2.0) * b * d - 64.0 * pow(a, 3.0) * e;
	float64 A = pow(D, 2.0) - 3.0 * F;
	float64 B = D * F - 9.0 * pow(E, 2.0);
	float64 C = pow(F, 2.0) - 3.0 * D * pow(E, 2.0);

	float64 del = pow(B, 2) - 4.0 * A * C;

	if (IsZero(D, -p_Error) && IsZero(E, -p_Error) && IsZero(F, -p_Error))
	{
		Roots[0] = -b / (4.0 * a);
		Roots[1] = Roots[0]; // -(2 * c) / (3 * b)
		Roots[2] = Roots[0]; // -(3 * d) / (2 * c)
		Roots[3] = Roots[0]; // -(4 * e) / d

		return 1;
	}

	if (!IsZero(D*E*F, -p_Error) && IsZero(A, -p_Error) && IsZero(B, -p_Error) && IsZero(C, -p_Error))
	{
		Roots[0] = (-b * D + 9. * E) / (4. * a * D);
		Roots[1] = (-b * D - 3. * E) / (4. * a * D);
		Roots[2] = Roots[1];
		Roots[3] = Roots[1];

		return 2;
	}

	if (IsZero(E, -p_Error) && IsZero(F, -p_Error) && !IsZero(D, -p_Error))
	{
		Roots[0] = (-b + sqrt<complex64>(D)) / (4. * a);
		Roots[1] = Roots[0];
		Roots[2] = (-b - sqrt<complex64>(D)) / (4. * a);
		Roots[3] = Roots[2];

		return 3;
	}

	if (!IsZero(A*B*C, -p_Error) && IsZero(del, -p_Error))
	{
		Roots[0] = (-b + (2. * A * E) / B + sqrt<complex64>((2. * B) / A)) / (4. * a);
		Roots[1] = (-b + (2. * A * E) / B - sqrt<complex64>((2. * B) / A)) / (4. * a);
		Roots[2] = (-b - (2. * A * E) / B) / (4. * a);
		Roots[3] = Roots[2];

		return 4;
	}

	if (del > 0)
	{
		float64 z1 = A * D + 3. * ((-B + sqrt(del)) / 2.);
		float64 z2 = A * D + 3. * ((-B - sqrt(del)) / 2.);
		complex64 z = pow(D, 2.) - D * (cbrt(z1) + cbrt(z2)) + pow((cbrt(z1) + cbrt(z2)), 2.) - 3. * A;

		Roots[0] = (-b + sgn(E) * sqrt((D + cbrt(z1) + cbrt(z2)) / 3.) + sqrt((2. * D - (cbrt(z1) + cbrt(z2)) + 2. * sqrt(z)) / 3.)) / (4. * a);
		Roots[1] = (-b + sgn(E) * sqrt((D + cbrt(z1) + cbrt(z2)) / 3.) - sqrt((2. * D - (cbrt(z1) + cbrt(z2)) + 2. * sqrt(z)) / 3.)) / (4. * a);
		Roots[2] = (-b - sgn(E) * sqrt((D + cbrt(z1) + cbrt(z2)) / 3.)) / (4. * a) + (sqrt((-2. * D + cbrt(z1) + cbrt(z2) + 2. * sqrt(z)) / 3.) / (4. * a)) * 1i;
		Roots[3] = (-b - sgn(E) * sqrt((D + cbrt(z1) + cbrt(z2)) / 3.)) / (4. * a) - (sqrt((-2. * D + cbrt(z1) + cbrt(z2) + 2. * sqrt(z)) / 3.) / (4. * a)) * 1i;

		return 5;
	}

	if (del < 0)
	{
		float64 tet = arccos((3. * B - 2. * A * D) / (2. * A * sqrt(A)));
		float64 y1 = (D - 2. * sqrt(A) * cos(tet / 3.)) / 3.;
		float64 y2 = (D + sqrt(A) * (cos(tet / 3.) + sqrt(3.) * sin(tet / 3.))) / 3.;
		float64 y3 = (D + sqrt(A) * (cos(tet / 3.) - sqrt(3.) * sin(tet / 3.))) / 3.;

		if (IsZero(E, -p_Error) && D > 0 && F > 0)
		{
			Roots[0] = (-b + sqrt(D + 2. * sqrt(F))) / (4. * a);
			Roots[1] = (-b - sqrt(D + 2. * sqrt(F))) / (4. * a);
			Roots[2] = (-b + sqrt(D - 2. * sqrt(F))) / (4. * a);
			Roots[3] = (-b - sqrt(D - 2. * sqrt(F))) / (4. * a);

			return 6;
		}

		if (IsZero(E, -p_Error) && D < 0 && F > 0)
		{
			Roots[0] = -b / (4. * a) + (sqrt(-D + 2. * sqrt(F)) / (4. * a)) * 1i;
			Roots[1] = -b / (4. * a) - (sqrt(-D + 2. * sqrt(F)) / (4. * a)) * 1i;
			Roots[2] = -b / (4. * a) + (sqrt(-D - 2. * sqrt(F)) / (4. * a)) * 1i;
			Roots[3] = -b / (4. * a) - (sqrt(-D - 2. * sqrt(F)) / (4. * a)) * 1i;

			return 7;
		}

		if (IsZero(E, -p_Error) && F < 0)
		{
			Roots[0] = (-2. * b + sqrt(2. * D + 2. * sqrt(A - F))) / (8. * a) + ((sqrt(-2. * D + 2. * sqrt(A - F))) / (8. * a)) * 1i;
			Roots[1] = (-2. * b + sqrt(2. * D + 2. * sqrt(A - F))) / (8. * a) - ((sqrt(-2. * D + 2. * sqrt(A - F))) / (8. * a)) * 1i;
			Roots[2] = (-2. * b - sqrt(2. * D + 2. * sqrt(A - F))) / (8. * a) + ((sqrt(-2. * D + 2. * sqrt(A - F))) / (8. * a)) * 1i;
			Roots[3] = (-2. * b - sqrt(2. * D + 2. * sqrt(A - F))) / (8. * a) - ((sqrt(-2. * D + 2. * sqrt(A - F))) / (8. * a)) * 1i;

			return 8;
		}

		if
		(
			!IsZero(E, -p_Error)
			//&& max(y1, y2, y3) == y2
		)
		{
			if (D > 0 && F > 0)
			{
				Roots[0] = (-b + sgn(E) * sqrt(y1) + (sqrt(y2) + sqrt(y3))) / (4. * a);
				Roots[1] = (-b + sgn(E) * sqrt(y1) - (sqrt(y2) + sqrt(y3))) / (4. * a);
				Roots[2] = (-b - sgn(E) * sqrt(y1) + (sqrt(y2) - sqrt(y3))) / (4. * a);
				Roots[3] = (-b - sgn(E) * sqrt(y1) - (sqrt(y2) - sqrt(y3))) / (4. * a);

				return 9;
			}

			else
			{
				Roots[0] = (-b - sqrt(y2)) / (4. * a) + ((sgn(E) * sqrt(-y1) + sqrt(-y3)) / (4. * a)) * 1i;
				Roots[1] = (-b - sqrt(y2)) / (4. * a) - ((sgn(E) * sqrt(-y1) + sqrt(-y3)) / (4. * a)) * 1i;
				Roots[2] = (-b + sqrt(y2)) / (4. * a) + ((sgn(E) * sqrt(-y1) - sqrt(-y3)) / (4. * a)) * 1i;
				Roots[3] = (-b + sqrt(y2)) / (4. * a) - ((sgn(E) * sqrt(-y1) - sqrt(-y3)) / (4. * a)) * 1i;

				return 10;
			}
		}
	}

	return 0;
}

_CSE_END

#endif