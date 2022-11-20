#ifndef __MATHFUNCS_TEMP__
#define __MATHFUNCS_TEMP__

#include "../headers/math/CSEM_Func.h"

_CSE_BEGIN

////////////////////////////////////// EXP /////////////////////////////////////

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl exp(_In_ genType _X, EXP_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE exp(_X[i], _Conf); }
	return _X;
}

////////////////////////////////////// LOG /////////////////////////////////////

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl log(_In_ float64 _Base, _In_ genType _X, LOG_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE log(_Base, _X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl log(_In_ genType _Base, _In_ genType _X, LOG_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE log(_Base[i], _X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl log(_In_ genType _X, LOG_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE log(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl ln(_In_ genType _X, LOG_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE ln(_X[i], _Conf); }
	return _X;
}

////////////////////////////////////// POWER /////////////////////////////////////

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl pow(_In_ genType _X, _In_ float64 _Power, POWER_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE pow(_X[i], _Power, _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl pow(_In_ genType _X, _In_ genType _Power, POWER_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE pow(_X[i], _Power[i], _Conf); }
	return _X;
}

template<typename _Ty, size_t _Size>
_Check_return_ _GL basic_matrix<_Ty, _Size, _Size> __cdecl pow(_In_ _GL basic_matrix<_Ty, _Size, _Size> _A, _In_ uint64 _Power)
{
	_STL_VERIFY(_Power > 0, "matrix power can't be zero.");
	_GL basic_matrix<_Ty, _Size, _Size> m0;
	for (size_t i = 0; i < _Power; i++)
	{
		m0 = _A * _A;
	}
	return m0;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl sqrt(_In_ genType _X, POWER_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE sqrt(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl inversesqrt(_In_ genType _X, POWER_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE inversesqrt(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl cbrt(_In_ genType _X, CBRT_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE cbrt(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl yroot(_In_ float64 _Expo, _In_ genType _X)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE yroot(_Expo, _X[i]); }
	return _X;
}

////////////////////////////////////// TRIGONOMETRY /////////////////////////////////////

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl sin(_In_ genType _X, SINCOS_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE sin(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl cos(_In_ genType _X, SINCOS_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE cos(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl tan(_In_ genType _X, SINCOS_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE tan(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl ctg(_In_ genType _X, SINCOS_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE ctg(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl sec(_In_ genType _X, SINCOS_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE sec(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl csc(_In_ genType _X, SINCOS_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE csc(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arcsin(_In_ genType _X, ATAN_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE arcsin(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arccos(_In_ genType _X, ATAN_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE arccos(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arctan(_In_ genType _X, ATAN_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE arctan(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arcctg(_In_ genType _X, ATAN_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE arcctg(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arcsec(_In_ genType _X, ATAN_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE arcsec(_X[i], _Conf); }
	return _X;
}

template<typename genType> requires vecType<genType>
_Check_return_ genType __cdecl arccsc(_In_ genType _X, ATAN_CONFIG _Conf)
{
	for (size_t i = 0; i < _X.size(); i++) { _X[i] = _CSE arccsc(_X[i], _Conf); }
	return _X;
}

_CSE_END

#endif