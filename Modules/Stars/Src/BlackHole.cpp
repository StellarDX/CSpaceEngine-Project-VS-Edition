#include "CSE/Stars/BlackHole.hpp"

_CSE_BEGIN

float64 SchwarzschildRadius(float64 Mass)
{
	return (2. * GravConstant * Mass) / pow(SpeedOfLight, 2);
}

float64 SchwarzschildDensity(float64 Mass)
{
	return (3. * pow(SpeedOfLight, 6)) / 
		(32. * CSE_PI * pow(GravConstant, 3) * pow(Mass, 2));
}

float64 BHIsValid1(BLACKHOLE BH)
{
	return pow(BH._M_q, 2) / (4. * CSE_PI * ElectricConst) + 
		(pow(SpeedOfLight, 2) * pow(BH._M_j, 2)) / (GravConstant * pow(BH._M_m, 2)) - 
		GravConstant * BH._M_m;
}

float64 BHIsValid2(BLACKHOLE BH)
{
	return BH._M_j - (GravConstant * pow(BH._M_m, 2)) / SpeedOfLight;
}

_CSE_END