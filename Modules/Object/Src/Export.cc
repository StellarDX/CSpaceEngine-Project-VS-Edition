#define _USE_CSE_DEFINES

#include "CSE/Object.h"

#include <sstream>
#include <iomanip>
#include <format>

using namespace std;

_CSE_BEGIN

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
		if (_Fl & (0x80000000U >> 1)) { ValueStr << fixed; }
		ValueStr << setprecision(_Preci) << _Value;
		_KV.Value = ValueStr.str();
#endif

		ostringstream _FmtStr;
		_FmtStr << "{:";
		if (_Preci != 0) { _FmtStr << '.' << _Preci; }
		if (_Fl & (0x80000000U >> 1)) { _FmtStr << 'f'; }
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
		if (_Fl & (0x80000000U >> 1)) { ValueStr << fixed; }
		ValueStr << setprecision(_Preci) << _Value;
		_KV.Value = ValueStr.str();
#endif

		ostringstream _ValueStr;
		ostringstream _FmtStr;
		_FmtStr << "{:";
		if (_Preci != 0) { _FmtStr << '.' << _Preci; }
		if (_Fl & (0x80000000U >> 1)) { _FmtStr << 'f'; }
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
	else { Log_OS.Out("OSCStream", "ERROR", "[Table] Failed to add : " + _Key + " because its value is invalid.", OLogLevel, true); }
}

template<typename genType> requires convertible_to<genType, string>
void AddKeyValue(_SC table* _Table, string _Key, genType _Value, _SC object_ostream::fmtflags _Fl, streamsize _Preci)
{
	Log_OS.Out("OSCStream", "INFO", "[Table] Adding Key : " + _Key, OLogLevel, true);
	if (_Value != "None")
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
	if (!(_Fl & (0x80000000U >> 2)))
	{
		Log_OS.Out("OSCStream", "INFO", "[Table] Adding Key : " + _Key, OLogLevel, true);
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
	if (_Fl & (0x80000000U >> 1)) { ValueStr << _STD fixed; }
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
		if (i < Obj.Name.size() - 1) { _KV.Value += '/'; }
	}
	_KV.Value += '\"';

	table Data;
	AddKeyValue(&Data, "DiscMethod", Obj.DiscMethod, _Os.flags(), _Os.precision());
	AddKeyValue(&Data, "DiscDate", Obj.DiscDate, _Os.flags(), _Os.precision());
	AddKeyValue(&Data, "ParentBody", Obj.ParentBody, _Os.flags(), _Os.precision());
	if (Obj.Type == "Star")
	{
		AddKeyValue(&Data, "Class", Obj.SpecClass, _Os.flags(), _Os.precision());
	}
	else
	{ 
		AddKeyValue(&Data, "Class", Obj.Class, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "AsterType", Obj.AsterType, _Os.flags(), _Os.precision());
	}

	AddKeyValue(&Data, "MassKg", Obj.Mass, _Os.flags(), _Os.precision());

	if (!any(isinf(Obj.Dimensions)) && (_Os.flags() & (0x80000000U >> 4)))
	{
		if (_Os.flags() & (0x80000000U >> 5))
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

		if ((_Os.flags() & (0x80000000U >> 3)) && Obj.TidalLocked)
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
		else { Orbit.Key = "Orbit"; }

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
				AddKeyValue(&_OrbitParams, "PeriodDays", Obj.Orbit.Period / SynodicDay, _Os.flags(), _Os.precision());
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
				AddKeyValue(&_OrbitParams, "PeriodDays", Obj.Orbit.Period / SynodicDay, _Os.flags(), _Os.precision());
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

	if (Obj.EnableCustomClimate && _Os.flags() & (1 << 15))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Cliamte parameters", OLogLevel, true);
		table::KeyValue Climate;
		Climate.Key = "Climate";
		table _Climate;

		AddKeyValue(&_Climate, "AtmoProfile", Obj.Climate.AtmoProfile, _Os.flags(), _Os.precision());
		AddKeyValue(&_Climate, "MinSurfaceTemp", Obj.Climate.MinSurfaceTemp, _Os.flags(), _Os.precision());
		AddKeyValue(&_Climate, "MaxSurfaceTemp", Obj.Climate.MaxSurfaceTemp, _Os.flags(), _Os.precision());
		AddKeyValue(&_Climate, "GlobalWindSpeed", Obj.Climate.GlobalWindSpeed, _Os.flags(), _Os.precision());

		Climate.SubTable = make_shared<table>(_Climate);
		Data.push(Climate);
	}

	AddKeyValue(&Data, "NoOcean", Obj.NoOcean, _Os.flags(), _Os.precision());
	if (!Obj.NoOcean && _Os.flags() & (1 << 6))
	{
		Log_OS.Out("OSCStream", "INFO", "[" + Obj.Name[0] + "] Getting - Ocean parameters", OLogLevel, true);
		table::KeyValue Ocean;
		Ocean.Key = "Ocean";
		table _Ocean;

		AddKeyValue(&_Ocean, "Height", Obj.Ocean.Height / 1000., _Os.flags(), _Os.precision());
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
		AddKeyValue(&_Atmosphere, "Height", Obj.Atmosphere.Height / 1000., _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Density", Obj.Atmosphere.Density, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Adiabat", Obj.Atmosphere.Adiabat, _Os.flags(), _Os.precision());
		AddKeyValue(&_Atmosphere, "Pressure", Obj.Atmosphere.Pressure / float64(StdAtm), _Os.flags(), _Os.precision());
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

OSCStream& operator<<(OSCStream& _Os, _CSE Object _Obj)
{
	_Os._Buf.push(_SC MakeTable(_Os, _Obj));
    return _Os;
}

_CSE_END