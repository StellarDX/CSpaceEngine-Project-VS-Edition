#pragma once

#ifndef __SEOBJECT__
#define __SEOBJECT__

#include <array>
#include <vector>
#include <map>

#include "CSECore.h"
#include "..\gl\gltypes.h"
#include "..\stream\iscstream.h"

// Default rotation model:
// 0 = Simple
// 1 = IAU
// 2 = Moon-DE(Will disable all the rotation options and force using Moon's ephemerides)
#define OBJ_DEFAULT_ROTATION_MODEL 1

// Default Static/fixed position format:
// 0 = XYZ
// 1 = Polar
#define OBJ_DEFAULT_STATIC_FIXED_POS 1

_CSE_BEGIN

/*------------------------------------------------------------------------*\
|                                 Rotation                                 |
\*------------------------------------------------------------------------*/

class RotationSimple // Simply uniform rotation parameters
{
public:
	float64 RotationPeriod = NO_DATA_FLOAT_INF; // Rotation Period in seconds
	float64 Obliquity = NO_DATA_FLOAT_INF; // Obliquity of the rotation axis
	float64 EqAscendNode = NO_DATA_FLOAT_INF; // Ascending Node of the equator
	float64 RotationOffset = NO_DATA_FLOAT_INF; // Rotation Offset in degrees (i.e. orientation adjustment)
	float64 RotationEpoch = NO_DATA_FLOAT_INF; // Rotation Epoch in Julian days.
	float64 Precession = NO_DATA_FLOAT_INF; // Period of the axial precession in years.
};

class RotationIAU
{
public:
	float64 Epoch = NO_DATA_FLOAT_INF; // J2000
	float64 PoleRA = NO_DATA_FLOAT_INF; // degrees
	float64 PoleRARate = NO_DATA_FLOAT_INF; // degrees/century
	float64 PoleDec = NO_DATA_FLOAT_INF; // degrees
	float64 PoleDecRate = NO_DATA_FLOAT_INF; // degrees/century
	float64 PrimeMeridian = NO_DATA_FLOAT_INF; // degrees
	float64 RotationRate = NO_DATA_FLOAT_INF; // degrees/day
	float64 RotationAccel = NO_DATA_FLOAT_INF; // degrees/century^2
	std::string PT = NO_DATA_STRING;
	std::vector<std::array<float64, 6>> PeriodicTerms; // PoleRA amp, PoleDec amp, PrimeMer amp, phase, freq, freq_rate
	// FinalRA = PoleRA + PoleRARate * Time + SUM(PoleRAAmp[i] * sin(Phase[i] + Freq[i] * Time + FreqRate * pow(Time, 2)))
	// FinalDec = PoleDec + PoleDecRate * Time + SUM(PoleDecAmp[i] * cos(Phase[i] + Freq[i] * Time + FreqRate * pow(Time, 2)))
	// FinalRotation = PrimeMeridian + RotationRate * Time + RotationAccel * pow(Time, 2) + SUM(PrimeMerAmp[i] * sin(Phase[i] + Freq[i] * Time + FreqRate * pow(Time, 2)))
};

/*-----------------------------------------------------------------------*\
|                                  Orbit                                  |
\*-----------------------------------------------------------------------*/

class OrbitParam
{
public:
	// Binary
	bool Binary = false;
	float64 Separation = NO_DATA_FLOAT_INF;
	float64 PositionAngle = NO_DATA_FLOAT_INF;

	// Non-Binary
	std::string AnalyticModel = NO_DATA_STRING;
	std::string RefPlane = NO_DATA_STRING;
	float64 Epoch = NO_DATA_FLOAT_INF;
	float64 Period = NO_DATA_FLOAT_INF;
	float64 PericenterDist = NO_DATA_FLOAT_INF;
	float64 GravParam = NO_DATA_FLOAT_INF;
	float64 Eccentricity = NO_DATA_FLOAT_INF;
	float64 Inclination = NO_DATA_FLOAT_INF;
	float64 AscendingNode = NO_DATA_FLOAT_INF;
	float64 AscNodePreces = NO_DATA_FLOAT_INF;
	float64 ArgOfPericenter = NO_DATA_FLOAT_INF;
	float64 ArgOfPeriPreces = NO_DATA_FLOAT_INF;
	float64 MeanAnomaly = NO_DATA_FLOAT_INF;
};

