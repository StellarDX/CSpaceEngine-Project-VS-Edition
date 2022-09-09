// Basic Matrix standard header (GLSL)

// Reference:
// [GLSL Document]https://www.khronos.org/registry/OpenGL/specs/gl/GLSLangSpec.4.20.pdf
// [GLM by G-Truc]https://glm.g-truc.net/0.9.4/api/a00142.html#gade0eb47c01f79384a6f38017ede17446
// [Introduction to Linear Algebra(5th Edition) by Gilbert Strang]
// https://github.com/Rumi381/All-Programming-E-Books-PDF/blob/master/Books/Introduction%20to%20Linear%20Algebra,%20Fifth%20Edition%20by%20Gilbert%20Strang%20(z-lib.org).pdf
// Port by StellarDX, 2022.

#pragma once

#ifndef __CSE_MATRIX__
#define __CSE_MATRIX__
#include <yvals_core.h>
#include "..\Core\CSECore.h"
// This matrix is based on STD array, and most functions are taken from STD Library and GLM Library.
#include <array>
#include <initializer_list>
#include <xutility>
#include <ostream>

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN
_GL_BEGIN

template<typename _Ty, size_t _Column, size_t _Line>
class _Matrix_const_iterator //: private _Iterator_base12_compatible
{
public:
    #ifdef __cpp_lib_concepts
    using iterator_concept  = _STD contiguous_iterator_tag;
    #endif // __cpp_lib_concepts
    using iterator_category = _STD random_access_iterator_tag;
    using value_type        = _Ty;
    using difference_type   = ptrdiff_t;
    using pointer           = const _STD array<_Ty, _Line>*;
    using pointer_Elem      = const _Ty*;
    using reference         = const _Ty&;

    enum _Option_Tag
    {
        Line, Column
    };

private:
    pointer      _Ptr;
    pointer_Elem _Elm;
    size_t       _IdxC;
    size_t       _IdxL;
    _Option_Tag  _Opt;

public:
    _CONSTEXPR17 _Matrix_const_iterator() noexcept : _Ptr(), _IdxC(0), _IdxL(0), _Opt(Column) {}

    _CONSTEXPR17 explicit _Matrix_const_iterator(pointer _Parg, size_t _OffC = 0, size_t _OffL = 0, _Option_Tag _Option = Column) noexcept : _Ptr(_Parg), _IdxC(_OffC), _IdxL(_OffL), _Opt(_Option) {}

    _NODISCARD _CONSTEXPR17 pointer_Elem operator->() const noexcept
    {
        _STL_VERIFY(_Ptr, "cannot dereference value-initialized matrix iterator");
        _STL_VERIFY(_IdxC < _Column && _IdxL < _Line, "cannot dereference out of range matrix iterator");
        return &(*(_Ptr + _IdxC))[_IdxL];
    }

    _NODISCARD _CONSTEXPR17 reference operator*() const noexcept
    {
        return *operator->();
    }

    _CONSTEXPR17 _Matrix_const_iterator& operator++() noexcept
    {
        _STL_VERIFY(_Ptr, "cannot increment value-initialized matrix iterator");
        _STL_VERIFY(_IdxC < _Column && _IdxL < _Line, "cannot increment matrix iterator past end");
        switch (_Opt)
        {
        case Line:
            ++_IdxC;
            if (_IdxC == _Column)
            {
                _IdxC -= _Column;
                ++_IdxL;
            }
            break;
        case Column:
            ++_IdxL;
            if (_IdxL == _Line)
            {
                _IdxL -= _Line;
                ++_IdxC;
            }
            break;
        default:
            break;
        }

        return *this;
    }

    _CONSTEXPR17 _Matrix_const_iterator operator++(int) noexcept
    {
        _Matrix_const_iterator _Tmp = *this;
        ++(*this);
        return _Tmp;
    }

    _CONSTEXPR17 _Matrix_const_iterator& operator--() noexcept
    {
        _STL_VERIFY(_Ptr, "cannot decrement value-initialized matrix iterator");
        _STL_VERIFY(_IdxC != 0 || _IdxL != 0, "cannot decrement matrix iterator before begin");
        switch (_Opt)
        {
        case Line:
            if (_IdxC == 0)
            {
                _IdxC += _Column;
                --_IdxL;
            }
            --_IdxC;
            break;
        case Column:
            if (_IdxL == 0)
            {
                _IdxL += _Line;
                --_IdxC;
            }
            --_IdxL;
            break;
        default:
            break;
        }
        return *this;
    }

