#include "..\headers\stream\ostream.h"

#define _USE_CSE_DEFINES

#include "..\headers\Core\ConstLists.h"

using namespace std;

_CSE_BEGIN

namespace sc
{
	const typename OSCManip::_Fmtflags OSCManip::Physics;
	const typename OSCManip::_Fmtflags OSCManip::Optical;
	const typename OSCManip::_Fmtflags OSCManip::Life;
	const typename OSCManip::_Fmtflags OSCManip::Interior;
	const typename OSCManip::_Fmtflags OSCManip::Surface;
	const typename OSCManip::_Fmtflags OSCManip::Ocean;
	const typename OSCManip::_Fmtflags OSCManip::Clouds;
	const typename OSCManip::_Fmtflags OSCManip::Atmosphere;
	const typename OSCManip::_Fmtflags OSCManip::Aurora;
	const typename OSCManip::_Fmtflags OSCManip::Rings;
	const typename OSCManip::_Fmtflags OSCManip::AccDisk;
	const typename OSCManip::_Fmtflags OSCManip::Corona;
	const typename OSCManip::_Fmtflags OSCManip::CometTail;

	const typename OSCManip::_Fmtflags OSCManip::RotationAuto;
	const typename OSCManip::_Fmtflags OSCManip::RotationSimple;
	const typename OSCManip::_Fmtflags OSCManip::RotationIAU;
	const typename OSCManip::_Fmtflags OSCManip::RotationMoonDE;
	const typename OSCManip::_Fmtflags OSCManip::OrbitAuto;
	const typename OSCManip::_Fmtflags OSCManip::OrbitDefault; // e, a, i, Ω, ω, M0
	const typename OSCManip::_Fmtflags OSCManip::OrbitPlanet;  // e, a, i, Ω, ϖ, L0
	const typename OSCManip::_Fmtflags OSCManip::OrbitComet;   // e, q, i, Ω, ω, T0
	const typename OSCManip::_Fmtflags OSCManip::OrbitTLE;     // e, i, Ω, ω, n, M0

	const typename OSCManip::_Fmtflags OSCManip::Simple;
	const typename OSCManip::_Fmtflags OSCManip::Detail;

	const typename OSCManip::_Fmtflags OSCManip::Watermark;
	const typename OSCManip::_Fmtflags OSCManip::Fixed;
	const typename OSCManip::_Fmtflags OSCManip::Booleans;
	const typename OSCManip::_Fmtflags OSCManip::ForceTidalLock;
}

OSCStream::fmtflags OSCStream::flags(fmtflags _Newfmtflags)
{
	const OSCStream::fmtflags _Oldfmtflags = _Fmtfl;
	_Fmtfl = _Newfmtflags & _Fmtmask;
	return _Oldfmtflags;
}

OSCStream::fmtflags OSCStream::setf(fmtflags _Newfmtflags)
{
	const OSCStream::fmtflags _Oldfmtflags = _Fmtfl;
	_Fmtfl |= _Newfmtflags & _Fmtmask;
	return _Oldfmtflags;
}

void OSCStream::unsetf(fmtflags _Mask)
{
	_Fmtfl &= ~_Mask;
}

OSCStream& operator<<(OSCStream& os, const Object& Obj)
{
	os.Catalog.push_back(Obj);
	return os;
}

OSCStream& OSCStream::operator<<(OSCStream& (__cdecl* _Pfn)(OSCStream&))
{
	_Pfn(*this);
	return *this;
}

void OSCStream::WaterMark()
{
	Output << "// " << "Objects made with SpaceEngine SDK(CSpaceEngine)\n";
	Output << "// " << "An open-sourced C++ static library by StellarDX\n";
	Output << "// " << "Compiler Version : " << COMPILER_VERSION << '\n';
	Output << "// " << "Date : " << "" << '\n';
	Output << hex << "// " << "Format flags : 0x" << _Fmtfl << '\n';
}


