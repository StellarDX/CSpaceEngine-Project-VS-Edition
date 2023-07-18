#include "CSE/Planets/Planet_Base.hpp"

_CSE_BEGIN

#if 0
typename UBox_PlanetBase_Composition::interior UBox_PlanetBase_Composition::Interior() const
{
	interior I;
	I.insert(std::pair<_STD string, float64>("Metals", _Par.Metals / Mass() * 100));
	I.insert(std::pair<_STD string, float64>("Silicates", _Par.Silicates / Mass() * 100));
	I.insert(std::pair<_STD string, float64>("Ices", _Par.Ices / Mass() * 100));
	I.insert(std::pair<_STD string, float64>("Hydrogen", _Par.Hydrogen / Mass() * 100));
	return I;
}

_CONSTEXPR20 float64 UBox_PlanetBase_Composition::Mass()const
{
	return _Par.Metals + _Par.Silicates + _Par.Ices + _Par.Hydrogen;
}

_CONSTEXPR20 float64 UBox_PlanetBase_Composition::Radius()const
{
	
}

_CONSTEXPR20 float64 UBox_PlanetBase_Composition::Density()const
{
	return Mass() / Volume();
}

_CONSTEXPR20 float64 UBox_PlanetBase_Composition::Volume() const
{
	return (4. / 3.) * CSE_PI * pow(Radius(), 3);
}

_CONSTEXPR20 float64 UBox_PlanetBase_Composition::RadialMassGain() const
{
	
}

_CONSTEXPR20 float64 UBox_PlanetBase_Composition::SurfaceGravity() const
{
	return GravConstant * Mass() / pow(Radius(), 2);
}

_CONSTEXPR20 float64 UBox_PlanetBase_Composition::EscapeVelocity() const
{
	return sqrt(2. * SurfaceGravity() * Radius());
}
#endif

// ---------- Functions ---------- //

Object RandomRockyPlanet()
{
	float64 MassScale = random.uniform(6.4 * MassEarth, 7.6 * MassEarth);
	float64 RadScale = random.uniform(2.8 * RadEarth, 4.4 * RadEarth);
	float64 BaseRadius = random.uniform(2500000, 2 * RadEarth); // min is 0.15*REarth
	TerrestrialPlanetBase Model(BaseRadius, BaseRadius, MassScale, RadScale);
	return Model(random);
}

Object RandomGasGiant()
{
	float64 BaseMass = random.uniform(15 * MassEarth, 12 * MassJupiter);
	GaseousPlanetBase Model(BaseMass);
	return Model(random);
}

Object RandomMoon()
{
	float64 MassScale = random.uniform(6.4, 7.6);
	float64 RadScale = random.uniform(2.8, 4.4);
	float64 BaseRadius = random.uniform(1000000, 2500000);
	TerrestrialPlanetBase Model(BaseRadius, BaseRadius, MassScale, RadScale);
	return Model(random);
}

Object RandomSmallMoon()
{
	float64 MassScale = random.uniform(6.4, 7.6);
	float64 RadScale = random.uniform(2.8, 4.4);
	float64 BaseRadius = random.uniform(200000, 1000000);
	TerrestrialPlanetBase Model(BaseRadius, BaseRadius, MassScale, RadScale);
	return Model(random);
}

Object RandomAsteroid()
{
	float64 MassScale = random.uniform(6.4, 7.6);
	float64 RadScale = random.uniform(2.8, 4.4);
	float64 BaseRadius = random.uniform(20000, 200000);
	TerrestrialPlanetBase Model(BaseRadius, BaseRadius, MassScale, RadScale);
	return Model(random);
}

_CSE_END
