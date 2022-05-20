#include <cassert>
#include "..\Core\CSECore.h"
#include "gl_vec3.h"

_CSE_BEGIN
_GL_BEGIN

template<typename genType>
inline constexpr genType& gl_vec3<genType>::operator[](size_t i)
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
	}
}

template<typename genType>
inline constexpr genType const& gl_vec3<genType>::operator[](size_t i) const
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
	}
}

template<typename genType>
constexpr std::ostream& operator<<(std::ostream& os, const gl_vec3<genType>& c)
{
	return os << '(' << c.x << ", " << c.y << ", " << c.z << ')';
}

template<typename genType>
constexpr gl_vec3<genType> operator+(gl_vec3<genType> const& v, genType scalar)
{
	return gl_vec3<genType>(v.x + scalar, v.y + scalar, v.z + scalar);
}

template<typename genType>
constexpr gl_vec3<genType> operator+(genType scalar, gl_vec3<genType> const& v)
{
	return gl_vec3<genType>(v.x + scalar, v.y + scalar, v.z + scalar);
}

template<typename genType>
constexpr gl_vec3<genType> operator+(gl_vec3<genType> const& v1, gl_vec3<genType> const& v2)
{
	return gl_vec3<genType>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template<typename genType>
constexpr gl_vec3<genType> operator-(gl_vec3<genType> const& v, genType scalar)
{
	return gl_vec3<genType>(v.x - scalar, v.y - scalar, v.z - scalar);
}

template<typename genType>
constexpr gl_vec3<genType> operator-(genType scalar, gl_vec3<genType> const& v)
{
	return gl_vec3<genType>(scalar - v.x , scalar - v.y, scalar - v.z);
}

template<typename genType>
constexpr gl_vec3<genType> operator-(gl_vec3<genType> const& v1, gl_vec3<genType> const& v2)
{
	return gl_vec3<genType>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template<typename genType>
constexpr gl_vec3<genType> operator*(gl_vec3<genType> const& v, genType scalar)
{
	return gl_vec3<genType>(v.x * scalar, v.y * scalar, v.z * scalar);
}

template<typename genType>
constexpr gl_vec3<genType> operator*(genType scalar, gl_vec3<genType> const& v)
{
	return gl_vec3<genType>(v.x * scalar, v.y * scalar, v.z * scalar);
}

template<typename genType>
constexpr gl_vec3<genType> operator*(gl_vec3<genType> const& v1, gl_vec3<genType> const& v2)
{
	return gl_vec3<genType>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

template<typename genType>
constexpr gl_vec3<genType> operator/(gl_vec3<genType> const& v, genType scalar)
{
	return gl_vec3<genType>(v.x / scalar, v.y / scalar, v.z / scalar);
}

template<typename genType>
constexpr gl_vec3<genType> operator/(genType scalar, gl_vec3<genType> const& v)
{
	return gl_vec3<genType>(scalar / v.x, scalar / v.y, scalar / v.z);
}

template<typename genType>
constexpr gl_vec3<genType> operator/(gl_vec3<genType> const& v1, gl_vec3<genType> const& v2)
{
	return gl_vec3<genType>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

_GL_END
_CSE_END