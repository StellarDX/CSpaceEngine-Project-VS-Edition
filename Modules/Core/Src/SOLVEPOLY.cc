#include "CSE/Core/CSEM_Func.h"

_CSE_BEGIN

using std::complex;

int SolveLinear(InputArray Coeffs, OutputArray Roots)
{
	if (Coeffs.size() != 2) { throw MathException("Number of Coefficients is not match."); }
	Roots.clear();
	float64 a = Coeffs[0];
	float64 b = Coeffs[1];

	if (a == 0) { throw MathException("Highest power of polynomial can't be zero."); }

	Roots.push_back(complex64(-b / a));

	return 0;
}

int SolveQuadratic(InputArray Coeffs, OutputArray Roots)
{
	if (Coeffs.size() != 3) { throw MathException("Number of Coefficients is not match."); }
	Roots.clear();
	float64 a = Coeffs[0];
	float64 b = Coeffs[1];
	float64 c = Coeffs[2];

	if (a == 0) { throw MathException("Highest power of polynomial can't be zero."); }

	float64 del = _CSE pow(b, 2) - 4.0 * a * c;

	Roots.push_back((-b + _CSE sqrt(complex64(del))[0]) / (2.0 * a));
	Roots.push_back((-b - _CSE sqrt(complex64(del))[0]) / (2.0 * a));

	return 0;
}

bool IsZero(float64 _Left, int64 _PErr) { return -_CSE pow(10, -float64(_PErr)) < _Left && _Left < +_CSE pow(10, -float64(_PErr)); }

int SolveCubic(InputArray Coeffs, OutputArray Roots, int64 p_Error)
{
	// Reference:
	// NATURAL SCIENCE JOURNAL OF HAINAN TEACHERES COLLEGE,Hainan Province,China. Vol. 2,No. 2; Dec, 1989,
	// A new extracting formula and a new distinguishing means on the one variable cubic equation., Fan Shengjin. PP.91-98 .
	if (Coeffs.size() != 4) { throw MathException("Number of Coefficients is not match."); }
	Roots.clear();
	float64 a = Coeffs[0];
	float64 b = Coeffs[1];
	float64 c = Coeffs[2];
	float64 d = Coeffs[3];

	if (a == 0) { throw MathException("Highest power of polynomial can't be zero."); }

	float64 A = _CSE pow(b, 2) - 3.0 * a * c;
	float64 B = b * c - 9.0 * a * d;
	float64 C = _CSE pow(c, 2) - 3.0 * b * d;

	float64 del = _CSE pow(B, 2) - 4.0 * A * C;
	//std::cout << del << '\n';

	if (IsZero(A, p_Error) && IsZero(B, p_Error))
	{
		Roots.push_back(-b / (3.0 * a));
		Roots.push_back(Roots[0]); // -c / b
		Roots.push_back(Roots[0]); // -3 * d / c

		return 1;
	}

	if (IsZero(del, p_Error))
	{
		float64 K = B / A;

		Roots.push_back(-b / a + K);
		Roots.push_back(-K / 2);
		Roots.push_back(Roots[1]);

		return 3;
	}

	if (del > 0)
	{
		float64 Y1 = A * b + (3. * a * (-B - _CSE sqrt(del)) / 2.);
		float64 Y2 = A * b + (3. * a * (-B + _CSE sqrt(del)) / 2.);

		Roots.push_back((-b - (_CSE cbrt(Y1) + _CSE cbrt(Y2))) / (3. * a));
		Roots.push_back((-2. * b + (_CSE cbrt(Y1) + _CSE cbrt(Y2)) + _CSE sqrt(3.) * (_CSE cbrt(Y1) - _CSE cbrt(Y2)) * 1i) / (6. * a));
		Roots.push_back((-2. * b + (_CSE cbrt(Y1) + _CSE cbrt(Y2)) - _CSE sqrt(3.) * (_CSE cbrt(Y1) - _CSE cbrt(Y2)) * 1i) / (6. * a));

		return 2;
	}

	if (del < 0)
	{
		float64 tet = arccos((2 * A * b - 3 * a * B) / (2. * _CSE sqrt(_CSE pow(A, 3)))); // (+2*k*PI)

		Roots.push_back((-b - 2. * _CSE sqrt(A) * _CSE cos(tet / 3.)) / (3. * a));
		Roots.push_back((-b + _CSE sqrt(A) * (_CSE cos(tet / 3) + _CSE sqrt(3) * _CSE sin(tet / 3))) / (3. * a));
		Roots.push_back((-b + _CSE sqrt(A) * (_CSE cos(tet / 3) - _CSE sqrt(3) * _CSE sin(tet / 3))) / (3. * a));

		return 4;
	}

	return 0;
}

