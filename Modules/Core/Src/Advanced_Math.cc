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

_EXTERN_C

_Check_return_ float64 _cdecl integral(_In_ _STD function<float64(float64)> f, _In_ float64 a, _In_ float64 b, float64 Logn)
{
	float64 MaxSamples = pow(10, Logn);
	float64 Step = (b - a) / MaxSamples;
	float64 Area = 0;
	for (size_t i = 0; i < MaxSamples; i++)
	{
		Area += (f(a + i * Step) + f(a + (i + 1) * Step)) * Step / 2.;
	}
	return Area;
}

_END_EXTERN_C

_CSE_END