    _CONSTEXPR17 _Matrix_const_iterator operator--(int) noexcept
    {
        _Matrix_const_iterator _Tmp = *this;
        --(*this);
        return _Tmp;
    }

    constexpr size_t _Reduce_Dimension() const noexcept
    {
        size_t _Idx;
        switch (_Opt)
        {
        case Line:
            _Idx = _Column * _IdxL + _IdxC;
            break;
        case Column:
            _Idx = _Line * _IdxC + _IdxL;
            break;
        default:
            break;
        }
        return _Idx;
    }

    constexpr void _Incrase_Dimension(size_t _Idx) noexcept
    {
        switch (_Opt)
        {
        case Line:
            _IdxL = _Idx / _Column;
            _IdxC = _Idx % _Column;
            break;
        case Column:
            _IdxC = _Idx / _Line;
            _IdxL = _Idx % _Line;
            break;
        default:
            break;
        }
    }

    constexpr void _Verify_offset(const ptrdiff_t _Off) const noexcept
    {
        if (_Off != 0)
        {
            _STL_VERIFY(_Ptr, "cannot seek value-initialized matrix iterator");
        }

        size_t _Idx = _Reduce_Dimension();

        if (_Off < 0) 
        {
            _STL_VERIFY(_Idx >= size_t{ 0 } - static_cast<size_t>(_Off), "cannot seek matrix iterator before begin");
        }

        if (_Off > 0)
        {
            _STL_VERIFY(_Column * _Line - _Idx >= static_cast<size_t>(_Off), "cannot seek array iterator after end");
        }
    }

    _CONSTEXPR17 _Matrix_const_iterator& operator+=(const ptrdiff_t _Off) noexcept
    {
        _Verify_offset(_Off);
        size_t _Idx = _Reduce_Dimension();
        _Idx += static_cast<size_t>(_Off);
        _Incrase_Dimension(_Idx);
        return *this;
    }

    _NODISCARD _CONSTEXPR17 _Matrix_const_iterator operator+(const ptrdiff_t _Off) const noexcept
    {
        _Matrix_const_iterator _Tmp = *this;
        _Tmp += _Off;
        return _Tmp;
    }

    _CONSTEXPR17 _Matrix_const_iterator& operator-=(const ptrdiff_t _Off) noexcept
    {
        return *this += -_Off;
    }

    _NODISCARD _CONSTEXPR17 _Matrix_const_iterator operator-(const ptrdiff_t _Off) const noexcept
    {
        _Matrix_const_iterator _Tmp = *this;
        _Tmp -= _Off;
        return _Tmp;
    }

    _NODISCARD _CONSTEXPR17 reference operator[](const ptrdiff_t _Off) const noexcept
    {
        return *(*this + _Off);
    }

    _CONSTEXPR17 void _Compat(const _Matrix_const_iterator& _Right) const noexcept
    { // test for compatible iterator pair
        _STL_VERIFY(_Ptr == _Right._Ptr, "matrix iterators incompatible");
    }

    _NODISCARD _CONSTEXPR17 bool operator==(const _Matrix_const_iterator& _Right) const noexcept
    {
        _Compat(_Right);
        return _IdxC == _Right._IdxC && _IdxL == _Right._IdxL;
    }
};

template<typename _Ty, size_t _Column, size_t _Line>
class _Matrix_iterator : public _Matrix_const_iterator<_Ty, _Column, _Line>
{
    public:
    using _Mybase           = _Matrix_const_iterator<_Ty, _Column, _Line>;
    #ifdef __cpp_lib_concepts
    using iterator_concept  = _STD contiguous_iterator_tag;
    #endif // __cpp_lib_concepts
    using iterator_category = _STD random_access_iterator_tag;
    using value_type        = _Ty;
    using difference_type   = ptrdiff_t;
    using pointer           = _STD array<_Ty, _Line>*;
    using pointer_Elem      = _Ty*;
    using reference         = _Ty&;

