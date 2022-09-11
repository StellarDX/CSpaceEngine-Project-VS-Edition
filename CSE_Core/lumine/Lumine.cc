// Luminosity and Magnitudes

#define _USE_CSE_DEFINES

#include "../headers/lumine/Lumine.h"

_CSE_BEGIN

CSELog LumineLog;

// Basic luminosity functions

_EXTERN_C

// Functions from earlier version of Star Luminosity and Magnitude calculator

/*AppMagn*/

float64 ToAppMagn1(float64 AbsMagn, float64 Dist_Pc, float64 Extinction)
{
	return 5 * (log(Dist_Pc) - 1) + AbsMagn + Extinction;
}

/*AbsMagn*/

float64 ToAbsMagn1(float64 AppMagn, float64 Dist_Pc, float64 Extinction)
{
	return AppMagn - 5 * (log(Dist_Pc) - 1) - Extinction;
}

float64 ToAbsMagn2(float64 Parallax, float64 AppMagn, float64 Extinction)
{
	return AppMagn + 5 * (log(Parallax) + 1) - Extinction;
}

float64 ToAbsMagn4(float64 LumBol)
{
	return -2.5 * log(LumBol / SolarLumBol);
}

/*Luminosity*/

float64 ToLuminosity1(float64 Radius, float64 Teff)
{
	float64 SurfArea = 4 * CSE_PI * pow(Radius, 2);
	return StBConstant * SurfArea * pow(Teff, 4);
}

float64 ToLuminosity3(float64 AbsMagnBol)
{
	return SolarLumBol * pow(10, (0 - 0.4 * AbsMagnBol));
}

_END_EXTERN_C

_CSE_END