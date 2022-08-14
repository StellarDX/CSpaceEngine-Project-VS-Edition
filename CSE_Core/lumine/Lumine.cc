// Luminosity and Magnitudes

#define _USE_CSE_DEFINES

#include "../headers/lumine/Lumine.h"

_CSE_BEGIN

CSELog LumineLog;

// Basic luminosity functions

float64 GetLuminosity(float64 Radius, float64 Teff)
{
	return 4. * CSE_PI * pow(Radius, 2) * StBConstant * pow(Teff, 4);
}

void GetLuminosity(Object* Src)
{
	if (Src->Type != "Star")
	{
		LumineLog.Out("Lumine", "ERROR", "Can't compute luminosity because the object is not a star.", SysLogLevel);
		return;
	}

	if (any(Equal(Src->Dimensions, vec3(NO_DATA_FLOAT_INF))) || isinf(Src->Teff))
	{
		LumineLog.Out("Lumine", "ERROR", "Can't compute luminosity because radius or temperature is unknown.", SysLogLevel);
		return;
	}

	float64 Radius = cbrt((Src->Dimensions.x / 2.) * (Src->Dimensions.y / 2.) * (Src->Dimensions.z / 2.));
	Src->Luminosity = GetLuminosity(Radius, Src->Teff);
}

float64 GetStarAbsMagn(float64 _AppMagn, float64 _Dist, float64 _Extinction)
{
	return _AppMagn - 5. * log(_Dist * Parsec - 1.) - _Extinction;
}

_CSE_END