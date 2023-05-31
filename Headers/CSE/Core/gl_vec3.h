#pragma once
// 3-component vector from glsl
#ifndef __GLVECTOR3__
#define __GLVECTOR3__

#include <array>
#include <vector>
#include <ostream>
#include "CSE/Core/CSECore.h"

_CSE_BEGIN
_GL_BEGIN

template<typename genType>
class gl_vec3
{
public:
	union { genType x, s, r; };
	union { genType y, t, g; };
	union { genType z, u, b; };

	// -- Component accesses --

	/// Return the count of components of the vector
	static constexpr size_t size() { return 3; }

	constexpr genType& operator[](size_t i);
	constexpr genType const& operator[](size_t i) const;

	// -- Implicit basic constructors --

	constexpr gl_vec3() = default;
	constexpr gl_vec3(gl_vec3 const& v) = default;

	// -- Explicit basic constructors --

	constexpr explicit gl_vec3(genType scalar) : x(scalar), y(scalar), z(scalar) {};
	constexpr gl_vec3(genType _x, genType _y, genType _z) : x(_x), y(_y), z(_z) {};

	// -- Conversion vector constructors --
	/// Explicit conversions (From section 5.4.1 Conversion and scalar constructors of GLSL 1.30.08 specification)
	template<typename genTypeA, typename genTypeB>
	constexpr gl_vec3(gl_vec2<genTypeA> const& _xy, genTypeB _z)
		: x(static_cast<genType>(_xy.x))
		, y(static_cast<genType>(_xy.y))
		, z(static_cast<genType>(_z)){}

	template<typename genTypeA, typename genTypeB>
	constexpr gl_vec3(genTypeA _x, gl_vec2<genTypeB> const& _yz)
		: x(static_cast<genType>(_x))
		, y(static_cast<genType>(_yz.x))
		, z(static_cast<genType>(_yz.y)){}

	constexpr gl_vec3& operator+=(const genType& scalar);
	constexpr gl_vec3& operator+=(const gl_vec3& v);
	constexpr gl_vec3& operator-=(const genType& scalar);
	constexpr gl_vec3& operator-=(const gl_vec3& v);
	constexpr gl_vec3& operator*=(const genType& scalar);
	constexpr gl_vec3& operator*=(const gl_vec3& v);
	constexpr gl_vec3& operator/=(const genType& scalar);
	constexpr gl_vec3& operator/=(const gl_vec3& v);

	// --STL Compatbility--

	constexpr explicit gl_vec3(std::array<genType, 3> _xyz)
	{
		x = _xyz[0];
		y = _xyz[1];
		z = _xyz[2];
	}

	operator std::array<genType, 3>()
	{
		return std::array<genType, 3>({ x, y, z });
	}

	constexpr explicit gl_vec3(std::vector<genType> _xyz)
	{
		x = _xyz[0];
		y = _xyz[1];
		z = _xyz[2];
	}

	operator std::vector<genType>()
	{
		return std::vector<genType>({ x, y, z });
	}
};

// -- Unary operators --

template<typename genType>
constexpr std::ostream& operator<<(std::ostream& os, const gl_vec3<genType>& c);

template<typename genType>
constexpr gl_vec3<genType> operator+(gl_vec3<genType> const& v);

template<typename genType>
constexpr gl_vec3<genType> operator-(gl_vec3<genType> const& v);

// -- Binary operators --

template<typename genType>
constexpr gl_vec3<genType> operator+(gl_vec3<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec3<genType> operator+(genType scalar, gl_vec3<genType> const& v);

template<typename genType>
constexpr gl_vec3<genType> operator+(gl_vec3<genType> const& v1, gl_vec3<genType> const& v2);

template<typename genType>
constexpr gl_vec3<genType> operator-(gl_vec3<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec3<genType> operator-(genType scalar, gl_vec3<genType> const& v);

template<typename genType>
constexpr gl_vec3<genType> operator-(gl_vec3<genType> const& v1, gl_vec3<genType> const& v2);

template<typename genType>
constexpr gl_vec3<genType> operator*(gl_vec3<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec3<genType> operator*(genType scalar, gl_vec3<genType> const& v);

template<typename genType>
constexpr gl_vec3<genType> operator*(gl_vec3<genType> const& v1, gl_vec3<genType> const& v2);

template<typename genType>
constexpr gl_vec3<genType> operator/(gl_vec3<genType> const& v, genType scalar);

template<typename genType>
constexpr gl_vec3<genType> operator/(genType scalar, gl_vec3<genType> const& v);

template<typename genType>
constexpr gl_vec3<genType> operator/(gl_vec3<genType> const& v1, gl_vec3<genType> const& v2);

_GL_END

using vec3 = _GL gl_vec3<float64>;
using ivec3 = _GL gl_vec3<int64>;
using bvec3 = _GL gl_vec3<bool>;
using uvec3 = _GL gl_vec3<uint64>;

_CSE_END

#include "CSE/Core/Inline/gl_vec3.inl"

#endif