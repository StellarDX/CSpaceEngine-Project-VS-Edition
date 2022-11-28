#include "CSE/Core/CSEM_Func.h"

_CSE_BEGIN

// Inverse trigonometric fuctions have no high-precision method (except STD math) just now...
// Function "atan_f32" from OpenCV can return degrees directly but it have a 0.3 degree error.
// IBM's implemension based on radians is too complicated although it's error is smaller than OpenCV's

_Check_return_ float64 __cdecl arctan(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	//#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return degrees(std::atan(_X));
	//#else

	// Nothing...

	//#endif
}

_Check_return_ float64 __cdecl arcsin(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return degrees(std::asin(_X));
	#else
	return arctan(_X / sqrt(1. - _X * _X), _Conf);
	#endif
}

_Check_return_ float64 __cdecl arccos(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return degrees(std::acos(_X));
	#else
	return 90 - arcsin(_X, _Conf);
	#endif
}

_Check_return_ float64 __cdecl arcctg(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arctan(1. / _X, _Conf);
}

_Check_return_ float64 __cdecl arcsec(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arccos(1. / _X, _Conf);
}

_Check_return_ float64 __cdecl arccsc(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arcsin(1. / _X, _Conf);
}

_Check_return_ float64 __cdecl arccrd(_In_ float64 _X, _In_ float64 _Radius, ATAN_CONFIG _Conf)
{
	return 2. * arcsin(_X / (2. * _Radius), _Conf);
}

_Check_return_ float64 __cdecl arcsiv(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arccos(1. - _X, _Conf);
}

_Check_return_ float64 __cdecl arcvcs(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arccos(_X - 1., _Conf);
}

_Check_return_ float64 __cdecl arccvs(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arcsin(1. - _X, _Conf);
}

_Check_return_ float64 __cdecl arccvc(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arcsin(_X - 1., _Conf);
}

_Check_return_ float64 __cdecl archvs(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arccos(1. - 2. * _X, _Conf);
}

_Check_return_ float64 __cdecl archvc(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arccos(2. * _X - 1., _Conf);
}

_Check_return_ float64 __cdecl archcv(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arcsin(1. - 2. * _X, _Conf);
}

_Check_return_ float64 __cdecl archcc(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arcsin(2. * _X - 1., _Conf);
}

_Check_return_ float64 __cdecl arcexs(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arcsec(_X + 1., _Conf);
}

_Check_return_ float64 __cdecl arcexc(_In_ float64 _X, ATAN_CONFIG _Conf)
{
	return arccsc(_X + 1., _Conf);
}

// Complex Form

_Check_return_ complex64 __cdecl arcsin(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx, LOG_CONFIG _Conf)
{
	return -1i * ln(_CSE sqrt(1. - _X * _X)[_SqrtIdx] + 1i * _X, _Conf);
}

_Check_return_ complex64 __cdecl arccos(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx, LOG_CONFIG _Conf)
{
	return -1i * ln(1i * _CSE sqrt(1. - _X * _X)[_SqrtIdx] + _X, _Conf);
}

_Check_return_ complex64 __cdecl arctan(_In_ complex64 _X, LOG_CONFIG _Conf)
{
	return -0.5i * ln((1i - _X) / (1i + _X), _Conf);
}

_Check_return_ complex64 __cdecl arcctg(_In_ complex64 _X, LOG_CONFIG _Conf)
{
	return -0.5i * ln((_X + 1i) / (_X - 1i), _Conf);
}

_Check_return_ complex64 __cdecl arcsec(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx, LOG_CONFIG _Conf)
{
	return -1i * ln(1i * _CSE sqrt(1. - 1. / (_X * _X))[_SqrtIdx] + 1. / _X, _Conf);
}

_Check_return_ complex64 __cdecl arccsc(_In_ complex64 _X, _In_range_(0, 1) int _SqrtIdx, LOG_CONFIG _Conf)
{
	return -1i * ln(_CSE sqrt(1. - 1. / (_X * _X))[_SqrtIdx] + 1.i / _X, _Conf);
}

_CSE_END