// CSE Expanded trigonometric functions

#include "CSE/Core/CSECore.h"
#include "CSE/Core/ConstLists.h"
#include "CSE/Core/IEEE754/IEEE754.h"
#include "CSE/Core/IEEE754/MathFuncs.h"

_CSE_BEGIN

// Sin, cos and tan defines
_Check_return_ _FType __cdecl sin(_In_ _FType _X) { return __sin(_X); }
_Check_return_ _FType __cdecl cos(_In_ _FType _X) { return __cos(_X); }
_Check_return_ _FType __cdecl tan(_In_ _FType _X) { return __tan(_X); }

// Cotangent and secants

_Check_return_ _FType __cdecl ctg(_In_ _FType _X)
{
	return 1. / _CSE tan(_X);
}

_Check_return_ _FType __cdecl sec(_In_ _FType _X)
{
	return 1. / _CSE cos(_X);
}

_Check_return_ _FType __cdecl csc(_In_ _FType _X)
{
	return 1. / _CSE sin(_X);
}

_Check_return_ int64 __cdecl Quadrant(_In_ _FType _X)
{
	while (_X < 0) { _X += 360; }
	while (_X >= 360) { _X -= 360; }
	if (_X == 0) { return 0; }
	if (_X > 0 && _X < 90) { return 1; }
	if (_X == 90) { return 2; }
	if (_X > 90 && _X < 180) { return 3; }
	if (_X == 180) { return 4; }
	if (_X > 180 && _X < 270) { return 5; }
	if (_X == 270) { return 6; }
	if (_X > 270 && _X < 360) { return 7; }
}

///////////////////////////////////// EXPAND ////////////////////////////////////

// Chord and Arcs
_Check_return_ _FType __cdecl crd(_In_ _FType _X, _In_ _FType _Radius)
{
	return 2. * _Radius * _CSE sin(_FType(_X / 2.));
}

_Check_return_ _FType __cdecl arc(_In_ _FType _X, _In_ _FType _Radius)
{
	return (_X * CSE_PI * _Radius) / 180.;
}

// Versines
_Check_return_ _FType __cdecl siv(_In_ _FType _X)
{
	return 1. - _CSE cos(_X);
}

_Check_return_ _FType __cdecl cvs(_In_ _FType _X)
{
	return 1. - _CSE sin(_X);
}

_Check_return_ _FType __cdecl vcs(_In_ _FType _X)
{
	return 1. + _CSE cos(_X);
}

_Check_return_ _FType __cdecl cvc(_In_ _FType _X)
{
	return 1. + _CSE sin(_X);
}

_Check_return_ _FType __cdecl hvs(_In_ _FType _X)
{
	return siv(_X) / 2.;
}

_Check_return_ _FType __cdecl hcv(_In_ _FType _X)
{
	return cvs(_X) / 2.;
}

_Check_return_ _FType __cdecl hvc(_In_ _FType _X)
{
	return vcs(_X) / 2.;
}

_Check_return_ _FType __cdecl hcc(_In_ _FType _X)
{
	return cvc(_X) / 2.;
}

// Exsecants
_Check_return_ _FType __cdecl exs(_In_ _FType _X)
{
	return sec(_X) - 1.;
}

_Check_return_ _FType __cdecl exc(_In_ _FType _X)
{
	return csc(_X) - 1.;
}

// Inverse functions

_Check_return_ _FType __cdecl arcctg(_In_ _FType _X)
{
	return arctan(_FType(1. / _X));
}

_Check_return_ _FType __cdecl arcsec(_In_ _FType _X)
{
	return arccos(_FType(1. / _X));
}

_Check_return_ _FType __cdecl arccsc(_In_ _FType _X)
{
	return arcsin(_FType(1. / _X));
}

_Check_return_ _FType __cdecl arccrd(_In_ _FType _X, _In_ _FType _Radius)
{
	return 2. * arcsin(_FType(_X / (2. * _Radius)));
}

_Check_return_ _FType __cdecl arcsiv(_In_ _FType _X)
{
	return arccos(_FType(1. - _X));
}

_Check_return_ _FType __cdecl arcvcs(_In_ _FType _X)
{
	return arccos(_FType(_X - 1.));
}

_Check_return_ _FType __cdecl arccvs(_In_ _FType _X)
{
	return arcsin(_FType(1. - _X));
}

_Check_return_ _FType __cdecl arccvc(_In_ _FType _X)
{
	return arcsin(_FType(_X - 1.));
}

_Check_return_ _FType __cdecl archvs(_In_ _FType _X)
{
	return arccos(_FType(1. - 2. * _X));
}

_Check_return_ _FType __cdecl archvc(_In_ _FType _X)
{
	return arccos(_FType(2. * _X - 1.));
}

_Check_return_ _FType __cdecl archcv(_In_ _FType _X)
{
	return arcsin(_FType(1. - 2. * _X));
}

_Check_return_ _FType __cdecl archcc(_In_ _FType _X)
{
	return arcsin(_FType(2. * _X - 1.));
}

_Check_return_ _FType __cdecl arcexs(_In_ _FType _X)
{
	return arcsec(_FType(_X + 1.));
}

_Check_return_ _FType __cdecl arcexc(_In_ _FType _X)
{
	return arccsc(_FType(_X + 1.));
}


_CSE_END