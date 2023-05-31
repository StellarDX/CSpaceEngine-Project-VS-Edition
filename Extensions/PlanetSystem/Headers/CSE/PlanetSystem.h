#pragma once

#ifndef __PLANET_SYSTEM__
#define __PLANET_SYSTEM__

#include "CSE/Core.h"
#include "CSE/Object.h"
#include <memory>
#include <vector>

#define _PSYS_BEGIN namespace sesystem {
#define _PSYS_END   }
#define _PSYS       sesystem::

_CSE_BEGIN
_PSYS_BEGIN

class __SpaceEngine_Planetary_System
{
public:
	using size_type = typename size_t;
	using reference = const __SpaceEngine_Planetary_System&;
	using pointer   = _STD shared_ptr<__SpaceEngine_Planetary_System>;

	struct _SNode
	{
		Object _Obj;
		_STD vector<pointer> _SubSys;
	}_Elems;

	__SpaceEngine_Planetary_System() {};
	__SpaceEngine_Planetary_System(Object Obj) { _Elems._Obj = Obj; }

	_NODISCARD Object obj()const { return _Elems._Obj; }

	_CONSTEXPR20 void push(const Object& _Val)
	{ // insert element at end, provide strong guarantee
		_Elems._SubSys.push_back(_STD make_shared<__SpaceEngine_Planetary_System>(_Val));
	}

	_NODISCARD _CONSTEXPR20 reference operator[](size_type _Off) noexcept
	{
		return *(_Elems._SubSys[_Off]);
	}
};

_PSYS_END

using SESystem = _PSYS __SpaceEngine_Planetary_System;

_CSE_END

#endif