    using _Mybase::_Option_Tag;

    _CONSTEXPR17 _Matrix_iterator() noexcept {}

    _CONSTEXPR17 explicit _Matrix_iterator(pointer _Parg, size_t _OffC = 0, size_t _OffL = 0, _Option_Tag _Option = _Mybase::Column) noexcept : _Mybase(_Parg, _OffC, _OffL, _Option) {}

    _NODISCARD _CONSTEXPR17 reference operator*() const noexcept
    {
        return const_cast<reference>(_Mybase::operator*());
    }

    _NODISCARD _CONSTEXPR17 pointer_Elem operator->() const noexcept
    {
        return const_cast<pointer_Elem>(_Mybase::operator->());
    }

    _CONSTEXPR17 _Matrix_iterator& operator++() noexcept
    {
        _Mybase::operator++();
        return *this;
    }

    _CONSTEXPR17 _Matrix_iterator operator++(int) noexcept
    {
        _Matrix_iterator _Tmp = *this;
        _Mybase::operator++();
        return _Tmp;
    }

    _CONSTEXPR17 _Matrix_iterator& operator--() noexcept
    {
        _Mybase::operator--();
        return *this;
    }
    
    _CONSTEXPR17 _Matrix_iterator operator--(int) noexcept
    {
        _Matrix_iterator _Tmp = *this;
        _Mybase::operator--();
        return _Tmp;
    }

    _CONSTEXPR17 _Matrix_iterator& operator+=(const ptrdiff_t _Off) noexcept
    {
        _Mybase::operator+=(_Off);
        return *this;
    }

    _NODISCARD _CONSTEXPR17 _Matrix_iterator operator+(const ptrdiff_t _Off) const noexcept
    {
        _Matrix_iterator _Tmp = *this;
        _Tmp += _Off;
        return _Tmp;
    }

    _CONSTEXPR17 _Matrix_iterator& operator-=(const ptrdiff_t _Off) noexcept
    {
        _Mybase::operator-=(_Off);
        return *this;
    }

    using _Mybase::operator-;

    _NODISCARD _CONSTEXPR17 _Matrix_iterator operator-(const ptrdiff_t _Off) const noexcept
    {
        _Matrix_iterator _Tmp = *this;
        _Tmp -= _Off;
        return _Tmp;
    }

    _NODISCARD _CONSTEXPR17 reference operator[](const ptrdiff_t _Off) const noexcept
    {
        return const_cast<reference>(_Mybase::operator[](_Off));
    }
};

template<typename _Ty, size_t _Column, size_t _Line>
class basic_matrix // fixed size matrix of values
{
public:
    using value_type      = _Ty;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using pointer         = _Ty*;
    using const_pointer   = const _Ty*;
    using reference       = _Ty&;
    using const_reference = const _Ty&;

    using line_type      = _STD array<_Ty, _Column>;
    using line_ref       = _STD array<_Ty, _Column>&;
    using const_line_ref = const _STD array<_Ty, _Column>&;
    using column_type    = _STD array<_Ty, _Line>;
    using column_ref     = _STD array<_Ty, _Line>&;
    using const_col_ref  = const _STD array<_Ty, _Line>&;

    using iterator       = _Matrix_iterator<_Ty, _Column, _Line>;
    using const_iterator = _Matrix_const_iterator<_Ty, _Column, _Line>;

    // -- Value container --
    _STD array<_Ty, _Line> _Elems[_Column]; // using GLM's method

    _CONSTEXPR20 void fill(const _Ty& _Value)
    {
        for (size_t i = 0; i < _Column; i++)
        {
            _Elems[i].fill(_Value);
        }
    }

    // -- Accesses --

    _NODISCARD constexpr size_type size() const noexcept
    {
        return _Column * _Line;
    }

    _NODISCARD constexpr size_type max_size() const noexcept
    {
        return _Column * _Line;
    }

    _NODISCARD constexpr size_type length() const noexcept
    {
        return _Column;
    }

    _NODISCARD _CONSTEXPR17 column_ref operator[](_In_range_(0, _Column - 1) size_type _CPos) noexcept /* strengthened */
    {
        #if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_CPos < _Column, "matrix subscript out of range");
        #endif // _CONTAINER_DEBUG_LEVEL > 0
        return _Elems[_CPos];
    }