void OSCStream::Write()
{
	Output.precision(BasicDataPrecision);

	if (_Fmtfl < 0)
	{
		WaterMark();
		Output << '\n';
	}

	if (_Fmtfl & (1 << 30)) { Output.setf(ios::fixed); }

	for (size_t i = 0; i < Catalog.size(); i++)
	{
		Output << Catalog[i].Type << ' ' << '\"';
		for (size_t j = 0; j < Catalog[i].Name.size(); j++)
		{
			Output << Catalog[i].Name[j];
			if (j < Catalog[i].Name.size() - 1)
			{
				Output << '/';
			}
		}
		Output << '\"' << '\n';

		Output << '{' << '\n';
		WriteKeyValue("ParentBody     ", Catalog[i].ParentBody);
		if (Catalog[i].Type == "Star")
		{
			WriteKeyValue("Class          ", Catalog[i].SpecClass);
		}
		else
		{
			WriteKeyValue("Class          ", Catalog[i].Class);
		}

		Physical(Catalog[i]);
		Optical(Catalog[i]);
		Output << '\n';
		Rotation(Catalog[i]);
		Output << '\n';
		if (!Catalog[i].NoLife) { Life(Catalog[i]); }
		/*else
		{
			WriteKeyValue("NoLife          ", Catalog[i].NoLife);
			if (_Fmtfl & (1 << BoolKey)) { Output << '\n'; }
		}*/
		if (!Catalog[i].Interior.empty()) { Interior(Catalog[i]); }
		if (Catalog[i].EnableSurface) { Surface(Catalog[i].Surface); }
		if (!Catalog[i].NoOcean) { Ocean(Catalog[i].Ocean); }
		else
		{
			WriteKeyValue("NoOcean         ", Catalog[i].NoOcean);
			if (_Fmtfl & (1 << BoolKey)) { Output << '\n'; }
		}
		if (!Catalog[i].NoClouds) { Clouds(Catalog[i].Clouds); }
		else
		{
			WriteKeyValue("NoClouds        ", Catalog[i].NoClouds);
			if (_Fmtfl & (1 << BoolKey)) { Output << '\n'; }
		}
		if (!Catalog[i].NoAtmosphere) { Atmosphere(Catalog[i].Atmosphere); }
		else
		{
			WriteKeyValue("NoAtmosphere    ", Catalog[i].NoAtmosphere);
			if (_Fmtfl & (1 << BoolKey)) { Output << '\n'; }
		}
		if (!Catalog[i].NoAurora) { Aurora(Catalog[i].Aurora); }
		else
		{
			WriteKeyValue("NoAurora        ", Catalog[i].NoAurora);
			if (_Fmtfl & (1 << BoolKey)) { Output << '\n'; }
		}
		if (!Catalog[i].NoRings) { Rings(Catalog[i].Rings); }
		else
		{
			WriteKeyValue("NoRings         ", Catalog[i].NoRings);
			if (_Fmtfl & (1 << BoolKey)) { Output << '\n'; }
		}
		if (!Catalog[i].NoAccretionDisk) { AccDisk(Catalog[i].AccretionDisk); }
		else
		{
			WriteKeyValue("NoAccretionDisk ", Catalog[i].NoAccretionDisk);
			if (_Fmtfl & (1 << BoolKey)) { Output << '\n'; }
		}
		if (!Catalog[i].NoCorona) { Corona(Catalog[i].Corona); }
		else
		{
			WriteKeyValue("NoCorona        ", Catalog[i].NoCorona);
			if (_Fmtfl & (1 << BoolKey)) { Output << '\n'; }
		}
		if (!Catalog[i].NoCometTail) { CometTail(Catalog[i].CometTail); }
		else
		{
			WriteKeyValue("NoCometTail     ", Catalog[i].NoCometTail);
			if (_Fmtfl & (1 << BoolKey)) { Output << '\n'; }
		}
		Orbit(Catalog[i]);

		Output << '}' << '\n';

		Output << '\n';
	}
}

void OSCStream::Physical(Object Obj)
{
	const int ControlKey = 17;
	WriteKeyValue("MassKg          ", Obj.Mass);
	if (Obj.Dimensions.x == Obj.Dimensions.y && Obj.Dimensions.y == Obj.Dimensions.z)
	{
		WriteKeyValue("Radius          ", Obj.Dimensions.x / 2.0 / 1000.0);
	}
	else
	{
		WriteKeyValue("Dimensions     ", Obj.Dimensions / 1000.0);
	}

	if (Obj.Type == "Star")
	{
		WriteKeyValue("Luminosity      ", Obj.Luminosity / SolarLum);
		WriteKeyValue("LumBol          ", Obj.LumBol / SolarLum);
		WriteKeyValue("Teff            ", Obj.Teff);
		WriteKeyValue("FeH             ", Obj.FeH);
		WriteKeyValue("Age             ", Obj.Age / 1000000000.0);
	}

	if (_Fmtfl & (1 << ControlKey))
	{
		WriteKeyValue("InertiaMoment   ", Obj.InertiaMoment);
		WriteKeyValue("AlbedoBond      ", Obj.AlbedoBond);
		WriteKeyValue("AlbedoGeom      ", Obj.AlbedoGeom);
		if (Obj.Type == "Star" /* || isRemnant(Obj) */)
		{
			WriteKeyValue("Luminosity      ", Obj.KerrSpin);
			WriteKeyValue("LumBol          ", Obj.KerrCharge);
		}
	}
}

void OSCStream::Optical(Object Obj)
{
	const int ControlKey = 16;
	if (_Fmtfl & (1 << ControlKey))
	{
		WriteKeyValue("AbsMagn         ", Obj.AbsMagn);
		WriteKeyValue("SlopeParam      ", Obj.SlopeParam);
		WriteKeyValue("Brightness      ", Obj.Brightness);
		WriteKeyValue("BrightnessReal  ", Obj.BrightnessReal);
		WriteKeyValue("Color          ", Obj.Color);
	}
}

void OSCStream::Rotation(Object Obj)
{
	Output.precision(RotationPrecision);
	const int ControlKey1 = 15;
	const int ControlKey2 = 14;
	const int TidKey = 28;

	auto Simple = [&]()->void
	{
		if (!Obj.TidalLocked)
		{
			WriteKeyValue("RotationPeriod  ", Obj.Rotation.RotationPeriod / 3600.0);
			WriteKeyValue("RotationEpoch   ", Obj.Rotation.RotationEpoch);
			WriteKeyValue("Precession      ", Obj.Rotation.Precession);
		}
		WriteKeyValue("Obliquity       ", Obj.Rotation.Obliquity);
		WriteKeyValue("EqAscendNode    ", Obj.Rotation.EqAscendNode);
		WriteKeyValue("RotationOffset  ", Obj.Rotation.RotationOffset);
		WriteKeyValue("TidalLocked     ", Obj.TidalLocked);
	};

	auto IAU = [&]()->void
	{
		Output << '\t' << "RotationModel	\"IAU\"\n";
		Output << '\t' << '{' << '\n';
		WriteKeyValue("Epoch			", Obj.IAU.Epoch, 2);
		WriteKeyValue("PoleRA      	", Obj.IAU.PoleRA, 2);
		WriteKeyValue("PoleRARate  	", Obj.IAU.PoleRARate, 2);
		WriteKeyValue("PoleDec     	", Obj.IAU.PoleDec, 2);
		WriteKeyValue("PoleDecRate 	", Obj.IAU.PoleDecRate, 2);
		WriteKeyValue("PrimeMeridian	", Obj.IAU.PrimeMeridian, 2);
		WriteKeyValue("RotationRate	", Obj.IAU.RotationRate, 2);
		WriteKeyValue("RotationAccel	", Obj.IAU.RotationAccel, 2);
		if (!Obj.IAU.PeriodicTerms.empty())
		{
			// Coming soon...
		}
		Output << '\t' << '}' << '\n';
	};

	auto MoonDE = [&]()->void
	{
		Output << "\tRotationModel  \"Moon-DE\" { }\n";
	};

	if (!(_Fmtfl & (1 << ControlKey1)) && !(_Fmtfl & (1 << ControlKey2)))
	{
		if (Obj.RotationModel == "Simple" || ((_Fmtfl & (1 << TidKey)) && Obj.TidalLocked)){ Simple(); }
		else if (Obj.RotationModel == "IAU") { IAU(); }
		else if (Obj.RotationModel == "Moon-DE") { MoonDE(); }
	}

	if (!(_Fmtfl & (1 << ControlKey1)) && (_Fmtfl & (1 << ControlKey2))){Simple();}
	if ((_Fmtfl & (1 << ControlKey1)) && !(_Fmtfl & (1 << ControlKey2))){IAU();}
	if ((_Fmtfl & (1 << ControlKey1)) && (_Fmtfl & (1 << ControlKey2))){MoonDE();}

	Output.precision(BasicDataPrecision);
}

