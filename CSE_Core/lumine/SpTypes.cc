#include "../headers/lumine/SpTypes.h"
#include <sstream>
#include <format>

using namespace std;

_CSE_BEGIN

// Spectal type parser
spectum::spectum(string _Str){ Parser(_Str); }

spectum::spectum(const char* _Str) { Parser(_Str); }

string spectum::str()
{
	if (this->empty()) { return ""; }
	if (LumMax == WD) { return to_WDstr(&Cls, &Cls2, &TyMin, &TyMax, &LumMax, &LumMin, &Data); }
	if (LumMax == sd) { return to_sdstr(&Cls, &TyMin, &TyMax, &Data); }

	string _Str;

	if (!empty(1))
	{
		_Str += to_str(&Cls, &TyMin, &TyMax, &LumMax, &LumMin, &Data);
		if (!empty(2))
		{
			_Str += '/' + to_str(&Cls2, &Ty2Min, &Ty2Max, &Lum2Max, &Lum2Min, &Data2);
		}
	}

	return _Str;
}

bool spectum::empty(uint8_t _Arg)
{
	auto Cls1Empty = Cls == static_cast<SpecClass>(-1);
	auto TyMax1Empty = TyMax == static_cast<Type>(-1);
	auto TyMin1Empty = TyMin == static_cast<Type>(-1);
	auto LumMax1Empty = LumMax == static_cast<LumClass>(-1);
	auto LumMin1Empty = LumMin == static_cast<LumClass>(-1);

	auto Cls2Empty = Cls2 == static_cast<SpecClass>(-1);
	auto TyMax2Empty = Ty2Max == static_cast<Type>(-1);
	auto TyMin2Empty = Ty2Min == static_cast<Type>(-1);
	auto LumMax2Empty = Lum2Max == static_cast<LumClass>(-1);
	auto LumMin2Empty = Lum2Min == static_cast<LumClass>(-1);

	auto Cls3Empty = Cls3 == static_cast<SpecClass>(-1);
	auto TyMax3Empty = Ty3Max == static_cast<Type>(-1);
	auto TyMin3Empty = Ty3Min == static_cast<Type>(-1);
	auto LumMax3Empty = Lum3Max == static_cast<LumClass>(-1);
	auto LumMin3Empty = Lum3Min == static_cast<LumClass>(-1);

	auto _Empty1 = Cls1Empty && (TyMax1Empty && TyMin1Empty) && (LumMax1Empty && LumMin1Empty);
	auto _Empty2 = Cls2Empty && (TyMax2Empty && TyMin2Empty) && (LumMax2Empty && LumMin2Empty);
	auto _Empty3 = Cls3Empty && (TyMax3Empty && TyMin3Empty) && (LumMax3Empty && LumMin3Empty);

	switch (_Arg)
	{
	case 0:
		return _Empty1 && _Empty2 && _Empty3;
		break;
	case 1:
		return _Empty1;
		break;
	case 2:
		return _Empty2;
		break;
	case 3:
		return _Empty3;
		break;
	default:
		return true;
		break;
	}
}

bool spectum::NoLumClass(uint8_t _Arg)
{
	auto LumMax1Empty = LumMax == static_cast<LumClass>(-1);
	auto LumMin1Empty = LumMin == static_cast<LumClass>(-1);

	auto LumMax2Empty = Lum2Max == static_cast<LumClass>(-1);
	auto LumMin2Empty = Lum2Min == static_cast<LumClass>(-1);

	auto LumMax3Empty = Lum3Max == static_cast<LumClass>(-1);
	auto LumMin3Empty = Lum3Min == static_cast<LumClass>(-1);

	auto _Empty1 = (LumMax1Empty && LumMin1Empty);
	auto _Empty2 = (LumMax2Empty && LumMin2Empty);
	auto _Empty3 = (LumMax3Empty && LumMin3Empty);

	switch (_Arg)
	{
	case 0:
		return _Empty1 && _Empty2 && _Empty3;
		break;
	case 1:
		return _Empty1;
		break;
	case 2:
		return _Empty2;
		break;
	case 3:
		return _Empty3;
		break;
	default:
		return true;
		break;
	}
}