int SolveQuartic(InputArray Coeffs, OutputArray Roots, int64 p_Error)
{
	if (Coeffs.size() != 5) { throw MathException("Number of Coefficients is not match."); }
	Roots.clear();
	float64 a = Coeffs[0];
	float64 b = Coeffs[1];
	float64 c = Coeffs[2];
	float64 d = Coeffs[3];
	float64 e = Coeffs[4];

	if (a == 0) { throw MathException("Highest power of polynomial can't be zero."); }

	float64 D = 3.0 * _CSE pow(b, 2.0) - 8.0 * a * c;
	float64 E = -_CSE pow(b, 3.0) + 4 * a * b * c - 8 * _CSE pow(a, 2.0) * d;
	float64 F = 3.0 * _CSE pow(b, 4.0) + 16.0 * _CSE pow(a, 2.0) * _CSE pow(c, 2.0) - 16.0 * a * _CSE pow(b, 2.0) * c + 16.0 * _CSE pow(a, 2.0) * b * d - 64.0 * _CSE pow(a, 3.0) * e;
	float64 A = _CSE pow(D, 2.0) - 3.0 * F;
	float64 B = D * F - 9.0 * _CSE pow(E, 2.0);
	float64 C = _CSE pow(F, 2.0) - 3.0 * D * _CSE pow(E, 2.0);

	float64 del = _CSE pow(B, 2) - 4.0 * A * C;

	if (IsZero(D, p_Error) && IsZero(E, p_Error) && IsZero(F, p_Error))
	{
		Roots.push_back(-b / (4.0 * a));
		Roots.push_back(Roots[0]); // -(2 * c) / (3 * b)
		Roots.push_back(Roots[0]); // -(3 * d) / (2 * c)
		Roots.push_back(Roots[0]); // -(4 * e) / d

		return 1;
	}

	if (!IsZero(D * E * F, p_Error) && IsZero(A, p_Error) && IsZero(B, p_Error) && IsZero(C, p_Error))
	{
		Roots.push_back((-b * D + 9. * E) / (4. * a * D));
		Roots.push_back((-b * D - 3. * E) / (4. * a * D));
		Roots.push_back(Roots[1]);
		Roots.push_back(Roots[1]);

		return 2;
	}

	if (IsZero(E, p_Error) && IsZero(F, p_Error) && !IsZero(D, p_Error))
	{
		Roots.push_back((-b + _CSE sqrt(complex64(D))[0]) / (4. * a));
		Roots.push_back(Roots[0]);
		Roots.push_back((-b - _CSE sqrt(complex64(D))[0]) / (4. * a));
		Roots.push_back(Roots[2]);

		return 3;
	}

	if (!IsZero(A * B * C, p_Error) && IsZero(del, p_Error))
	{
		Roots.push_back((-b + (2. * A * E) / B + _CSE sqrt(complex64((2. * B) / A))[0]) / (4. * a));
		Roots.push_back((-b + (2. * A * E) / B - _CSE sqrt(complex64((2. * B) / A))[0]) / (4. * a));
		Roots.push_back((-b - (2. * A * E) / B) / (4. * a));
		Roots.push_back(Roots[2]);

		return 4;
	}

	if (del > 0)
	{
		float64 z1 = A * D + 3. * ((-B + _CSE sqrt(del)) / 2.);
		float64 z2 = A * D + 3. * ((-B - _CSE sqrt(del)) / 2.);
		complex64 z = _CSE pow(D, 2.) - D * (_CSE cbrt(z1) + _CSE cbrt(z2)) + pow((_CSE cbrt(z1) + _CSE cbrt(z2)), 2.) - 3. * A;

		Roots.push_back((-b + sgn(E) * _CSE sqrt((D + _CSE cbrt(z1) + _CSE cbrt(z2)) / 3.) + _CSE sqrt((2. * D - (_CSE cbrt(z1) + _CSE cbrt(z2)) + 2. * _CSE sqrt(z)[0]) / 3.)[0]) / (4. * a));
		Roots.push_back((-b + sgn(E) * _CSE sqrt((D + _CSE cbrt(z1) + _CSE cbrt(z2)) / 3.) - _CSE sqrt((2. * D - (_CSE cbrt(z1) + _CSE cbrt(z2)) + 2. * _CSE sqrt(z)[0]) / 3.)[0]) / (4. * a));
		Roots.push_back((-b - sgn(E) * _CSE sqrt((D + _CSE cbrt(z1) + _CSE cbrt(z2)) / 3.)) / (4. * a) + (_CSE sqrt((-2. * D + _CSE cbrt(z1) + _CSE cbrt(z2) + 2. * _CSE sqrt(z)[0]) / 3.)[0] / (4. * a)) * 1i);
		Roots.push_back((-b - sgn(E) * _CSE sqrt((D + _CSE cbrt(z1) + _CSE cbrt(z2)) / 3.)) / (4. * a) - (_CSE sqrt((-2. * D + _CSE cbrt(z1) + _CSE cbrt(z2) + 2. * _CSE sqrt(z)[0]) / 3.)[0] / (4. * a)) * 1i);

		return 5;
	}

	if (del < 0)
	{
		float64 tet = arccos((3. * B - 2. * A * D) / (2. * A * _CSE sqrt(A)));
		float64 y1 = (D - 2. * _CSE sqrt(A) * _CSE cos(tet / 3.)) / 3.;
		float64 y2 = (D + _CSE sqrt(A) * (_CSE cos(tet / 3.) + _CSE sqrt(3.) * _CSE sin(tet / 3.))) / 3.;
		float64 y3 = (D + _CSE sqrt(A) * (_CSE cos(tet / 3.) - _CSE sqrt(3.) * _CSE sin(tet / 3.))) / 3.;

		if (IsZero(E, p_Error) && D > 0 && F > 0)
		{
			Roots.push_back((-b + _CSE sqrt(D + 2. * _CSE sqrt(F))) / (4. * a));
			Roots.push_back((-b - _CSE sqrt(D + 2. * _CSE sqrt(F))) / (4. * a));
			Roots.push_back((-b + _CSE sqrt(D - 2. * _CSE sqrt(F))) / (4. * a));
			Roots.push_back((-b - _CSE sqrt(D - 2. * _CSE sqrt(F))) / (4. * a));

			return 6;
		}

		if (IsZero(E, p_Error) && D < 0 && F > 0)
		{
			Roots.push_back(-b / (4. * a) + (_CSE sqrt(-D + 2. * _CSE sqrt(F)) / (4. * a)) * 1i);
			Roots.push_back(-b / (4. * a) - (_CSE sqrt(-D + 2. * _CSE sqrt(F)) / (4. * a)) * 1i);
			Roots.push_back(-b / (4. * a) + (_CSE sqrt(-D - 2. * _CSE sqrt(F)) / (4. * a)) * 1i);
			Roots.push_back(-b / (4. * a) - (_CSE sqrt(-D - 2. * _CSE sqrt(F)) / (4. * a)) * 1i);

			return 7;
		}

		if (IsZero(E, p_Error) && F < 0)
		{
			Roots.push_back((-2. * b + _CSE sqrt(2. * D + 2. * _CSE sqrt(A - F))) / (8. * a) + ((_CSE sqrt(-2. * D + 2. * _CSE sqrt(A - F))) / (8. * a)) * 1i);
			Roots.push_back((-2. * b + _CSE sqrt(2. * D + 2. * _CSE sqrt(A - F))) / (8. * a) - ((_CSE sqrt(-2. * D + 2. * _CSE sqrt(A - F))) / (8. * a)) * 1i);
			Roots.push_back((-2. * b - _CSE sqrt(2. * D + 2. * _CSE sqrt(A - F))) / (8. * a) + ((_CSE sqrt(-2. * D + 2. * _CSE sqrt(A - F))) / (8. * a)) * 1i);
			Roots.push_back((-2. * b - _CSE sqrt(2. * D + 2. * _CSE sqrt(A - F))) / (8. * a) - ((_CSE sqrt(-2. * D + 2. * _CSE sqrt(A - F))) / (8. * a)) * 1i);

			return 8;
		}

		if
		(
			!IsZero(E, p_Error)
			//&& max(y1, y2, y3) == y2
		)
		{
			if (D > 0 && F > 0)
			{
				Roots.push_back((-b + sgn(E) * _CSE sqrt(y1) + (_CSE sqrt(y2) + _CSE sqrt(y3))) / (4. * a));
				Roots.push_back((-b + sgn(E) * _CSE sqrt(y1) - (_CSE sqrt(y2) + _CSE sqrt(y3))) / (4. * a));
				Roots.push_back((-b - sgn(E) * _CSE sqrt(y1) + (_CSE sqrt(y2) - _CSE sqrt(y3))) / (4. * a));
				Roots.push_back((-b - sgn(E) * _CSE sqrt(y1) - (_CSE sqrt(y2) - _CSE sqrt(y3))) / (4. * a));

				return 9;
			}

			else
			{
				Roots.push_back((-b - _CSE sqrt(y2)) / (4. * a) + ((sgn(E) * _CSE sqrt(-y1) + _CSE sqrt(-y3)) / (4. * a)) * 1i);
				Roots.push_back((-b - _CSE sqrt(y2)) / (4. * a) - ((sgn(E) * _CSE sqrt(-y1) + _CSE sqrt(-y3)) / (4. * a)) * 1i);
				Roots.push_back((-b + _CSE sqrt(y2)) / (4. * a) + ((sgn(E) * _CSE sqrt(-y1) - _CSE sqrt(-y3)) / (4. * a)) * 1i);
				Roots.push_back((-b + _CSE sqrt(y2)) / (4. * a) - ((sgn(E) * _CSE sqrt(-y1) - _CSE sqrt(-y3)) / (4. * a)) * 1i);

				return 10;
			}
		}
	}

	return 0;
}

