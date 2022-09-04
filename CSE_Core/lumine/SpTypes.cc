#include "../headers/lumine/SpTypes.h"
#include <sstream>

using namespace std;

_CSE_BEGIN

// Spectal type parser
spectum::spectum(string _Str)
{
	if (_Str.find('/'))
	{
		MultipleSpectumParse(_Str);
	}
	else
	{
		SingleSpectumParse(_Str, &Cls, &Cls2, &TyMax, &TyMin, &LumMax, &LumMin, &Data);
	}
}

_CONSTEXPR20 void spectum::SingleSpectumParse(std::string _Str, SpecClass* _Cls, SpecClass* _Cls2, Type* _TyMax, Type* _TyMin, LumClass* _LumMax, LumClass* _LumMin, ExtData* _Data)
{
	auto it = size_t(0);
	auto end = _Str.size();

	auto SkipWhiteSpace = [&](size_t& it, const size_t& end)
	{
		if (it != end && isspace(_Str[it])){++it;}
	};

	SkipWhiteSpace(it, end);

	#define SP_CHECK_OUT_OF_RANGE if (it >= end){return;}

	switch (_Str[it])
	{
		// Normal Classes
	case 'O':
		if (_Str.substr(it, 4) == "Ofpe") { *_Cls = Ofpe; it += 4; }
		else { *_Cls = O; ++it; }
		break;
	case 'B':
		if (_Str.substr(it, 9) == "BlackHole") { *_Cls = X; ++it; }
		else { *_Cls = B; ++it; }
		break;
	case 'A':
		*_Cls = A;
		++it;
		break;
	case 'F':
		*_Cls = F;
		++it;
		break;
	case 'G':
		*_Cls = G;
		++it;
		break;
	case 'K':
		*_Cls = K;
		++it;
		break;
	case 'M':
		if (_Str.substr(it, 2) == "MS") { *_Cls = MS; it += 2; }
		else { *_Cls = M; ++it; }
		break;

		// Wolf-Rayet Stars
	case 'W':
		if (_Str.substr(it, 3) == "WNC") { *_Cls = WNC; it += 3; }
		else if (_Str.substr(it, 4) == "WN/C") { *_Cls = WNC; it += 4; }
		else if (_Str.substr(it, 2) == "WN") { *_Cls = WN; it += 2; }
		else if (_Str.substr(it, 2) == "WC") { *_Cls = WC; it += 2; }
		else if (_Str.substr(it, 2) == "WO") { *_Cls = WO; it += 2; }
		else if (_Str.substr(it, 2) == "WD") { *_LumMax = WD; it += 2; }
		break;

		// Sub-stellar Objects
	case 'L':
		*_Cls = L;
		++it;
		break;
	case 'T':
		*_Cls = T;
		++it;
		break;
	case 'Y':
		*_Cls = Y;
		++it;
		break;

		// Carbon Stars
	case 'C':
		if (_Str.substr(it, 3) == "C-R") { *_Cls = C_R; it += 3; }
		else if (_Str.substr(it, 3) == "C-N") { *_Cls = C_N; it += 3; }
		else if (_Str.substr(it, 3) == "C-J") { *_Cls = C_J; it += 3; }
		else if (_Str.substr(it, 4) == "C-Hd") { *_Cls = C_H; it += 4; }
		else if (_Str.substr(it, 3) == "C-H") { *_Cls = C_Hd; it += 3; }
		else { *_Cls = C_R; ++it; }
		break;
	case 'S':
		if (_Str.substr(it, 2) == "SC") { *_Cls = SC; it += 2; }
		else { *_Cls = S; ++it; }
		break;
	case 'R':
		*_Cls = C_R;
		++it;
		break;
	case 'N':
		if (_Str.substr(it, 7) == "Neutron") { *_Cls = Q; it += 7; }
		else { *_Cls = C_N; ++it; }
		break;
	case 'J':
		*_Cls = C_J;
		++it;
		break;
	case 'H':
		if (_Str.substr(it, 2) == "Hd") { *_Cls = C_Hd; it += 2; }
		else { *_Cls = C_H; ++it; }
		break;

		// White Dwarfs
	case 'D':
		if (_Str.substr(it, 2) == "DA") { *_Cls = DA; it += 2; }
		else if (_Str.substr(it, 2) == "DB") { *_Cls = DB; it += 2; }
		else if (_Str.substr(it, 2) == "DO") { *_Cls = DO; it += 2; }
		else if (_Str.substr(it, 2) == "DQ") { *_Cls = DQ; it += 2; }
		else if (_Str.substr(it, 2) == "DZ") { *_Cls = DZ; it += 2; }
		else if (_Str.substr(it, 2) == "DC") { *_Cls = DC; it += 2; }
		else if (_Str.substr(it, 2) == "DX") { *_Cls = DX; it += 2; }
		*_LumMax = WD;
		break;

		// Stellar remnants
	case 'Q':
		*_Cls = Q;
		++it;
		break;
	case 'X':
		*_Cls = X;
		++it;
		break;

		// Others
	case 's':
		if (_Str.substr(it, 2) == "sd") { *_LumMax = sd; it += 2; }
		break;
	default:
		*_Cls = Exotic;
		break;
	}

	SP_CHECK_OUT_OF_RANGE

	if (*_LumMax == sd)
	{
		switch (_Str[it])
		{
		case 'O':
			*_Cls = O;
			++it;
			break;
		case 'B':
			*_Cls = B;
			++it;
			break;
		case 'A':
			*_Cls = A;
			++it;
			break;
		case 'F':
			*_Cls = F;
			++it;
			break;
		case 'G':
			*_Cls = G;
			++it;
			break;
		case 'K':
			*_Cls = K;
			++it;
			break;
		case 'M':
			*_Cls = M;
			++it;
			break;
		}
	}

	SP_CHECK_OUT_OF_RANGE

	if (*_LumMax == WD)
	{
		auto AddWDClass = [&](SpecClass _Cl)
		{
			if (*_Cls == -1){*_Cls = _Cl;}
			else { *_Cls = _Cl; }
		};

		switch (_Str[it])
		{
		case 'A':
			AddWDClass(DA);
			++it;
			break;
		case 'B':
			AddWDClass(DB);
			++it;
			break;
		case 'O':
			AddWDClass(DO);
			++it;
			break;
		case 'Q':
			AddWDClass(DQ);
			++it;
			break;
		case 'Z':
			AddWDClass(DZ);
			++it;
			break;
		case 'C':
			AddWDClass(DC);
			++it;
			break;
		case 'X':
			AddWDClass(DX);
			++it;
			break;
		}
	}

	SkipWhiteSpace(it, end);
	SP_CHECK_OUT_OF_RANGE

	_STD string _NBuf;
	while (isdigit(_Str[it]) || _Str[it] == '.')
	{
		_NBuf += _Str[it];
		++it;
	}
	istringstream _ToFloat(_NBuf);
	_ToFloat >> *_TyMin;
	_NBuf.clear();

	SP_CHECK_OUT_OF_RANGE

	if (_Str[it] == '-')
	{
		++it;
		while (isdigit(_Str[it]) || _Str[it] == '.')
		{
			_NBuf += _Str[it];
			++it;
		}
		istringstream _ToFloat(_NBuf);
		_ToFloat >> *_TyMax;
		_NBuf.clear();
	}

	SkipWhiteSpace(it, end);
	SP_CHECK_OUT_OF_RANGE

	switch (_Str[it])
	{
	case 'I':
		if (_Str.substr(it, 2) == "IV") { *_LumMax = IV; it += 2; }
		else if (_Str.substr(it, 3) == "III") { *_LumMax = III; it += 3; }
		else if (_Str.substr(it, 2) == "II") { *_LumMax = II; it += 2; }
		else if (_Str.substr(it, 2) == "Ib") { *_LumMax = Ib; it += 2; }
		else if (_Str.substr(it, 3) == "Iab") { *_LumMax = Iab; it += 3; }
		else if (_Str.substr(it, 3) == "Ia+") { *_LumMax = I0; it += 3; }
		else if (_Str.substr(it, 2) == "Ia") { *_LumMax = Ia; it += 2; }
		else { *_LumMax = Ia; ++it; }
		break;

	case 'V':
		if (_Str.substr(it, 2) == "VI") { *_LumMax = sd; it += 2; }
		else { *_LumMax = V; ++it; }

	default:
		break;
	}

	SP_CHECK_OUT_OF_RANGE

	if (_Str[it] == '-')
	{
		++it;
		switch (_Str[it])
		{
		case 'I':
			if (_Str.substr(it, 2) == "IV") { *_LumMin = IV; it += 2; }
			else if (_Str.substr(it, 3) == "III") { *_LumMin = III; it += 3; }
			else if (_Str.substr(it, 2) == "II") { *_LumMin = II; it += 2; }
			else if (_Str.substr(it, 2) == "Ib") { *_LumMin = Ib; it += 2; }
			else if (_Str.substr(it, 3) == "Iab") { *_LumMin = Iab; it += 3; }
			else if (_Str.substr(it, 3) == "Ia+") { *_LumMin = I0; it += 3; }
			else if (_Str.substr(it, 2) == "Ia") { *_LumMin = Ia; it += 2; }
			else { *_LumMin = Ia; ++it; }
			break;

		case 'V':
			if (_Str.substr(it, 2) == "VI") { *_LumMin = sd; it += 2; }
			else { *_LumMin = V; ++it; }

		default:
			break;
		}
	}

	SP_CHECK_OUT_OF_RANGE

	while (it < end && _Str[it] != '/')
	{
		*_Data += _Str[it];
		++it;
	}
}

void spectum::MultipleSpectumParse(std::string _MultiStr)
{
	istringstream _Is(_MultiStr);
	string _SStr;
	string _Sp[3];
	uint8_t _Cnt = 0;
	while (getline(_Is, _SStr, '/'))
	{
		_Sp[_Cnt] = _SStr;
		++_Cnt;
	}
	if (!_Sp[0].empty())
	{
		SingleSpectumParse(_Sp[0], &Cls, nullptr, &TyMax, &TyMin, &LumMax, &LumMin, &Data);
	}
	if (!_Sp[1].empty())
	{
		SingleSpectumParse(_Sp[1], &Cls2, nullptr, &Ty2Max, &Ty2Min, &Lum2Max, &Lum2Min, &Data2);
	}
	if (!_Sp[2].empty())
	{
		SingleSpectumParse(_Sp[2], &Cls3, nullptr, &Ty3Max, &Ty3Min, &Lum3Max, &Lum3Min, &Data3);
	}
}

_CSE_END