void spectum::Parser(string _Str)
{
	if (_Str.find('/') != _Str.npos)
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
		}
	}

	SP_CHECK_OUT_OF_RANGE

	if (*_LumMax == WD)
	{
		auto AddWDClass = [&](SpecClass _Cl)
		{
			if (*_Cls == -1){*_Cls = _Cl;}
			else if (_Cls2) { *_Cls2 = _Cl; }
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

	//SkipWhiteSpace(it, end);
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

	if (*_Cls == WN)
	{
		switch (_Str[it])
		{
		case 'E':
			*_Cls = WNE;
			++it;
			break;
		case 'L':
			*_Cls = WNL;
			++it;
			break;
		case 'h':
			if (_Str.substr(it, 2) == "ha") { *_Cls = WNha; it += 2; }
			else { *_Cls = WNh; ++it; }
		default:
			break;
		}
	}
	else if (*_Cls == WC)
	{
		switch (_Str[it])
		{
		case 'E':
			*_Cls = WCE;
			++it;
			break;
		case 'L':
			*_Cls = WCL;
			++it;
			break;
		case 'd':
			*_Cls = WCd;
			++it;
			break;
		default:
			break;
		}
	}
	else
	{
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
			break;

		case '0':
			*_LumMax = I0;
			++it;
			break;

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

string _GenTyFmtStr(spectum::Type _Ty)
{
	if (_Ty > 0 && _Ty < 1) { return "{:.1g}"; }
	else { return "{:.2g}"; }
}

string spectum::to_str(const SpecClass* _Spec, const Type* _Ty1, const Type* _Ty2, const LumClass* _Lum1, const LumClass* _Lum2, const ExtData* _Data)
{
	string _Str;
	if (_Ty1 && *_Ty1 != -1)
	{
		string _FormatStr = _SpClassFmtStrings[*_Spec];
		string _Ty;

		if (_Ty2 && *_Ty2 != -1){_Ty = vformat(_GenTyFmtStr(*_Ty1) + '-' + _GenTyFmtStr(*_Ty2), make_format_args(*_Ty1, *_Ty2)); }
		else{_Ty = vformat(_GenTyFmtStr(*_Ty1), make_format_args(*_Ty1));}
		_Str += vformat(_FormatStr, make_format_args(_Ty));
	}
	else { _Str += _SpClassNoFmtStrings[*_Spec]; }
	
	if (_Lum1 && *_Lum1 != -1)
	{
		if (_Lum2 && *_Lum2 != -1)
		{
			string _Lum = vformat("{}-{}", make_format_args(_LumClassStrings[*_Lum1], _LumClassStrings[*_Lum2]));
			_Str += _Lum;
		}
		else
		{
			_Str += _LumClassStrings[*_Lum1];
		}
	}

	if (!_Data->empty()) { _Str += *_Data; }
	return _Str;
}

string spectum::to_WDstr(const SpecClass* _Spec, const SpecClass* _Spec2, const Type* _Ty1, const Type* _Ty2, const LumClass* _Lum1, const LumClass* _Lum2, const ExtData* _Data)
{
	if (*_Spec == D)
	{
		return "WD";
	}

	string _Str = "D";
	_Str += _SpClassNoFmtStrings[*_Spec];
	if (_Spec2 && *_Spec2 != -1) { _Str += _SpClassNoFmtStrings[*_Spec2]; }
	if (_Ty1 && *_Ty1 != -1)
	{
		string _FormatStr = "{}";
		string _Ty;
		if (_Ty2 && *_Ty2 != -1) { _Ty = vformat(_GenTyFmtStr(*_Ty1) + '-' + _GenTyFmtStr(*_Ty2), make_format_args(*_Ty1, *_Ty2)); }
		else { _Ty = vformat(_GenTyFmtStr(*_Ty1), make_format_args(*_Ty1)); }
		_Str += vformat(_FormatStr, make_format_args(_Ty));
	}

	return _Str;
}

string spectum::to_sdstr(const SpecClass* _Spec, const Type* _Ty1, const Type* _Ty2, const ExtData* _Data)
{
	string _Str;
	if (_Ty1 && *_Ty1 != -1)
	{
		string _FormatStr = "sd" + _SpClassFmtStrings[*_Spec];
		string _Ty;

		if (_Ty2 && *_Ty2 != -1) { _Ty = vformat(_GenTyFmtStr(*_Ty1) + '-' + _GenTyFmtStr(*_Ty2), make_format_args(*_Ty1, *_Ty2)); }
		else { _Ty = vformat(_GenTyFmtStr(*_Ty1), make_format_args(*_Ty1)); }
		_Str += vformat(_FormatStr, make_format_args(_Ty));
	}
	else
	{
		_Str = "sd" + _SpClassNoFmtStrings[*_Spec];
		_Str.pop_back();
	}

	if (!_Data->empty()) { _Str += *_Data; }
	return _Str;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Stellar Classification Function Declarations
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

bool IsGiant(SPECSTR _Spec)
{
	auto _NLumClass = [&](SPECSTR::LumClass _Cl)
	{
		return _Cl >= 0 && _Cl <= 5;
	};

	return _NLumClass(_Spec.LumMax) || _NLumClass(_Spec.LumMin) ||
		_NLumClass(_Spec.Lum2Max) || _NLumClass(_Spec.Lum2Min) ||
		_NLumClass(_Spec.Lum3Max) || _NLumClass(_Spec.Lum3Min);
}

bool IsNormalGiant(SPECSTR _Spec)
{
	auto _NLumClass = [&](SPECSTR::LumClass _Cl)
	{
		return _Cl == 5;
	};

	return _NLumClass(_Spec.LumMax) || _NLumClass(_Spec.LumMin) ||
		_NLumClass(_Spec.Lum2Max) || _NLumClass(_Spec.Lum2Min) ||
		_NLumClass(_Spec.Lum3Max) || _NLumClass(_Spec.Lum3Min);
}

bool IsBrightGiant(SPECSTR _Spec)
{
	auto _NLumClass = [&](SPECSTR::LumClass _Cl)
	{
		return _Cl == 4;
	};

	return _NLumClass(_Spec.LumMax) || _NLumClass(_Spec.LumMin) ||
		_NLumClass(_Spec.Lum2Max) || _NLumClass(_Spec.Lum2Min) ||
		_NLumClass(_Spec.Lum3Max) || _NLumClass(_Spec.Lum3Min);
}

bool IsSuperGiant(SPECSTR _Spec)
{
	auto _NLumClass = [&](SPECSTR::LumClass _Cl)
	{
		return _Cl >= 1 && _Cl <= 3;
	};

	return _NLumClass(_Spec.LumMax) || _NLumClass(_Spec.LumMin) ||
		_NLumClass(_Spec.Lum2Max) || _NLumClass(_Spec.Lum2Min) ||
		_NLumClass(_Spec.Lum3Max) || _NLumClass(_Spec.Lum3Min);
}

bool IsHyperGiant(SPECSTR _Spec)
{
	auto _NLumClass = [&](SPECSTR::LumClass _Cl)
	{
		return _Cl == 0;
	};

	return _NLumClass(_Spec.LumMax) || _NLumClass(_Spec.LumMin) ||
		_NLumClass(_Spec.Lum2Max) || _NLumClass(_Spec.Lum2Min) ||
		_NLumClass(_Spec.Lum3Max) || _NLumClass(_Spec.Lum3Min);
}

bool IsSubGiant(SPECSTR _Spec)
{
	auto _NLumClass = [&](SPECSTR::LumClass _Cl)
	{
		return _Cl == 6;
	};

	return _NLumClass(_Spec.LumMax) || _NLumClass(_Spec.LumMin) ||
		_NLumClass(_Spec.Lum2Max) || _NLumClass(_Spec.Lum2Min) ||
		_NLumClass(_Spec.Lum3Max) || _NLumClass(_Spec.Lum3Min);
}

bool IsMainSequence(SPECSTR _Spec)
{
	auto _NLumClass = [&](SPECSTR::LumClass _Cl)
	{
		return _Cl == 7;
	};

	return _NLumClass(_Spec.LumMax) || _NLumClass(_Spec.LumMin) ||
		_NLumClass(_Spec.Lum2Max) || _NLumClass(_Spec.Lum2Min) ||
		_NLumClass(_Spec.Lum3Max) || _NLumClass(_Spec.Lum3Min);
}

bool IsSubDwarf(SPECSTR _Spec)
{
	auto _NLumClass = [&](SPECSTR::LumClass _Cl)
	{
		return _Cl == 8;
	};

	return _NLumClass(_Spec.LumMax) || _NLumClass(_Spec.LumMin) ||
		_NLumClass(_Spec.Lum2Max) || _NLumClass(_Spec.Lum2Min) ||
		_NLumClass(_Spec.Lum3Max) || _NLumClass(_Spec.Lum3Min);
}

bool IsOType(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 0;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) ||_NSpecClass(_Spec.Cls3);
}

bool IsBType(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 1;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsAType(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 2;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsFType(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 3;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsGType(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 4;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsKType(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 5;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsMType(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 6;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsWolfRayet(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl >= 7 && _Cl <= 19;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsBrownDwarf(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl >= 20 && _Cl <= 22;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsLType(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 20;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsTType(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 21;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsYType(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 22;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsCarbonStar(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl >= 23 && _Cl <= 30;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsWhiteDwarf(SPECSTR _Spec)
{
	auto _NLumClass = [&](SPECSTR::LumClass _Cl)
	{
		return _Cl == 9;
	};

	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl >= 31 && _Cl <= 38;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3) || 
		_NLumClass(_Spec.LumMax) || _NLumClass(_Spec.LumMin) ||
		_NLumClass(_Spec.Lum2Max) || _NLumClass(_Spec.Lum2Min) ||
		_NLumClass(_Spec.Lum3Max) || _NLumClass(_Spec.Lum3Min);;
}

bool IsNeutronStar(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 39;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsBlackHole(SPECSTR _Spec)
{
	auto _NSpecClass = [&](SPECSTR::SpecClass _Cl)
	{
		return _Cl == 40;
	};

	return _NSpecClass(_Spec.Cls) || _NSpecClass(_Spec.Cls2) || _NSpecClass(_Spec.Cls3);
}

bool IsStarRemnant(SPECSTR _Spec)
{
	return IsWhiteDwarf(_Spec.Cls) || IsNeutronStar(_Spec.Cls2) || IsBlackHole(_Spec.Cls3);
}

_CSE_END