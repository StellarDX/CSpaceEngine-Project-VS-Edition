#include "../../headers/random/planets.h"

_CSE_BEGIN

void 
__CRTDECL MakeOrbit
(Object& _Primary, Object& _Companion, 
	std::string _RefSystem, float64 _Epoch, float64 _Separation, float64 _PositionAngle)
{
}

_Check_return_ Object 
__CRTDECL MakeBinary
(Object& _Primary, Object& _Companion,
	std::string _RefSystem, float64 _Epoch, float64 _Separation, float64 _PositionAngle)
{
	return Object();
}

_CSE_END