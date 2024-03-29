﻿#include <cassert>
#include "CSE/Core/gl/matrix.h"
#include "CSE/Core/CSECore.h"

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
inline constexpr basic_matrix<_Ty, _Column, _Line>::basic_matrix(std::initializer_list<std::initializer_list<_Ty>> _Ilist)
{
	this->fill(0);
	auto it = _Ilist.begin();
	auto end = _Ilist.end();
	if (it == end) { return; }
	uint64 _CurCol = 0;
	for (; it != end; ++it)
	{
		auto it2 = it->begin();
		auto end2 = it->end();
		uint64 _CurLin = 0;
		for ( ; it2 != end2; ++it2)
		{
			this->_Elems[_CurCol][_CurLin] = *it2;
			if (_CurLin == _Line - 1) { break; }
			++_CurLin;
		}

		if (_CurCol == _Column - 1){break;}
		++_CurCol;
	}
}

template<typename _Ty, size_t _Column, size_t _Line>
inline basic_matrix<_Ty, _Column - 1, _Line - 1> basic_matrix<_Ty, _Column, _Line>::Minor(size_type _CPos, size_type _LPos) const
{
	//if (_Column != _Line) { return basic_matrix<_Ty, _Column - 1, _Line - 1>(wrtval(Q_NAN_DOUBLE)); }
	_STL_VERIFY(_Line >= 2 && _Column >= 2, "can't generate minor because size of matrix is less than 2x2");
	basic_matrix<_Ty, _Column - 1, _Line - 1> _Minor;
	uint64 _M_Line = 0, _M_Col = 0;
	for (size_t i = 0; i < _Line; i++)
	{
		for (size_t j = 0; j < _Column; j++)
		{
			if (i != _LPos - 1 && j != _CPos - 1)
			{
				_Minor[_M_Col][_M_Line] = this->_Elems[j][i];
				++_M_Col;

				if (_M_Col == _Column - 1)
				{
					_M_Col -= _Column - 1;
					++_M_Line;
				}
			}
		}
	}
	return _Minor;
}

// Laplace Formula for finding determinant, very inefficient.
#if 0
template<typename _Ty, size_t _Size> requires (_Size == 1)
inline constexpr _Ty _Determinent(const _STD array<_Ty, _Size>* _Elems, uint64 n = _Size)
{
	return _Elems[0][0];
}

template<typename _Ty, size_t _Size> requires (_Size == 2)
inline constexpr _Ty _Determinent(const _STD array<_Ty, _Size>* _Elems, uint64 n = _Size)
{
	return _Elems[0][0] * _Elems[1][1] - _Elems[0][1] * _Elems[1][0];
}

template<typename _Ty, size_t _Size> requires (_Size > 2)
inline constexpr _Ty _Determinent(const _STD array<_Ty, _Size>* _Elems, uint64 n = _Size)
{
	_Ty _Total = 0;
	_STD array<_Ty, _Size - 1> _Minor[_Size - 1];
	for (int x = 0; x < n; x++)
	{
		int subi = 0;
		for (int i = 1; i < n; i++)
		{
			int subj = 0;
			for (int j = 0; j < n; j++)
			{
				if (j == x) { continue; }
				_Minor[subj][subi] = _Elems[j][i];
				subj++;
			}
			subi++;
		}
		_Total += (std::pow(-1, x) * _Elems[x][0] * _Determinent(_Minor, n - 1));
	}
	return _Total;
}
#endif
// A new algorithm based on gaussian elimination for determinants, more efficiency.

template<typename _Ty, size_t _Column, size_t _Line>
inline float64 basic_matrix<_Ty, _Column, _Line>::Determinant() const
{
	if (_Column != _Line) { return wrtval(Q_NAN_DOUBLE); }
	if (this->length() == 1) { return _Elems[0][0]; }

	basic_matrix<_Ty, _Column, _Line> matrix = *this;
	uint64 N = static_cast<uint64>(_Line);
	float64 det = 1;

	for (uint64 i = 0; i < N; ++i) 
	{
		float64 pivotElement = matrix[i][i];
		uint64 pivotRow = i;
		for (uint64 row = i + 1; row < N; ++row)
		{
			if (std::abs(matrix[i][row]) > std::abs(pivotElement))
			{
				pivotElement = matrix[i][row];
				pivotRow = row;
			}
		}
		if (pivotElement == 0.0)
		{
			return 0.0;
		}
		if (pivotRow != i)
		{
			matrix = matrix.swap(i + 1, pivotRow + 1, "Line");
			det *= -1.0;
		}
		det *= pivotElement;

		for (uint64 row = i + 1; row < N; ++row)
		{
			for (uint64 col = i + 1; col < N; ++col)
			{
				matrix[col][row] -= matrix[i][row] * matrix[col][i] / pivotElement;
			}
		}
	}

	return det;
}