void OSCStream::Orbit(Object Obj)
{
	Output.precision(OrbitPrecision);
	const int ControlKey1 = 13;
	const int ControlKey2 = 12;
	const int ControlKey3 = 11;

	auto Default = [&]()->void
	{
		WriteKeyValue("AnalyticModel  ", Obj.Orbit.AnalyticModel, 2);
		WriteKeyValue("RefPlane        ", Obj.Orbit.RefPlane, 2);
		WriteKeyValue("Epoch           ", Obj.Orbit.Epoch, 2);
		WriteKeyValue("Eccentricity    ", Obj.Orbit.Eccentricity, 2);
		WriteKeyValue("SemiMajorAxis   ", Obj.Orbit.PericenterDist / (1.0 - Obj.Orbit.Eccentricity) / AU, 2);
		WriteKeyValue("PeriodDays      ", Obj.Orbit.Period / SynodicDay, 2);
		WriteKeyValue("Inclination     ", Obj.Orbit.Inclination, 2);
		WriteKeyValue("AscendingNode   ", Obj.Orbit.AscendingNode, 2);
		WriteKeyValue("ArgOfPericenter ", Obj.Orbit.ArgOfPericenter, 2);
		WriteKeyValue("MeanAnomaly     ", Obj.Orbit.MeanAnomaly, 2);
	};

	auto Binary = [&]()->void
	{
		//WriteKeyValue("AnalyticModel  ", Obj.Orbit.AnalyticModel, 2);
		WriteKeyValue("RefPlane        ", Obj.Orbit.RefPlane, 2);
		WriteKeyValue("PeriodDays      ", Obj.Orbit.Period / SynodicDay, 2);
		WriteKeyValue("Eccentricity    ", Obj.Orbit.Eccentricity, 2);
		WriteKeyValue("Inclination     ", Obj.Orbit.Inclination, 2);
		WriteKeyValue("AscendingNode   ", Obj.Orbit.AscendingNode, 2);
		WriteKeyValue("Epoch           ", Obj.Orbit.Epoch, 2);
		WriteKeyValue("ArgOfPericenter ", Obj.Orbit.ArgOfPericenter, 2);
		WriteKeyValue("MeanAnomaly     ", Obj.Orbit.MeanAnomaly, 2);
	};

	auto MajorPlanet = [&]()->void
	{
		WriteKeyValue("AnalyticModel  ", Obj.Orbit.AnalyticModel, 2);
		WriteKeyValue("RefPlane        ", Obj.Orbit.RefPlane, 2);
		WriteKeyValue("Epoch           ", Obj.Orbit.Epoch, 2);
		WriteKeyValue("Eccentricity    ", Obj.Orbit.Eccentricity, 2);
		WriteKeyValue("SemiMajorAxis   ", Obj.Orbit.PericenterDist / (1.0 - Obj.Orbit.Eccentricity) / AU, 2);
		WriteKeyValue("Inclination     ", Obj.Orbit.Inclination, 2);
		WriteKeyValue("AscendingNode   ", Obj.Orbit.AscendingNode, 2);
		float64 LongOfPericen = Obj.Orbit.AscendingNode + Obj.Orbit.ArgOfPericenter;
		if (LongOfPericen > 360) { LongOfPericen -= 360; }
		WriteKeyValue("LongOfPericen   ", LongOfPericen, 2);
		float64 MeanLongitude = Obj.Orbit.AscendingNode + Obj.Orbit.ArgOfPericenter + Obj.Orbit.MeanAnomaly;
		if (MeanLongitude > 360) { MeanLongitude -= 360; }
		WriteKeyValue("MeanLongitude   ", MeanLongitude, 2);
	};

	auto Comet = [&]()->void
	{
		//WriteKeyValue("AnalyticModel  ", Obj.Orbit.AnalyticModel, 2);
		WriteKeyValue("RefPlane        ", Obj.Orbit.RefPlane, 2);
		WriteKeyValue("Eccentricity    ", Obj.Orbit.Eccentricity, 2);
		WriteKeyValue("PericenterDist  ", Obj.Orbit.PericenterDist / AU, 2);
		WriteKeyValue("Inclination     ", Obj.Orbit.Inclination, 2);
		WriteKeyValue("AscendingNode   ", Obj.Orbit.AscendingNode, 2);
		WriteKeyValue("ArgOfPericenter ", Obj.Orbit.ArgOfPericenter, 2);
		WriteKeyValue("Epoch           ", Obj.Orbit.Epoch, 2);
		WriteKeyValue("MeanAnomaly     ", Obj.Orbit.MeanAnomaly, 2);
	};

	auto TLE = [&]()->void
	{
		//WriteKeyValue("AnalyticModel  ", Obj.Orbit.AnalyticModel, 2);
		WriteKeyValue("RefPlane        ", Obj.Orbit.RefPlane, 2);
		WriteKeyValue("Epoch           ", Obj.Orbit.Epoch, 2);
		WriteKeyValue("Eccentricity    ", Obj.Orbit.Eccentricity, 2);
		WriteKeyValue("Inclination     ", Obj.Orbit.Inclination, 2);
		WriteKeyValue("AscendingNode   ", Obj.Orbit.AscendingNode, 2);
		WriteKeyValue("ArgOfPericenter ", Obj.Orbit.ArgOfPericenter, 2);
		WriteKeyValue("MeanMotion      ", 360.0 / ((Obj.Orbit.Period / SynodicDay)), 2);
		WriteKeyValue("MeanAnomaly     ", Obj.Orbit.MeanAnomaly, 2);
	};

	if (Obj.Orbit.RefPlane == "None")
	{
		#if OBJ_DEFAULT_STATIC_FIXED_POS == 0
		WriteKeyValue("StaticPosXYZ   ", Obj.StaticPos);
		WriteKeyValue("FixedPosXYZ    ", Obj.FixedPos);
		#elif OBJ_DEFAULT_STATIC_FIXED_POS == 1
		WriteKeyValue("StaticPosPolar ", Obj.StaticPos);
		WriteKeyValue("FixedPosPolar  ", Obj.FixedPos);
		#endif
	}

	else
	{
		Output << '\t' << "Orbit" << '\n';
		Output << '\t' << '{' << '\n';
		if (!(_Fmtfl & (1 << ControlKey1)) && !(_Fmtfl & (1 << ControlKey2)) && !(_Fmtfl & (1 << ControlKey3)))
		{
			if (Obj.Type == "Star") { Binary(); }
			else if (Obj.Type == "Planet") { MajorPlanet(); }
			else if (Obj.Type == "Asteroid" || Obj.Type == "DwarfPlanet") { Default(); }
			else if (Obj.Type == "Comet") { Comet(); }
			else if (Obj.Type == "Spacecraft" || Obj.Type == "Structure" || Obj.Type == "Artifact") { Comet(); }
			else { Default(); }
		}

		if ((_Fmtfl & (1 << ControlKey1)) && (_Fmtfl & (1 << ControlKey2)) && (_Fmtfl & (1 << ControlKey3))) { Default(); }
		else if (!(_Fmtfl & (1 << ControlKey1)) && !(_Fmtfl & (1 << ControlKey2)) && (_Fmtfl & (1 << ControlKey3))) { Binary(); }
		else if (!(_Fmtfl & (1 << ControlKey1)) && (_Fmtfl & (1 << ControlKey2)) && !(_Fmtfl & (1 << ControlKey3))) { MajorPlanet(); }
		else if (!(_Fmtfl & (1 << ControlKey1)) && (_Fmtfl & (1 << ControlKey2)) && (_Fmtfl & (1 << ControlKey3))) { Comet(); }
		else if ((_Fmtfl & (1 << ControlKey1)) && !(_Fmtfl & (1 << ControlKey2)) && !(_Fmtfl & (1 << ControlKey3))) { TLE(); }
		Output << '\t' << '}' << '\n';
	}

	Output.precision(BasicDataPrecision);
}

