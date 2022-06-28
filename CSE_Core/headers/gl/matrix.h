// Basic Matrix standard header (GLSL)

// Reference:
// [GLSL Document]https://www.khronos.org/registry/OpenGL/specs/gl/GLSLangSpec.4.20.pdf
// [GLM by G-Truc]https://glm.g-truc.net/0.9.4/api/a00142.html#gade0eb47c01f79384a6f38017ede17446
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
class _Matrix_const_iterator
{
    // TODO...
};

template<typename _Ty, size_t _Column, size_t _Line>
class _Matrix_iterator : public _Matrix_const_iterator<_Ty, _Line, _Column>
{
    // TODO...
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

    using reverse_iterator       = _STD reverse_iterator<iterator>;
    using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

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
    /// Reference: https://www.tutorialspoint.com/cplusplus-program-to-compute-determinant-of-a-matrix
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

_CSE_END

#include "matrix.inl"

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif