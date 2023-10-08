#define _USE_CSE_DEFINES

#include "CSE/Object.h"

#include <sstream>
#include <iomanip>
#include <format>

using namespace std;

_CSE_BEGIN

vector<array<float64, 6>> GetPerodicTermsParameters(string _Params)
{
	istringstream _Is(_Params);
	string _Line;
	vector<array<float64, 6>> _Final;
	while (getline(_Is, _Line, '\n'))
	{
		array<float64, 6> _LL;
		istringstream _Val(_Line);
		string _Col;
		uint64 _Index = 0;
		auto it0 = _Line.begin();
		auto end0 = _Line.end();
		while (it0 != end0 && isspace(*it0)) { ++it0; }
		if (it0 == end0) { continue; }
		while (getline(_Val, _Col, ',') && _Index < 6)
		{
			string _ValueStr;
			auto it = _Col.begin();
			auto end = _Col.end();
			if (it == end) { continue; }
			while (it != end && isspace(*it)){++it;}
			while (it != end && !isspace(*it))
			{
				_ValueStr += *it;
				++it;
			}
			istringstream _ToVal(_ValueStr);
			_ToVal >> _LL[_Index];
			++_Index;
		}
		_Final.push_back(_LL);
	}
	return _Final;
}

Object ObjectLoader(_STD vector<_CSE _SC table::KeyValue>::iterator& it)
{
	Object _Obj;

	_Obj.Type = it->Key;
	_Obj.Name = _SC ValueStr(it->Value).as<_SC Names>()->get();

	using _SC GetAs;

	_Obj.DiscMethod = GetAs<string>(it, "DiscMethod");
	_Obj.DiscDate = GetAs<string>(it, "DiscDate");

	_Obj.ParentBody = GetAs<string>(it, "ParentBody");

	if ("Star" == _Obj.Type)
	{
		_Obj.Class = "Sun";
		_Obj.SpecClass = GetAs<string>(it, "Class");
	}
	else 
	{ 
		_Obj.Class = GetAs<string>(it, "Class");
		_Obj.AsterType = GetAs<string>(it, "AsterType");
	}

	Log_IS.Out("Object Loader", "INFO", "[" + _Obj.Name[0] + "] " + "Loading - Physical properties", ILogLevel, true);

	auto FoundMassKg = it->SubTable->find("MassKg") != it->SubTable->end();
	auto FoundMassEarth = it->SubTable->find("Mass") != it->SubTable->end();
	auto FoundMassSol = it->SubTable->find("MassSol") != it->SubTable->end();
	if (FoundMassKg) { _Obj.Mass = GetAs<float64>(it, "MassKg"); }
	else if (FoundMassEarth) { _Obj.Mass = MassEarth * GetAs<float64>(it, "Mass"); }
	else if (FoundMassSol) { _Obj.Mass = MassSol * GetAs<float64>(it, "MassSol"); }

	auto FoundRadius = it->SubTable->find("Radius") != it->SubTable->end();
	auto FoundDimensions = it->SubTable->find("Dimensions") != it->SubTable->end();
	auto FoundRadSol = it->SubTable->find("RadSol") != it->SubTable->end();
	auto FoundRadiusSol = it->SubTable->find("RadiusSol") != it->SubTable->end();
	auto FoundDimSol = it->SubTable->find("DimensionsSol") != it->SubTable->end();
	auto FoundOblateness = it->SubTable->find("Oblateness") != it->SubTable->end();
	if (FoundRadius) { _Obj.Dimensions = 2. * 1000. * vec3(GetAs<float64>(it, "Radius")); }
	else if (FoundRadSol) { _Obj.Dimensions = 2. * float64(RadSol) * vec3(GetAs<float64>(it, "RadSol")); }
	else if (FoundRadiusSol) { _Obj.Dimensions = 2. * float64(RadSol) * vec3(GetAs<float64>(it, "RadiusSol")); }
	if (FoundOblateness)
	{
		auto OblateVector = it->SubTable->find("Oblateness")->Value.find('(') != string::npos;
		if (OblateVector)
		{
			vec3 _Q = GetAs<vec3, float64>(it, "Oblateness");
			_Obj.Dimensions -= _Obj.Dimensions * _Q;
		}
		else
		{
			float64 _Q = GetAs<float64>(it, "Oblateness");
			_Obj.Dimensions.y -= _Obj.Dimensions.y * _Q;
		}
	}
	if (FoundDimensions){ _Obj.Dimensions = 1000. * GetAs<vec3, float64>(it, "Dimensions"); }
	else if (FoundDimSol){ _Obj.Dimensions = float64(RadSol) * GetAs<vec3, float64>(it, "DimensionsSol");}

	_Obj.InertiaMoment = GetAs<float64>(it, "InertiaMoment");
	if (it->SubTable->find("Albedo") != it->SubTable->end())
	{
		_Obj.AlbedoBond = GetAs<float64>(it, "Albedo");
		_Obj.AlbedoGeom = GetAs<float64>(it, "Albedo");
	}
	else
	{
		_Obj.AlbedoBond = GetAs<float64>(it, "AlbedoBond");
		_Obj.AlbedoGeom = GetAs<float64>(it, "AlbedoGeom");
	}

	if (_Obj.Type == "Star")
	{
		_Obj.Luminosity = GetAs<float64>(it, "Luminosity") * SolarLum;
		_Obj.LumBol = GetAs<float64>(it, "LumBol") * SolarLum;
		_Obj.Teff = GetAs<float64>(it, "Teff");
		_Obj.FeH = GetAs<float64>(it, "FeH");
		_Obj.CtoO = GetAs<float64>(it, "CtoO");
		_Obj.Age = GetAs<float64>(it, "Age") * 1E+9;

		// Kerr blackhole parameters
		_Obj.KerrSpin = GetAs<float64>(it, "KerrSpin");
		_Obj.KerrCharge = GetAs<float64>(it, "KerrCharge");
	}

	Log_IS.Out("Object Loader", "INFO", "[" + _Obj.Name[0] + "] " + "Loading - Optical properties", ILogLevel, true);
	
	_Obj.Brightness = GetAs<float64>(it, "Brightness");
	_Obj.BrightnessReal = GetAs<float64>(it, "BrightnessReal");

	_Obj.Color = GetAs<vec3, float64>(it, "Color");

	_Obj.AbsMagn = GetAs<float64>(it, "AbsMagn");
	_Obj.SlopeParam = GetAs<float64>(it, "SlopeParam");

	_Obj.EndogenousHeating = GetAs<float64>(it, "EndogenousHeating");
	_Obj.ThermalLuminosity = GetAs<float64>(it, "ThermalLuminosity");
	_Obj.ThermalLuminosityBol = GetAs<float64>(it, "ThermalLuminosityBol");

	Log_IS.Out("Object Loader", "INFO", "[" + _Obj.Name[0] + "] " + "Loading - Rotation parameters", ILogLevel, true);

	_Obj.RotationModel = GetAs<string>(it, "RotationModel");
	if (_Obj.RotationModel == NO_DATA_STRING) { _Obj.RotationModel = "Simple"; }
	_Obj.TidalLocked = GetAs<bool>(it, "TidalLocked");

	#if 0
	if (_Obj.RotationModel == NO_DATA_STRING)
	{
	#endif
		_Obj.Rotation.RotationPeriod = 3600. * GetAs<float64>(it, "RotationPeriod");
		_Obj.Rotation.RotationEpoch = GetAs<float64>(it, "RotationEpoch");
		_Obj.Rotation.RotationOffset = GetAs<float64>(it, "RotationOffset");
		_Obj.Rotation.Precession = GetAs<float64>(it, "Precession");
		_Obj.Rotation.Obliquity = GetAs<float64>(it, "Obliquity");
		_Obj.Rotation.EqAscendNode = GetAs<float64>(it, "EqAscendNode");
	#if 0
	}
	else if (_Obj.RotationModel == "IAU")
	{
	#else
	if (_Obj.RotationModel == "IAU")
	{
	#endif
		auto RotationIAU = it->SubTable->find("RotationModel");
		_Obj.IAU.Epoch = GetAs<float64>(RotationIAU, "Epoch");
		_Obj.IAU.PoleRA = GetAs<float64>(RotationIAU, "PoleRA");
		_Obj.IAU.PoleRARate = GetAs<float64>(RotationIAU, "PoleRARate");
		_Obj.IAU.PoleDec = GetAs<float64>(RotationIAU, "PoleDec");
		_Obj.IAU.PoleDecRate = GetAs<float64>(RotationIAU, "PoleDecRate");
		_Obj.IAU.PrimeMeridian = GetAs<float64>(RotationIAU, "PrimeMeridian");
		_Obj.IAU.RotationRate = GetAs<float64>(RotationIAU, "RotationRate");
		_Obj.IAU.RotationAccel = GetAs<float64>(RotationIAU, "RotationAccel");

		auto Diurnal = RotationIAU->SubTable->find("PeriodicTermsDiurnal") != RotationIAU->SubTable->end();
		auto Secular = RotationIAU->SubTable->find("PeriodicTermsSecular") != RotationIAU->SubTable->end();

		if (Diurnal)
		{
			_Obj.IAU.PT = "Diurnal"; // Time unit = days
			_Obj.IAU.PeriodicTerms = GetPerodicTermsParameters(GetAs<string>(RotationIAU, "PeriodicTermsDiurnal"));
		}
		else if (Secular)
		{
			_Obj.IAU.PT = "Secular"; // Time unit = century
			_Obj.IAU.PeriodicTerms = GetPerodicTermsParameters(GetAs<string>(RotationIAU, "PeriodicTermsSecular"));
		} 
	}

	Log_IS.Out("Object Loader", "INFO", "[" + _Obj.Name[0] + "] " + "Loading - Orbit parameters", ILogLevel, true);

	auto _Static =
		it->SubTable->find("StaticPosXYZ") != it->SubTable->end() ||
		it->SubTable->find("StaticPosPolar") != it->SubTable->end() ||
		it->SubTable->find("FixedPosXYZ") != it->SubTable->end() ||
		it->SubTable->find("FixedPosPolar") != it->SubTable->end();
	auto _Orbit = 
		it->SubTable->find("Orbit") != it->SubTable->end();
	auto _Binary =
		it->SubTable->find("BinaryOrbit") != it->SubTable->end();
	if (_Static)
	{
		#if OBJ_DEFAULT_STATIC_FIXED_POS == 0
		_Obj.StaticPos = GetAs<vec3, float64>(it, "StaticPosXYZ");
		_Obj.FixedPos = GetAs<vec3, float64>(it, "FixedPosXYZ");
		#elif OBJ_DEFAULT_STATIC_FIXED_POS == 1
		_Obj.StaticPos = GetAs<vec3, float64>(it, "StaticPosPolar");
		_Obj.FixedPos = GetAs<vec3, float64>(it, "FixedPosPolar");
		#endif
	}
	else if (_Orbit || _Binary)
	{
		auto OrbitParams = it->SubTable->find("Orbit");
		if (OrbitParams == it->SubTable->end())
		{
			OrbitParams = it->SubTable->find("BinaryOrbit");
			_Obj.Orbit.Binary = true;
		}
		_Obj.Orbit.AnalyticModel = GetAs<string>(OrbitParams, "AnalyticModel");
		_Obj.Orbit.RefPlane = GetAs<string>(OrbitParams, "RefPlane");
		if (_Obj.Orbit.RefPlane == NO_DATA_STRING) { _Obj.Orbit.RefPlane = "Extrasolar"; }
		_Obj.Orbit.Epoch = GetAs<float64>(OrbitParams, "Epoch");

		_Obj.Orbit.Separation = GetAs<float64>(OrbitParams, "Separation") * AU;
		_Obj.Orbit.PositionAngle = GetAs<float64>(OrbitParams, "PositionAngle");

		_Obj.Orbit.Eccentricity = GetAs<float64>(OrbitParams, "Eccentricity");

		if (_Obj.Orbit.Eccentricity < 1 || isinf(_Obj.Orbit.Eccentricity))
		{
			// Periods is only available when eccentricity smaller than 1.
			auto FoundPeriodDays = OrbitParams->SubTable->find("PeriodDays") != OrbitParams->SubTable->end();
			auto FoundPeriod = OrbitParams->SubTable->find("Period") != OrbitParams->SubTable->end();
			auto FoundMeanMotion = OrbitParams->SubTable->find("MeanMotion") != OrbitParams->SubTable->end();
			if (FoundPeriodDays) { _Obj.Orbit.Period = SynodicDay * GetAs<float64>(OrbitParams, "PeriodDays"); }
			else if (FoundPeriod) { _Obj.Orbit.Period = TropicalYear * GetAs<float64>(OrbitParams, "Period"); }
			else if (FoundMeanMotion) { _Obj.Orbit.Period = 360.0 / GetAs<float64>(OrbitParams, "MeanMotion") * SynodicDay; }
		}
		
		auto FoundSemiMajorAxis = OrbitParams->SubTable->find("SemiMajorAxis") != OrbitParams->SubTable->end();
		auto FoundSemiMajorAxisKm = OrbitParams->SubTable->find("SemiMajorAxisKm") != OrbitParams->SubTable->end();
		auto FoundPericenterDist = OrbitParams->SubTable->find("PericenterDist") != OrbitParams->SubTable->end();
		if (FoundSemiMajorAxis && _Obj.Orbit.Eccentricity < 1)
		{ 
			float64 SemiMajorAxis = AU * GetAs<float64>(OrbitParams, "SemiMajorAxis");
			_Obj.Orbit.PericenterDist = SemiMajorAxis - SemiMajorAxis * _Obj.Orbit.Eccentricity;
		}
		else if (FoundSemiMajorAxisKm && _Obj.Orbit.Eccentricity < 1)
		{
			float64 SemiMajorAxis = 1000. * GetAs<float64>(OrbitParams, "SemiMajorAxisKm");
			_Obj.Orbit.PericenterDist = SemiMajorAxis - SemiMajorAxis * _Obj.Orbit.Eccentricity;
		}
		else if (FoundPericenterDist || _Obj.Orbit.Eccentricity >= 1)
		{
			_Obj.Orbit.PericenterDist = AU * GetAs<float64>(OrbitParams, "PericenterDist");
		}

		_Obj.Orbit.GravParam = GetAs<float64>(OrbitParams, "GravParam");

		_Obj.Orbit.Inclination = GetAs<float64>(OrbitParams, "Inclination");
		_Obj.Orbit.AscendingNode = GetAs<float64>(OrbitParams, "AscendingNode");
		
		auto FoundArgOfPeri = OrbitParams->SubTable->find("ArgOfPericenter") != OrbitParams->SubTable->end();
		auto FoundArgOfPeri2 = OrbitParams->SubTable->find("ArgOfPericen") != OrbitParams->SubTable->end();
		auto FoundLongOfPeri = OrbitParams->SubTable->find("LongOfPericen") != OrbitParams->SubTable->end();
		auto FoundMeanAnomaly = OrbitParams->SubTable->find("MeanAnomaly") != OrbitParams->SubTable->end();
		auto FoundMeanLongitude = OrbitParams->SubTable->find("MeanLongitude") != OrbitParams->SubTable->end();
		if (FoundArgOfPeri) { _Obj.Orbit.ArgOfPericenter = GetAs<float64>(OrbitParams, "ArgOfPericenter"); }
		else if (FoundArgOfPeri2) { _Obj.Orbit.ArgOfPericenter = GetAs<float64>(OrbitParams, "ArgOfPericen"); }
		else if (FoundLongOfPeri) { _Obj.Orbit.ArgOfPericenter = GetAs<float64>(OrbitParams, "LongOfPericen") - _Obj.Orbit.AscendingNode; }
		if (FoundMeanAnomaly) { _Obj.Orbit.MeanAnomaly = GetAs<float64>(OrbitParams, "MeanAnomaly"); }
		else if (FoundMeanLongitude) { _Obj.Orbit.MeanAnomaly = GetAs<float64>(OrbitParams, "MeanLongitude") - _Obj.Orbit.AscendingNode - _Obj.Orbit.ArgOfPericenter; }
	}

	Log_IS.Out("Object Loader", "INFO", "[" + _Obj.Name[0] + "] " + "Loading - Life parameters", ILogLevel, true);

	auto FindLife = it->SubTable->begin();
	auto LifeList = _SC GetMultipleSubTables(FindLife, it->SubTable->end(), "Life");
	_Obj.NoLife = GetAs<bool>(it, "NoLife");
	_Obj.LifeCount = min(LifeList.size(), 2);
	if (LifeList.size() > 2)
	{
		Log_IS.Out("ISCStream", "WARNING", "[" + _Obj.Name[0] + "] " + "The third or more life tags are found, these tags will be ignored.", ILogLevel, true);
	}
	if (!(_Obj.NoLife || LifeList.empty()))
	{
		for (size_t i = 0; i < _Obj.LifeCount; i++)
		{
			_Obj.Life[i].Class = GetAs<string>(LifeList[i], "Class");
			_Obj.Life[i].Type = GetAs<string>(LifeList[i], "Type");
			_Obj.Life[i].Biome = GetAs<_SC Names>(LifeList[i], "Biome");
			_Obj.Life[i].Panspermia = GetAs<bool>(LifeList[i], "Panspermia");
		}
	}

	Log_IS.Out("Object Loader", "INFO", "[" + _Obj.Name[0] + "] " + "Loading - Interior", ILogLevel, true);

	auto FoundInterior = it->SubTable->find("Interior") != it->SubTable->end();
	if (FoundInterior)
	{
		auto Interior = it->SubTable->find("Interior");
		auto FoundComposition = Interior->SubTable->find("Composition") != Interior->SubTable->end();
		if (FoundComposition)
		{
			_Obj.Interior = Interior->SubTable->find("Composition")->SubTable->ToMap<float64>();
		}
	}

	Log_IS.Out("Object Loader", "INFO", "[" + _Obj.Name[0] + "] " + "Loading - Landscape parameters", ILogLevel, true);

	auto FoundSurface = it->SubTable->find("Surface") != it->SubTable->end();
	if (FoundSurface)
	{
		auto Surface = it->SubTable->find("Surface");
		_Obj.EnableSurface = true;
		_Obj.Surface.Preset = GetAs<string>(Surface, "Preset");
		_Obj.Surface.SurfStyle = GetAs<float64>(Surface, "SurfStyle");
		_Obj.Surface.Randomize = GetAs<vec3, float64>(Surface, "Randomize");
		_Obj.Surface.colorParams = vec4
		(
			GetAs<float64>(Surface, "colorDistMagn"),
			GetAs<float64>(Surface, "colorDistFreq"),
			GetAs<float64>(Surface, "icecapLatitude"),
			GetAs<float64>(Surface, "tropicLatitude")
		);
		_Obj.Surface.climateParams = vec4
		(
			GetAs<float64>(Surface, "climatePole"),
			GetAs<float64>(Surface, "climateTropic"),
			GetAs<float64>(Surface, "climateEquator"),
			GetAs<float64>(Surface, "tropicWidth")
		);
		_Obj.Surface.plantsParams1 = vec4
		(
			GetAs<float64>(Surface, "climateGrassMin"),
			GetAs<float64>(Surface, "climateGrassMax"),
			GetAs<float64>(Surface, "climateForestMin"),
			GetAs<float64>(Surface, "climateForestMax")
		);
		_Obj.Surface.plantsParams2 = vec4
		(
			GetAs<float64>(Surface, "climateSteppeMin"),
			GetAs<float64>(Surface, "climateSteppeMax"),
			GetAs<float64>(Surface, "humidity"),
			GetAs<float64>(Surface, "plantsBiomeOffset")
		);
		_Obj.Surface.mainParams = vec4
		(
			GetAs<float64>(Surface, "mainFreq"),
			GetAs<float64>(Surface, "terraceProb"),
			GetAs<float64>(Surface, "oceanType"),
			GetAs<float64>(Surface, "tidalLock")
		);
		_Obj.Surface.textureParams = vec4
		(
			GetAs<float64>(Surface, "detailScale"),
			NO_DATA_FLOAT_INF,
			GetAs<float64>(Surface, "venusMagn"),
			GetAs<float64>(Surface, "venusFreq")
		);
		_Obj.Surface.mareParams = vec4
		(
			GetAs<float64>(Surface, "seaLevel"),
			GetAs<float64>(Surface, "mareFreq"),
			GetAs<float64>(Surface, "mareDensity"),
			GetAs<float64>(Surface, "icecapHeight")
		);
		_Obj.Surface.montesParams = vec4
		(
			GetAs<float64>(Surface, "montesMagn"),
			GetAs<float64>(Surface, "montesFreq"),
			GetAs<float64>(Surface, "montesFraction"),
			GetAs<float64>(Surface, "montesSpiky")
		);
		_Obj.Surface.dunesParams = vec4
		(
			GetAs<float64>(Surface, "dunesMagn"),
			GetAs<float64>(Surface, "dunesFreq"),
			GetAs<float64>(Surface, "dunesDensity"),
			GetAs<float64>(Surface, "drivenDarkening")
		);
		_Obj.Surface.hillsParams = vec4
		(
			GetAs<float64>(Surface, "hillsMagn"),
			GetAs<float64>(Surface, "hillsFreq"),
			GetAs<float64>(Surface, "hillsFraction"),
			GetAs<float64>(Surface, "hills2Fraction")
		);
		_Obj.Surface.riversParams = vec4
		(
			GetAs<float64>(Surface, "riversMagn"),
			GetAs<float64>(Surface, "riversFreq"),
			GetAs<float64>(Surface, "riversSin"),
			GetAs<float64>(Surface, "beachWidth")
		);
		_Obj.Surface.riftsParams = vec4
		(
			GetAs<float64>(Surface, "riftsMagn"),
			GetAs<float64>(Surface, "riftsFreq"),
			GetAs<float64>(Surface, "riftsSin"),
			NO_DATA_FLOAT_INF
		);
		_Obj.Surface.canyonsParams = vec4
		(
			GetAs<float64>(Surface, "canyonsMagn"),
			GetAs<float64>(Surface, "canyonsFreq"),
			GetAs<float64>(Surface, "canyonsFraction"),
			GetAs<float64>(Surface, "erosion")
		);
		_Obj.Surface.cracksParams = vec4
		(
			GetAs<float64>(Surface, "cracksMagn"),
			GetAs<float64>(Surface, "cracksFreq"),
			GetAs<float64>(Surface, "cracksOctaves"),
			GetAs<float64>(Surface, "craterRayedFactor")
		);
		_Obj.Surface.craterParams = vec4
		(
			GetAs<float64>(Surface, "craterMagn"),
			GetAs<float64>(Surface, "craterFreq"),
			GetAs<float64>(Surface, "craterDensity"),
			GetAs<float64>(Surface, "craterOctaves")
		);
		_Obj.Surface.volcanoParams1 = vec4
		(
			GetAs<float64>(Surface, "volcanoMagn"),
			GetAs<float64>(Surface, "volcanoFreq"),
			GetAs<float64>(Surface, "volcanoDensity"),
			GetAs<float64>(Surface, "volcanoOctaves")
		);
		_Obj.Surface.volcanoParams2 = vec4
		(
			GetAs<float64>(Surface, "volcanoActivity"),
			GetAs<float64>(Surface, "volcanoFlows"),
			GetAs<float64>(Surface, "volcanoRadius"),
			GetAs<float64>(Surface, "volcanoTemp")
		);
		_Obj.Surface.lavaParams = vec4
		(
			GetAs<float64>(Surface, "lavaCoverage"),
			GetAs<float64>(Surface, "snowLevel"),
			GetAs<float64>(Surface, "surfTemperature"),
			GetAs<float64>(Surface, "heightTempGrad")
		);
		_Obj.Surface.lavaDetails = vec4
		(
			GetAs<float64>(Surface, "lavaCoverTidal"),
			GetAs<float64>(Surface, "lavaCoverSun"),
			GetAs<float64>(Surface, "lavaCoverYoung"),
			NO_DATA_FLOAT_INF
		);
		_Obj.Surface.cycloneParams = vec4
		(
			GetAs<float64>(Surface, "cycloneMagn"),
			GetAs<float64>(Surface, "cycloneFreq"),
			GetAs<float64>(Surface, "cycloneDensity"),
			GetAs<float64>(Surface, "cycloneOctaves")
		);
		_Obj.Surface.cycloneParams2 = vec4
		(
			GetAs<float64>(Surface, "cyclone2Magn"),
			GetAs<float64>(Surface, "cyclone2Freq"),
			GetAs<float64>(Surface, "cyclone2Density"),
			GetAs<float64>(Surface, "cyclone2Octaves")
		);

		_Obj.Surface.DiffMap = GetAs<string>(Surface, "DiffMap");
		_Obj.Surface.DiffMapAlpha = GetAs<string>(Surface, "DiffMapAlpha");
		_Obj.Surface.BumpMap = GetAs<string>(Surface, "BumpMap");
		_Obj.Surface.BumpHeight = GetAs<float64>(Surface, "BumpHeight");
		_Obj.Surface.BumpOffset = GetAs<float64>(Surface, "BumpOffset");
		_Obj.Surface.GlowMap = GetAs<string>(Surface, "GlowMap");
		_Obj.Surface.GlowMode = GetAs<string>(Surface, "GlowMode");
		_Obj.Surface.GlowColor = GetAs<vec3, float64>(Surface, "GlowColor");
		_Obj.Surface.GlowBright = GetAs<float64>(Surface, "GlowBright");
		_Obj.Surface.SpecMap = GetAs<string>(Surface, "SpecMap");
		_Obj.Surface.RoughnessBias = GetAs<float64>(Surface, "RoughnessBias");
		_Obj.Surface.RoughnessScale = GetAs<float64>(Surface, "RoughnessScale");
		_Obj.Surface.SpecParams = vec4
		(
			GetAs<float64>(Surface, "SpecBrightIce"),
			GetAs<float64>(Surface, "SpecBrightWater"),
			GetAs<float64>(Surface, "SpecPowerIce"),
			GetAs<float64>(Surface, "SpecPowerWater")
		);
		_Obj.Surface.SpecularScale = GetAs<float64>(Surface, "SpecularScale");

		_Obj.Surface.Hapke = GetAs<float64>(Surface, "Hapke");
		_Obj.Surface.HapkeParams = vec4
		(
			GetAs<float64>(Surface, "SpotBright"),
			GetAs<float64>(Surface, "SpotWidth"),
			GetAs<float64>(Surface, "SpotBrightCB"),
			GetAs<float64>(Surface, "SpotWidthCB")
		);
		_Obj.Surface.DayAmbient = GetAs<float64>(Surface, "DayAmbient");
		_Obj.Surface.ModulateColor = GetAs<vec3, float64>(Surface, "ModulateColor");

		if (_Obj.Surface.RingsWinter == NO_DATA_FLOAT_INF)
		{
			_Obj.Surface.RingsWinter = GetAs<float64>(Surface, "RingsWinter");
		}
	}

	Log_IS.Out("Object Loader", "INFO", "[" + _Obj.Name[0] + "] " + "Loading - Custom climate model parameters", ILogLevel, true);

	auto FoundClimate = it->SubTable->find("Climate") != it->SubTable->end();
	if (FoundClimate)
	{
		_Obj.EnableCustomClimate = true;
		auto Climate = it->SubTable->find("Climate");
		_Obj.Climate.AtmoProfile = GetAs<string>(Climate, "AtmoProfile");
		_Obj.Climate.MinSurfaceTemp = GetAs<float64>(Climate, "MinSurfaceTemp");
		_Obj.Climate.MaxSurfaceTemp = GetAs<float64>(Climate, "MaxSurfaceTemp");
		_Obj.Climate.GlobalWindSpeed = GetAs<float64>(Climate, "GlobalWindSpeed");
	}

	Log_IS.Out("Object Loader", "INFO", "[" + _Obj.Name[0] + "] " + "Loading - Ocean parameters", ILogLevel, true);

	auto FoundOcean = it->SubTable->find("Ocean") != it->SubTable->end();
	_Obj.NoOcean = GetAs<bool>(it, "NoOcean") || !FoundOcean;
	if (!_Obj.NoOcean && FoundOcean)
	{
		auto Ocean = it->SubTable->find("Ocean");
		if (Ocean->SubTable->find("Height") != Ocean->SubTable->end())
		{
			_Obj.Ocean.Height = GetAs<float64>(Ocean, "Height") * 1000.;
		}
		else if (Ocean->SubTable->find("Depth") != Ocean->SubTable->end())
		{
			_Obj.Ocean.Height = GetAs<float64>(Ocean, "Depth") * 1000.;
		}

		_Obj.Ocean.Hapke = GetAs<float64>(Ocean, "Hapke");
		_Obj.Ocean.HapkeParams = vec4
		(
			GetAs<float64>(Ocean, "SpotBright"),
			GetAs<float64>(Ocean, "SpotWidth"),
			GetAs<float64>(Ocean, "SpotBrightCB"),
			GetAs<float64>(Ocean, "SpotWidthCB")
		);
		_Obj.Ocean.DayAmbient = GetAs<float64>(Ocean, "DayAmbient");
		_Obj.Ocean.ModulateBright = GetAs<float64>(Ocean, "ModulateBright");

		if (Ocean->SubTable->find("Composition") != Ocean->SubTable->end())
		{
			_Obj.Ocean.Composition = Ocean->SubTable->find("Composition")->SubTable->ToMap<float64>();
		}
	}

	auto FindClouds = it->SubTable->begin();
	auto CloudsList = _SC GetMultipleSubTables(FindClouds, it->SubTable->end(), "Clouds");
	_Obj.NoClouds = GetAs<bool>(it, "NoClouds") || CloudsList.empty();
	if (!_Obj.NoClouds && !CloudsList.empty())
	{
		_Obj.Clouds.cloudsParams2.x = 0.0;
		_Obj.Clouds.cloudsParams2.y = float64(CloudsList.size());
		for (size_t i = 0; i < _Obj.Clouds.cloudsParams2.y; i++)
		{
			if (_Obj.Clouds.cloudsParams1.x == NO_DATA_FLOAT_INF)
			{ 
				_Obj.Clouds.cloudsParams1.x = GetAs<float64>(CloudsList[i], "mainFreq");
			}
			if (_Obj.Clouds.cloudsParams1.y == NO_DATA_FLOAT_INF)
			{
				_Obj.Clouds.cloudsParams1.y = GetAs<float64>(CloudsList[i], "mainOctaves");
			}
			if (_Obj.Clouds.cloudsParams1.z == NO_DATA_FLOAT_INF)
			{
				_Obj.Clouds.cloudsParams1.z = GetAs<float64>(CloudsList[i], "stripeZones");
			}
			if (_Obj.Clouds.cloudsParams1.w == NO_DATA_FLOAT_INF)
			{
				_Obj.Clouds.cloudsParams1.w = GetAs<float64>(CloudsList[i], "stripeTwist");
			}

			if (_Obj.Clouds.cloudsParams2.z == NO_DATA_FLOAT_INF)
			{
				_Obj.Clouds.cloudsParams2.z = GetAs<float64>(CloudsList[i], "stripeFluct");
			}
			if (_Obj.Clouds.cloudsParams2.w == NO_DATA_FLOAT_INF)
			{
				_Obj.Clouds.cloudsParams2.w = GetAs<float64>(CloudsList[i], "Coverage");
			}

			if (_Obj.Surface.RingsWinter == NO_DATA_FLOAT_INF)
			{
				_Obj.Surface.RingsWinter = GetAs<float64>(CloudsList[i], "RingsWinter");
			}

			CloudsParam::LayerDetails Layer;
			Layer.DiffMap = GetAs<string>(CloudsList[i], "DiffMap");
			Layer.BumpMap = GetAs<string>(CloudsList[i], "BumpMap");
			Layer.Height = GetAs<float64>(CloudsList[i], "Height");
			Layer.Velocity = GetAs<float64>(CloudsList[i], "Velocity");
			Layer.BumpHeight = GetAs<float64>(CloudsList[i], "BumpHeight");
			Layer.BumpOffset = GetAs<float64>(CloudsList[i], "BumpOffset");
			
			Layer.Hapke = GetAs<float64>(CloudsList[i], "Hapke");
			Layer.HapkeParams = vec4
			(
				GetAs<float64>(CloudsList[i], "SpotBright"),
				GetAs<float64>(CloudsList[i], "SpotWidth"),
				GetAs<float64>(CloudsList[i], "SpotBrightCB"),
				GetAs<float64>(CloudsList[i], "SpotWidthCB")
			);
			Layer.DayAmbient = GetAs<float64>(CloudsList[i], "DayAmbient");
			Layer.ModulateColor = vec4
			(
				GetAs<vec3, float64>(CloudsList[i], "ModulateColor"), 
				GetAs<float64>(CloudsList[i], "Opacity")
			);
			Layer.ModulateBright = GetAs<float64>(CloudsList[i], "ModulateBright");
			_Obj.Clouds.Layer.push_back(Layer);
		}
	}

	auto FoundAtm = it->SubTable->find("Atmosphere") != it->SubTable->end();
	_Obj.NoAtmosphere = GetAs<bool>(it, "NoAtmosphere") || !FoundAtm;
	if (!_Obj.NoAtmosphere && FoundAtm)
	{
		auto Atm = it->SubTable->find("Atmosphere");
		_Obj.Atmosphere.Model = GetAs<string>(Atm, "Model");
		_Obj.Atmosphere.Height = GetAs<float64>(Atm, "Height") * 1000;
		_Obj.Atmosphere.Density = GetAs<float64>(Atm, "Density");
		_Obj.Atmosphere.Adiabat = GetAs<float64>(Atm, "Adiabat");
		_Obj.Atmosphere.Pressure = GetAs<float64>(Atm, "Pressure") * StdAtm;
		_Obj.Atmosphere.Greenhouse = GetAs<float64>(Atm, "Greenhouse");
		_Obj.Atmosphere.Bright = GetAs<float64>(Atm, "Bright");
		_Obj.Atmosphere.Opacity = GetAs<float64>(Atm, "Opacity");
		_Obj.Atmosphere.SkyLight = GetAs<float64>(Atm, "SkyLight");
		_Obj.Atmosphere.Hue = GetAs<float64>(Atm, "Hue");
		_Obj.Atmosphere.Saturation = GetAs<float64>(Atm, "Saturation");

		if (Atm->SubTable->find("Composition") != Atm->SubTable->end())
		{
			_Obj.Atmosphere.Composition = Atm->SubTable->find("Composition")->SubTable->ToMap<float64>();
		}
	}

	auto FoundAurora = it->SubTable->find("Aurora") != it->SubTable->end();
	_Obj.NoAurora = GetAs<bool>(it, "NoAurora") || !FoundAurora;
	if (!_Obj.NoAurora && FoundAurora)
	{
		auto Aurora = it->SubTable->find("Aurora");
		_Obj.Aurora.Height = GetAs<float64>(Aurora, "Height");
		_Obj.Aurora.North.Lat = GetAs<float64>(Aurora, "NorthLat");
		_Obj.Aurora.North.Lon = GetAs<float64>(Aurora, "NorthLon");
		_Obj.Aurora.North.Radius = GetAs<float64>(Aurora, "NorthRadius");
		_Obj.Aurora.North.Width = GetAs<float64>(Aurora, "NorthWidth");
		_Obj.Aurora.North.Rings = GetAs<float64>(Aurora, "NorthRings");
		_Obj.Aurora.North.Bright = GetAs<float64>(Aurora, "NorthBright");
		_Obj.Aurora.North.FlashFreq = GetAs<float64>(Aurora, "NorthFlashFreq");
		_Obj.Aurora.North.MoveSpeed = GetAs<float64>(Aurora, "NorthMoveSpeed");
		_Obj.Aurora.North.Particles = GetAs<float64>(Aurora, "NorthParticles");
		_Obj.Aurora.South.Lat = GetAs<float64>(Aurora, "SouthLat");
		_Obj.Aurora.South.Lon = GetAs<float64>(Aurora, "SouthLon");
		_Obj.Aurora.South.Radius = GetAs<float64>(Aurora, "SouthRadius");
		_Obj.Aurora.South.Width = GetAs<float64>(Aurora, "SouthWidth");
		_Obj.Aurora.South.Rings = GetAs<float64>(Aurora, "SouthRings");
		_Obj.Aurora.South.Bright = GetAs<float64>(Aurora, "SouthBright");
		_Obj.Aurora.South.FlashFreq = GetAs<float64>(Aurora, "SouthFlashFreq");
		_Obj.Aurora.South.MoveSpeed = GetAs<float64>(Aurora, "SouthMoveSpeed");
		_Obj.Aurora.South.Particles = GetAs<float64>(Aurora, "SouthParticles");
		_Obj.Aurora.TopColor = GetAs<vec3, float64>(Aurora, "TopColor");
		_Obj.Aurora.BottomColor = GetAs<vec3, float64>(Aurora, "BottomColor");
	}

	auto FoundRings = it->SubTable->find("Rings") != it->SubTable->end();
	_Obj.NoRings = GetAs<bool>(it, "NoRings") || !FoundRings;
	if (!_Obj.NoRings && FoundRings)
	{
		auto Rings = it->SubTable->find("Rings");
		_Obj.Rings.Texture = GetAs<string>(Rings, "Texture");
		_Obj.Rings.Radiuses = vec4
		(
			GetAs<float64>(Rings, "InnerRadius"),
			GetAs<float64>(Rings, "OuterRadius"),
			GetAs<float64>(Rings, "EdgeRadius"),
			GetAs<float64>(Rings, "MeanRadius")
		);
		_Obj.Rings.Thickness = GetAs<float64>(Rings, "Thickness");
		_Obj.Rings.RockParam = vec3
		(
			GetAs<float64>(Rings, "RocksMaxSize"),
			GetAs<float64>(Rings, "RocksSpacing"),
			GetAs<float64>(Rings, "DustDrawDist")
		);
		_Obj.Rings.ChartRadius = GetAs<float64>(Rings, "ChartRadius");
		_Obj.Rings.RotationPeriod = GetAs<float64>(Rings, "RotationPeriod");
		_Obj.Rings.Brightness = vec3
		(
			GetAs<float64>(Rings, "Brightness"),
			GetAs<float64>(Rings, "FrontBright"),
			GetAs<float64>(Rings, "BackBright")
		);
		_Obj.Rings.ShadowParam = vec4
		(
			GetAs<float64>(Rings, "Density"),
			GetAs<float64>(Rings, "Opacity"),
			GetAs<float64>(Rings, "SelfShadow"),
			GetAs<float64>(Rings, "PlanetShadow")
		);

		_Obj.Rings.Hapke = GetAs<float64>(Rings, "Hapke");
		_Obj.Rings.HapkeParams = vec4
		(
			GetAs<float64>(Rings, "SpotBright"),
			GetAs<float64>(Rings, "SpotWidth"),
			GetAs<float64>(Rings, "SpotBrightCB"),
			GetAs<float64>(Rings, "SpotWidthCB")
		);
		_Obj.Rings.Shape = vec4
		(
			GetAs<float64>(Rings, "frequency"),
			GetAs<float64>(Rings, "densityScale"),
			GetAs<float64>(Rings, "densityOffset"),
			GetAs<float64>(Rings, "densityPower")
		);
		_Obj.Rings.colorContrast = GetAs<float64>(Rings, "colorContrast");
		_Obj.Rings.FrontColor = GetAs<vec3, float64>(Rings, "FrontColor");
		_Obj.Rings.BackThickColor = GetAs<vec3, float64>(Rings, "BackThickColor");
		_Obj.Rings.BackIceColor = GetAs<vec3, float64>(Rings, "BackIceColor");
		_Obj.Rings.BackDustColor = GetAs<vec3, float64>(Rings, "BackDustColor");
	}

	auto FoundAccDisk = it->SubTable->find("AccretionDisk") != it->SubTable->end();
	_Obj.NoAccretionDisk = GetAs<bool>(it, "NoAccretionDisk") || !FoundAccDisk;
	if (!_Obj.NoAccretionDisk && FoundAccDisk)
	{
		auto AccDisk = it->SubTable->find("AccretionDisk");
		auto FoundInRadKm = AccDisk->SubTable->find("InnerRadiusKm") != AccDisk->SubTable->end();
		auto FoundInRadAU = AccDisk->SubTable->find("InnerRadius") != AccDisk->SubTable->end();
		if (FoundInRadKm){_Obj.AccretionDisk.Radiuses.x = 1000. * GetAs<float64>(AccDisk, "InnerRadiusKm");}
		else if (FoundInRadAU) { _Obj.AccretionDisk.Radiuses.x = AU * GetAs<float64>(AccDisk, "InnerRadius"); }

		auto FoundOutRadKm = AccDisk->SubTable->find("OuterRadiusKm") != AccDisk->SubTable->end();
		auto FoundOutRadAU = AccDisk->SubTable->find("OuterRadius") != AccDisk->SubTable->end();
		if (FoundOutRadKm) { _Obj.AccretionDisk.Radiuses.y = 1000. * GetAs<float64>(AccDisk, "OuterRadiusKm"); }
		else if (FoundOutRadAU) { _Obj.AccretionDisk.Radiuses.y = AU * GetAs<float64>(AccDisk, "OuterRadius"); }

		_Obj.AccretionDisk.DiskParams1.x = GetAs<float64>(AccDisk, "Temperature");
		_Obj.AccretionDisk.DiskParams1.y = GetAs<float64>(AccDisk, "TwistMagn");

		_Obj.AccretionDisk.DiskParams2.y = GetAs<float64>(AccDisk, "Brightness");
		_Obj.AccretionDisk.DiskParams2.z = GetAs<float64>(AccDisk, "Opacity");

		auto FoundInThkKm = AccDisk->SubTable->find("InnerThicknessKm") != AccDisk->SubTable->end();
		auto FoundInThkAU = AccDisk->SubTable->find("InnerThickness") != AccDisk->SubTable->end();
		if (FoundInThkKm) { _Obj.AccretionDisk.DiskParams3.x = 1000. * GetAs<float64>(AccDisk, "InnerThicknessKm"); }
		else if (FoundInThkAU) { _Obj.AccretionDisk.DiskParams3.x = AU * GetAs<float64>(AccDisk, "InnerThickness"); }

		auto FoundOutThkKm = AccDisk->SubTable->find("OuterThicknessKm") != AccDisk->SubTable->end();
		auto FoundOutThkAU = AccDisk->SubTable->find("OuterThickness") != AccDisk->SubTable->end();
		if (FoundOutThkKm) { _Obj.AccretionDisk.DiskParams3.y = 1000. * GetAs<float64>(AccDisk, "OuterThicknessKm"); }
		else if (FoundOutThkAU) { _Obj.AccretionDisk.DiskParams3.y = AU * GetAs<float64>(AccDisk, "OuterThickness"); }

		_Obj.AccretionDisk.DiskParams3.z = GetAs<float64>(AccDisk, "DetailScaleR");
		_Obj.AccretionDisk.DiskParams3.w = GetAs<float64>(AccDisk, "DetailScaleV");

		_Obj.AccretionDisk.ThicknessPow = GetAs<float64>(AccDisk, "ThicknessPow");
		_Obj.AccretionDisk.AccretionRate = GetAs<float64>(AccDisk, "AccretionRate");
		_Obj.AccretionDisk.Density = GetAs<float64>(AccDisk, "Density");
		_Obj.AccretionDisk.Luminosity = GetAs<float64>(AccDisk, "Luminosity") * SolarLum;
		_Obj.AccretionDisk.LumBol = GetAs<float64>(AccDisk, "LuminosityBol") * SolarLum;

		_Obj.AccretionDisk.DiskParams4.x = GetAs<float64>(AccDisk, "OctaveDistortionX");
		_Obj.AccretionDisk.DiskParams4.y = GetAs<float64>(AccDisk, "OctaveDistortionY");
		_Obj.AccretionDisk.DiskParams4.z = GetAs<float64>(AccDisk, "OctaveDistortionZ");
		_Obj.AccretionDisk.DiskParams4.w = GetAs<float64>(AccDisk, "OctaveScale");

		_Obj.AccretionDisk.DiskParams5.x = GetAs<float64>(AccDisk, "DiskNoiseContrast");
		_Obj.AccretionDisk.DiskParams5.z = GetAs<float64>(AccDisk, "DiskTempContrast");
		_Obj.AccretionDisk.DiskParams5.y = GetAs<float64>(AccDisk, "DiskOuterSpin");

		_Obj.AccretionDisk.LightingBright = GetAs<float64>(AccDisk, "LightingBright");

		// ---------- Jet Params ---------- //

		auto FoundJetRadSKm = AccDisk->SubTable->find("JetStartRadiusKm") != AccDisk->SubTable->end();
		auto FoundJetRadSAU = AccDisk->SubTable->find("JetStartRadius") != AccDisk->SubTable->end();
		if (FoundJetRadSKm) { _Obj.AccretionDisk.Jet.JetParams2.x = 1000. * GetAs<float64>(AccDisk, "JetStartRadiusKm"); }
		else if (FoundJetRadSAU) { _Obj.AccretionDisk.Jet.JetParams2.x = AU * GetAs<float64>(AccDisk, "JetStartRadius"); }

		auto FoundJetRadEKm = AccDisk->SubTable->find("JetEndRadiusKm") != AccDisk->SubTable->end();
		auto FoundJetRadEAU = AccDisk->SubTable->find("JetEndRadius") != AccDisk->SubTable->end();
		if (FoundJetRadEKm) { _Obj.AccretionDisk.Jet.JetParams2.y = 1000. * GetAs<float64>(AccDisk, "JetEndRadiusKm"); }
		else if (FoundJetRadEAU) { _Obj.AccretionDisk.Jet.JetParams2.y = AU * GetAs<float64>(AccDisk, "JetEndRadiusKm"); }

		auto FoundJetLenKm = AccDisk->SubTable->find("JetLengthKm") != AccDisk->SubTable->end();
		auto FoundJetLenAU = AccDisk->SubTable->find("JetLength") != AccDisk->SubTable->end();
		if (FoundJetLenKm) { _Obj.AccretionDisk.Jet.JetParams2.z = 1000. * GetAs<float64>(AccDisk, "JetLengthKm"); }
		else if (FoundJetLenAU) { _Obj.AccretionDisk.Jet.JetParams2.z = AU * GetAs<float64>(AccDisk, "JetLength"); }

		_Obj.AccretionDisk.Jet.JetParams2.w = GetAs<float64>(AccDisk, "JetVelocity");

		_Obj.AccretionDisk.Jet.JetParams1.x = GetAs<float64>(AccDisk, "JetStartTemp");
		_Obj.AccretionDisk.Jet.JetParams1.y = GetAs<float64>(AccDisk, "JetEndTemp");
		_Obj.AccretionDisk.Jet.JetParams1.z = GetAs<float64>(AccDisk, "ShadowContrast");
		_Obj.AccretionDisk.Jet.JetParams1.w = GetAs<float64>(AccDisk, "ShadowLength");

		_Obj.AccretionDisk.Jet.JetParams3.x = GetAs<float64>(AccDisk, "JetDensity");
		_Obj.AccretionDisk.Jet.JetParams3.y = GetAs<float64>(AccDisk, "JetBrightness");
		_Obj.AccretionDisk.Jet.JetParams3.z = GetAs<float64>(AccDisk, "JetDistortion");
		_Obj.AccretionDisk.Jet.JetParams3.w = GetAs<float64>(AccDisk, "JetThickness");
	}

	auto FoundCorona = it->SubTable->find("Corona") != it->SubTable->end();
	_Obj.NoCorona = GetAs<bool>(it, "NoCorona") || !FoundCorona;
	if (!_Obj.NoCorona && FoundCorona)
	{
		auto Corona = it->SubTable->find("Corona");
		_Obj.Corona.Shape = vec4
		(
			GetAs<float64>(Corona, "RayDensity"),
			GetAs<float64>(Corona, "RayCurv"),
			GetAs<float64>(Corona, "Radius"),
			GetAs<float64>(Corona, "SinFreq")
		);
		_Obj.Corona.Period = GetAs<float64>(Corona, "Period");
		_Obj.Corona.Bright = GetAs<float64>(Corona, "Brightness");
	}

	auto FoundCTail = it->SubTable->find("CometTail") != it->SubTable->end();
	_Obj.NoCometTail = GetAs<bool>(it, "NoCometTail") || !FoundCTail;
	if (!_Obj.NoCometTail && FoundCTail)
	{
		auto CTail = it->SubTable->find("CometTail");
		_Obj.CometTail.MaxLength = GetAs<float64>(CTail, "MaxLength");
		_Obj.CometTail.GasToDust = GetAs<float64>(CTail, "GasToDust");
		_Obj.CometTail.Particles = GetAs<float64>(CTail, "Particles");
		_Obj.CometTail.GasBright = GetAs<float64>(CTail, "GasBright");
		_Obj.CometTail.DustBright = GetAs<float64>(CTail, "DustBright");
		_Obj.CometTail.GasColor = GetAs<vec3, float64>(CTail, "GasColor");
		_Obj.CometTail.DustColor = GetAs<vec3, float64>(CTail, "DustColor");
	}

	return _Obj;
}

Object GetSEObject(ISCStream _Is, string _Name)
{
	// Find object
	auto it = _Is->Catalogue.begin();
	auto end = _Is->Catalogue.end();
	_SC Names _N;
	while (it != end)
	{
		_N = _SC ValueStr(it->Value).as<_SC Names>()->get();
		if (_STD find(_N.begin(), _N.end(), _Name) != _N.end()) { break; }
		++it;
	}
	_CSE_VERIFY(it != end, _SC ParseException("Object \"" + _Name + "\" is not found in this stream."));
	Log_IS.Out("Object Loader", "INFO", "[CSE Object loader] Loading object - " + _Name, ILogLevel, true);
	return ObjectLoader(it);
}

_CSE_END