    _NODISCARD constexpr const_col_ref operator[](_In_range_(0, _Column - 1) size_type _CPos) const noexcept /* strengthened */
    {
        #if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_CPos < _Column, "matrix subscript out of range");
        #endif // _CONTAINER_DEBUG_LEVEL > 0
        return _Elems[_CPos];
    }

    _NODISCARD _CONSTEXPR17 reference front() noexcept /* strengthened */ 
    {
        return _Elems[0][0];
    }

    _NODISCARD constexpr const_reference front() const noexcept /* strengthened */ 
    {
        return _Elems[0][0];
    }

    _NODISCARD _CONSTEXPR17 column_ref frontcolumn() noexcept /* strengthened */
    {
        return _Elems[0];
    }

    _NODISCARD constexpr const_col_ref frontcolumn() const noexcept /* strengthened */
    {
        return _Elems[0];
    }

    _NODISCARD _CONSTEXPR17 reference back() noexcept /* strengthened */ 
    {
        return _Elems[_Column - 1][_Line - 1];
    }

    _NODISCARD constexpr const_reference back() const noexcept /* strengthened */ 
    {
        return _Elems[_Column - 1][_Line - 1];
    }

    _NODISCARD _CONSTEXPR17 column_ref backcolumn() noexcept /* strengthened */
    {
        return _Elems[_Column - 1];
    }

    _NODISCARD constexpr const_col_ref backcolumn() const noexcept /* strengthened */
    {
        return _Elems[_Column - 1];
    }

    _NODISCARD constexpr bool empty() const noexcept
    {
        return false;
    }

    _NODISCARD _CONSTEXPR17 column_ref at(size_type _CPos) 
    {
        if (_Column <= _CPos) { _Xran(); }
        return _Elems[_CPos];
    }

    _NODISCARD constexpr const_col_ref at(size_type _CPos) const 
    {
        if (_Column <= _CPos) { _Xran(); }
        return _Elems[_CPos];
    }

    [[noreturn]] void _Xran() const 
    {
        std::_Xout_of_range("invalid matrix subscript");
    }

    template<size_type _NewCol, size_type _NewLine>
    _NODISCARD constexpr basic_matrix<_Ty, _NewCol, _NewLine> submat(const size_type _CPos, const size_type _LPos) const
    {
        basic_matrix<_Ty, _NewCol, _NewLine> _SubMat;
        for (size_t i = 0; i < _NewLine; i++)
        {
            for (size_t j = 0; j < _NewCol; j++)
            {
                _SubMat[j][i] = this->_Elems[_CPos + j - 1][_LPos + i - 1];
            }
        }
        return _SubMat;
    }

    // -- Constructors --

    constexpr basic_matrix() = default;
    constexpr basic_matrix(const basic_matrix<_Ty, _Column, _Line>& m);
    constexpr explicit basic_matrix(_Ty _Scalar);
    constexpr basic_matrix(_STD initializer_list<_Ty> _Ilist);
    constexpr basic_matrix(_STD initializer_list<_STD initializer_list<_Ty>> _Ilist);

    // -- Matrix conversions --

    template<size_t _Column2, size_t _Line2>
    constexpr basic_matrix(const basic_matrix<_Ty, _Column2, _Line2>& x);

    // -- STL Iterators --

    _NODISCARD _CONSTEXPR17 iterator cbegin() noexcept
    {
        return iterator(_Elems, 0, 0, iterator::Column);
    }

    _NODISCARD _CONSTEXPR17 iterator lbegin() noexcept
    {
        return iterator(_Elems, 0, 0, iterator::Line);
    }

    _NODISCARD _CONSTEXPR17 iterator cend() noexcept
    {
        return iterator(_Elems, _Column, 0, iterator::Column);
    }

    _NODISCARD _CONSTEXPR17 iterator lend() noexcept
    {
        return iterator(_Elems, 0, _Line, iterator::Line);
    }
    
    _NODISCARD _CONSTEXPR17 const_iterator cbegin()const noexcept
    {
        return const_iterator(_Elems, 0, 0, const_iterator::Column);
    }