class OrbitState
{
public:
	std::string RefPlane;
	float64 GravParam = NO_DATA_FLOAT_INF;
	float64 Time = NO_DATA_FLOAT_INF;
	vec3 Position = vec3(NO_DATA_FLOAT_INF);
	vec3 Velocity = vec3(NO_DATA_FLOAT_INF);
};

/*------------------------------------------------------------------------*\
|                                   Life                                   |
\*------------------------------------------------------------------------*/

struct LifeParam
{
	std::string Class = NO_DATA_STRING; // Organic, Exotic
	std::string Type = NO_DATA_STRING; // Unicellular, Multicellular
	std::vector<std::string> Biome; // Subglacial, Marine, Terrestrial, Aerial(Floaters)
	bool Panspermia = false;
};

/*-------------------------------------------------------------------------*\
|                                  Surface                                  |
\*-------------------------------------------------------------------------*/

struct Landscape // Many parameters were merged.
{
	// Procedural
	std::string Preset = NO_DATA_STRING;
	float64 SurfStyle = NO_DATA_FLOAT_INF;
	vec3    Randomize = vec3(NO_DATA_FLOAT_INF);      // Randomize
	vec4    colorParams = vec4(NO_DATA_FLOAT_INF);    // (colorDistMagn,    colorDistFreq,    latIceCaps,          latTropic)
	vec4    climateParams = vec4(NO_DATA_FLOAT_INF);  // (climatePole,      climateTropic,    climateEquator,      tropicWidth)
	vec4    plantsParams1 = vec4(NO_DATA_FLOAT_INF);  // (climateGrassMin,  climateGrassMax,  climateForestMin,    climateForestMax)
	vec4    plantsParams2 = vec4(NO_DATA_FLOAT_INF);  // (climateSteppeMin, climateSteppeMax, humidity,            plantsBiomeOffset)
	vec4    mainParams = vec4(NO_DATA_FLOAT_INF);     // (mainFreq,         terraceProb,      oceanType,           tidalLock)
	vec4    textureParams = vec4(NO_DATA_FLOAT_INF);  // (texScale,         #NO_DATA,         venusMagn,           venusFreq)
	vec4    mareParams = vec4(NO_DATA_FLOAT_INF);     // (seaLevel,         mareFreq,         sqrt(mareDensity),   icecapHeight)
	vec4    montesParams = vec4(NO_DATA_FLOAT_INF);   // (montesMagn,       montesFreq,       montesFraction,      montesSpiky)
	vec4    dunesParams = vec4(NO_DATA_FLOAT_INF);    // (dunesMagn,        dunesFreq,        dunesDensity,        drivenDarkening)
	vec4    hillsParams = vec4(NO_DATA_FLOAT_INF);    // (hillsMagn,        hillsFreq,        hillsDensity,        hills2Density)
	vec4    riversParams = vec4(NO_DATA_FLOAT_INF);   // (riversMagn,       riversFreq,       riversSin,           beachWidth)
	vec4    riftsParams = vec4(NO_DATA_FLOAT_INF);    // (riftsMagn,        riftsFreq,        riftsSin,            #NO_DATA)
	vec4    canyonsParams = vec4(NO_DATA_FLOAT_INF);  // (canyonsMagn,      canyonsFreq,      canyonsFraction,     erosion)
	vec4    cracksParams = vec4(NO_DATA_FLOAT_INF);   // (cracksMagn,       cracksFreq,       cracksOctaves,       craterRayedFactor)
	vec4    craterParams = vec4(NO_DATA_FLOAT_INF);   // (craterMagn,       craterFreq,       sqrt(craterDensity), craterOctaves)
	vec4    volcanoParams1 = vec4(NO_DATA_FLOAT_INF); // (volcanoMagn,      volcanoFreq,      volcanoDensity,      volcanoOctaves)
	vec4    volcanoParams2 = vec4(NO_DATA_FLOAT_INF); // (volcanoActivity,  volcanoFlows,     volcanoRadius,       volcanoTemp)
	vec4    lavaParams = vec4(NO_DATA_FLOAT_INF);     // (lavaCoverage,     snowLevel,        surfTemperature,     heightTempGrad)
	vec4    lavaDetails = vec4(NO_DATA_FLOAT_INF);    // (Tidal,            Sun,              Young,               #NO_DATA)
	vec4    cycloneParams = vec4(NO_DATA_FLOAT_INF);  // (cycloneMagn,      cycloneFreq,      cycloneDensity,      cycloneOctaves)
	vec4    cycloneParams2 = vec4(NO_DATA_FLOAT_INF); // (cycloneMagn2,     cycloneFreq2,     cycloneDensity2,     cycloneOctaves2)