void OSCStream::Life(Object Obj)
{
	const int ControlKey = 10;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }
	for (int64 i = 0; i < Obj.LifeCount; i++)
	{
		Output << "\tLife\n";
		Output << "\t{\n";
		WriteKeyValue("Class   ", Obj.Life[i].Class, 2);
		WriteKeyValue("Type    ", Obj.Life[i].Type, 2);
		Output << "\t\tBiome   " << '\"';
		for (size_t j = 0; j < Obj.Life[i].Biome.size(); j++)
		{
			Output << Obj.Life[i].Biome[j];
			if (j < Obj.Life[i].Biome.size() - 1) { Output << '/'; }
		}
		Output << "\"" << '\n';
		Output << "\t}\n";
		if (i < Obj.LifeCount - 1) { Output << '\n'; }
	}

	Output << '\n';
}

void OSCStream::Interior(Object Obj)
{
	const int ControlKey = 9;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }

	Output.precision(InteriorPrecision);
	Output << "\tInterior\n";
	Output << "\t{\n";
	Output << "\t\tComposition\n";
	Output << "\t\t{\n";
	for (auto i = Obj.Interior.begin(); i != Obj.Interior.end(); i++)
	{
		Output << "\t\t\t" << i->first << "       	" << i->second << '\n';
	}
	Output << "\t\t}\n";
	Output << "\t}\n";

	Output.precision(BasicDataPrecision);
	Output << '\n';
}

