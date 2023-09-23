#include "CSE/Stars/StarBarycenter.h"
#include "CSE/Lumine.h"

_CSE_BEGIN

Location::operator vec3()
{
	return vec3(RA, Dec, Dist);
}

Object& StarLocation::operator*() const noexcept
{
	return *(Pointer.get());
}

Object* StarLocation::operator->() const noexcept
{
	return Pointer.get();
}

// Star and StarBarycenter parse

StarBarycenter GetBarycenter(ISCStream _Is, _STD string _Name, Object* StarDetails)
{
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
	Log_IS.Out("Barycenter Loader", "INFO", "[Barycenter loader] Loading object - " + _Name, ILogLevel, true);
	return BarycenterLoader(it, StarDetails);
}

StarLocation GetSEStar(ISCStream _Is, _STD string _Name)
{
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
	Log_IS.Out("Barycenter Loader", "INFO", "[Barycenter loader] Loading object - " + _Name, ILogLevel, true);
	return StarLoader(it);
}

void ParseRADec(_STD vector<_CSE _SC table::KeyValue>::iterator& it, StarBarycenter* Bar)
{
	using _SC GetAs;
	float64 RAh, RAm, RAs, Decd, Decm, Decs;
	uint64 RAVCnt = 0, DecVCnt = 0;
	auto RAIt = it->SubTable->find("RA"), DecIt = it->SubTable->find("Dec");
	RAh = GetAs<float64>(it, "RA");
	Decd = GetAs<float64>(it, "Dec");
	if (it->SubTable)
	{
		RAm = GetAs<float64>(RAIt, "UnnamedValue#1");
		RAs = GetAs<float64>(RAIt, "UnnamedValue#2");
		Decm = GetAs<float64>(DecIt, "UnnamedValue#1");
		Decs = GetAs<float64>(DecIt, "UnnamedValue#2");
	}
	else
	{
		RAVCnt = 1;
		DecVCnt = 1;
	}

	if (!RAVCnt && !DecVCnt)
	{
		RAVCnt = isinf(RAs) ? (isinf(RAm) ? 1 : 2) : 3;
		DecVCnt = isinf(Decs) ? (isinf(Decm) ? 1 : 2) : 3;
	}

	// Parse RA
	switch (RAVCnt)
	{
	case 1:
	default:
		Bar->RA = RAh;
		break;
	case 2:
		RAs = (RAm - int(RAm)) * 60;
	case 3:
		Bar->RA = UCoordinate24(uint64(RAh), uint64(RAm), RAs);
		break;
	}

	// Parse Dec
	switch (DecVCnt)
	{
	case 1:
	default:
		Bar->Dec = Decd;
		break;
	case 2:
		Decs = (Decm - int(Decm)) * 60;
	case 3:
		Bar->Dec = Coordinate90(int64(Decd), uint64(Decm), Decs);
		break;
	}
}

void ParseStarDetails(_STD vector<_CSE _SC table::KeyValue>::iterator& it, Object* StarDetails)
{
	StarDetails->Type = "Star";
	StarDetails->Name = _SC ValueStr(it->Value).as<_SC Names>()->get();
	StarDetails->Class = "Sun";
	StarDetails->SpecClass = GetAs<_STD string>(it, "Class");

	auto FoundMassKg = it->SubTable->find("MassKg") != it->SubTable->end();
	auto FoundMassEarth = it->SubTable->find("Mass") != it->SubTable->end();
	auto FoundMassSol = it->SubTable->find("MassSol") != it->SubTable->end();
	if (FoundMassKg) { StarDetails->Mass = GetAs<float64>(it, "MassKg"); }
	else if (FoundMassEarth) { StarDetails->Mass = MassEarth * GetAs<float64>(it, "Mass"); }
	else if (FoundMassSol) { StarDetails->Mass = MassSol * GetAs<float64>(it, "MassSol"); }

	auto FoundRadius = it->SubTable->find("Radius") != it->SubTable->end();
	auto FoundDimensions = it->SubTable->find("Dimensions") != it->SubTable->end();
	auto FoundRadSol = it->SubTable->find("RadSol") != it->SubTable->end();
	auto FoundRadiusSol = it->SubTable->find("RadiusSol") != it->SubTable->end();
	auto FoundDimSol = it->SubTable->find("DimensionsSol") != it->SubTable->end();
	auto FoundOblateness = it->SubTable->find("Oblateness") != it->SubTable->end();
	if (FoundRadius) { StarDetails->Dimensions = 2. * 1000. * vec3(GetAs<float64>(it, "Radius")); }
	else if (FoundRadSol) { StarDetails->Dimensions = 2. * float64(RadSol) * vec3(GetAs<float64>(it, "RadSol")); }
	else if (FoundRadiusSol) { StarDetails->Dimensions = 2. * float64(RadSol) * vec3(GetAs<float64>(it, "RadiusSol")); }
	if (FoundOblateness)
	{
		auto OblateVector = it->SubTable->find("Oblateness")->Value.find('(') != _STD string::npos;
		if (OblateVector)
		{
			vec3 _Q = GetAs<vec3, float64>(it, "Oblateness");
			StarDetails->Dimensions -= StarDetails->Dimensions * _Q;
		}
		else
		{
			float64 _Q = GetAs<float64>(it, "Oblateness");
			StarDetails->Dimensions.y -= StarDetails->Dimensions.y * _Q;
		}
	}
	if (FoundDimensions) { StarDetails->Dimensions = 1000. * GetAs<vec3, float64>(it, "Dimensions"); }
	else if (FoundDimSol) { StarDetails->Dimensions = float64(RadSol) * GetAs<vec3, float64>(it, "DimensionsSol"); }

	StarDetails->Luminosity = GetAs<float64>(it, "Luminosity") * SolarLum;
	StarDetails->LumBol = GetAs<float64>(it, "LumBol") * SolarLum;
	StarDetails->Teff = GetAs<float64>(it, "Teff");
	StarDetails->FeH = GetAs<float64>(it, "FeH");
	StarDetails->CtoO = GetAs<float64>(it, "CtoO");
	StarDetails->Age = GetAs<float64>(it, "Age") * 1E+9;

	auto FoundAppMagn = it->SubTable->find("AppMagn") != it->SubTable->end();
	auto FoundAbsMagn = it->SubTable->find("AbsMagn") != it->SubTable->end();
	if (FoundAppMagn && !FoundAbsMagn)
	{
		StarDetails->AbsMagn = ToAbsMagn1(GetAs<float64>(it, "AppMagn"), GetAs<float64>(it, "Dist"));
	}
	else { StarDetails->AbsMagn = GetAs<float64>(it, "AbsMagn"); }
}