uint64 SolvePoly(InputArray Coeffs, OutputArray Roots, _SOLVEPOLY_CONFIG Conf)
{
	std::vector<float64> _Coeffs = Coeffs;
	Roots.clear();

	if (_Coeffs[0] == 0 || _Coeffs.empty())
	{
		throw MathException("Highest power of polynomial can't be zero.");
	}
	float64 Base = _Coeffs[0];
	for (size_t i = 0; i < _Coeffs.size(); i++)
	{
		_Coeffs[i] /= Base;
	}

	// The clue to the method now is to combine the fixed-point iteration for P with similar iterations for Q, R, S into a simultaneous iteration for all roots.
	// Initialize p, q, r, s:
	for (size_t i = 0; i < _Coeffs.size() - 1; i++)
	{
		Roots.push_back(_CSE pow(Conf.BASE, complex64((float64)i)));
	}
	
	// Make the substitutions
	auto f = [&](complex64 x)->complex64
	{
		complex64 SIG = 0;
		for (size_t i = 0; i < _Coeffs.size(); i++)
		{
			SIG += _Coeffs[i] * _CSE pow(x, (complex64)(_Coeffs.size() - i - 1.));
		}
		return SIG;
	};

	auto g = [&](complex64 x, uint64 Ignore)->complex64
	{
		complex64 PI = 1;
		for (size_t i = 0; i < Roots.size(); i++)
		{
			if (i != Ignore) { PI *= x - Roots[i]; }
		}
		return PI;
	};

	uint64 it = 0;

	while (it < _CSE pow(10, Conf.MAXITER_LOG))
	{
		std::vector<float64> Diffs;
		for (size_t i = 0; i < _Coeffs.size() - 1; i++)
		{
			complex64 Diff = f(Roots[i]) / g(Roots[i], i);
			Diffs.push_back(abs(Diff));
			Roots[i] -= Diff;
		}

		// Re-iterate until the numbers p, q, r, s essentially stop changing relative to the desired precision. 
		// They then have the values P, Q, R, S in some order and in the chosen precision. So the problem is solved.
		auto MaxDiff = _CSE max(Diffs.begin(), Diffs.end());
		if (-pow(10, (float64)-Conf.P_ERROR) < *MaxDiff && *MaxDiff < pow(10, (float64)-Conf.P_ERROR))
		{
			break;
		}

		++it;
	}

	return it;
}

_CSE_END