void OSCStream::Surface(Landscape Surf)
{
	const int ControlKey = 8;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }

	Output.precision(SurfacePrecision);
	Output << "\tSurface\n";
	Output << "\t{\n";
	WriteKeyValue("Preset         ", Surf.Preset, 2);
	WriteKeyValue("SurfStyle       ", Surf.SurfStyle, 2);
	WriteKeyValue("Randomize      ", Surf.Randomize, 2);
	WriteKeyValue("colorDistMagn   ", Surf.colorParams.x, 2);
	WriteKeyValue("colorDistFreq   ", Surf.colorParams.y, 2);
	WriteKeyValue("icecapLatitude  ", Surf.colorParams.z, 2);
	WriteKeyValue("tropicLatitude  ", Surf.colorParams.w, 2);
	WriteKeyValue("climatePole     ", Surf.climateParams.x, 2);
	WriteKeyValue("climateTropic   ", Surf.climateParams.y, 2);
	WriteKeyValue("climateEquator  ", Surf.climateParams.z, 2);
	WriteKeyValue("tropicWidth     ", Surf.climateParams.w, 2);
	WriteKeyValue("climateGrassMin  ", Surf.plantsParams1.x, 2);
	WriteKeyValue("climateGrassMax  ", Surf.plantsParams1.y, 2);
	WriteKeyValue("climateForestMin ", Surf.plantsParams1.z, 2);
	WriteKeyValue("climateForestMax ", Surf.plantsParams1.w, 2);
	WriteKeyValue("climateSteppeMin ", Surf.plantsParams2.x, 2);
	WriteKeyValue("climateSteppeMax ", Surf.plantsParams2.y, 2);
	WriteKeyValue("humidity        ", Surf.plantsParams2.z, 2);
	//WriteKeyValue("plantsBiomeOffset ", Surf.plantsParams2.w, 2);
	WriteKeyValue("mainFreq        ", Surf.mainParams.x, 2);
	WriteKeyValue("terraceProb     ", Surf.mainParams.y, 2);
	//WriteKeyValue("oceanType       ", Surf.mainParams.z, 2);
	//WriteKeyValue("TidalLocked     ", Surf.mainParams.w, 2);
	WriteKeyValue("detailScale     ", Surf.textureParams.x, 2);
	//WriteKeyValue("                ", Surf.textureParams.y, 2);
	WriteKeyValue("venusMagn       ", Surf.textureParams.z, 2);
	WriteKeyValue("venusFreq       ", Surf.textureParams.w, 2);
	WriteKeyValue("seaLevel        ", Surf.mareParams.x, 2);
	WriteKeyValue("mareFreq        ", Surf.mareParams.y, 2);
	WriteKeyValue("mareDensity     ", Surf.mareParams.z, 2);
	WriteKeyValue("icecapHeight    ", Surf.mareParams.w, 2);
	WriteKeyValue("montesMagn      ", Surf.montesParams.x, 2);
	WriteKeyValue("montesFreq      ", Surf.montesParams.y, 2);
	WriteKeyValue("montesSpiky     ", Surf.montesParams.z, 2);
	WriteKeyValue("montesFraction  ", Surf.montesParams.w, 2);
	WriteKeyValue("dunesMagn       ", Surf.dunesParams.x, 2);
	WriteKeyValue("dunesFreq       ", Surf.dunesParams.y, 2);
	WriteKeyValue("dunesFraction   ", Surf.dunesParams.z, 2);
	WriteKeyValue("drivenDarkening ", Surf.dunesParams.w, 2);
	WriteKeyValue("hillsMagn       ", Surf.hillsParams.x, 2);
	WriteKeyValue("hillsFreq       ", Surf.hillsParams.y, 2);
	WriteKeyValue("hillsFraction   ", Surf.hillsParams.z, 2);
	WriteKeyValue("hills2Fraction  ", Surf.hillsParams.w, 2);
	WriteKeyValue("riversMagn      ", Surf.riversParams.x, 2);
	WriteKeyValue("riversFreq      ", Surf.riversParams.y, 2);
	WriteKeyValue("riversSin       ", Surf.riversParams.z, 2);
	WriteKeyValue("beachWidth      ", Surf.riversParams.w, 2);
	WriteKeyValue("riftsMagn       ", Surf.riftsParams.x, 2);
	WriteKeyValue("riftsFreq       ", Surf.riftsParams.y, 2);
	WriteKeyValue("riftsSin        ", Surf.riftsParams.z, 2);
	//WriteKeyValue("                ", Surf.riftsParams.w, 2);
	WriteKeyValue("canyonsMagn     ", Surf.canyonsParams.x, 2);
	WriteKeyValue("canyonsFreq     ", Surf.canyonsParams.y, 2);
	WriteKeyValue("canyonsFraction ", Surf.canyonsParams.z, 2);
	WriteKeyValue("erosion         ", Surf.canyonsParams.w, 2);
	WriteKeyValue("cracksMagn      ", Surf.cracksParams.x, 2);
	WriteKeyValue("cracksFreq      ", Surf.cracksParams.y, 2);
	WriteKeyValue("cracksOctaves   ", Surf.cracksParams.z, 2);
	WriteKeyValue("craterRayedFactor ", Surf.cracksParams.w, 2);
	WriteKeyValue("craterMagn      ", Surf.craterParams.x, 2);
	WriteKeyValue("craterFreq      ", Surf.craterParams.y, 2);
	WriteKeyValue("craterDensity   ", Surf.craterParams.z, 2);
	WriteKeyValue("craterOctaves   ", Surf.craterParams.w, 2);
	WriteKeyValue("volcanoMagn     ", Surf.volcanoParams1.x, 2);
	WriteKeyValue("volcanoFreq     ", Surf.volcanoParams1.y, 2);
	WriteKeyValue("volcanoDensity  ", Surf.volcanoParams1.z, 2);
	WriteKeyValue("volcanoOctaves  ", Surf.volcanoParams1.w, 2);
	WriteKeyValue("volcanoActivity ", Surf.volcanoParams2.x, 2);
	WriteKeyValue("volcanoFlows    ", Surf.volcanoParams2.y, 2);
	WriteKeyValue("volcanoRadius   ", Surf.volcanoParams2.z, 2);
	WriteKeyValue("volcanoTemp     ", Surf.volcanoParams2.w, 2);
	//WriteKeyValue("lavaCoverage    ", Surf.lavaParams.x, 2);
	WriteKeyValue("snowLevel       ", Surf.lavaParams.y, 2);
	//WriteKeyValue("surfTemperature ", Surf.lavaParams.z, 2);
	WriteKeyValue("heightTempGrad  ", Surf.lavaParams.w, 2);
	WriteKeyValue("lavaCoverTidal  ", Surf.lavaDetails.x, 2);
	WriteKeyValue("lavaCoverSun    ", Surf.lavaDetails.y, 2);
	WriteKeyValue("lavaCoverYoung  ", Surf.lavaDetails.z, 2);
	WriteKeyValue("cycloneMagn     ", Surf.cycloneParams.x, 2);
	WriteKeyValue("cycloneFreq     ", Surf.cycloneParams.y, 2);
	WriteKeyValue("cycloneDensity  ", Surf.cycloneParams.z, 2);
	WriteKeyValue("cycloneOctaves  ", Surf.cycloneParams.w, 2);
	WriteKeyValue("cycloneMagn2    ", Surf.cycloneParams2.x, 2);
	WriteKeyValue("cycloneFreq2    ", Surf.cycloneParams2.y, 2);
	WriteKeyValue("cycloneLatitude2 ", Surf.cycloneParams2.z, 2);
	WriteKeyValue("cycloneOctaves2 ", Surf.cycloneParams2.w, 2);

	WriteKeyValue("DiffMap        ", Surf.DiffMap, 2);
	WriteKeyValue("DiffMapAlpha   ", Surf.DiffMapAlpha, 2);
	WriteKeyValue("BumpMap        ", Surf.BumpMap, 2);
	WriteKeyValue("BumpHeight      ", Surf.BumpHeight, 2);
	WriteKeyValue("BumpOffset      ", Surf.BumpOffset, 2);
	WriteKeyValue("GlowMap        ", Surf.GlowMap, 2);
	WriteKeyValue("GlowMode       ", Surf.GlowMode, 2);
	WriteKeyValue("GlowColor      ", Surf.GlowColor, 2);
	WriteKeyValue("GlowBright      ", Surf.GlowBright, 2);
	WriteKeyValue("SpecMap        ", Surf.SpecMap, 2);
	WriteKeyValue("RoughnessBias   ", Surf.RoughnessBias, 2);
	WriteKeyValue("RoughnessScale  ", Surf.RoughnessScale, 2);
	WriteKeyValue("SpecBrightIce   ", Surf.SpecParams.x, 2);
	WriteKeyValue("SpecBrightWater ", Surf.SpecParams.y, 2);
	WriteKeyValue("SpecPowerIce    ", Surf.SpecParams.z, 2);
	WriteKeyValue("SpecPowerWater  ", Surf.SpecParams.w, 2);
	WriteKeyValue("SpecularScale   ", Surf.SpecularScale, 2);
	WriteKeyValue("Hapke           ", Surf.Hapke, 2);
	WriteKeyValue("SpotBright      ", Surf.HapkeParams.x, 2);
	WriteKeyValue("SpotWidth       ", Surf.HapkeParams.y, 2);
	//WriteKeyValue("SpotBrightCB    ", Surf.HapkeParams.z, 2);
	//WriteKeyValue("SpotWidthCB     ", Surf.HapkeParams.w, 2);
	WriteKeyValue("DayAmbient      ", Surf.DayAmbient, 2);
	WriteKeyValue("ModulateColor  ", Surf.ModulateColor, 2);
	Output << "\t}\n";

	Output.precision(BasicDataPrecision);
	Output << '\n';
}