template<typename _Ty, size_t _Column, size_t _Line>
inline basic_matrix<_Ty, _Line, _Column> basic_matrix<_Ty, _Column, _Line>::Transpose() const
{
	basic_matrix<_Ty, _Line, _Column> _Transpose;
	for (size_t i = 0; i < _Column; i++)
	{
		for (size_t j = 0; j < _Line; j++)
		{
			_Transpose[j][i] = this->_Elems[i][j];
		}
	}
	return _Transpose;
}

template<typename _Ty, size_t _Column, size_t _Line>
inline basic_matrix<_Ty, _Line, _Column> basic_matrix<_Ty, _Column, _Line>::Adjugate() const
{
	basic_matrix<_Ty, _Column, _Line> m0;
	if (_Column != _Line) { return basic_matrix<_Ty, _Line, _Column>(wrtval(Q_NAN_DOUBLE)); }
	if (this->length() == 1) { return basic_matrix<_Ty, _Line, _Column>(1. / _Elems[0][0]); }
	else
	{
		for (size_t i = 1; i <= this->length(); i++)
		{
			for (size_t j = 1; j <= this->length(); j++)
			{
				m0[j - 1][i - 1] = std::pow(-1, i + j) * this->Minor(i, j).Determinant();
			}
		}
	}
	return m0;
}

#if 0
template<typename _Ty, size_t _Column, size_t _Line>
inline basic_matrix<_Ty, _Line, _Column> basic_matrix<_Ty, _Column, _Line>::Inverse() const
{
	if (_Column != _Line) { return basic_matrix<_Ty, _Line, _Column>(wrtval(Q_NAN_DOUBLE)); }
	return (1.0 / this->Determinant()) * this->Adjugate();
}
#endif

template<typename _Ty, size_t _Column, size_t _Line>
inline basic_matrix<_Ty, _Line, _Column> basic_matrix<_Ty, _Column, _Line>::Inverse() const
{
	if (_Column != _Line) { return basic_matrix<_Ty, _Line, _Column>(wrtval(Q_NAN_DOUBLE)); }
	
	// Matrix Inverse Using Gauss Jordan Method C++ Program
	// Reference: https://www.codesansar.com/numerical-methods/matrix-inverse-using-gauss-jordan-cpp-output.htm
	// This method is much faster than above because this doesn't need to calculate determinant value.

	/* 1. Reading order of matrix */
	/* 2. Reading Matrix */
	basic_matrix<_Ty, _Column, _Line> _Cpy = *this;

	/* Augmenting Identity Matrix of Order n */
	basic_matrix<_Ty, _Column * 2, _Line> _Arg;
	basic_matrix<_Ty, _Column, _Line> _Ident(1);
	for (size_t i = 0;i < _Line;i++)
	{
		for (size_t j = 0;j < _Column;j++)
		{
			_Arg[j][i] = _Cpy[j][i];
			_Arg[j + _Column][i] = _Ident[j][i];
		}
	}

	/* Applying Gauss Jordan Elimination */
	for (size_t i = 0;i < _Line;i++)
	{
		if (_Arg[i][i] == 0.0)
		{
			if (i == _Line - 1)
			{
				_STL_VERIFY(_Arg[i][i] != 0.0, "Matrix can't be inversed.");
				return basic_matrix<_Ty, _Line, _Column>(wrtval(Q_NAN_DOUBLE));
			}
			_Arg = _Arg.swap(i + 1, i + 2, "Line");
		}

		for (size_t j = 0;j < _Column;j++)
		{
			if (i != j)
			{
				float64 ratio = _Arg[i][j] / _Arg[i][i];
				for (size_t k = 1;k < 2 * _Column;k++)
				{
					_Arg[k][j] = _Arg[k][j] - ratio * _Arg[k][i];
				}
			}
		}
	}

	/* Row Operation to Make Principal Diagonal to 1 */
	for (size_t i = 0;i < _Line;i++)
	{
		for (size_t j = _Column;j < 2 * _Column;j++)
		{
			_Arg[j][i] = _Arg[j][i] / _Arg[i][i];
		}
	}

	/* Exporting Inverse Matrix */
	basic_matrix<_Ty, _Column, _Line> _Inv;
	for (size_t i = 0;i < _Line;i++)
	{
		for (size_t j = 0;j < _Column;j++)
		{
			_Inv[j][i] = _Arg[j + _Column][i];
		}
	}

	return _Inv;
}