	// Non-procedural
	std::string DiffMap = NO_DATA_STRING;
	std::string DiffMapAlpha = NO_DATA_STRING;

	std::string BumpMap = NO_DATA_STRING;
	float64 BumpHeight = NO_DATA_FLOAT_INF;
	float64 BumpOffset = NO_DATA_FLOAT_INF;

	std::string GlowMap = NO_DATA_STRING;
	std::string GlowMode = NO_DATA_STRING;
	vec3 GlowColor = vec3(NO_DATA_FLOAT_INF);
	float64 GlowBright = NO_DATA_FLOAT_INF;

	std::string SpecMap = NO_DATA_STRING;
	float64 RoughnessBias = NO_DATA_FLOAT_INF;
	float64 RoughnessScale = NO_DATA_FLOAT_INF;
	vec4 SpecParams = vec4(NO_DATA_FLOAT_INF); // ice specular bright, water specular bright, ice specular power / roughness bias, water specular power / roughness scale
	float64 SpecularScale = NO_DATA_FLOAT_INF;

	float64 Hapke = NO_DATA_FLOAT_INF;
	vec4 HapkeParams = vec4(NO_DATA_FLOAT_INF); // Hapke spot bright, 1 / Hapke spot width, Hapke CB spot bright, 1 / Hapke CB spot width
	float64 DayAmbient = NO_DATA_FLOAT_INF;
	vec3 ModulateColor = vec3(NO_DATA_FLOAT_INF);

	float64 RingsWinter = NO_DATA_FLOAT_INF;
};

/*-------------------------------------------------------------------------*\
|                                   Ocean                                   |
\*-------------------------------------------------------------------------*/

struct OceanParam
{
	float64 Height = NO_DATA_FLOAT_INF;
	float64 Hapke = NO_DATA_FLOAT_INF;
	vec4 HapkeParams = vec4(NO_DATA_FLOAT_INF); // Hapke spot bright, 1 / Hapke spot width, Hapke CB spot bright, 1 / Hapke CB spot width
	float64 DayAmbient = NO_DATA_FLOAT_INF;
	float64 ModulateBright = NO_DATA_FLOAT_INF;
	std::map<std::string, float64> Composition;
};

/*--------------------------------------------------------------------------*\
|                                   Clouds                                   |
\*--------------------------------------------------------------------------*/