void OSCStream::Ocean(OceanParam Sea)
{
	const int ControlKey = 7;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }

	Output.precision(OceanPrecision);
	Output << "\tOcean\n";
	Output << "\t{\n";
	WriteKeyValue("Height          ", Sea.Height, 2);
	WriteKeyValue("Hapke           ", Sea.Hapke, 2);
	WriteKeyValue("SpotBright      ", Sea.HapkeParams.x, 2);
	WriteKeyValue("SpotWidth       ", Sea.HapkeParams.y, 2);
	//WriteKeyValue("SpotBrightCB    ", Sea.HapkeParams.z, 2);
	//WriteKeyValue("SpotWidthCB     ", Sea.HapkeParams.w, 2);
	WriteKeyValue("DayAmbient      ", Sea.DayAmbient, 2);
	if (!Sea.Composition.empty())
	{
		Output << '\n';
		Output << "\t\tComposition\n";
		Output << "\t\t{\n";
		for (auto i = Sea.Composition.begin(); i != Sea.Composition.end(); i++)
		{
			Output << "\t\t\t" << i->first << "       	" << i->second << '\n';
		}
		Output << "\t\t}\n";
	}
	Output << "\t}\n";

	Output.precision(BasicDataPrecision);
	Output << '\n';
}

void OSCStream::Clouds(CloudsParam Cld)
{
	const int ControlKey = 6;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }

	Output.precision(CloudsPrecision);
	for (size_t i = 0; i < Cld.Layer.size(); i++)
	{
		Output << "\tClouds\n";
		Output << "\t{\n";
		WriteKeyValue("DiffMap        ", Cld.Layer[i].DiffMap, 2);
		WriteKeyValue("BumpMap        ", Cld.Layer[i].BumpMap, 2);
		WriteKeyValue("Height          ", Cld.Layer[i].Height, 2);
		WriteKeyValue("Velocity        ", Cld.Layer[i].Velocity, 2);
		WriteKeyValue("BumpHeight      ", Cld.Layer[i].BumpHeight, 2);
		WriteKeyValue("BumpOffset      ", Cld.Layer[i].BumpOffset, 2);
		WriteKeyValue("Hapke           ", Cld.Layer[i].Hapke, 2);
		WriteKeyValue("SpotBright      ", Cld.Layer[i].HapkeParams.x, 2);
		WriteKeyValue("SpotWidth       ", Cld.Layer[i].HapkeParams.y, 2);
		//WriteKeyValue("SpotBrightCB    ", Cld.Layer[i].HapkeParams.z, 2);
		//WriteKeyValue("SpotWidthCB     ", Cld.Layer[i].HapkeParams.w, 2);
		WriteKeyValue("DayAmbient      ", Cld.Layer[i].DayAmbient, 2);
		WriteKeyValue("ModulateColor  ", vec3(Cld.Layer[i].ModulateColor.x, Cld.Layer[i].ModulateColor.y, Cld.Layer[i].ModulateColor.z), 2);
		WriteKeyValue("Opacity         ", Cld.Layer[i].ModulateColor.w, 2);
		WriteKeyValue("ModulateBright  ", Cld.Layer[i].ModulateBright, 2);
		if (i == 0)
		{
			WriteKeyValue("mainFreq        ", Cld.cloudsParams1.x, 2);
			WriteKeyValue("mainOctaves     ", Cld.cloudsParams1.y, 2);
			WriteKeyValue("stripeZones     ", Cld.cloudsParams1.z, 2);
			WriteKeyValue("stripeTwist     ", Cld.cloudsParams1.w, 2);
			//WriteKeyValue("cloudsLayer     ", Cld.cloudsParams2.x, 2);
			//WriteKeyValue("cloudsNLayers   ", Cld.cloudsParams2.y, 2);
			WriteKeyValue("stripeFluct     ", Cld.cloudsParams2.z, 2);
			WriteKeyValue("Coverage        ", Cld.cloudsParams2.w, 2);
			WriteKeyValue("RingsWinter     ", Cld.RingsWinter, 2);
		}
		Output << "\t}\n";
		Output << '\n';
	}
	Output.precision(BasicDataPrecision);
}

