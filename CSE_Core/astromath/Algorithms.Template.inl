#ifndef __ALGORITHMS_TEMP__
#define __ALGORITHMS_TEMP__

#include "../headers/math/Algorithms.h"

_CSE_BEGIN

// Abs Template
template<typename genType> requires vecType<genType>
genType abs(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = abs(x[i]);
	}
	return x;
}

template<typename genIType> requires vecIType<genIType>
genIType abs(genIType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = abs(x[i]);
	}
	return x;
}

// Sign Template
template<typename genType> requires vecType<genType>
genType sgn(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = sgn(x[i]);
	}
	return x;
}

template<typename genIType> requires vecIType<genIType>
genIType sgn(genIType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = sgn(x[i]);
	}
	return x;
}

// Floor Template
template<typename genType> requires vecType<genType>
genType floor(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = floor(x[i]);
	}
	return x;
}

// Ceil Template
template<typename genType> requires vecType<genType>
genType ceil(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = ceil(x[i]);
	}
	return x;
}

// Fract Template
template<typename genType> requires vecType<genType>
genType FractionalPart(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = FractionalPart(x[i]);
	}
	return x;
}

// Mod Template
template<typename genType> requires vecType<genType>
genType mod(genType x, genType y)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = mod(x[i], y[i]);
	}
	return x;
}

// Min Template
template<typename genType> requires vecType<genType>
genType min(genType x, genType y)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = min(x[i], y[i]);
	}
	return x;
}

template<typename genIType> requires vecIType<genIType>
genIType min(genIType x, genIType y)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = min(x[i], y[i]);
	}
	return x;
}

template<typename genUType> requires vecUType<genUType>
genUType min(genUType x, genUType y)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = min(x[i], y[i]);
	}
	return x;
}

template<typename iterator> requires std::random_access_iterator<iterator>
iterator min(iterator begin, iterator end)
{
	if (begin > end)
	{
		MathLog.Out("Math-Algorithms", "ERROR", "iterator cse::min(iterator, iterator): Failed to calculate because the begin iterator is after end.", SysLogLevel);
		return end;
	}

	auto It = begin;
	while (begin != end)
	{
		It = *begin < *It ? begin : It;
		++begin;
	}

	return It;
}

template<typename genType>
genType min(initializer_list<genType> Array)
{
	genType Current = *(Array.begin());
	for (auto Pos : Array)
	{
		Current = Pos < Current ? Pos : Current;
	}
	return Current;
}

// Max Template
template<typename genType> requires vecType<genType>
genType max(genType x, genType y)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = max(x[i], y[i]);
	}
	return x;
}

template<typename genIType> requires vecIType<genIType>
genIType max(genIType x, genIType y)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = max(x[i], y[i]);
	}
	return x;
}

template<typename genUType> requires vecUType<genUType>
genUType max(genUType x, genUType y)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = max(x[i], y[i]);
	}
	return x;
}

template<typename iterator> requires std::random_access_iterator<iterator>
iterator max(iterator begin, iterator end)
{
	if (begin > end)
	{
		MathLog.Out("Math-Algorithms", "ERROR", "iterator cse::max(iterator, iterator): Failed to calculate because the begin iterator is after end.", SysLogLevel);
		return end;
	}

	auto It = begin;
	while (begin != end)
	{
		It = *begin > *It ? begin : It;
		++begin;
	}

	return It;
}

template<typename genType>
genType max(initializer_list<genType> Array)
{
	genType Current = *(Array.begin());
	for (auto Pos : Array)
	{
		Current = Pos > Current ? Pos : Current;
	}
	return Current;
}

// Clamp Template
template<typename genType> requires vecType<genType>
genType clamp(genType x, genType MinVal, genType MaxVal)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = clamp(x[i], MinVal[i], MaxVal[i]);
	}
	return x;
}

template<typename genIType> requires vecIType<genIType>
genIType clamp(genIType x, genIType MinVal, genIType MaxVal)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = clamp(x[i], MinVal[i], MaxVal[i]);
	}
	return x;
}

template<typename genUType> requires vecUType<genUType>
genUType clamp(genUType x, genUType MinVal, genUType MaxVal)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = clamp(x[i], MinVal[i], MaxVal[i]);
	}
	return x;
}

// Mix Template
template<typename genType> requires vecType<genType>
genType mix(genType x, genType y, genType a)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = mix(x[i], y[i], a[i]);
	}
	return x;
}

template<typename genType, typename genBType> requires vecType<genType> && vecBType<genBType>
genType mix(genType x, genType y, genBType a)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = mix(x[i], y[i], a[i]);
	}
	return x;
}

template<typename genIType, typename genBType> requires vecIType<genIType> && vecBType<genBType>
genIType mix(genIType x, genIType y, genBType a)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = mix(x[i], y[i], a[i]);
	}
	return x;
}

template<typename genUType, typename genBType> requires vecUType<genUType> && vecBType<genBType>
genUType mix(genUType x, genUType y, genBType a)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = mix(x[i], y[i], a[i]);
	}
	return x;
}

template<typename genBType> requires vecBType<genBType>
genBType mix(genBType x, genBType y, genBType a)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = mix(x[i], y[i], a[i]);
	}
	return x;
}

// Step Template
template<typename genType> requires vecType<genType>
genType step(genType edge, genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = step(edge[i], x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType smoothstep(genType edge0, genType edge1, genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = smoothstep(edge0[i], edge1[i], x[i]);
	}
	return x;
}

_CSE_END

#endif