struct CloudsParam
{
	vec4 cloudsParams1 = vec4(NO_DATA_FLOAT_INF);  // (cloudsFreq,       cloudsOctaves,    stripeZones,         stripeTwist)
	vec4 cloudsParams2 = vec4(NO_DATA_FLOAT_INF);  // (cloudsLayer,      cloudsNLayers,    stripeFluct,         cloudsCoverage)
	struct LayerDetails
	{
		std::string DiffMap = NO_DATA_STRING;
		std::string BumpMap = NO_DATA_STRING;
		float64 Height = NO_DATA_FLOAT_INF;
		float64 Velocity = NO_DATA_FLOAT_INF;
		float64 BumpHeight = NO_DATA_FLOAT_INF;
		float64 BumpOffset = NO_DATA_FLOAT_INF;
		float64 Hapke = NO_DATA_FLOAT_INF;
		vec4 HapkeParams = vec4(NO_DATA_FLOAT_INF); // Hapke spot bright, 1 / Hapke spot width, Hapke CB spot bright, 1 / Hapke CB spot width
		float64 DayAmbient = NO_DATA_FLOAT_INF;
		vec4 ModulateColor = vec4(NO_DATA_FLOAT_INF); // Modulate color, opacity
		float64 ModulateBright = NO_DATA_FLOAT_INF;
	};
	std::vector<LayerDetails> Layer;
};

/*--------------------------------------------------------------------------*\
|                                 Atmosphere                                 |
\*--------------------------------------------------------------------------*/

struct AtmParam
{
	std::string Model = NO_DATA_STRING;
	float64 Height = NO_DATA_FLOAT_INF;
	float64 Density = NO_DATA_FLOAT_INF;
	float64 Adiabat = NO_DATA_FLOAT_INF;
	float64 Pressure = NO_DATA_FLOAT_INF;
	float64 Greenhouse = NO_DATA_FLOAT_INF;
	float64 Bright = NO_DATA_FLOAT_INF;
	float64 Opacity = NO_DATA_FLOAT_INF;
	float64 SkyLight = NO_DATA_FLOAT_INF;
	float64 Hue = NO_DATA_FLOAT_INF;
	float64 Saturation = NO_DATA_FLOAT_INF;
	std::map<std::string, float64> Composition;
};

/*--------------------------------------------------------------------------*\
|                                   Aurora                                   |
\*--------------------------------------------------------------------------*/

struct AurParam
{
	float64 Height = NO_DATA_FLOAT_INF;
	struct AurDetails
	{
		float64 Lat = NO_DATA_FLOAT_INF;
		float64 Lon = NO_DATA_FLOAT_INF;
		float64 Radius = NO_DATA_FLOAT_INF;
		float64 Width = NO_DATA_FLOAT_INF;
		float64 Rings = NO_DATA_FLOAT_INF;
		float64 Bright = NO_DATA_FLOAT_INF;
		float64 FlashFreq = NO_DATA_FLOAT_INF;
		float64 MoveSpeed = NO_DATA_FLOAT_INF;
		float64 Particles = NO_DATA_FLOAT_INF;
	};
	AurDetails North;
	AurDetails South;
	vec3 TopColor = vec3(NO_DATA_FLOAT_INF);
	vec3 BottomColor = vec3(NO_DATA_FLOAT_INF);
};

/*--------------------------------------------------------------------------*\
|                          Rings and Accretion Disk                          |
\*--------------------------------------------------------------------------*/