StarBarycenter BarycenterLoader(_STD vector<_CSE _SC table::KeyValue>::iterator& it, Object* StarDetails)
{
	using _SC GetAs;
	StarBarycenter Barycen;
	Barycen.Name = _SC ValueStr(it->Value).as<_SC Names>()->get();
	Barycen.CenterOf = GetAs<_STD string>(it, "CenterOf");
	ParseRADec(it, &Barycen);
	Barycen.Dist = GetAs<float64>(it, "Dist");
	if (StarDetails) { ParseStarDetails(it, StarDetails); }
	return Barycen;
}

StarLocation StarLoader(::std::vector<_CSE _SC table::KeyValue>::iterator& it)
{
	using _SC GetAs;
	StarLocation Barycen;
	Barycen.Name = _SC ValueStr(it->Value).as<_SC Names>()->get();
	Barycen.CenterOf = GetAs<_STD string>(it, "CenterOf");
	ParseRADec(it, &Barycen);
	Barycen.Dist = GetAs<float64>(it, "Dist");
	Object Obj;
	ParseStarDetails(it, &Obj);
	Barycen.Pointer = _STD make_shared<Object>(Obj);
	return Barycen;
}

// OStream capabilities

void AddEmptyTag(_SC table* _Table);

void AddKeyValue(_SC table* _Table, _STD string _Key, float64 _Value, _SC object_ostream::fmtflags _Fl, _STD streamsize _Preci);

template<typename genType> requires (vecType<genType> || vecIType<genType> || vecUType<genType>)
void AddKeyValue(_SC table* _Table, _STD string _Key, genType _Value, _SC object_ostream::fmtflags _Fl, _STD streamsize _Preci);

template<typename genType> requires _STD convertible_to<genType, _STD string>
void AddKeyValue(_SC table* _Table, _STD string _Key, genType _Value, _SC object_ostream::fmtflags _Fl, _STD streamsize _Preci);

void AddKeyValue(_SC table* _Table, _STD string _Key, bool _Value, _SC object_ostream::fmtflags _Fl, _STD streamsize _Preci);

template<typename CoordType>
void AddRADec(_SC table* _Table, _STD string _Key, CoordType _Value, bool _Sign, _SC object_ostream::fmtflags _Fl, _STD streamsize _Preci)
{
	Log_OS.Out("OSCStream", "INFO", "[Table] Adding Key : " + _Key, OLogLevel, true);
	if (!isinf(_Value))
	{
		_SC table::KeyValue _KV;
		_KV.Key = _Key;
		_STD ostringstream _FmtStr;
		_FmtStr << "{:";
		if (_Sign) { _FmtStr << "+"; }
		_FmtStr << "02} {:02} ";
		_FmtStr << "{:";
		if (_Preci != 0) { _FmtStr << '.' << _Preci; }
		if (_Fl & (0x80000000U >> 1)) { _FmtStr << 'f'; }
		else { _FmtStr << 'g'; }
		_FmtStr << '}';
		_KV.Value = _STD vformat(_FmtStr.str(), _STD make_format_args((_Value.neg()?-1.:1.)*_Value.deg(), _Value.min(), _Value.sec()));

		_Table->push(_KV);
	}
	else { Log_OS.Out("OSCStream", "ERROR", "[Table] Failed to add : " + _Key + " because its value is invalid.", OLogLevel, true); }
}

