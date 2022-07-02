// Luminosity and Magnitudes

#define _USE_CSE_DEFINES

#include "../headers/lumine/Lumine.h"

// Basic luminosity functions

_CSE_BEGIN

CSELog LumineLog;

float64 _Lum_Stefan_Boltzmann_Law(float64 _Radius, float64 _Eff_Temp)
{
	return 4. * CSE_PI * pow(_Radius, 2) * StBConstant * pow(_Eff_Temp, 4);
}

float64 GetLuminosity(float64 Radius, float64 Teff)
{
	return _Lum_Stefan_Boltzmann_Law(Radius, Teff);
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
	Src->Luminosity = _Lum_Stefan_Boltzmann_Law(Radius, Src->Teff);
}

_CSE_END