struct RingsParam
{
	std::string Texture = NO_DATA_STRING;
	vec4 Radiuses = vec4(NO_DATA_FLOAT_INF); // (Inner, Outer, Edge, Mean)
	float64 Thickness = NO_DATA_FLOAT_INF;
	vec3 RockParam = vec3(NO_DATA_FLOAT_INF); // (RocksMaxSize, RocksSpacing, DustDrawDist)
	float64 ChartRadius = NO_DATA_FLOAT_INF;
	float64 RotationPeriod = NO_DATA_FLOAT_INF;
	vec3 Brightness = vec3(NO_DATA_FLOAT_INF); // (Brightness, FrontBright, BackBright)
	vec4 ShadowParam = vec4(NO_DATA_FLOAT_INF); // (Density, Opacity, SelfShadow, PlanetShadow)
	float64 Hapke = NO_DATA_FLOAT_INF;
	vec4 HapkeParams = vec4(NO_DATA_FLOAT_INF); // Hapke spot bright, 1 / Hapke spot width, Hapke CB spot bright, 1 / Hapke CB spot width
	vec4 Shape = vec4(NO_DATA_FLOAT_INF); // (frequency, densityScale, densityOffset, densityPower)
	float64 colorContrast = NO_DATA_FLOAT_INF;
	vec3 FrontColor = vec3(NO_DATA_FLOAT_INF);
	vec3 BackThickColor = vec3(NO_DATA_FLOAT_INF);
	vec3 BackIceColor = vec3(NO_DATA_FLOAT_INF);
	vec3 BackDustColor = vec3(NO_DATA_FLOAT_INF);
};

struct AccDiskParam
{
	vec4 Radiuses = vec4(NO_DATA_FLOAT_INF);       // (disk inner radius, disk outer radius, black hole Rg, grav lens scale)
	float64 JetRadius = NO_DATA_FLOAT_INF;
	float64 JetLength = NO_DATA_FLOAT_INF;
	vec4 DiskParams1 = vec4(NO_DATA_FLOAT_INF);     // (disk max temperature, disk twist, star animation time, disk animation time)
	vec4 DiskParams2 = vec4(NO_DATA_FLOAT_INF);     // (temperature shift, disk brightness, disk opacity, star rotation angle)
	vec4 DiskParams3 = vec4(NO_DATA_FLOAT_INF);     // (disk inner thickness inner, disk outer thickness, disk radial detail scale, disk vertical detail scale)
	float64 ThicknessPow = NO_DATA_FLOAT_INF;
	float64 AccretionRate = NO_DATA_FLOAT_INF;
	float64 Density = NO_DATA_FLOAT_INF;
	float64 Luminosity = NO_DATA_FLOAT_INF;
};

/*-------------------------------------------------------------------------*\
|                                Star Corona                                |
\*-------------------------------------------------------------------------*/

struct CoronaParam
{
	//float64 Radius = NO_DATA_FLOAT_INF;
	vec4 Shape = vec4(NO_DATA_FLOAT_INF); // (RayDensity, RayCurvature, Scale, SinFreq)
	float64 Period = NO_DATA_FLOAT_INF;
	float64 Bright = NO_DATA_FLOAT_INF; // (Gamma, Brightness, RayBrightness, SpotBrightness)
};

/*--------------------------------------------------------------------------*\
|                                 Comet Tail                                 |
\*--------------------------------------------------------------------------*/

struct CometTailParam
{
	float64 MaxLength = NO_DATA_FLOAT_INF;
	float64 GasToDust = NO_DATA_FLOAT_INF;
	float64 Particles = NO_DATA_FLOAT_INF;
	float64 GasBright = NO_DATA_FLOAT_INF;
	float64 DustBright = NO_DATA_FLOAT_INF;
	vec3 GasColor = vec3(NO_DATA_FLOAT_INF);
	vec3 DustColor = vec3(NO_DATA_FLOAT_INF);
};

////////////////////////////////////////////////////////////////////////////////
//                          SpaceEngine Object Class                          //
////////////////////////////////////////////////////////////////////////////////
class Object
{
public:
	// Basic Info
	std::string Type = NO_DATA_STRING;
	std::vector<std::string> Name;
	std::string ParentBody = NO_DATA_STRING;
	std::string Class = NO_DATA_STRING;

	// The following properties only used when object type is "Star"
	std::string SpecClass = NO_DATA_STRING;

	// ------------------------------------------------------------ //

	// Physical properties
	float64 Mass = NO_DATA_FLOAT_INF; // Mass in Kg
	vec3 Dimensions = vec3(NO_DATA_FLOAT_INF); // Dimensions in 3 directions
	float64 InertiaMoment = NO_DATA_FLOAT_INF; // Moment of Inertia
	float64 AlbedoBond = NO_DATA_FLOAT_INF; // Bond albedo
	float64 AlbedoGeom = NO_DATA_FLOAT_INF; // Geometric albedo

