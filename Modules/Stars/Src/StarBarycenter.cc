#include "CSE/Stars/StarBarycenter.h"

_CSE_BEGIN

Location::operator vec3()
{
	return vec3(RA, Dec, Dist);
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

StarBarycenter BarycenterLoader(_STD vector<_CSE _SC table::KeyValue>::iterator& it, Object* StarDetails)
{
	using _SC GetAs;
	StarBarycenter Barycen;
	ParseRADec(it, &Barycen);
	Barycen.Dist = GetAs<float64>(it, "Dist");

	return Barycen;
}

StarLocation StarLoader(::std::vector<_CSE _SC table::KeyValue>::iterator& it)
{
	return StarLocation();
}

_CSE_END