_SC_BEGIN

table::KeyValue MakeTable(starbarycen_ostream& _Os, StarBarycenter Bar)
{
	using _SC table;
	table::KeyValue _KV;

	_KV.Key = "StarBarycenter";
	_KV.Value += '\"';
	for (size_t i = 0; i < Bar.Name.size(); i++)
	{
		_KV.Value += Bar.Name[i];
		if (i < Bar.Name.size() - 1) { _KV.Value += '/'; }
	}
	_KV.Value += '\"';

	table Data;
	AddKeyValue(&Data, "CenterOf", Bar.CenterOf, _Os.flags(), _Os.precision());
	if (_Os.flags() & (0x80000000U >> 2))
	{
		AddKeyValue(&Data, "RA", Bar.RA, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "Dec", Bar.Dec, _Os.flags(), _Os.precision());
	}
	else
	{
		AddRADec(&Data, "RA", Bar.RA, 0, _Os.flags(), _Os.precision());
		AddRADec(&Data, "Dec", Bar.Dec, 1, _Os.flags(), _Os.precision());
	}
	AddKeyValue(&Data, "Dist", Bar.Dist, _Os.flags(), _Os.precision());

	_KV.SubTable = _STD make_shared<table>(Data);
	return _KV;
}

table::KeyValue MakeTable(starbarycen_ostream& _Os, StarLocation Bar)
{
	using _SC table;
	table::KeyValue _KV;

	_KV.Key = "Star";
	_KV.Value += '\"';
	for (size_t i = 0; i < Bar.Name.size(); i++)
	{
		_KV.Value += Bar.Name[i];
		if (i < Bar.Name.size() - 1) { _KV.Value += '/'; }
	}
	_KV.Value += '\"';

	table Data;

	AddKeyValue(&Data, "CenterOf", Bar.CenterOf, _Os.flags(), _Os.precision());

	// Observation
	if (_Os.flags() & (0x80000000U >> 2))
	{
		AddKeyValue(&Data, "RA", Bar.RA, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "Dec", Bar.Dec, _Os.flags(), _Os.precision());
	}
	else
	{
		AddRADec(&Data, "RA", Bar.RA, 0, _Os.flags(), _Os.precision());
		AddRADec(&Data, "Dec", Bar.Dec, 1, _Os.flags(), _Os.precision());
	}
	if (Bar.Pointer && !isinf(Bar->AbsMagn)) { AddKeyValue(&Data, "AppMagn", ToAppMagn1(Bar->AbsMagn, Bar.Dist), _Os.flags(), _Os.precision()); }
	AddEmptyTag(&Data);

	// Characteristics
	if (Bar.Pointer) { AddKeyValue(&Data, "Class", Bar->SpecClass, _Os.flags(), _Os.precision()); }
	AddEmptyTag(&Data);

	// Astrometry
	AddKeyValue(&Data, "Dist", Bar.Dist, _Os.flags(), _Os.precision());
	if (Bar.Pointer) { AddKeyValue(&Data, "AbsMagn", Bar->AbsMagn, _Os.flags(), _Os.precision()); }
	AddEmptyTag(&Data);
	
	// Details
	if (Bar.Pointer)
	{
		AddKeyValue(&Data, "MassSol", Bar->Mass / MassSol, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "Radius", (max({ Bar->Dimensions.x, Bar->Dimensions.y, Bar->Dimensions.z })/2.) / 1000., _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "Luminosity", Bar->Luminosity / SolarLum, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "LumBol", Bar->LumBol / SolarLum, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "Teff", Bar->Teff, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "FeH", Bar->FeH, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "CtoO", Bar->CtoO, _Os.flags(), _Os.precision());
		AddKeyValue(&Data, "Age", Bar->Age, _Os.flags(), _Os.precision());
	}

	_KV.SubTable = _STD make_shared<table>(Data);
	return _KV;
}

_SC_END

OBarycenterStream& operator<<(OBarycenterStream& _Os, _CSE StarBarycenter _Obj)
{
	_Os._Buf.push(_SC MakeTable(_Os, _Obj));
	return _Os;
}

OBarycenterStream& operator<<(OBarycenterStream& _Os, _CSE StarLocation _Obj)
{
	_Os._Buf.push(_SC MakeTable(_Os, _Obj));
	return _Os;
}

_CSE_END