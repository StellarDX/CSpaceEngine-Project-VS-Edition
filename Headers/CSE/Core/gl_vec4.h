#pragma once
// 3-component vector from glsl
#ifndef __GLVECTOR4__
#define __GLVECTOR4__

#include <array>
#include <vector>
#include <ostream>
#include "CSE/Core/CSECore.h"

_CSE_BEGIN
_GL_BEGIN

template<typename genType>
class gl_vec4
{
public:
	union { genType x, s, r; };
	union { genType y, t, g; };
	union { genType z, u, b; };
	union { genType w, v, a; };

	// -- Component accesses --

	/// Return the count of components of the vector
	static constexpr size_t size() { return 4; }

	constexpr genType& operator[](size_t i);
	constexpr genType const& operator[](size_t i) const;

	// -- Implicit basic constructors --

	constexpr gl_vec4() = default;
	constexpr gl_vec4(gl_vec4 const& v) = default;

	// -- Explicit basic constructors --

	constexpr explicit gl_vec4(genType scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {};
	constexpr gl_vec4(genType _x, genType _y, genType _z, genType _w) : x(_x), y(_y), z(_z), w(_w) {};

	// -- Conversion vector constructors --
	/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
	template<typename genTypeA, typename genTypeB, typename genTypeC>
	constexpr gl_vec4(gl_vec2<genTypeA> const& _xy, genTypeB _z, genTypeC _w)
		: x(static_cast<genType>(_xy.x))
		, y(static_cast<genType>(_xy.y))
		, z(static_cast<genType>(_z))
		, w(static_cast<genType>(_w)){}

	template<typename genTypeA, typename genTypeB, typename genTypeC>
	constexpr gl_vec4(genTypeA _x, gl_vec2<genTypeB> const& _yz, genTypeC _w)
		: x(static_cast<genType>(_x))
		, y(static_cast<genType>(_yz.x))
		, z(static_cast<genType>(_yz.y))
		, w(static_cast<genType>(_w)){}

	template<typename genTypeA, typename genTypeB, typename genTypeC>
	constexpr gl_vec4(genTypeA _x, genTypeB _y, gl_vec2<genTypeC> const& _zw)
		: x(static_cast<genType>(_x))
		, y(static_cast<genType>(_y))
		, z(static_cast<genType>(_zw.x))
		, w(static_cast<genType>(_zw.y)){}

	template<typename genTypeA, typename genTypeB>
	constexpr gl_vec4(gl_vec3<genTypeA> const& _xyz, genTypeB _w)
		: x(static_cast<genType>(_xyz.x))
		, y(static_cast<genType>(_xyz.y))
		, z(static_cast<genType>(_xyz.z))
		, w(static_cast<genType>(_w)){}

	template<typename genTypeA, typename genTypeB>
	constexpr gl_vec4(gl_vec2<genTypeA> const& _xy, gl_vec2<genTypeB> const& _zw)
		: x(static_cast<genType>(_xy.x))
		, y(static_cast<genType>(_xy.y))
		, z(static_cast<genType>(_zw.x))
		, w(static_cast<genType>(_zw.y)){}

	template<typename genTypeA, typename genTypeB>
	constexpr gl_vec4(genTypeA _x, gl_vec3<genTypeB> const& _yzw)
		: x(static_cast<genType>(_x))
		, y(static_cast<genType>(_yzw.x))
		, z(static_cast<genType>(_yzw.y))
		, w(static_cast<genType>(_yzw.z)){}

	constexpr gl_vec4& operator+=(const genType& scalar);
	constexpr gl_vec4& operator+=(const gl_vec4& v);
	constexpr gl_vec4& operator-=(const genType& scalar);
	constexpr gl_vec4& operator-=(const gl_vec4& v);
	constexpr gl_vec4& operator*=(const genType& scalar);
	constexpr gl_vec4& operator*=(const gl_vec4& v);
	constexpr gl_vec4& operator/=(const genType& scalar);
	constexpr gl_vec4& operator/=(const gl_vec4& v);

	// --STL Compatbility--

	constexpr explicit gl_vec4(std::array<genType, 4> _xyzw)
	{
		x = _xyzw[0];
		y = _xyzw[1];
		z = _xyzw[2];
		w = _xyzw[3];
	}

	operator std::array<genType, 4>()
	{
		return std::array<genType, 4>(x, y, z, w);
	}

	constexpr explicit gl_vec4(std::vector<genType> _xyzw)
	{
		x = _xyzw[0];
		y = _xyzw[1];
		z = _xyzw[2];
		w = _xyzw[3];
	}

	operator std::vector<genType>()
	{
		return std::vector<genType>({ x, y, z, w });
	}
};

// -- Unary operators --

template<typename genType>
constexpr std::ostream& operator<<(std::ostream& os, const gl_vec4<genType>& c);

template<typename genType>
constexpr gl_vec4<genType> operator+(gl_vec4<genType> const& v);

template<typename genType>
constexpr gl_vec4<genType> operator-(gl_vec4<genType> const& v);

// -- Binary operators --

template<typename genType>
constexpr gl_vec4<genType> operator+(gl_vec4<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec4<genType> operator+(genType scalar, gl_vec4<genType> const& v);

template<typename genType>
constexpr gl_vec4<genType> operator+(gl_vec4<genType> const& v1, gl_vec4<genType> const& v2);

template<typename genType>
constexpr gl_vec4<genType> operator-(gl_vec4<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec4<genType> operator-(genType scalar, gl_vec4<genType> const& v);

template<typename genType>
constexpr gl_vec4<genType> operator-(gl_vec4<genType> const& v1, gl_vec4<genType> const& v2);

template<typename genType>
constexpr gl_vec4<genType> operator*(gl_vec4<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec4<genType> operator*(genType scalar, gl_vec4<genType> const& v);

template<typename genType>
constexpr gl_vec4<genType> operator*(gl_vec4<genType> const& v1, gl_vec4<genType> const& v2);

template<typename genType>
constexpr gl_vec4<genType> operator/(gl_vec4<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec4<genType> operator/(genType scalar, gl_vec4<genType> const& v);

template<typename genType>
constexpr gl_vec4<genType> operator/(gl_vec4<genType> const& v1, gl_vec4<genType> const& v2);

_GL_END

using vec4 = _GL gl_vec4<float64>;
using ivec4 = _GL gl_vec4<int64>;
using bvec4 = _GL gl_vec4<bool>;
using uvec4 = _GL gl_vec4<uint64>;

_CSE_END

#include "CSE/Core/Inline/gl_vec4.inl"

#endif