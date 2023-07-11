#include <cassert>
#include "CSE/Core/CSECore.h"
#include "CSE/Core/gl/gl_vec4.h"

_CSE_BEGIN
_GL_BEGIN

template<typename genType>
inline constexpr genType& gl_vec4<genType>::operator[](size_t i)
{
	assert(i >= 0 && i < this->size());
	switch (i)
	{
	default:
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}
}

template<typename genType>
inline constexpr genType const& gl_vec4<genType>::operator[](size_t i) const
{
	assert(i >= 0 && i < this->size());
	switch (i)
	{
	default:
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}
}

template<typename genType>
inline constexpr gl_vec4<genType>& gl_vec4<genType>::operator+=(const genType& scalar)
{
	this->x += scalar;
	this->y += scalar;
	this->z += scalar;
	this->w += scalar;
	return *this;
}

template<typename genType>
inline constexpr gl_vec4<genType>& gl_vec4<genType>::operator+=(const gl_vec4& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;
	return *this;
}

template<typename genType>
inline constexpr gl_vec4<genType>& gl_vec4<genType>::operator-=(const genType& scalar)
{
	this->x -= scalar;
	this->y -= scalar;
	this->z -= scalar;
	this->w -= scalar;
	return *this;
}

template<typename genType>
inline constexpr gl_vec4<genType>& gl_vec4<genType>::operator-=(const gl_vec4& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;
	return *this;
}

template<typename genType>
inline constexpr gl_vec4<genType>& gl_vec4<genType>::operator*=(const genType& scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
	this->w *= scalar;
	return *this;
}

template<typename genType>
inline constexpr gl_vec4<genType>& gl_vec4<genType>::operator*=(const gl_vec4& v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	this->w *= v.w;
	return *this;
}

template<typename genType>
inline constexpr gl_vec4<genType>& gl_vec4<genType>::operator/=(const genType& scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	this->z /= scalar;
	this->w /= scalar;
	return *this;
}

template<typename genType>
inline constexpr gl_vec4<genType>& gl_vec4<genType>::operator/=(const gl_vec4& v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	this->w /= v.w;
	return *this;
}

template<typename genType>
constexpr std::ostream& operator<<(std::ostream& os, const gl_vec4<genType>& c)
{
	return os << '(' << c.x << ", " << c.y << ", " << c.z << ", " << c.w << ')';
}

template<typename genType>
constexpr gl_vec4<genType> operator+(gl_vec4<genType> const& v)
{
	return v;
}

template<typename genType>
constexpr gl_vec4<genType> operator-(gl_vec4<genType> const& v)
{
	return gl_vec4<genType>(-v.x, -v.y, -v.z, -v.w);
}

template<typename genType>
constexpr gl_vec4<genType> operator+(gl_vec4<genType> const& v, genType scalar)
{
	return gl_vec4<genType>(v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar);
}

template<typename genType>
constexpr gl_vec4<genType> operator+(genType scalar, gl_vec4<genType> const& v)
{
	return gl_vec4<genType>(v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar);
}

template<typename genType>
constexpr gl_vec4<genType> operator+(gl_vec4<genType> const& v1, gl_vec4<genType> const& v2)
{
	return gl_vec4<genType>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

template<typename genType>
constexpr gl_vec4<genType> operator-(gl_vec4<genType> const& v, genType scalar)
{
	return gl_vec4<genType>(v.x - scalar, v.y - scalar, v.z - scalar, v.w - scalar);
}

template<typename genType>
constexpr gl_vec4<genType> operator-(genType scalar, gl_vec4<genType> const& v)
{
	return gl_vec4<genType>(scalar - v.x, scalar - v.y, scalar - v.z, scalar - v.w);
}

template<typename genType>
constexpr gl_vec4<genType> operator-(gl_vec4<genType> const& v1, gl_vec4<genType> const& v2)
{
	return gl_vec4<genType>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

template<typename genType>
constexpr gl_vec4<genType> operator*(gl_vec4<genType> const& v, genType scalar)
{
	return gl_vec4<genType>(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

template<typename genType>
constexpr gl_vec4<genType> operator*(genType scalar, gl_vec4<genType> const& v)
{
	return gl_vec4<genType>(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
}

template<typename genType>
constexpr gl_vec4<genType> operator*(gl_vec4<genType> const& v1, gl_vec4<genType> const& v2)
{
	return gl_vec4<genType>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

template<typename genType>
constexpr gl_vec4<genType> operator/(gl_vec4<genType> const& v, genType scalar)
{
	return gl_vec4<genType>(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
}

template<typename genType>
constexpr gl_vec4<genType> operator/(genType scalar, gl_vec4<genType> const& v)
{
	return gl_vec4<genType>(scalar / v.x, scalar / v.y, scalar / v.z, scalar / v.w);
}

template<typename genType>
constexpr gl_vec4<genType> operator/(gl_vec4<genType> const& v1, gl_vec4<genType> const& v2)
{
	return gl_vec4<genType>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

_GL_END
_CSE_END