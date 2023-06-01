#pragma once

#ifndef __PLANET_SYSTEM__
#define __PLANET_SYSTEM__

#include "CSE/Core.h"
#include "CSE/Object.h"
#include <xutility>
#include <memory>
#include <vector>
#include <map>
#include <queue>

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
	_NODISCARD _STD vector<pointer> subsys()const { return _Elems._SubSys; }

	_CONSTEXPR20 void push(const Object& _Val)
	{ // insert element at end, provide strong guarantee
		_Elems._SubSys.push_back(_STD make_shared<__SpaceEngine_Planetary_System>(_Val));
	}

	_NODISCARD reference operator[](size_type _Off) noexcept
	{
		return *(_Elems._SubSys[_Off]);
	}
};

_PSYS_END

using SESNode = _PSYS __SpaceEngine_Planetary_System;
using SESystem = _PSYS __SpaceEngine_Planetary_System::pointer;

template<typename _Iter>
_CONSTEXPR20 _STD map<_Iter, _STD vector<_Iter>> _Link_Parent_Unchecked(_Iter _First, const _Iter _Last)
{
	_STD map<_Iter, _STD vector<_Iter>> Res;
	if (_First != _Last)
	{
		auto it = _First;
		auto end = _Last;
		for (; it != end; ++it)
		{
			auto it2 = _First;
			auto end2 = _Last;
			for (; it2 != end2; ++it2)
			{
				if (_STD find(it->Name.begin(), it->Name.end(), it2->ParentBody) != it->Name.end())
				{
					if (_STD find(it2->Name.begin(), it2->Name.end(), it2->ParentBody) != it2->Name.end()) { continue; }
					auto it3 = Res.find(it);
					if (it3 == Res.end())
					{
						Res.insert({ it, {it2} });
					}
					else
					{
						it3->second.push_back(it2);
					}
				}
			}
		}
	}
	return Res;
}

template<typename _Iter, typename _MapIt = typename _STD map<_Iter, _STD vector<_Iter>>>
_CONSTEXPR20 _MapIt _Find_Barycenter_Unchecked(_MapIt _First, _MapIt _Last)
{
	//_STD _Adl_verify_range(_First, _Last);
	for (; _First != _Last; ++_First)
	{
		if (find(_First->first->Name.begin(), _First->first->Name.end(), _First->first->ParentBody) != _First->first->Name.end())
		{
			break;
		}
	}
	return _First;
}

template<typename _Iter>
_CONSTEXPR20 void _DFS_Build_Unchecked(_STD map<_Iter, _STD vector<_Iter>> Sys, _Iter _Ptr, SESNode* Dst)
{
	auto it = Sys.find(_Ptr);
	if (it == Sys.end()) { return; }
	for (auto i : it->second)
	{
		Dst->push(*(i));
		_DFS_Build_Unchecked(Sys, i, Dst->_Elems._SubSys.back().get());
	}
}

/// <summary>
/// Building system with servial objects
/// </summary>
template<typename _Iter = _STD vector<Object>::iterator> requires _STD random_access_iterator<_Iter>
SESystem MakeSystem(_Iter _First, _Iter _Last)
{
	_STD _Adl_verify_range(_First, _Last);
	const auto _UFirst = _STD _Get_unwrapped(_First);
	const auto _ULast = _STD _Get_unwrapped(_Last);
	auto ParentList = _Link_Parent_Unchecked(_UFirst, _ULast);
	auto Barycenter = _Find_Barycenter_Unchecked<Object*>(ParentList.begin(), ParentList.end());
	if (Barycenter == ParentList.end()) { throw _STD exception("Barycenter is not found in this scope."); }
	SESNode Sys(*(Barycenter->first));
	_DFS_Build_Unchecked(ParentList, Barycenter->first, &Sys);
	return _STD make_shared<SESNode>(Sys);
}

_CSE_END

#endif