    _NODISCARD _CONSTEXPR17 const_iterator lbegin()const noexcept
    {
        return const_iterator(_Elems, 0, 0, const_iterator::Line);
    }

    _NODISCARD _CONSTEXPR17 const_iterator cend()const noexcept
    {
        return const_iterator(_Elems, _Column, 0, const_iterator::Column);
    }

    _NODISCARD _CONSTEXPR17 const_iterator lend()const noexcept
    {
        return const_iterator(_Elems, 0, _Line, const_iterator::Line);
    }
    
    _NODISCARD _CONSTEXPR17 iterator begin() noexcept
    {
        return cbegin();
    }

    _NODISCARD _CONSTEXPR17 iterator end() noexcept
    {
        return cend();
    }
    
    _NODISCARD _CONSTEXPR17 const_iterator begin()const noexcept
    {
        return cbegin();
    }

    _NODISCARD _CONSTEXPR17 const_iterator end()const noexcept
    {
        return cend();
    }
    
    // -- Elementary row operations --

    _NODISCARD constexpr basic_matrix<_Ty, _Column, _Line> swap(size_type _Pos1, size_type _Pos2, _STD string _Option = "Line")
    {
        _STL_VERIFY(_Option == "Line" || _Option == "Column", "invalid option");
        basic_matrix<_Ty, _Column, _Line> _Cpy = *this;
        if (_Option == "Line")
        {
            _STL_VERIFY(_Pos1 <= _Line && _Pos2 <= _Line, "matrix subscript out of range");
            for (size_t i = 0; i < _Column; i++)
            {
                _Cpy[i][_Pos1 - 1] = this->_Elems[i][_Pos2 - 1];
                _Cpy[i][_Pos2 - 1] = this->_Elems[i][_Pos1 - 1];
            }
        }
        else if (_Option == "Column")
        {
            _STL_VERIFY(_Pos1 <= _Column && _Pos2 <= _Column, "matrix subscript out of range");
            _Cpy[_Pos1 - 1].swap(_Cpy[_Pos2 - 1]);
        }
        return _Cpy;
    }

    _NODISCARD constexpr basic_matrix<_Ty, _Column, _Line> scale(size_type _Pos, _Ty _Multiply, _STD string _Option = "Line")
    {
        _STL_VERIFY(_Option == "Line" || _Option == "Column", "invalid option");
        basic_matrix<_Ty, _Column, _Line> _Cpy = *this;
        if (_Option == "Line")
        {
            _STL_VERIFY(_Pos <= _Line, "matrix subscript out of range");
            for (size_t i = 0; i < _Column; i++)
            {
                _Cpy[i][_Pos - 1] *= _Multiply;
            }
        }
        else if (_Option == "Column")
        {
            _STL_VERIFY(_Pos <= _Column, "matrix subscript out of range");
            for (size_t i = 0; i < _Line; i++)
            {
                _Cpy[_Pos - 1][i] *= _Multiply;
            }
        }
        return _Cpy;
    }

    _NODISCARD constexpr basic_matrix<_Ty, _Column, _Line> add(size_type _Pos1, size_type _Pos2, _Ty _Multiply, _STD string _Option = "Line")
    {
        _STL_VERIFY(_Option == "Line" || _Option == "Column", "invalid option");
        basic_matrix<_Ty, _Column, _Line> _Cpy = *this;
        if (_Option == "Line")
        {
            _STL_VERIFY(_Pos1 <= _Line && _Pos2 <= _Line, "matrix subscript out of range");
            for (size_t i = 0; i < _Column; i++)
            {
                _Cpy[i][_Pos2 - 1] += _Cpy[i][_Pos1 - 1] * _Multiply;
            }
        }
        else if (_Option == "Column")
        {
            _STL_VERIFY(_Pos1 <= _Column && _Pos2 <= _Column, "matrix subscript out of range");
            for (size_t i = 0; i < _Line; i++)
            {
                _Cpy[_Pos2 - 1][i] += _Cpy[_Pos1 - 1][i] * _Multiply;
            }
        }
        return _Cpy;
    }

    // -- Functions in linear algebra --

