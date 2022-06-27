#pragma once
// 2-component vector from glsl
#ifndef __GLVECTOR2__
#define __GLVECTOR2__

#include <ostream>
#include "..\Core\CSECore.h"

_CSE_BEGIN
_GL_BEGIN

template<typename genType>
class gl_vec2
{
public:
	union { genType x, s, r; };
	union { genType y, t, g; };

	// -- Component accesses --

	/// Return the count of components of the vector
	static constexpr size_t size() { return 2; }

	constexpr genType& operator[](size_t i);
	constexpr genType const& operator[](size_t i) const;

	// -- Implicit basic constructors --

	constexpr gl_vec2() = default;
	constexpr gl_vec2(gl_vec2 const& v) = default;

	// -- Explicit basic constructors --

	constexpr explicit gl_vec2(genType scalar) : x(scalar), y(scalar){};
	constexpr gl_vec2(genType _x, genType _y) : x(_x), y(_y) {};

	// -- Unary arithmetic operators --

	constexpr gl_vec2& operator+=(const genType& scalar);
	constexpr gl_vec2& operator+=(const gl_vec2& v);
	constexpr gl_vec2& operator-=(const genType& scalar);
	constexpr gl_vec2& operator-=(const gl_vec2& v);
	constexpr gl_vec2& operator*=(const genType& scalar);
	constexpr gl_vec2& operator*=(const gl_vec2& v);
	constexpr gl_vec2& operator/=(const genType& scalar);
	constexpr gl_vec2& operator/=(const gl_vec2& v);
};

// -- Unary operators --

template<typename genType>
constexpr std::ostream& operator<<(std::ostream& os, const gl_vec2<genType>& c);

template<typename genType>
constexpr gl_vec2<genType> operator+(gl_vec2<genType> const& v);

template<typename genType>
constexpr gl_vec2<genType> operator-(gl_vec2<genType> const& v);

// -- Binary operators --

template<typename genType>
constexpr gl_vec2<genType> operator+(gl_vec2<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec2<genType> operator+(gl_vec2<genType> const& v1, gl_vec2<genType> const& v2);

template<typename genType>
constexpr gl_vec2<genType> operator+(genType scalar, gl_vec2<genType> const& v);

template<typename genType>
constexpr gl_vec2<genType> operator-(gl_vec2<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec2<genType> operator-(gl_vec2<genType> const& v1, gl_vec2<genType> const& v2);

template<typename genType>
constexpr gl_vec2<genType> operator-(genType scalar, gl_vec2<genType> const& v);

template<typename genType>
constexpr gl_vec2<genType> operator*(gl_vec2<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec2<genType> operator*(gl_vec2<genType> const& v1, gl_vec2<genType> const& v2);

template<typename genType>
constexpr gl_vec2<genType> operator*(genType scalar, gl_vec2<genType> const& v);

template<typename genType>
constexpr gl_vec2<genType> operator/(gl_vec2<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec2<genType> operator/(gl_vec2<genType> const& v1, gl_vec2<genType> const& v2);

template<typename genType>
constexpr gl_vec2<genType> operator/(genType scalar, gl_vec2<genType> const& v);

_GL_END

using vec2 = _GL gl_vec2<float64>;
using ivec2 = _GL gl_vec2<int64>;
using bvec2 = _GL gl_vec2<bool>;
using uvec2 = _GL gl_vec2<uint64>;

_CSE_END

#include "gl_vec2.inl"

#endif