#include <iostream>
#include <fstream>
#include <format>
#include "CSE/Core.h"

using namespace std;
using namespace cse;

CSELog CoreLog;

float64 func(float64 x) {return cse::pow(x, 2);}

int main()
{
	ofstream Report("TestReport_CVMathFuncs.txt");
	#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	cout.precision(15);
	Report.precision(15);
	cout << "CSpaceEngine is now using STD math, so it doesn't need to test math functions.\n";
	#else
	Report << "Testing: EXP" << '\n';
	float64 i = -25;
	while (i < 25)
	{
   		float64 e1 = cse::exp(i);
   		float64 e2 = std::exp(i);
   		float64 er = (e1 - e2) / e2;
		cout << format("result: {} ----- exp(x) = {} ----- error: {:e}\n", e1, e2, er);
		Report << format("result: {} ----- exp(x) = {} ----- error: {:e}\n", e1, e2, er);
		i += 0.2;
	}

	Report << "Testing: LOG" << '\n';
	i = 0;
	while (i < 50)
	{
		float64 e1 = cse::ln(i);
		float64 e2 = std::log(i);
		float64 er = (e1 - e2) / e2;
		cout << format("result: {} ----- ln(x) = {} ----- error: {:e}\n", e1, e2, er);
		Report << format("result: {} ----- ln(x) = {} ----- error: {:e}\n", e1, e2, er);
		i += 0.2;
	}

	Report << "Testing: POWER" << '\n';
	i = -25;
	float64 j = 50;
	while (i < 25)
	{
		float64 e1 = cse::pow(i, j);
		float64 e2 = std::pow(i, j);
		float64 er = (e1 - e2) / e2;
		cout << format("result: {} ----- pow(x, y) = {} ----- error: {:e}\n", e1, e2, er);
		Report << format("result: {} ----- pow(x, y) = {} ----- error: {:e}\n", e1, e2, er);
		i += 0.2;
	}

	Report << "Testing: SINCOS" << '\n';
	i = -180;
	while (i < 180)
	{
		float64 e1 = cse::sin(i);
		float64 e2 = std::sin(radians(i));
		float64 er = (e1 - e2) / e2;
		cout << format("result: {} ----- sin(x) = {} ----- error: {:e}\n", e1, e2, er);
		Report << format("result: {} ----- sin(x) = {} ----- error: {:e}\n", e1, e2, er);
		i += 1;
	}
	i = -180;
	while (i < 180)
	{
		float64 e1 = cse::cos(i);
		float64 e2 = std::cos(radians(i));
		float64 er = (e1 - e2) / e2;
		cout << format("result: {} ----- cos(x) = {} ----- error: {:e}\n", e1, e2, er);
		Report << format("result: {} ----- cos(x) = {} ----- error: {:e}\n", e1, e2, er);
		i += 1;
	}
	#endif

	Report << "Testing: SOLVEPOLY" << '\n';
	InputArray Coeffs1({ 1, -2, -8 });
	InputArray Coeffs2({ -(3./2.), -4, 1});
	InputArray Coeffs3({ 3, -2*cse::sqrt(3), 1});
	vector<complex64> Roots;
	SolveQuadratic(Coeffs1, Roots);
	cout << "Quadratic Result 1: " << Roots[0] << ' ' << Roots[1] << '\n';
	Report << "Quadratic Result 1: " << Roots[0] << ' ' << Roots[1] << '\n';
	SolveQuadratic(Coeffs2, Roots);
	cout << "Quadratic Result 2: " << Roots[0] << ' ' << Roots[1] << '\n';
	Report << "Quadratic Result 2: " << Roots[0] << ' ' << Roots[1] << '\n';
	SolveQuadratic(Coeffs3, Roots);
	cout << "Quadratic Result 3: " << Roots[0] << ' ' << Roots[1] << '\n';
	Report << "Quadratic Result 3: " << Roots[0] << ' ' << Roots[1] << '\n';

	InputArray Coeffs4({ 1, +5.4, +9.72, +5.832 });
	InputArray Coeffs5({ 2, +11, +182, +255 });
	InputArray Coeffs6({ 1, +5.5, +9.92, +5.888 });
	InputArray Coeffs7({ 100, -420, +467, -105 });
	InputArray Coeffs8({ 1, -67.4, +1417.92, -9539.712 });
	SolveCubic(Coeffs4, Roots);
	cout << "Cubic Result 1: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << '\n';
	Report << "Cubic Result 1: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << '\n';
	SolveCubic(Coeffs5, Roots);
	cout << "Cubic Result 2: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << '\n';
	Report << "Cubic Result 2: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << '\n';
	SolveCubic(Coeffs6, Roots);
	cout << "Cubic Result 3: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << '\n';
	Report << "Cubic Result 3: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << '\n';
	SolveCubic(Coeffs7, Roots);
	cout << "Cubic Result 4: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << '\n';
	Report << "Cubic Result 4: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << '\n';
	SolveCubic(Coeffs8, Roots);
	cout << "Cubic Result 5: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << '\n';
	Report << "Cubic Result 5: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << '\n';

	InputArray Coeffs9({ 1, +1, -12, -14, +16 });
	InputArray Coeffs10({ 1, -8, +16, -32, +48 });
	InputArray Coeffs11({ 1, 0, 0, -4, -1 });
	InputArray Coeffs12({ 1, -4, -24, +56, +52 });
	InputArray Coeffs13({ 1./4., -7./3., +2, +8, 0 });
	InputArray Coeffs14({ 1, -0.78, +1.86, -0.58, +0.72 });
	InputArray Coeffs15({ 1, +2, -1, -2, -11880 });
	SolveQuartic(Coeffs9, Roots);
	cout << "Quartic Result 1: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	Report << "Quartic Result 1: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	SolveQuartic(Coeffs10, Roots);
	cout << "Quartic Result 2: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	Report << "Quartic Result 2: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	SolveQuartic(Coeffs11, Roots);
	cout << "Quartic Result 3: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	Report << "Quartic Result 3: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	SolveQuartic(Coeffs12, Roots);
	cout << "Quartic Result 4: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	Report << "Quartic Result 4: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	SolveQuartic(Coeffs13, Roots);
	cout << "Quartic Result 5: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	Report << "Quartic Result 5: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	SolveQuartic(Coeffs14, Roots);
	cout << "Quartic Result 6: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	Report << "Quartic Result 6: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	SolveQuartic(Coeffs15, Roots);
	cout << "Quartic Result 7: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';
	Report << "Quartic Result 7: " << Roots[0] << ' ' << Roots[1] << ' ' << Roots[2] << ' ' << Roots[3] << '\n';

	InputArray Quintic({ 1, -979, -25203 , +1135447 , -1054954 , -2214624 });
	InputArray Sextic({ 6, +3, +14, -12, -5, +6, -7 });
	InputArray Octic({ 1,-3,+5,0,-2,0,+5,-3,1 });
	InputArray CoeffsEx1({ 1, +(12), +(66), +(220), +(495), +(792), +(924), +(792), +(495), +(220), +(66), +(12), 1./10. });
	InputArray CoeffsEx2({ 5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-6,+8,0,0,0,-5,0,0 });
	SolvePoly(Quintic, Roots);
	cout << "Quintic Result: ";
	Report << "Quintic Result: ";
	for (size_t i = 0; i < Roots.size(); i++)
	{
		cout << Roots[i] << ' ';
		Report << Roots[i] << ' ';
	}
	cout << '\n';
	Report << '\n';

	SolvePoly(Sextic, Roots);
	cout << "Sextic Result: ";
	Report << "Sextic Result: ";
	for (size_t i = 0; i < Roots.size(); i++)
	{
		cout << Roots[i] << ' ';
		Report << Roots[i] << ' ';
	}
	cout << '\n';
	Report << '\n';

	SolvePoly(Octic, Roots);
	cout << "Octic Result: ";
	Report << "Octic Result: ";
	for (size_t i = 0; i < Roots.size(); i++)
	{
		cout << Roots[i] << ' ';
		Report << Roots[i] << ' ';
	}
	cout << '\n';
	Report << '\n';

	SolvePoly(CoeffsEx1, Roots);
	cout << "Extreme Result 1: ";
	Report << "Extreme Result 1: ";
	for (size_t i = 0; i < Roots.size(); i++)
	{
		cout << Roots[i] << ' ';
		Report << Roots[i] << ' ';
	}
	cout << '\n';
	Report << '\n';

	SolvePoly(CoeffsEx2, Roots);
	cout << "Extreme Result 2: ";
	Report << "Extreme Result 2: ";
	for (size_t i = 0; i < Roots.size(); i++)
	{
		cout << Roots[i] << ' ';
		Report << Roots[i] << ' ';
	}
	cout << '\n';
	Report << '\n';

	float64 Integ = cse::integral(func, 0, 1, 5);
	cout << Integ << '\n';
	Report << Integ << '\n';
}