void OSCStream::Atmosphere(AtmParam Atm)
{
	const int ControlKey = 5;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }

	Output.precision(AtmPrecision);
	Output << "\tAtmosphere\n";
	Output << "\t{\n";
	WriteKeyValue("Model          ", Atm.Model, 2);
	WriteKeyValue("Height          ", Atm.Height, 2);
	WriteKeyValue("Density         ", Atm.Density, 2);
	WriteKeyValue("Pressure        ", Atm.Pressure, 2);
	WriteKeyValue("Greenhouse      ", Atm.Greenhouse, 2);
	WriteKeyValue("Bright          ", Atm.Bright, 2);
	WriteKeyValue("Opacity         ", Atm.Opacity, 2);
	WriteKeyValue("SkyLight        ", Atm.SkyLight, 2);
	WriteKeyValue("Hue             ", Atm.Hue, 2);
	WriteKeyValue("Saturation      ", Atm.Saturation, 2);
	if (!Atm.Composition.empty())
	{
		Output << '\n';
		Output << "\t\tComposition\n";
		Output << "\t\t{\n";
		for (auto i = Atm.Composition.begin(); i != Atm.Composition.end(); i++)
		{
			Output << "\t\t\t" << i->first << "       	" << i->second << '\n';
		}
		Output << "\t\t}\n";
	}
	Output << "\t}\n";

	Output.precision(BasicDataPrecision);
	Output << '\n';
}

void OSCStream::Aurora(AurParam Aur)
{
	const int ControlKey = 4;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }

	Output.precision(AuroraPrecision);
	Output << "\tAurora\n";
	Output << "\t{\n";
	WriteKeyValue("Height         ", Aur.Height, 2);
	WriteKeyValue("NorthLat       ", Aur.North.Lat, 2);
	WriteKeyValue("NorthLon       ", Aur.North.Lon, 2);
	WriteKeyValue("NorthRadius    ", Aur.North.Radius, 2);
	WriteKeyValue("NorthWidth     ", Aur.North.Width, 2);
	WriteKeyValue("NorthRings     ", Aur.North.Rings, 2);
	WriteKeyValue("NorthBright    ", Aur.North.Bright, 2);
	WriteKeyValue("NorthFlashFreq ", Aur.North.FlashFreq, 2);
	WriteKeyValue("NorthMoveSpeed ", Aur.North.MoveSpeed, 2);
	WriteKeyValue("NorthParticles ", Aur.North.Particles, 2);
	WriteKeyValue("SouthLat       ", Aur.South.Lat, 2);
	WriteKeyValue("SouthLon       ", Aur.South.Lon, 2);
	WriteKeyValue("SouthRadius    ", Aur.South.Radius, 2);
	WriteKeyValue("SouthWidth     ", Aur.South.Width, 2);
	WriteKeyValue("SouthRings     ", Aur.South.Rings, 2);
	WriteKeyValue("SouthBright    ", Aur.South.Bright, 2);
	WriteKeyValue("SouthFlashFreq ", Aur.South.FlashFreq, 2);
	WriteKeyValue("SouthMoveSpeed ", Aur.South.MoveSpeed, 2);
	WriteKeyValue("SouthParticles ", Aur.South.Particles, 2);
	WriteKeyValue("TopColor    ", Aur.TopColor, 2);
	WriteKeyValue("BottomColor ", Aur.BottomColor, 2);
	Output << "\t}\n";

	Output.precision(BasicDataPrecision);
	Output << '\n';
}

