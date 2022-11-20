#define _USE_CSE_DEFINES

#include "../headers/Core/CelObject.h"
#include "../headers/stream/oscstream.h"
#include "../headers/Core/ConstLists.h"
#include "../headers/math/AsMath.h"

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
	Object _Obj;

	_Obj.Type = it->Key;
	_Obj.Name = _N;

	using _SC GetAs;

	_Obj.ParentBody = GetAs<string>(it, "ParentBody");

	if ("Star" == _Obj.Type)
	{
		_Obj.Class = "Sun";
		_Obj.SpecClass = GetAs<string>(it, "Class");
	}
	else { _Obj.Class = GetAs<string>(it, "Class"); }

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
	//else
	//{
		_Obj.AlbedoBond = GetAs<float64>(it, "AlbedoBond");
		_Obj.AlbedoGeom = GetAs<float64>(it, "AlbedoGeom");
	//}

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

	Log_IS.Out("Object Loader", "INFO", "[" + _Obj.Name[0] + "] " + "Loading - Ocean parameters", ILogLevel, true);

	auto FoundOcean = it->SubTable->find("Ocean") != it->SubTable->end();
	_Obj.NoOcean = GetAs<bool>(it, "NoOcean") || !FoundOcean;
	if (!_Obj.NoOcean && FoundOcean)
	{
		auto Ocean = it->SubTable->find("Ocean");
		if (Ocean->SubTable->find("Height") != Ocean->SubTable->end())
		{
			_Obj.Ocean.Height = GetAs<float64>(Ocean, "Height");
		}
		else if (Ocean->SubTable->find("Depth") != Ocean->SubTable->end())
		{
			_Obj.Ocean.Height = GetAs<float64>(Ocean, "Depth");
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
		_Obj.Atmosphere.Height = GetAs<float64>(Atm, "Height");
		_Obj.Atmosphere.Density = GetAs<float64>(Atm, "Density");
		_Obj.Atmosphere.Adiabat = GetAs<float64>(Atm, "Adiabat");
		_Obj.Atmosphere.Pressure = GetAs<float64>(Atm, "Pressure");
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
		_Obj.AccretionDisk.Luminosity = GetAs<float64>(AccDisk, "Luminosity");
		_Obj.AccretionDisk.LumBol = GetAs<float64>(AccDisk, "LuminosityBol");

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

void AddEmptyTag(_SC table* _Table)
{
	_Table->push(_SC table::KeyValue());
}

void AddKeyValue(_SC table* _Table, string _Key, float64 _Value, _SC object_ostream::fmtflags _Fl, streamsize _Preci)
{
	Log_OS.Out("OSCStream", "INFO", "[Table] Adding Key : " + _Key, OLogLevel, true);
	if (!isinf(_Value))
	{
		_SC table::KeyValue _KV;
		_KV.Key = _Key;

		#if 0
		ostringstream ValueStr;
		if (_Fl & (1 << 15)) { ValueStr << fixed; }
		ValueStr << setprecision(_Preci) << _Value;
		_KV.Value = ValueStr.str();
		#endif

		ostringstream _FmtStr;
		_FmtStr << "{:";
		if (_Preci != 0) { _FmtStr << '.' << _Preci; }
		if (_Fl & (1 << 15)) { _FmtStr << 'f'; }
		else { _FmtStr << 'g'; }
		_FmtStr << '}';
		_KV.Value = vformat(_FmtStr.str(), make_format_args(_Value));

		_Table->push(_KV);
	}
	else { Log_OS.Out("OSCStream", "ERROR", "[Table] Failed to add : " + _Key + " because its value is invalid.", OLogLevel, true); }
}

template<typename genType> requires (vecType<genType> || vecIType<genType> || vecUType<genType>)
void AddKeyValue(_SC table* _Table, string _Key, genType _Value, _SC object_ostream::fmtflags _Fl, streamsize _Preci)
{
	Log_OS.Out("OSCStream", "INFO", "[Table] Adding Key : " + _Key, OLogLevel, true);
	if (!any(isinf(_Value)))
	{
		_SC table::KeyValue _KV;
		_KV.Key = _Key;

		#if 0
		ostringstream ValueStr;
		if (_Fl & (1 << 15)) { ValueStr << fixed; }
		ValueStr << setprecision(_Preci) << _Value;
		_KV.Value = ValueStr.str();
		#endif

		ostringstream _ValueStr;
		ostringstream _FmtStr;
		_FmtStr << "{:";
		if (_Preci != 0) { _FmtStr << '.' << _Preci; }
		if (_Fl & (1 << 15)) { _FmtStr << 'f'; }
		else { _FmtStr << 'g'; }
		_FmtStr << '}';
		_ValueStr << '(';
		for (size_t i = 0; i < _Value.size(); i++)
		{
			_ValueStr << vformat(_FmtStr.str(), make_format_args(_Value[i]));
			if (i != _Value.size() - 1) { _ValueStr << ", "; }
		}
		_ValueStr << ')';
		_KV.Value = _ValueStr.str();

		_Table->push(_KV);
	}
	else { Log_OS.Out("OSCStream", "ERROR", "[Table] Failed to add : " + _Key + " because its value is invalid.", OLogLevel); }
}

template<typename genType> requires convertible_to<genType, string>
void AddKeyValue(_SC table* _Table, string _Key, genType _Value, _SC object_ostream::fmtflags _Fl, streamsize _Preci)
{
	Log_OS.Out("OSCStream", "INFO", "[Table] Adding Key : " + _Key, OLogLevel);
	if (_Value!="None")
	{
		_SC table::KeyValue _KV;
		_KV.Key = _Key;
		_KV.Value = '\"' + _Value + '\"';
		_Table->push(_KV);
	}
	else { Log_OS.Out("OSCStream", "ERROR", "[Table] Failed to add : " + _Key + " because its value is invalid.", OLogLevel, true); }
}

void AddKeyValue(_SC table* _Table, string _Key, bool _Value, _SC object_ostream::fmtflags _Fl, streamsize _Preci)
{
	if (!(_Fl & (1 << 16)))
	{
		Log_OS.Out("OSCStream", "INFO", "[Table] Adding Key : " + _Key, OLogLevel);
		if (_Value)
		{
			_SC table::KeyValue _KV;
			_KV.Key = _Key;
			_KV.Value = "true";
			_Table->push(_KV);
		}
		else { Log_OS.Out("OSCStream", "ERROR", "[Table] Failed to add : " + _Key + " because its value is invalid.", OLogLevel, true); }
	}
}

void AddPTMatrix(_SC table* _Table, string _Unit, vector<_STD array<float64, 6>> PT, _SC object_ostream::fmtflags _Fl, streamsize _Preci, uint64 _WSpace)
{
	ostringstream ValueStr;
	if (_Fl & (1 << 15)) { ValueStr << _STD fixed; }
	ValueStr << setprecision(_Preci);
	Log_OS.Out("OSCStream", "INFO", "[Table] Adding Key : PeriodicTerms", OLogLevel);
	if (!PT.empty())
	{
		_SC table::KeyValue _KV;
		if (_Unit == "Diurnal") { _KV.Key = "PeriodicTermsDiurnal"; }
		else if (_Unit == "Secular") { _KV.Key = "PeriodicTermsSecular"; }
		for (size_t line = 0; line < PT.size(); line++)
		{
			for (size_t i = 0; i < _WSpace; i++) { ValueStr << '\t'; }
			// Due to some technical problems, The Precision of PeriodicTerms is Non-adjustable.
			ValueStr << _STD format
			(
				// PoleRA amp, PoleDec amp, PrimeMer amp, phase, freq, freq_rate
				"{:+9.6f}, {:+9.6f}, {:+11.8f}, {:10.6f}, {:+15.8f}, {}\n",
				PT[line][0], PT[line][1], PT[line][2], PT[line][3], PT[line][4], PT[line][5]
			);
		}
		_KV.Value = ValueStr.str();
		_Table->push(_KV);
	}
	else { Log_OS.Out("OSCStream", "ERROR", "[Table] Failed to add key because PeriodicTerms is empty.", OLogLevel, true); }
}

_SC_BEGIN

table::KeyValue MakeTable(object_ostream& _Os, Object Obj)
{
	using _SC table;
	table::KeyValue _KV;

	Log_OS.Out("OSCStream", "INFO", "[CSE Smart Output] Making table for object : " + Obj.Name[0], OLogLevel, true);

	_KV.Key = Obj.Type;
	_KV.Value += '\"';
	for (size_t i = 0; i < Obj.Name.size(); i++)
	{
		_KV.Value += Obj.Name[i];
		if (i < Obj.Name.size() - 1){_KV.Value += '/';}
	}
	_KV.Value += '\"';

	table Data;
	AddKeyValue(&Data, "ParentBody", Obj.ParentBody, _Os.flags(), _Os.precision());
	if (Obj.Type == "Star")
	{
		AddKeyValue(&Data, "Class", Obj.SpecClass, _Os.flags(), _Os.precision());
	}
	else { AddKeyValue(&Data, "Class", Obj.Class, _Os.flags(), _Os.precision()); }
	
	AddKeyValue(&Data, "MassKg", Obj.Mass, _Os.flags(), _Os.precision());

	if (!any(isinf(Obj.Dimensions)) && _Os.flags() & (1 << 18))
	{
		if (_Os.flags() & (1 << 19))
		{
			float64 Radius = max({ Obj.Dimensions.x, Obj.Dimensions.y, Obj.Dimensions.z }) / 2;
			vec3 Oblateness = vec3
			(
				(Radius - Obj.Dimensions.x / 2) / Radius,
				(Radius - Obj.Dimensions.y / 2) / Radius,
				(Radius - Obj.Dimensions.z / 2) / Radius
			);
			AddKeyValue(&Data, "Radius", Radius / 1000, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "Oblateness", Oblateness, _Os.flags(), _Os.precision());
		}
		else
		{
			AddKeyValue(&Data, "Dimensions", Obj.Dimensions / 1000., _Os.flags(), _Os.precision());
		}
	}
	else
	{
		if (Obj.Dimensions.x == Obj.Dimensions.y && Obj.Dimensions.y == Obj.Dimensions.z)
		{
			AddKeyValue(&Data, "Radius", Obj.Dimensions.x / 2000, _Os.flags(), _Os.precision());
		}
		else
		{
			AddKeyValue(&Data, "Dimensions", Obj.Dimensions / 1000., _Os.flags(), _Os.precision());
		}
	}

	if (_Os.flags() & (1 << 0))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Physical properties", OLogLevel, true);

		AddKeyValue(&Data, "AbsMagn", Obj.AbsMagn, _Os.flags(), _Os.precision());
		if (Obj.Type == "Star")
		{
			AddKeyValue(&Data, "Luminosity", Obj.Luminosity / SolarLum, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "LumBol", Obj.LumBol / SolarLum, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "Teff", Obj.Teff, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "FeH", Obj.FeH, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "CtoO", Obj.CtoO, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "Age", Obj.Age / 1E+9, _Os.flags(), _Os.precision());

			//if (IsStarRemnant(Obj.SpecClass))
			//{
			AddKeyValue(&Data, "KerrSpin", Obj.KerrSpin, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "KerrCharge", Obj.KerrCharge, _Os.flags(), _Os.precision());
			//}
		}
		else
		{
			AddKeyValue(&Data, "InertiaMoment", Obj.InertiaMoment, _Os.flags(), _Os.precision());
		}
	}

	if (_Os.flags() & (1 << 1) && Obj.Type != "Star")
	{
		AddKeyValue(&Data, "SlopeParam", Obj.SlopeParam, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "AlbedoBond", Obj.AlbedoBond, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "AlbedoGeom", Obj.AlbedoGeom, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "Brightness", Obj.Brightness, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "BrightnessReal", Obj.BrightnessReal, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "Color", Obj.Color, _Os.flags(), _Os.precision());

		AddKeyValue(&Data, "EndogenousHeating", Obj.EndogenousHeating, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "ThermalLuminosity", Obj.ThermalLuminosity, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "ThermalLuminosityBol", Obj.ThermalLuminosityBol, _Os.flags(), _Os.precision());
	}

	if (_Os.flags() & (1 << 2))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Rotation parameters", OLogLevel, true);

		if (_Os.flags() & (1 << 17) && Obj.TidalLocked)
		{
			AddKeyValue(&Data, "TidalLocked", Obj.TidalLocked, _Os.flags(), _Os.precision());
		}
		else
		{
			//AddEmptyTag(&Data);
			AddKeyValue(&Data, "RotationEpoch", Obj.Rotation.RotationEpoch, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "RotationOffset", Obj.Rotation.RotationOffset, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "RotationPeriod", Obj.Rotation.RotationPeriod / 3600., _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "Obliquity", Obj.Rotation.Obliquity, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "EqAscendNode", Obj.Rotation.EqAscendNode, _Os.flags(), _Os.precision());
			AddKeyValue(&Data, "Precession", Obj.Rotation.Precession, _Os.flags(), _Os.precision());

			if (Obj.RotationModel == "IAU")
			{
				table::KeyValue RModel;
				RModel.Key = "RotationModel";
				RModel.Value = "\"IAU\"";
				table IAU;
				AddKeyValue(&IAU, "Epoch", Obj.IAU.Epoch, _Os.flags(), _Os.precision());
				AddKeyValue(&IAU, "PoleRA", Obj.IAU.PoleRA, _Os.flags(), _Os.precision());
				AddKeyValue(&IAU, "PoleRARate", Obj.IAU.PoleRARate, _Os.flags(), _Os.precision());
				AddKeyValue(&IAU, "PoleDec", Obj.IAU.PoleDec, _Os.flags(), _Os.precision());
				AddKeyValue(&IAU, "PoleDecRate", Obj.IAU.PoleDecRate, _Os.flags(), _Os.precision());
				AddKeyValue(&IAU, "PrimeMeridian", Obj.IAU.PrimeMeridian, _Os.flags(), _Os.precision());
				AddKeyValue(&IAU, "RotationRate", Obj.IAU.RotationRate, _Os.flags(), _Os.precision());
				AddKeyValue(&IAU, "RotationAccel", Obj.IAU.RotationAccel, _Os.flags(), _Os.precision());
				AddPTMatrix(&IAU, Obj.IAU.PT, Obj.IAU.PeriodicTerms, _Os.flags(), _Os.precision(), 3);
				RModel.SubTable = make_shared<table>(IAU);
				Data.push(RModel);
			}
		}
	}

	Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Orbital parameters", OLogLevel, true);

	#if OBJ_DEFAULT_STATIC_FIXED_POS == 0
	if (!any(isinf(Obj.StaticPos))) { AddKeyValue(&Data, "StaticPosXYZ", Obj.StaticPos, _Os.flags(), _Os.precision()); }
	else if (!any(isinf(Obj.FixedPos))) { AddKeyValue(&Data, "FixedPosXYZ", Obj.FixedPos, _Os.flags(), _Os.precision()); }
	#elif OBJ_DEFAULT_STATIC_FIXED_POS == 1
	if (!any(isinf(Obj.StaticPos))) { AddKeyValue(&Data, "StaticPosPolar", Obj.StaticPos, _Os.flags(), _Os.precision()); }
	else if (!any(isinf(Obj.FixedPos))) { AddKeyValue(&Data, "FixedPosPolar", Obj.FixedPos, _Os.flags(), _Os.precision()); }
	#endif
	else if (Obj.Orbit.RefPlane != NO_DATA_STRING)
	{
		table::KeyValue Orbit;
		table _OrbitParams;

		AddKeyValue(&_OrbitParams, "AnalyticModel", Obj.Orbit.AnalyticModel, _Os.flags(), _Os.precision());
		AddKeyValue(&_OrbitParams, "RefPlane", Obj.Orbit.RefPlane, _Os.flags(), _Os.precision());
		AddKeyValue(&_OrbitParams, "Epoch", Obj.Orbit.Epoch, _Os.flags(), _Os.precision());
		AddKeyValue(&_OrbitParams, "GravParam", Obj.Orbit.GravParam, _Os.flags(), _Os.precision());

		if (Obj.Orbit.Binary)
		{ 
			Orbit.Key = "BinaryOrbit";
			AddKeyValue(&_OrbitParams, "Separation", Obj.Orbit.Separation / AU, _Os.flags(), _Os.precision());
			AddKeyValue(&_OrbitParams, "PositionAngle", Obj.Orbit.PositionAngle, _Os.flags(), _Os.precision());
		}
		else{Orbit.Key = "Orbit";}

		// Smart orbit elements output
		// Reference: https://en.wikipedia.org/wiki/Orbital_elements
		// Object	            Elements used
		// Major planet	        e, a, i, Ω, ϖ, L0
		// Comet	            e, q, i, Ω, ω, T0
		// Asteroid	            e, a, i, Ω, ω, M0
		// Two - line elements	e, i, Ω, ω, n, M0

		if (_Os.flags() & (1 << 14))
		{
			if (Obj.Type == "Planet")
			{
				AddKeyValue(&_OrbitParams, "Eccentricity", Obj.Orbit.Eccentricity, _Os.flags(), _Os.precision());
				if (Obj.Orbit.Eccentricity < 1)
				{
					AddKeyValue(&_OrbitParams, "SemiMajorAxisKm", (Obj.Orbit.PericenterDist / (1. - Obj.Orbit.Eccentricity)) / 1000., _Os.flags(), _Os.precision());
				}
				else
				{
					AddKeyValue(&_OrbitParams, "PericenterDist", Obj.Orbit.PericenterDist / AU, _Os.flags(), _Os.precision());
				}
				AddKeyValue(&_OrbitParams, "Inclination", Obj.Orbit.Inclination, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "AscendingNode", Obj.Orbit.AscendingNode, _Os.flags(), _Os.precision());
				float64 LongOfPericen, MeanLongitude;
				if (isinf(Obj.Orbit.ArgOfPericenter)) { LongOfPericen = Obj.Orbit.AscendingNode; }
				else { LongOfPericen = Obj.Orbit.AscendingNode + Obj.Orbit.ArgOfPericenter; }
				if (isinf(Obj.Orbit.MeanAnomaly)) { MeanLongitude = LongOfPericen; }
				else { MeanLongitude = LongOfPericen + Obj.Orbit.MeanAnomaly; }
				AddKeyValue(&_OrbitParams, "LongOfPericen", LongOfPericen, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "MeanLongitude", MeanLongitude, _Os.flags(), _Os.precision());
			}
			else if (Obj.Type == "Comet")
			{
				AddKeyValue(&_OrbitParams, "Eccentricity", Obj.Orbit.Eccentricity, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "PericenterDist", Obj.Orbit.PericenterDist / AU, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "Inclination", Obj.Orbit.Inclination, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "AscendingNode", Obj.Orbit.AscendingNode, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "ArgOfPericenter", Obj.Orbit.ArgOfPericenter, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "MeanAnomaly", Obj.Orbit.MeanAnomaly, _Os.flags(), _Os.precision());
			}
			else if (Obj.Type == "Asteroid")
			{
				AddKeyValue(&_OrbitParams, "Eccentricity", Obj.Orbit.Eccentricity, _Os.flags(), _Os.precision());
				if (Obj.Orbit.Eccentricity < 1)
				{
					AddKeyValue(&_OrbitParams, "SemiMajorAxisKm", (Obj.Orbit.PericenterDist / (1. - Obj.Orbit.Eccentricity)) / 1000., _Os.flags(), _Os.precision());
				}
				else
				{
					AddKeyValue(&_OrbitParams, "PericenterDist", Obj.Orbit.PericenterDist / AU, _Os.flags(), _Os.precision());
				}
				AddKeyValue(&_OrbitParams, "Inclination", Obj.Orbit.Inclination, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "AscendingNode", Obj.Orbit.AscendingNode, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "ArgOfPericenter", Obj.Orbit.ArgOfPericenter, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "MeanAnomaly", Obj.Orbit.MeanAnomaly, _Os.flags(), _Os.precision());
			}
			else if (Obj.Type == "Spacecraft" || Obj.Type == "Structure" || Obj.Type == "Artificial")
			{
				AddKeyValue(&_OrbitParams, "Eccentricity", Obj.Orbit.Eccentricity, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "Inclination", Obj.Orbit.Inclination, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "AscendingNode", Obj.Orbit.AscendingNode, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "ArgOfPericenter", Obj.Orbit.ArgOfPericenter, _Os.flags(), _Os.precision());
				if (Obj.Orbit.Eccentricity < 1 && !isinf(Obj.Orbit.Period))
				{
					AddKeyValue(&_OrbitParams, "MeanMotion", 360. / (Obj.Orbit.Period / SynodicDay), _Os.flags(), _Os.precision());
				}
				else
				{
					AddKeyValue(&_OrbitParams, "PericenterDist", Obj.Orbit.PericenterDist / AU, _Os.flags(), _Os.precision());
				}
				AddKeyValue(&_OrbitParams, "MeanAnomaly", Obj.Orbit.MeanAnomaly, _Os.flags(), _Os.precision());
			}
			else
			{
				AddKeyValue(&_OrbitParams, "Eccentricity", Obj.Orbit.Eccentricity, _Os.flags(), _Os.precision());
				if (Obj.Orbit.Eccentricity < 1)
				{
					if (!isinf(Obj.Orbit.Period))
					{
						AddKeyValue(&_OrbitParams, "PeriodDays", Obj.Orbit.Period / SynodicDay, _Os.flags(), _Os.precision());
					}
					if (!isinf(Obj.Orbit.PericenterDist))
					{
						AddKeyValue(&_OrbitParams, "SemiMajorAxisKm", (Obj.Orbit.PericenterDist / (1. - Obj.Orbit.Eccentricity)) / 1000., _Os.flags(), _Os.precision());
					}
				}
				else
				{
					AddKeyValue(&_OrbitParams, "PericenterDist", Obj.Orbit.PericenterDist / AU, _Os.flags(), _Os.precision());
				}
				AddKeyValue(&_OrbitParams, "Inclination", Obj.Orbit.Inclination, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "AscendingNode", Obj.Orbit.AscendingNode, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "ArgOfPericenter", Obj.Orbit.ArgOfPericenter, _Os.flags(), _Os.precision());
				AddKeyValue(&_OrbitParams, "MeanAnomaly", Obj.Orbit.MeanAnomaly, _Os.flags(), _Os.precision());
			}
		}

		else
		{
			// SpaceEngine Original style output
			if (Obj.Orbit.Eccentricity < 1 && !isinf(Obj.Orbit.Period))
			{
				AddKeyValue(&_OrbitParams, "PeriodDays", Obj.Orbit.Period / SynodicDay, _Os.flags(), _Os.precision());
			}
			
			if (Obj.Orbit.Eccentricity < 1 && !isinf(Obj.Orbit.PericenterDist))
			{
				AddKeyValue(&_OrbitParams, "SemiMajorAxisKm", (Obj.Orbit.PericenterDist / (1. - Obj.Orbit.Eccentricity)) / 1000., _Os.flags(), _Os.precision());
			}
			else
			{
				AddKeyValue(&_OrbitParams, "PericenterDist", Obj.Orbit.PericenterDist / AU, _Os.flags(), _Os.precision());
			}
			AddKeyValue(&_OrbitParams, "Eccentricity", Obj.Orbit.Eccentricity, _Os.flags(), _Os.precision());
			AddKeyValue(&_OrbitParams, "Inclination", Obj.Orbit.Inclination, _Os.flags(), _Os.precision());
			AddKeyValue(&_OrbitParams, "AscendingNode", Obj.Orbit.AscendingNode, _Os.flags(), _Os.precision());
			AddKeyValue(&_OrbitParams, "ArgOfPericenter", Obj.Orbit.ArgOfPericenter, _Os.flags(), _Os.precision());
			AddKeyValue(&_OrbitParams, "MeanAnomaly", Obj.Orbit.MeanAnomaly, _Os.flags(), _Os.precision());
		}

		Orbit.SubTable = make_shared<table>(_OrbitParams);
		Data.push(Orbit);
	}

	if (_Os.flags() & (1 << 3))
	{
		if (Obj.LifeCount != 0)
		{
			Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Life parameters", OLogLevel, true);
		}
		for (size_t i = 0; i < Obj.LifeCount; i++)
		{
			table::KeyValue Life;
			Life.Key = "Life";
			table _LifeParams;
			AddKeyValue(&_LifeParams, "Class", Obj.Life[i].Class, _Os.flags(), _Os.precision());
			AddKeyValue(&_LifeParams, "Type", Obj.Life[i].Type, _Os.flags(), _Os.precision());
			ostringstream _Biomes;
			for (size_t j = 0; j < Obj.Life[i].Biome.size(); j++)
			{
				_Biomes << Obj.Life[i].Biome[j];
				if (j != Obj.Life[i].Biome.size() - 1) { _Biomes << '/'; }
			}
			AddKeyValue(&_LifeParams, "Biome", _Biomes.str(), _Os.flags(), _Os.precision());
			AddKeyValue(&_LifeParams, "Panspermia", Obj.Life[i].Panspermia, _Os.flags(), _Os.precision());
			Life.SubTable = make_shared<table>(_LifeParams);
			Data.push(Life);
		}
	}

	if (_Os.flags() & (1 << 4) && !Obj.Interior.empty())
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Interior", OLogLevel, true);
		table::KeyValue Interior;
		Interior.Key = "Interior";
		table _Interior;
		table::KeyValue Composition;
		Composition.Key = "Composition";
		table _Composition;
		auto it = Obj.Interior.begin();
		auto end = Obj.Interior.end();

		while (it != end)
		{
			AddKeyValue(&_Composition, it->first, it->second, _Os.flags(), _Os.precision());
			++it;
		}

		Composition.SubTable = make_shared<table>(_Composition);
		_Interior.push(Composition);
		Interior.SubTable = make_shared<table>(_Interior);
		Data.push(Interior);
	}

	if (Obj.EnableSurface && _Os.flags() & (1 << 5))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Landscape parameters", OLogLevel, true);
		table::KeyValue Surface;
		Surface.Key = "Surface";
		table _Surface;

		AddKeyValue(&_Surface, "DiffMap", Obj.Surface.DiffMap, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "DiffMapAlpha", Obj.Surface.DiffMapAlpha, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "BumpMap", Obj.Surface.BumpMap, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "BumpHeight", Obj.Surface.BumpHeight, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "BumpOffset", Obj.Surface.BumpOffset, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "GlowMap", Obj.Surface.GlowMap, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "GlowMode", Obj.Surface.GlowMode, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "GlowColor", Obj.Surface.GlowColor, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "GlowBright", Obj.Surface.GlowBright, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SpecMap", Obj.Surface.SpecMap, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "RoughnessBias", Obj.Surface.RoughnessBias, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "RoughnessScale", Obj.Surface.RoughnessScale, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SpecBrightIce", Obj.Surface.SpecParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SpecBrightWater", Obj.Surface.SpecParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SpecPowerIce", Obj.Surface.SpecParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SpecPowerWater", Obj.Surface.SpecParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SpecularScale", Obj.Surface.SpecularScale, _Os.flags(), _Os.precision());

		//AddEmptyTag(&_Surface);

		AddKeyValue(&_Surface, "Preset", Obj.Surface.Preset, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SurfStyle", Obj.Surface.SurfStyle, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "Randomize", Obj.Surface.Randomize, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "colorDistMagn", Obj.Surface.colorParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "colorDistFreq", Obj.Surface.colorParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "icecapLatitude", Obj.Surface.colorParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "tropicLatitude", Obj.Surface.colorParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "climatePole", Obj.Surface.climateParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "climateTropic", Obj.Surface.climateParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "climateEquator", Obj.Surface.climateParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "tropicWidth", Obj.Surface.climateParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "climateGrassMin", Obj.Surface.plantsParams1.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "climateGrassMax", Obj.Surface.plantsParams1.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "climateForestMin", Obj.Surface.plantsParams1.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "climateForestMax", Obj.Surface.plantsParams1.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "climateSteppeMin", Obj.Surface.plantsParams2.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "climateSteppeMax", Obj.Surface.plantsParams2.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "humidity", Obj.Surface.plantsParams2.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "plantsBiomeOffset", Obj.Surface.plantsParams2.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "mainFreq", Obj.Surface.mainParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "terraceProb", Obj.Surface.mainParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "oceanType", Obj.Surface.mainParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "tidalLock", Obj.Surface.mainParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "detailScale", Obj.Surface.textureParams.x, _Os.flags(), _Os.precision());
		//AddKeyValue(&_Surface, "", Obj.Surface.textureParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "venusMagn", Obj.Surface.textureParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "venusFreq", Obj.Surface.textureParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "seaLevel", Obj.Surface.mareParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "mareFreq", Obj.Surface.mareParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "mareDensity", Obj.Surface.mareParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "icecapHeight", Obj.Surface.mareParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "montesMagn", Obj.Surface.montesParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "montesFreq", Obj.Surface.montesParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "montesFraction", Obj.Surface.montesParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "montesSpiky", Obj.Surface.montesParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "dunesMagn", Obj.Surface.dunesParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "dunesFreq", Obj.Surface.dunesParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "dunesDensity", Obj.Surface.dunesParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "drivenDarkening", Obj.Surface.dunesParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "hillsMagn", Obj.Surface.hillsParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "hillsFreq", Obj.Surface.hillsParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "hillsFraction", Obj.Surface.hillsParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "hills2Fraction", Obj.Surface.hillsParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "riversMagn", Obj.Surface.riversParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "riversFreq", Obj.Surface.riversParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "riversSin", Obj.Surface.riversParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "beachWidth", Obj.Surface.riversParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "riftsMagn", Obj.Surface.riftsParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "riftsFreq", Obj.Surface.riftsParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "riftsSin", Obj.Surface.riftsParams.z, _Os.flags(), _Os.precision());
		//AddKeyValue(&_Surface, "", Obj.Surface.riftsParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "canyonsMagn", Obj.Surface.canyonsParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "canyonsFreq", Obj.Surface.canyonsParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "canyonsFraction", Obj.Surface.canyonsParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "erosion", Obj.Surface.canyonsParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cracksMagn", Obj.Surface.cracksParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cracksFreq", Obj.Surface.cracksParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cracksOctaves", Obj.Surface.cracksParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "craterRayedFactor", Obj.Surface.cracksParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "craterMagn", Obj.Surface.craterParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "craterFreq", Obj.Surface.craterParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "craterDensity", Obj.Surface.craterParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "craterOctaves", Obj.Surface.craterParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "volcanoMagn", Obj.Surface.volcanoParams1.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "volcanoFreq", Obj.Surface.volcanoParams1.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "volcanoDensity", Obj.Surface.volcanoParams1.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "volcanoOctaves", Obj.Surface.volcanoParams1.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "volcanoActivity", Obj.Surface.volcanoParams2.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "volcanoFlows", Obj.Surface.volcanoParams2.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "volcanoRadius", Obj.Surface.volcanoParams2.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "volcanoTemp", Obj.Surface.volcanoParams2.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "lavaCoverage", Obj.Surface.lavaParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "snowLevel", Obj.Surface.lavaParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "surfTemperature", Obj.Surface.lavaParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "heightTempGrad", Obj.Surface.lavaParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "lavaCoverTidal", Obj.Surface.lavaDetails.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "lavaCoverSun", Obj.Surface.lavaDetails.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "lavaCoverYoung", Obj.Surface.lavaDetails.z, _Os.flags(), _Os.precision());
		//AddKeyValue(&_Surface, "", Obj.Surface.lavaDetails.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cycloneMagn", Obj.Surface.cycloneParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cycloneFreq", Obj.Surface.cycloneParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cycloneDensity", Obj.Surface.cycloneParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cycloneOctaves", Obj.Surface.cycloneParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cyclone2Magn", Obj.Surface.cycloneParams2.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cyclone2Freq", Obj.Surface.cycloneParams2.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cyclone2Density", Obj.Surface.cycloneParams2.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "cyclone2Octaves", Obj.Surface.cycloneParams2.w, _Os.flags(), _Os.precision());

		//AddEmptyTag(&_Surface);
		
		AddKeyValue(&_Surface, "Hapke", Obj.Surface.Hapke, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SpotBright", Obj.Surface.HapkeParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SpotWidth", Obj.Surface.HapkeParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SpotBrightCB", Obj.Surface.HapkeParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "SpotWidthCB", Obj.Surface.HapkeParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "DayAmbient", Obj.Surface.DayAmbient, _Os.flags(), _Os.precision());
		AddKeyValue(&_Surface, "ModulateColor", Obj.Surface.ModulateColor, _Os.flags(), _Os.precision());

		Surface.SubTable = make_shared<table>(_Surface);
		Data.push(Surface);
	}

	AddKeyValue(&Data, "NoOcean", Obj.NoOcean, _Os.flags(), _Os.precision());
	if (!Obj.NoOcean && _Os.flags() & (1 << 6))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Ocean parameters", OLogLevel, true);
		table::KeyValue Ocean;
		Ocean.Key = "Ocean";
		table _Ocean;

		AddKeyValue(&_Ocean, "Height", Obj.Ocean.Height, _Os.flags(), _Os.precision());
		AddKeyValue(&_Ocean, "Hapke", Obj.Ocean.Hapke, _Os.flags(), _Os.precision());
		AddKeyValue(&_Ocean, "SpotBright", Obj.Ocean.HapkeParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Ocean, "SpotWidth", Obj.Ocean.HapkeParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Ocean, "SpotBrightCB", Obj.Ocean.HapkeParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Ocean, "SpotWidthCB", Obj.Ocean.HapkeParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Ocean, "DayAmbient", Obj.Ocean.DayAmbient, _Os.flags(), _Os.precision());
		AddKeyValue(&_Ocean, "ModulateBright", Obj.Ocean.ModulateBright, _Os.flags(), _Os.precision());

		if (!Obj.Ocean.Composition.empty())
		{
			table::KeyValue Composition;
			Composition.Key = "Composition";
			table _Composition;
			auto it = Obj.Ocean.Composition.begin();
			auto end = Obj.Ocean.Composition.end();

			while (it != end)
			{
				AddKeyValue(&_Composition, it->first, it->second, _Os.flags(), _Os.precision());
				++it;
			}

			Composition.SubTable = make_shared<table>(_Composition);
			_Ocean.push(Composition);
		}

		Ocean.SubTable = make_shared<table>(_Ocean);
		Data.push(Ocean);
	}

	AddKeyValue(&Data, "NoClouds", Obj.NoClouds, _Os.flags(), _Os.precision());
	if (!Obj.NoClouds && _Os.flags() & (1 << 7))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Clouds parameters", OLogLevel, true);
		for (size_t i = 0; i < Obj.Clouds.Layer.size(); i++)
		{
			table::KeyValue Clouds;
			Clouds.Key = "Clouds";
			table _Clouds;

			AddKeyValue(&_Clouds, "DiffMap", Obj.Clouds.Layer[i].DiffMap, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "BumpMap", Obj.Clouds.Layer[i].BumpMap, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "Height", Obj.Clouds.Layer[i].Height, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "Velocity", Obj.Clouds.Layer[i].Velocity, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "BumpHeight", Obj.Clouds.Layer[i].BumpHeight, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "BumpOffset", Obj.Clouds.Layer[i].BumpOffset, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "Hapke", Obj.Clouds.Layer[i].Hapke, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "SpotBright", Obj.Clouds.Layer[i].HapkeParams.x, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "SpotWidth", Obj.Clouds.Layer[i].HapkeParams.y, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "SpotBrightCB", Obj.Clouds.Layer[i].HapkeParams.z, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "SpotWidthCB", Obj.Clouds.Layer[i].HapkeParams.w, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "DayAmbient", Obj.Clouds.Layer[i].DayAmbient, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "ModulateColor", vec3(Obj.Clouds.Layer[i].ModulateColor.x, Obj.Clouds.Layer[i].ModulateColor.y, Obj.Clouds.Layer[i].ModulateColor.z), _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "Opacity", Obj.Clouds.Layer[i].ModulateColor.w, _Os.flags(), _Os.precision());
			AddKeyValue(&_Clouds, "ModulateBright", Obj.Clouds.Layer[i].ModulateBright, _Os.flags(), _Os.precision());

			if (i == 0)
			{
				AddKeyValue(&_Clouds, "mainFreq", Obj.Clouds.cloudsParams1.x, _Os.flags(), _Os.precision());
				AddKeyValue(&_Clouds, "mainOctaves", Obj.Clouds.cloudsParams1.y, _Os.flags(), _Os.precision());
				AddKeyValue(&_Clouds, "stripeZones", Obj.Clouds.cloudsParams1.z, _Os.flags(), _Os.precision());
				AddKeyValue(&_Clouds, "stripeTwist", Obj.Clouds.cloudsParams1.w, _Os.flags(), _Os.precision());
				//AddKeyValue(&_Clouds, "cloudsLayer", Obj.Clouds.cloudsParams2.x, _Os.flags(), _Os.precision());
				//AddKeyValue(&_Clouds, "cloudsNLayers", Obj.Clouds.cloudsParams2.y, _Os.flags(), _Os.precision());
				AddKeyValue(&_Clouds, "stripeFluct", Obj.Clouds.cloudsParams2.z, _Os.flags(), _Os.precision());
				AddKeyValue(&_Clouds, "Coverage", Obj.Clouds.cloudsParams2.w, _Os.flags(), _Os.precision());
				AddKeyValue(&_Clouds, "RingsWinter", Obj.Surface.RingsWinter, _Os.flags(), _Os.precision());
			}

			Clouds.SubTable = make_shared<table>(_Clouds);
			Data.push(Clouds);
		}
	}

	AddKeyValue(&Data, "NoAtmosphere", Obj.NoAtmosphere, _Os.flags(), _Os.precision());
	if (!Obj.NoAtmosphere && _Os.flags() & (1 << 8))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Atmosphere parameters", OLogLevel, true);
		table::KeyValue Atmosphere;
		Atmosphere.Key = "Atmosphere";
		table _Atmosphere;

		AddKeyValue(&_Atmosphere, "Model", Obj.Atmosphere.Model, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Height", Obj.Atmosphere.Height, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Density", Obj.Atmosphere.Density, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Adiabat", Obj.Atmosphere.Adiabat, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Pressure", Obj.Atmosphere.Pressure, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Greenhouse", Obj.Atmosphere.Greenhouse, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Bright", Obj.Atmosphere.Bright, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Opacity", Obj.Atmosphere.Opacity, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "SkyLight", Obj.Atmosphere.SkyLight, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Hue", Obj.Atmosphere.Hue, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Saturation", Obj.Atmosphere.Saturation, _Os.flags(), _Os.precision());

		if (!Obj.Atmosphere.Composition.empty())
		{
			table::KeyValue Composition;
			Composition.Key = "Composition";
			table _Composition;
			auto it = Obj.Atmosphere.Composition.begin();
			auto end = Obj.Atmosphere.Composition.end();

			while (it != end)
			{
				AddKeyValue(&_Composition, it->first, it->second, _Os.flags(), _Os.precision());
				++it;
			}

			Composition.SubTable = make_shared<table>(_Composition);
			_Atmosphere.push(Composition);
		}

		Atmosphere.SubTable = make_shared<table>(_Atmosphere);
		Data.push(Atmosphere);
	}

	AddKeyValue(&Data, "NoAurora", Obj.NoAurora, _Os.flags(), _Os.precision());
	if (!Obj.NoAurora && _Os.flags() & (1 << 9))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Aurora parameters", OLogLevel, true);
		table::KeyValue Aurora;
		Aurora.Key = "Aurora";
		table _Aurora;

		AddKeyValue(&_Aurora, "Height", Obj.Aurora.Height, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "NorthLat", Obj.Aurora.North.Lat, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "NorthLon", Obj.Aurora.North.Lon, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "NorthRadius", Obj.Aurora.North.Radius, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "NorthWidth", Obj.Aurora.North.Width, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "NorthRings", Obj.Aurora.North.Rings, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "NorthBright", Obj.Aurora.North.Bright, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "NorthFlashFreq", Obj.Aurora.North.FlashFreq, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "NorthMoveSpeed", Obj.Aurora.North.MoveSpeed, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "NorthParticles", Obj.Aurora.North.Particles, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "SouthLat", Obj.Aurora.South.Lat, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "SouthLon", Obj.Aurora.South.Lon, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "SouthRadius", Obj.Aurora.South.Radius, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "SouthWidth", Obj.Aurora.South.Width, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "SouthRings", Obj.Aurora.South.Rings, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "SouthBright", Obj.Aurora.South.Bright, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "SouthFlashFreq", Obj.Aurora.South.FlashFreq, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "SouthMoveSpeed", Obj.Aurora.South.MoveSpeed, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "SouthParticles", Obj.Aurora.South.Particles, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "TopColor", Obj.Aurora.TopColor, _Os.flags(), _Os.precision());
		AddKeyValue(&_Aurora, "BottomColor", Obj.Aurora.BottomColor, _Os.flags(), _Os.precision());

		Aurora.SubTable = make_shared<table>(_Aurora);
		Data.push(Aurora);
	}

	AddKeyValue(&Data, "NoRings", Obj.NoRings, _Os.flags(), _Os.precision());
	if (!Obj.NoRings && _Os.flags() & (1 << 10))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Rings parameters", OLogLevel, true);
		table::KeyValue Rings;
		Rings.Key = "Rings";
		table _Rings;

		AddKeyValue(&_Rings, "Texture", Obj.Rings.Texture, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "InnerRadius", Obj.Rings.Radiuses.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "OuterRadius", Obj.Rings.Radiuses.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "EdgeRadius", Obj.Rings.Radiuses.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "MeanRadius", Obj.Rings.Radiuses.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "Thickness", Obj.Rings.Thickness, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "RocksMaxSize", Obj.Rings.RockParam.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "RocksSpacing", Obj.Rings.RockParam.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "DustDrawDist", Obj.Rings.RockParam.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "ChartRadius", Obj.Rings.ChartRadius, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "RotationPeriod", Obj.Rings.RotationPeriod, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "Brightness", Obj.Rings.Brightness.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "FrontBright", Obj.Rings.Brightness.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "BackBright", Obj.Rings.Brightness.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "Density", Obj.Rings.ShadowParam.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "Opacity", Obj.Rings.ShadowParam.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "SelfShadow", Obj.Rings.ShadowParam.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "PlanetShadow", Obj.Rings.ShadowParam.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "Hapke", Obj.Rings.Hapke, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "SpotBright", Obj.Rings.HapkeParams.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "SpotWidth", Obj.Rings.HapkeParams.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "SpotBrightCB", Obj.Rings.HapkeParams.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "SpotWidthCB", Obj.Rings.HapkeParams.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "frequency", Obj.Rings.Shape.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "densityScale", Obj.Rings.Shape.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "densityOffset", Obj.Rings.Shape.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "densityPower", Obj.Rings.Shape.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "colorContrast", Obj.Rings.colorContrast, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "FrontColor", Obj.Rings.FrontColor, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "BackThickColor", Obj.Rings.BackThickColor, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "BackIceColor", Obj.Rings.BackIceColor, _Os.flags(), _Os.precision());
		AddKeyValue(&_Rings, "BackDustColor", Obj.Rings.BackDustColor, _Os.flags(), _Os.precision());

		Rings.SubTable = make_shared<table>(_Rings);
		Data.push(Rings);
	}

	AddKeyValue(&Data, "NoAccretionDisk", Obj.NoAccretionDisk, _Os.flags(), _Os.precision());
	if (!Obj.NoAccretionDisk && _Os.flags() & (1 << 11))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Accretion Disk parameters", OLogLevel, true);
		table::KeyValue AccretionDisk;
		AccretionDisk.Key = "AccretionDisk";
		table _AccretionDisk;

		AddKeyValue(&_AccretionDisk, "InnerRadiusKm", Obj.AccretionDisk.Radiuses.x / 1000., _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "OuterRadiusKm", Obj.AccretionDisk.Radiuses.y / 1000., _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "BlackHoleRg", Obj.AccretionDisk.Radiuses.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "GravLensScale", Obj.AccretionDisk.Radiuses.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "Temperature", Obj.AccretionDisk.DiskParams1.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "TwistMagn", Obj.AccretionDisk.DiskParams1.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "StarAnimationTime", Obj.AccretionDisk.DiskParams1.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "DiskAnimationTime", Obj.AccretionDisk.DiskParams1.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "TemperatureShift", Obj.AccretionDisk.DiskParams2.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "Brightness", Obj.AccretionDisk.DiskParams2.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "Opacity", Obj.AccretionDisk.DiskParams2.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "StarRotationAngle", Obj.AccretionDisk.DiskParams2.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "InnerThicknessKm", Obj.AccretionDisk.DiskParams3.x / 1000., _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "OuterThicknessKm", Obj.AccretionDisk.DiskParams3.y / 1000., _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "DetailScaleR", Obj.AccretionDisk.DiskParams3.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "DetailScaleV", Obj.AccretionDisk.DiskParams3.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "ThicknessPow", Obj.AccretionDisk.ThicknessPow, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "AccretionRate", Obj.AccretionDisk.AccretionRate, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "Density", Obj.AccretionDisk.Density, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "Luminosity", Obj.AccretionDisk.Luminosity, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "LuminosityBol", Obj.AccretionDisk.LumBol, _Os.flags(), _Os.precision());

		AddKeyValue(&_AccretionDisk, "OctaveDistortionX", Obj.AccretionDisk.DiskParams4.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "OctaveDistortionY", Obj.AccretionDisk.DiskParams4.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "OctaveDistortionZ", Obj.AccretionDisk.DiskParams4.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "OctaveScale", Obj.AccretionDisk.DiskParams4.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "DiskNoiseContrast", Obj.AccretionDisk.DiskParams5.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "DiskTempContrast", Obj.AccretionDisk.DiskParams5.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "DiskOuterSpin", Obj.AccretionDisk.DiskParams5.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "LightingBright", Obj.AccretionDisk.LightingBright, _Os.flags(), _Os.precision());

		AddKeyValue(&_AccretionDisk, "JetStartRadiusKm", Obj.AccretionDisk.Jet.JetParams2.x / 1000., _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "JetEndRadiusKm", Obj.AccretionDisk.Jet.JetParams2.y / 1000., _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "JetLengthKm", Obj.AccretionDisk.Jet.JetParams2.z / 1000., _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "JetVelocity", Obj.AccretionDisk.Jet.JetParams2.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "JetStartTemp", Obj.AccretionDisk.Jet.JetParams1.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "JetEndTemp", Obj.AccretionDisk.Jet.JetParams1.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "ShadowContrast", Obj.AccretionDisk.Jet.JetParams1.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "ShadowLength", Obj.AccretionDisk.Jet.JetParams1.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "JetBrightness", Obj.AccretionDisk.Jet.JetParams3.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "JetDensity", Obj.AccretionDisk.Jet.JetParams3.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "JetDistortion", Obj.AccretionDisk.Jet.JetParams3.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_AccretionDisk, "JetThickness", Obj.AccretionDisk.Jet.JetParams3.w, _Os.flags(), _Os.precision());

		AccretionDisk.SubTable = make_shared<table>(_AccretionDisk);
		Data.push(AccretionDisk);
	}

	AddKeyValue(&Data, "NoCorona", Obj.NoCorona, _Os.flags(), _Os.precision());
	if (Obj.Type == "Star" && !Obj.NoCorona && _Os.flags() & (1 << 12))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Star Corona parameters", OLogLevel, true);
		table::KeyValue Corona;
		Corona.Key = "Corona";
		table _Corona;

		AddKeyValue(&_Corona, "RayDensity", Obj.Corona.Shape.x, _Os.flags(), _Os.precision());
		AddKeyValue(&_Corona, "RayCurv", Obj.Corona.Shape.y, _Os.flags(), _Os.precision());
		AddKeyValue(&_Corona, "Radius", Obj.Corona.Shape.z, _Os.flags(), _Os.precision());
		AddKeyValue(&_Corona, "SinFreq", Obj.Corona.Shape.w, _Os.flags(), _Os.precision());
		AddKeyValue(&_Corona, "Period", Obj.Corona.Period, _Os.flags(), _Os.precision());
		AddKeyValue(&_Corona, "Brightness", Obj.Corona.Bright, _Os.flags(), _Os.precision());

		Corona.SubTable = make_shared<table>(_Corona);
		Data.push(Corona);
	}

	AddKeyValue(&Data, "NoCometTail", Obj.NoCometTail, _Os.flags(), _Os.precision());
	if (!Obj.NoCometTail && _Os.flags() & (1 << 13))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Comet Tail parameters", OLogLevel, true);
		table::KeyValue CometTail;
		CometTail.Key = "CometTail";
		table _CometTail;

		AddKeyValue(&_CometTail, "MaxLength", Obj.CometTail.MaxLength, _Os.flags(), _Os.precision());
		AddKeyValue(&_CometTail, "GasToDust", Obj.CometTail.GasToDust, _Os.flags(), _Os.precision());
		AddKeyValue(&_CometTail, "Particles", Obj.CometTail.Particles, _Os.flags(), _Os.precision());
		AddKeyValue(&_CometTail, "GasBright", Obj.CometTail.GasBright, _Os.flags(), _Os.precision());
		AddKeyValue(&_CometTail, "DustBright", Obj.CometTail.DustBright, _Os.flags(), _Os.precision());
		AddKeyValue(&_CometTail, "GasColor", Obj.CometTail.GasColor, _Os.flags(), _Os.precision());
		AddKeyValue(&_CometTail, "DustColor", Obj.CometTail.DustColor, _Os.flags(), _Os.precision());

		CometTail.SubTable = make_shared<table>(_CometTail);
		Data.push(CometTail);
	}

	_KV.SubTable = make_shared<table>(Data);
	return _KV;
}

_SC_END

_CSE_END