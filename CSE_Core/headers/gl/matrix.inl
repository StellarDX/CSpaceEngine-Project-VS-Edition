#include <cassert>
#include "matrix.h"
#include "..\Core\CSECore.h"

_CSE_BEGIN
_GL_BEGIN

template<typename _Ty, size_t _Column, size_t _Line>
inline constexpr basic_matrix<_Ty, _Column, _Line>::basic_matrix(const basic_matrix<_Ty, _Column, _Line>& m)
{
	this->fill(0);
	for (size_t i = 0; i < _Column; i++)
	{
		this->_Elems[i] = m._Elems[i];
	}
}

template<typename _Ty, size_t _Column, size_t _Line>
inline constexpr basic_matrix<_Ty, _Column, _Line>::basic_matrix(_Ty _Scalar)
{
	this->fill(0);
	for (size_t i = 0; i < (_Column < _Line ? _Column : _Line); i++)
	{
		this->_Elems[i][i] = _Scalar;
	}
}

template<typename _Ty, size_t _Column, size_t _Line>
inline constexpr basic_matrix<_Ty, _Column, _Line>::basic_matrix(std::initializer_list<_Ty> _Ilist)
{
	this->fill(0);
	auto it = _Ilist.begin();
	auto end = _Ilist.end();
	if (it == end) { return; }
	uint64 _CurCol = 0;
	uint64 _CurLin = 0;
	for (; it != end; ++it)
	{
		this->_Elems[_CurCol][_CurLin] = *it;
		if (_CurCol == _Column - 1 && _CurLin == _Line - 1) { break; }
		++_CurLin;
		if (_CurLin == _Line)
		{
			_CurLin -= _Line;
			++_CurCol;
		}
	}
}

template<typename _Ty, size_t _Column, size_t _Line>
template<size_t _Column2, size_t _Line2>
inline constexpr basic_matrix<_Ty, _Column, _Line>::basic_matrix(const basic_matrix<_Ty, _Column2, _Line2>& x)
{
	this->fill(0);
	uint64 _CurCol = 0;
	uint64 _CurLin = 0;
	while(_CurCol != _Column && _CurCol != _Column2)
	{
		this->_Elems[_CurCol][_CurLin] = x[_CurCol][_CurLin];
		++_CurLin;
		if (_CurLin == _Line2)
		{
			_CurLin -= _Line2;
			++_CurCol;
		}
		else if (_CurLin == _Line)
		{
			_CurLin -= _Line;
			++_CurCol;
		}
	}
}

template<typename _Ty, size_t _Column, size_t _Line>
constexpr std::ostream& operator<<(std::ostream& os, const basic_matrix<_Ty, _Column, _Line>& c)
{
	os << '[';
	for (size_t i = 0; i < _Line; i++)
	{
		if (i != 0) { os << ' '; }
		for (size_t j = 0; j < _Column; j++)
		{
			os << c[j][i];
			if (j < _Column - 1) { os << ", "; }
			else { os << ';'; }
		}
		if (i < _Line - 1) { os << '\n'; }
		else { os << ']'; }
	}
	return os;
}

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator+(const basic_matrix<_Ty, _Column, _Line>& m)
{
	return m;
}

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator-(const basic_matrix<_Ty, _Column, _Line>& m)
{
	basic_matrix<_Ty, _Column, _Line> m0;
	for (size_t i = 0; i < _Column; i++)
	{
		for (size_t j = 0; j < _Line; j++)
		{
			m0[i][j] = -m[i][j];
		}
	}
	return m0;
}

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator+(const basic_matrix<_Ty, _Column, _Line>& m1, const basic_matrix<_Ty, _Column, _Line>& m2)
{
	basic_matrix<_Ty, _Column, _Line> m0;
	for (size_t i = 0; i < _Column; i++)
	{
		for (size_t j = 0; j < _Line; j++)
		{
			m0[i][j] = m1[i][j] + m2[i][j];
		}
	}
	return m0;
}

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator-(const basic_matrix<_Ty, _Column, _Line>& m1, const basic_matrix<_Ty, _Column, _Line>& m2)
{
	basic_matrix<_Ty, _Column, _Line> m0;
	for (size_t i = 0; i < _Column; i++)
	{
		for (size_t j = 0; j < _Line; j++)
		{
			m0[i][j] = m1[i][j] - m2[i][j];
		}
	}
	return m0;
}

_GL_END
_CSE_END