	// The following properties only used when object type is "Star"
	float64 Teff = NO_DATA_FLOAT_INF; // Effective temperature in Kelvin
	float64 Luminosity = NO_DATA_FLOAT_INF; // Visual Luminosity in Watts
	float64 LumBol = NO_DATA_FLOAT_INF; // Bolometric Luminosity in Watts
	float64 FeH = NO_DATA_FLOAT_INF;
	float64 CtoO = NO_DATA_FLOAT_INF;
	float64 Age = NO_DATA_FLOAT_INF; // Age in Years

	// ------------------------------------------------------------ //

	// Optical properties(Only used when object is not a star)
	float64 Brightness = NO_DATA_FLOAT_INF;
	float64 BrightnessReal = NO_DATA_FLOAT_INF;

	vec3 Color = vec3(NO_DATA_FLOAT_INF);

	// The following properties only used when Comet tail is enabled
	float64 AbsMagn = NO_DATA_FLOAT_INF;
	float64 SlopeParam = NO_DATA_FLOAT_INF;

	// The following properties only used for star remnants
	float64 KerrSpin = NO_DATA_FLOAT_INF;
	float64 KerrCharge = NO_DATA_FLOAT_INF;

	// Hack keys
	float64 EndogenousHeating = NO_DATA_FLOAT_INF;
	float64 ThermalLuminosity = NO_DATA_FLOAT_INF;
	float64 ThermalLuminosityBol = NO_DATA_FLOAT_INF;

	// ------------------------------------------------------------ //

	// Rotation parameters
	std::string RotationModel = "Simple";
	#if OBJ_DEFAULT_ROTATION_MODEL <= 1
	RotationSimple Rotation;
	#if OBJ_DEFAULT_ROTATION_MODEL == 1
	RotationIAU IAU;
	#endif
	#elif OBJ_DEFAULT_ROTATION_MODEL == 2
	// TODO: Ephemerides function is coming soon...
	#endif
	bool TidalLocked = false;

	// ------------------------------------------------------------ //

	// Orbital parameters
	vec3 StaticPos = vec3(NO_DATA_FLOAT_INF);
	vec3 FixedPos = vec3(NO_DATA_FLOAT_INF);
	OrbitParam Orbit;

	// ------------------------------------------------------------ //

	// Life parameters
	bool NoLife = true;
	uint64 LifeCount = 0;
	LifeParam Life[2];
	// Interior
	std::map<std::string, float64> Interior;
	// Landscape parameters
	bool EnableSurface = false;
	Landscape Surface;
	// Ocean/Lava parameters
	bool NoOcean = true;
	OceanParam Ocean;
	// Clouds parameters
	bool NoClouds = true;
	CloudsParam Clouds;
	// Atmosphere parameters
	bool NoAtmosphere = true;
	AtmParam Atmosphere;
	// Aurora parameters
	bool NoAurora = true;
	AurParam Aurora;
	// Rings and Accretion Disk
	bool NoRings = true;
	bool NoAccretionDisk = true; // only available for White dwarfs, Neutron stars(Pulsars) and Black holes
	RingsParam Rings;
	AccDiskParam AccretionDisk; // only available for White dwarfs, Neutron stars(Pulsars) and Black holes
	// Star Corona parameters
	bool NoCorona = true; // only available for stars
	CoronaParam Corona; // only available for stars
	// Comet tail parameters
	bool NoCometTail = true;
	CometTailParam CometTail;
};

/////////////////////////////////////////////////////////////////////////////////
//                                  Functions                                  //
/////////////////////////////////////////////////////////////////////////////////

// Loading Object
Object GetSEObject(ISCStream _Is, _STD string _Name);

_CSE_END

#endif