    /// <summary>
    /// Returns minor of the entry in the i th row and j th column
    /// </summary>
    /// <param name="_CPos">column</param>
    /// <param name="_LPos">row</param>
    basic_matrix<_Ty, _Column - 1, _Line - 1> Minor(_In_range_(1, _Column) size_type _CPos, _In_range_(1, _Line) size_type _LPos)const;

    /// <summary>
    /// Returns the determinant value of the matrix.
    /// WARNING: This function is EXTREMELY INEFFICIENT for high-dimension matrices, with a time complexity in RIDICULOUS big O notation of O(n!)
    /// (But a new algorithm has found, the O notation has been reduced to O(n^3).)
    /// Reference: 
    /// https://www.tutorialspoint.com/cplusplus-program-to-compute-determinant-of-a-matrix
    /// https://codereview.stackexchange.com/questions/204135/determinant-using-gauss-elimination
    /// </summary>
    float64 Determinant()const;

    /// <summary>
    /// Returns the transpose of a matrix
    /// </summary>
    basic_matrix<_Ty, _Line, _Column> Transpose()const;

    /// <summary>
    /// Returns the Adjugate matrix
    /// </summary>
    basic_matrix<_Ty, _Line, _Column> Adjugate()const;

    /// <summary>
    /// Returns the Inverse matrix
    /// </summary>
    basic_matrix<_Ty, _Line, _Column> Inverse()const;

    /// <summary>
    /// Finding the rank of a matrix with Gaussian elimination.
    /// Reference: https://cp-algorithms.com/linear_algebra/rank-matrix.html#implementation
    /// </summary>
    uint64 Rank()const;
};

// -- Unary operators --

template<typename _Ty, size_t _Column, size_t _Line> // OpenCV style output
constexpr std::ostream& operator<<(std::ostream& os, const basic_matrix<_Ty, _Column, _Line>& c);

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator+(const basic_matrix<_Ty, _Column, _Line>& m);

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator-(const basic_matrix<_Ty, _Column, _Line>& m);

// -- Binary operators --

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator+(const basic_matrix<_Ty, _Column, _Line>& m1, const basic_matrix<_Ty, _Column, _Line>& m2);

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator-(const basic_matrix<_Ty, _Column, _Line>& m1, const basic_matrix<_Ty, _Column, _Line>& m2);

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator*(const basic_matrix<_Ty, _Column, _Line>& m1, const _Ty& scalar);

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator*(const _Ty& scalar, const basic_matrix<_Ty, _Column, _Line>& m1);

template<typename _Ty, size_t _Column, size_t _Line, size_t _Size>
basic_matrix<_Ty, _Column, _Line> operator*(const basic_matrix<_Ty, _Size, _Line>& m1, const basic_matrix<_Ty, _Column, _Size>& m2);

template<typename _Ty, size_t _Column, size_t _Line>
basic_matrix<_Ty, _Column, _Line> operator/(const basic_matrix<_Ty, _Column, _Line>& m1, const _Ty& scalar);

template<typename _Ty, size_t _Column, size_t _Line, size_t _Size>
basic_matrix<_Ty, _Column, _Line> operator/(const basic_matrix<_Ty, _Size, _Line>& m1, const basic_matrix<_Ty, _Column, _Size>& m2);

_GL_END

using mat2   = _GL basic_matrix<float64, 2, 2>;
using mat2x2 = _GL basic_matrix<float64, 2, 2>;
using mat2x3 = _GL basic_matrix<float64, 2, 3>;
using mat2x4 = _GL basic_matrix<float64, 2, 4>;
using mat3x2 = _GL basic_matrix<float64, 3, 2>;
using mat3   = _GL basic_matrix<float64, 3, 3>;
using mat3x3 = _GL basic_matrix<float64, 3, 3>;
using mat3x4 = _GL basic_matrix<float64, 3, 4>;
using mat4x2 = _GL basic_matrix<float64, 4, 2>;
using mat4x3 = _GL basic_matrix<float64, 4, 3>;
using mat4   = _GL basic_matrix<float64, 4, 4>;
using mat4x4 = _GL basic_matrix<float64, 4, 4>;

template<typename _Ty, size_t _Column, size_t _Line>
using matrix = _GL basic_matrix<_Ty, _Column, _Line>;

_CSE_END

#include "matrix.inl"

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif