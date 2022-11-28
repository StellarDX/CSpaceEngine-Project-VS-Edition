#ifndef __ALGORITHMS_TEMP__
#define __ALGORITHMS_TEMP__

#include "CSE/Core/CSEM_Algo.h"

_CSE_BEGIN

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType __cdecl abs(_In_ genType _Xx)noexcept
{
	for (size_t i = 0; i < _Xx.size(); i++) { _Xx[i] = _CSE abs(_Xx[i]); }
	return _Xx;
}

template<typename genIType> requires vecIType<genIType>
_NODISCARD _Check_return_ genIType __cdecl abs(_In_ genIType _Xx)noexcept
{
	for (size_t i = 0; i < _Xx.size(); i++) { _Xx[i] = _CSE abs(_Xx[i]); }
	return _Xx;
}

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType __cdecl sgn(_In_ genType _Xx)noexcept
{
	for (size_t i = 0; i < _Xx.size(); i++) { _Xx[i] = _CSE sgn(_Xx[i]); }
	return _Xx;
}

template<typename genIType> requires vecIType<genIType>
_NODISCARD _Check_return_ genIType __cdecl sgn(_In_ genIType _Xx)noexcept
{
	for (size_t i = 0; i < _Xx.size(); i++) { _Xx[i] = _CSE sgn(_Xx[i]); }
	return _Xx;
}

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType floor(_In_ genType _Xx)noexcept
{
	for (size_t i = 0; i < _Xx.size(); i++) { _Xx[i] = _CSE floor(_Xx[i]); }
	return _Xx;
}

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType ceil(_In_ genType _Xx)noexcept
{
	for (size_t i = 0; i < _Xx.size(); i++) { _Xx[i] = _CSE ceil(_Xx[i]); }
	return _Xx;
}

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType FractionalPart(genType _Xx)noexcept
{
	for (size_t i = 0; i < _Xx.size(); i++) { _Xx[i] = _CSE FractionalPart(_Xx[i]); }
	return _Xx;
}

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType mod(genType _Left, float64 _Right)noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE mod(_Left[i], _Right); }
	return _Left;
}

template<typename genType> requires vecType<genType>
_NODISCARD _Check_return_ genType mod(genType _Left, genType _Right)noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE mod(_Left[i], _Right[i]); }
	return _Left;
}

template<typename genType> requires vecType<genType>
_NODISCARD constexpr genType min(genType _Left, genType _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE min(_Left[i], _Right[i]); }
	return _Left;
}

template<typename genType> requires vecType<genType>
_NODISCARD constexpr genType min(genType _Left, float64 _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE min(_Left[i], _Right); }
	return _Left;
}

template<typename genIType> requires vecIType<genIType>
_NODISCARD constexpr genIType min(genIType _Left, genIType _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE min(_Left[i], _Right[i]); }
	return _Left;
}

template<typename genIType> requires vecIType<genIType>
_NODISCARD constexpr genIType min(genIType _Left, int64 _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE min(_Left[i], _Right); }
	return _Left;
}

template<typename genUType> requires vecUType<genUType>
_NODISCARD constexpr genUType min(genUType _Left, genUType _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE min(_Left[i], _Right[i]); }
	return _Left;
}

template<typename genUType> requires vecUType<genUType>
_NODISCARD constexpr genUType min(genUType _Left, uint64 _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE min(_Left[i], _Right); }
	return _Left;
}

template<typename iterator> requires std::random_access_iterator<iterator>
_NODISCARD constexpr iterator min(iterator begin, iterator end)noexcept
{
	if (begin >= end){return end;}

	auto It = begin;
	while (begin != end)
	{
		It = *begin < *It ? begin : It;
		++begin;
	}

	return It;
}

template<typename genType>
_NODISCARD constexpr genType min(std::initializer_list<genType> _Ilist)noexcept
{
	return *(_CSE min(_Ilist.begin(), _Ilist.end()));
}

template<typename genType> requires vecType<genType>
_NODISCARD constexpr genType max(genType _Left, genType _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE max(_Left[i], _Right[i]); }
	return _Left;
}

template<typename genType> requires vecType<genType>
_NODISCARD constexpr genType max(genType _Left, float64 _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE max(_Left[i], _Right); }
	return _Left;
}

template<typename genIType> requires vecIType<genIType>
_NODISCARD constexpr genIType max(genIType _Left, genIType _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE max(_Left[i], _Right[i]); }
	return _Left;
}

template<typename genIType> requires vecIType<genIType>
_NODISCARD constexpr genIType max(genIType _Left, int64 _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE max(_Left[i], _Right); }
	return _Left;
}

template<typename genUType> requires vecUType<genUType>
_NODISCARD constexpr genUType max(genUType _Left, genUType _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE max(_Left[i], _Right[i]); }
	return _Left;
}

template<typename genUType> requires vecUType<genUType>
_NODISCARD constexpr genUType max(genUType _Left, uint64 _Right) noexcept
{
	for (size_t i = 0; i < _Left.size(); i++) { _Left[i] = _CSE max(_Left[i], _Right); }
	return _Left;
}

template<typename iterator> requires std::random_access_iterator<iterator>
_NODISCARD constexpr iterator max(iterator begin, iterator end)noexcept
{
	if (begin >= end){return end;}

	auto It = begin;
	while (begin != end)
	{
		It = *begin > *It ? begin : It;
		++begin;
	}

	return It;
}

template<typename genType>
_NODISCARD constexpr genType max(std::initializer_list<genType> _Ilist)noexcept
{
	return *(_CSE max(_Ilist.begin(), _Ilist.end()));
}

template<typename genType> requires vecType<genType>
genType clamp(genType x, genType MinVal, genType MaxVal)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE clamp(x[i], MinVal[i], MaxVal[i]); }
	return x;
}

template<typename genType> requires vecType<genType>
genType clamp(genType x, float64 MinVal, float64 MaxVal)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE clamp(x[i], MinVal, MaxVal); }
	return x;
}

template<typename genIType> requires vecIType<genIType>
genIType clamp(genIType x, genIType MinVal, genIType MaxVal)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE clamp(x[i], MinVal[i], MaxVal[i]); }
	return x;
}

template<typename genIType> requires vecIType<genIType>
genIType clamp(genIType x, int64 MinVal, int64 MaxVal)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE clamp(x[i], MinVal, MaxVal); }
	return x;
}

template<typename genUType> requires vecUType<genUType>
genUType clamp(genUType x, genUType MinVal, genUType MaxVal)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE clamp(x[i], MinVal[i], MaxVal[i]); }
	return x;
}

template<typename genUType> requires vecUType<genUType>
genUType clamp(genUType x, uint64 MinVal, uint64 MaxVal)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE clamp(x[i], MinVal, MaxVal); }
	return x;
}

template<typename genType> requires vecType<genType>
genType mix(genType x, genType y, genType a)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE mix(x[i], y[i], a[i]); }
	return x;
}

template<typename genType> requires vecType<genType>
genType mix(genType x, genType y, float64 a)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE mix(x[i], y[i], a); }
	return x;
}

template<typename genType, typename genBType> requires vecType<genType> && vecBType<genBType>
genType mix(genType x, genType y, genBType a)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE mix(x[i], y[i], a[i]); }
	return x;
}

template<typename genIType, typename genBType> requires vecIType<genIType> && vecBType<genBType>
genIType mix(genIType x, genIType y, genBType a)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE mix(x[i], y[i], a[i]); }
	return x;
}

template<typename genUType, typename genBType> requires vecUType<genUType> && vecBType<genBType>
genUType mix(genUType x, genUType y, genBType a)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE mix(x[i], y[i], a[i]); }
	return x;
}

template<typename genBType> requires vecBType<genBType>
genBType mix(genBType x, genBType y, genBType a)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE mix(x[i], y[i], a[i]); }
	return x;
}

template<typename genType> requires vecType<genType>
genType step(genType edge, genType x)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE step(edge[i], x[i]); }
	return x;
}

template<typename genType> requires vecType<genType>
genType step(float64 edge, genType x)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE step(edge, x[i]); }
	return x;
}

template<typename genType> requires vecType<genType>
genType smoothstep(genType edge0, genType edge1, genType x)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE smoothstep(edge0[i], edge1[i], x[i]); }
	return x;
}

template<typename genType> requires vecType<genType>
genType smoothstep(float64 edge0, float64 edge1, genType x)
{
	for (size_t i = 0; i < x.size(); i++) { x[i] = _CSE smoothstep(edge0, edge1, x[i]); }
	return x;
}

_CSE_END

#endif