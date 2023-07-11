#include <cassert>
#include "CSE/Core/CSECore.h"
#include "CSE/Core/gl/gl_vec2.h"

_CSE_BEGIN
_GL_BEGIN

template<typename genType>
inline constexpr genType& gl_vec2<genType>::operator[](size_t i)
{
	assert(i >= 0 && i < this->size());
	switch (i)
	{
	default:
	case 0:
		return x;
	case 1:
		return y;
	}
}

template<typename genType>
inline constexpr genType const& gl_vec2<genType>::operator[](size_t i) const
{
	assert(i >= 0 && i < this->size());
	switch (i)
	{
	default:
	case 0:
		return x;
	case 1:
		return y;
	}
}

template<typename genType>
inline constexpr gl_vec2<genType>& gl_vec2<genType>::operator+=(const genType& scalar)
{
	this->x += scalar;
	this->y += scalar;
	return *this;
}

template<typename genType>
inline constexpr gl_vec2<genType>& gl_vec2<genType>::operator+=(const gl_vec2& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

template<typename genType>
inline constexpr gl_vec2<genType>& gl_vec2<genType>::operator-=(const genType& scalar)
{
	this->x -= scalar;
	this->y -= scalar;
	return *this;
}

template<typename genType>
inline constexpr gl_vec2<genType>& gl_vec2<genType>::operator-=(const gl_vec2& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

template<typename genType>
inline constexpr gl_vec2<genType>& gl_vec2<genType>::operator*=(const genType& scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	return *this;
}

template<typename genType>
inline constexpr gl_vec2<genType>& gl_vec2<genType>::operator*=(const gl_vec2& v)
{
	this->x *= v.x;
	this->y *= v.y;
	return *this;
}

template<typename genType>
inline constexpr gl_vec2<genType>& gl_vec2<genType>::operator/=(const genType& scalar)
{
	this->x /= scalar;
	this->y /= scalar;
	return *this;
}

template<typename genType>
inline constexpr gl_vec2<genType>& gl_vec2<genType>::operator/=(const gl_vec2& v)
{
	this->x /= v.x;
	this->y /= v.y;
	return *this;
}

template<typename genType>
constexpr std::ostream& operator<<(std::ostream& os, const gl_vec2<genType>& c)
{
	return os << '(' << c.x << ", " << c.y << ')';
}

template<typename genType>
constexpr gl_vec2<genType> operator+(gl_vec2<genType> const& v)
{
	return v;
}

template<typename genType>
constexpr gl_vec2<genType> operator-(gl_vec2<genType> const& v)
{
	return gl_vec2<genType>(-v.x, -v.y);
}

template<typename genType>
constexpr gl_vec2<genType> operator+(gl_vec2<genType> const& v, genType scalar)
{
	return gl_vec2<genType>(v.x + scalar, v.y + scalar);
}

template<typename genType>
constexpr gl_vec2<genType> operator+(gl_vec2<genType> const& v1, gl_vec2<genType> const& v2)
{
	return gl_vec2<genType>(v1.x + v2.x, v1.y + v2.y);
}

template<typename genType>
constexpr gl_vec2<genType> operator+(genType scalar, gl_vec2<genType> const& v)
{
	return gl_vec2<genType>(v.x + scalar, v.y + scalar);
}

template<typename genType>
constexpr gl_vec2<genType> operator-(gl_vec2<genType> const& v, genType scalar)
{
	return gl_vec2<genType>(v.x - scalar, v.y - scalar);
}

template<typename genType>
constexpr gl_vec2<genType> operator-(gl_vec2<genType> const& v1, gl_vec2<genType> const& v2)
{
	return gl_vec2<genType>(v1.x - v2.x, v1.y - v2.y);
}

template<typename genType>
constexpr gl_vec2<genType> operator-(genType scalar, gl_vec2<genType> const& v)
{
	return gl_vec2<genType>(scalar - v.x, scalar - v.y);
}

template<typename genType>
constexpr gl_vec2<genType> operator*(gl_vec2<genType> const& v, genType scalar)
{
	return gl_vec2<genType>(v.x * scalar, v.y * scalar);
}

template<typename genType>
constexpr gl_vec2<genType> operator*(gl_vec2<genType> const& v1, gl_vec2<genType> const& v2)
{
	return gl_vec2<genType>(v1.x * v2.x, v1.y * v2.y);
}

template<typename genType>
constexpr gl_vec2<genType> operator*(genType scalar, gl_vec2<genType> const& v)
{
	return gl_vec2<genType>(v.x * scalar, v.y * scalar);
}

template<typename genType>
constexpr gl_vec2<genType> operator/(gl_vec2<genType> const& v, genType scalar)
{
	return gl_vec2<genType>(v.x / scalar, v.y / scalar);
}

template<typename genType>
constexpr gl_vec2<genType> operator/(gl_vec2<genType> const& v1, gl_vec2<genType> const& v2)
{
	return gl_vec2<genType>(v1.x / v2.x, v1.y / v2.y);
}

template<typename genType>
constexpr gl_vec2<genType> operator/(genType scalar, gl_vec2<genType> const& v)
{
	return gl_vec2<genType>(scalar / v.x, scalar / v.y);
}

_GL_END
_CSE_END