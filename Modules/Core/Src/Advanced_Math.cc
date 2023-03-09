#include "CSE/Core/CSEM_Adv.h"
#include "CSE/Core/CSEM_Func.h"

_CSE_BEGIN

_Check_return_ vec2 _cdecl XYToPolar(_In_ vec2 XY)
{
	return vec2(sqrt(XY.x * XY.x + XY.y * XY.y), arctan(XY.y / XY.x));
}

_Check_return_ vec3 _cdecl XYZToPolar(_In_ vec3 XYZ)
{
	return vec3
	(
		arctan(XYZ.x / XYZ.z),
		arcsin(XYZ.y / sqrt(XYZ.x * XYZ.x + XYZ.y * XYZ.y + XYZ.z * XYZ.z)),
		sqrt(XYZ.x * XYZ.x + XYZ.y * XYZ.y + XYZ.z * XYZ.z)
	);
}

_Check_return_ vec2 _cdecl PolarToXY(_In_ vec2 Polar)
{
	return vec2(Polar.x * cse::cos(Polar.y), Polar.x * cse::sin(Polar.y));
}

_Check_return_ vec3 _cdecl PolarToXYZ(_In_ vec3 Polar)
{
	return vec3
	(
		-Polar.z * cse::cos(Polar.y) * cse::sin(Polar.x),
		+Polar.z * cse::sin(Polar.y),
		-Polar.z * cse::cos(Polar.y) * cse::cos(Polar.x)
	);
}

// ------------------------------ INTEGRAL ----------------------------- //

matrix<float64, 5, 5> RombergAnalysis(_STD function<float64(float64)> _Func, float64 _Min, float64 _Max)
{
	enum {hm, T, S, C, R};
	matrix<float64, 5, 5> Sequenses(0);
	for (size_t i = 0; i < 5; i++)
	{
		Sequenses[hm][i] = (_Max - _Min) / pow(2, (float64)i);
	}

	auto fa = _Func(_Min);
	auto fb = _Func(_Max);

	auto t0 = (1. / 2.) * (_Max - _Min) * (fa + fb);
	Sequenses[T][0] = t0;

	for (size_t i = 1; i < 5; i++)
	{
		float64 sum = 0;
		for (size_t each = 1; each < pow(2, (float64)i); each += 2)
		{
			sum += Sequenses[hm][i] * _Func(_Min + each * Sequenses[hm][i]);
		}
		auto temp1 = 1. / 2. * Sequenses[T][i - 1];
		auto temp2 = sum;
		auto temp = temp1 + temp2;
		Sequenses[T][i] = temp;
	}

	for (size_t i = 0; i < 4; i++)
	{
		Sequenses[S][i] = (4 * Sequenses[T][i + 1] - Sequenses[T][i]) / 3.;
	}

	for (size_t i = 0; i < 3; i++)
	{
		Sequenses[C][i] = (16 * Sequenses[S][i + 1] - Sequenses[S][i]) / 15.;
	}

	for (size_t i = 0; i < 2; i++)
	{
		Sequenses[R][i] = (64 * Sequenses[C][i + 1] - Sequenses[C][i]) / 63.;
	}

	return Sequenses; // Report
}

_CSE_END