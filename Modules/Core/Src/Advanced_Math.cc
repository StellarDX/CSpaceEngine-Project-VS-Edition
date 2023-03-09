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

_CSE_END