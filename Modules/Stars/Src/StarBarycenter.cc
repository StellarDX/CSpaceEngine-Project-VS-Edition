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

_CSE_END