template<typename _Ty, size_t _Column, size_t _Line>
inline uint64 basic_matrix<_Ty, _Column, _Line>::Rank() const
{
	float64 _ZERO_ = 1E-9;
	uint64 _Rank = 0;
	_STD array<bool, _Column> _SelLine;
	_SelLine.fill(false);
	basic_matrix<_Ty, _Column, _Line> _Cpy = *this;
	for (size_t i = 0; i < _Line; i++)
	{
		size_t j;
		for (j = 0; j < _Column; j++)
		{
			if (!_SelLine[j] && std::abs(_Cpy[j][i]) > _ZERO_) { break; }
		}

		if (j != _Column)
		{
			++_Rank;
			_SelLine[j] = true;
			for (size_t p = i + 1; p < _Line; p++)
			{
				_Cpy[j][p] /= _Cpy[j][i];
			}
			for (size_t k = 0; k < _Column; k++)
			{
				if (k != j && std::abs(_Cpy[k][i]) > _ZERO_)
				{
					for (size_t p = i + 1; p < _Line; p++)
					{
						_Cpy[k][p] -= _Cpy[j][p] * _Cpy[k][i];
					}
				}
			}
		}
	}
	return _Rank;
}

template<typename _Ty, size_t _Column, size_t _Line>
inline _Ty basic_matrix<_Ty, _Column, _Line>::Trace() const
{
	if (_Column != _Line) { return basic_matrix<_Ty, _Line, _Column>(wrtval(Q_NAN_DOUBLE)); }
	_Ty _Trace = 0;

	for (size_t i = 0; i < _Column; i++)
	{
		_Trace += _Elems[i][i];
	}
	
	return _Trace;
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

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator*(const basic_matrix<_Ty, _Column, _Line>& m1, const _Ty& scalar)
{
	basic_matrix<_Ty, _Column, _Line> m0;
	for (size_t i = 0; i < _Column; i++)
	{
		for (size_t j = 0; j < _Line; j++)
		{
			m0[i][j] = m1[i][j] * scalar;
		}
	}
	return m0;
}

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator*(const _Ty& scalar, const basic_matrix<_Ty, _Column, _Line>& m1)
{
	basic_matrix<_Ty, _Column, _Line> m0;
	for (size_t i = 0; i < _Column; i++)
	{
		for (size_t j = 0; j < _Line; j++)
		{
			m0[i][j] = m1[i][j] * scalar;
		}
	}
	return m0;
}

template<typename _Ty, size_t _Column, size_t _Line, size_t _Size>
basic_matrix<_Ty, _Column, _Line> operator*(const basic_matrix<_Ty, _Size, _Line>& m1, const basic_matrix<_Ty, _Column, _Size>& m2)
{
	basic_matrix<_Ty, _Column, _Line> m0;
	for (size_t i = 0; i < _Line; i++)
	{
		for (size_t j = 0; j < _Column; j++)
		{
			_Ty _Total = 0;
			for (size_t k = 0; k < _Size; k++)
			{
				_Total += m1[k][i] * m2[j][k];
			}
			m0[j][i] = _Total;
		}
	}
	return m0;
}

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator/(const basic_matrix<_Ty, _Column, _Line>& m1, const _Ty& scalar)
{
	basic_matrix<_Ty, _Column, _Line> m0;
	for (size_t i = 0; i < _Column; i++)
	{
		for (size_t j = 0; j < _Line; j++)
		{
			m0[i][j] = m1[i][j] / scalar;
		}
	}
	return m0;
}

template<typename _Ty, size_t _Column, size_t _Line, size_t _Size>
basic_matrix<_Ty, _Column, _Line> operator/(const basic_matrix<_Ty, _Size, _Line>& m1, const basic_matrix<_Ty, _Column, _Size>& m2)
{
	basic_matrix<_Ty, _Column, _Line> m0;
	basic_matrix<_Ty, _Column, _Size> _m2 = m2.Inverse();
	for (size_t i = 0; i < _Line; i++)
	{
		for (size_t j = 0; j < _Column; j++)
		{
			_Ty _Total = 0;
			for (size_t k = 0; k < _Size; k++)
			{
				_Total += m1[k][i] * _m2[j][k];
			}
			m0[j][i] = _Total;
		}
	}
	return m0;
}

_GL_END

_CSE_END