void OSCStream::Rings(RingsParam Rng)
{
	const int ControlKey = 3;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }

	Output.precision(RingsPrecision);
	Output << "\tRings\n";
	Output << "\t{\n";
	WriteKeyValue("Texture        ", Rng.Texture, 2);
	WriteKeyValue("InnerRadius     ", Rng.Radiuses.x, 2);
	WriteKeyValue("OuterRadius     ", Rng.Radiuses.y, 2);
	WriteKeyValue("EdgeRadius      ", Rng.Radiuses.z, 2);
	WriteKeyValue("MeanRadius      ", Rng.Radiuses.w, 2);
	WriteKeyValue("Thickness       ", Rng.Thickness, 2);
	WriteKeyValue("RocksMaxSize    ", Rng.RockParam.x, 2);
	WriteKeyValue("RocksSpacing    ", Rng.RockParam.y, 2);
	WriteKeyValue("DustDrawDist    ", Rng.RockParam.z, 2);
	WriteKeyValue("ChartRadius     ", Rng.ChartRadius, 2);
	WriteKeyValue("RotationPeriod  ", Rng.RotationPeriod, 2);
	WriteKeyValue("Brightness      ", Rng.Brightness.x, 2);
	WriteKeyValue("FrontBright     ", Rng.Brightness.y, 2);
	WriteKeyValue("BackBright      ", Rng.Brightness.z, 2);
	WriteKeyValue("Density         ", Rng.ShadowParam.x, 2);
	WriteKeyValue("Opacity         ", Rng.ShadowParam.y, 2);
	WriteKeyValue("SelfShadow      ", Rng.ShadowParam.z, 2);
	WriteKeyValue("PlanetShadow    ", Rng.ShadowParam.w, 2);
	WriteKeyValue("Hapke           ", Rng.Hapke, 2);
	WriteKeyValue("SpotBright      ", Rng.HapkeParams.x, 2);
	WriteKeyValue("SpotWidth       ", Rng.HapkeParams.y, 2);
	WriteKeyValue("SpotBrightCB    ", Rng.HapkeParams.z, 2);
	WriteKeyValue("SpotWidthCB     ", Rng.HapkeParams.w, 2);
	WriteKeyValue("frequency       ", Rng.Shape.x, 2);
	WriteKeyValue("densityScale    ", Rng.Shape.y, 2);
	WriteKeyValue("densityOffset   ", Rng.Shape.z, 2);
	WriteKeyValue("densityPower    ", Rng.Shape.w, 2);
	WriteKeyValue("colorContrast   ", Rng.colorContrast, 2);
	WriteKeyValue("FrontColor      ", Rng.FrontColor, 2);
	WriteKeyValue("BackThickColor  ", Rng.BackThickColor, 2);
	WriteKeyValue("BackIceColor    ", Rng.BackIceColor, 2);
	WriteKeyValue("BackDustColor   ", Rng.BackIceColor, 2);
	Output << "\t}\n";

	Output.precision(BasicDataPrecision);
	Output << '\n';
}

void OSCStream::AccDisk(AccDiskParam ADK)
{
	const int ControlKey = 2;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }

	Output.precision(AccDiskPrecision);
	Output << "\tAccretionDisk\n";
	Output << "\t{\n";
	WriteKeyValue("InnerRadiusKm   ", ADK.Radiuses.x / 1000.0, 2);
	WriteKeyValue("OuterRadiusKm   ", ADK.Radiuses.y / 1000.0, 2);
	//WriteKeyValue("BlackHoleRg     ", ADK.Radiuses.z, 2);
	//WriteKeyValue("GravLensScale   ", ADK.Radiuses.w, 2);
	WriteKeyValue("JetRadiusKm     ", ADK.JetRadius / 1000.0, 2);
	WriteKeyValue("JetLength       ", ADK.JetLength, 2);
	WriteKeyValue("Temperature     ", ADK.DiskParams1.x, 2);
	WriteKeyValue("TwistMagn       ", ADK.DiskParams1.y, 2);
	//WriteKeyValue("starTime        ", ADK.DiskParams1.z, 2);
	//WriteKeyValue("diskTime        ", ADK.DiskParams1.w, 2);
	//WriteKeyValue("tempShift       ", ADK.DiskParams2.x, 2);
	WriteKeyValue("Brightness      ", ADK.DiskParams2.y, 2);
	WriteKeyValue("Opacity         ", ADK.DiskParams2.z, 2);
	//WriteKeyValue("starRotAng      ", ADK.DiskParams2.w, 2);
	WriteKeyValue("InnerThicknessKm ", ADK.DiskParams3.x / 1000.0, 2);
	WriteKeyValue("OuterThicknessKm ", ADK.DiskParams3.y / 1000.0, 2);
	WriteKeyValue("DetailScaleR    ", ADK.DiskParams3.z, 2);
	WriteKeyValue("DetailScaleV    ", ADK.DiskParams3.w, 2);
	WriteKeyValue("ThicknessPow    ", ADK.ThicknessPow, 2);
	WriteKeyValue("AccretionRate   ", ADK.AccretionRate, 2);
	WriteKeyValue("Density         ", ADK.Density, 2);
	WriteKeyValue("Luminosity      ", ADK.Luminosity, 2);
	Output << "\t}\n";

	Output.precision(BasicDataPrecision);
	Output << '\n';
}

void OSCStream::Corona(CoronaParam Crn)
{
	const int ControlKey = 1;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }

	Output.precision(CoronaPrecision);
	Output << "\tCorona\n";
	Output << "\t{\n";
	WriteKeyValue("RayDensity  ", Crn.Shape.x, 2);
	WriteKeyValue("RayCurv     ", Crn.Shape.y, 2);
	WriteKeyValue("Radius      ", Crn.Shape.z, 2);
	//WriteKeyValue("SinFreq     ", Crn.Shape.w, 2);
	WriteKeyValue("Period      ", Crn.Period, 2);
	WriteKeyValue("Brightness  ", Crn.Bright, 2);
	Output << "\t}\n";

	Output.precision(BasicDataPrecision);
	Output << '\n';
}

void OSCStream::CometTail(CometTailParam Com)
{
	const int ControlKey = 0;
	if (!(_Fmtfl & (1 << ControlKey))) { return; }

	Output.precision(CometTailPrecision);
	Output << "\tCometTail\n";
	Output << "\t{\n";
	WriteKeyValue("MaxLength   ", Com.MaxLength, 2);
	WriteKeyValue("GasToDust   ", Com.GasToDust, 2);
	WriteKeyValue("Particles   ", Com.Particles, 2);
	WriteKeyValue("GasBright   ", Com.GasBright, 2);
	WriteKeyValue("DustBright  ", Com.DustBright, 2);
	WriteKeyValue("GasColor   ", Com.GasColor, 2);
	WriteKeyValue("DustColor  ", Com.DustColor, 2);
	Output << "\t}\n";

	Output.precision(BasicDataPrecision);
	Output << '\n';
}

_CSE_END
