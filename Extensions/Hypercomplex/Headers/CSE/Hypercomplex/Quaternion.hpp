#pragma once

// Hypercomplex - Quaternion

#ifndef _HYPERCOMPLEX_QUATEERNION
#define _HYPERCOMPLEX_QUATEERNION

#include "Hypercomplex_Base.h"

// Reference:
// https://en.wikipedia.org/wiki/Quaternion

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

template <class _Ty>
class Quaternion : public Hypercomplex<_Ty, _GL gl_vec4<_Ty>>
{
public:
	using _Mybase = Hypercomplex<_Ty, _GL gl_vec4<_Ty>>;

	//_DEPRECATE_NONFLOATING_HYPERCOMPLEX
	constexpr Quaternion(const _Ty& _Realval = _Ty(), const _Ty& _Imagval0 = _Ty(), 
		const _Ty& _Imagval1 = _Ty(), const _Ty& _Imagval2 = _Ty())
	: _Mybase(_GL gl_vec4<_Ty>(_Realval, _Imagval0, _Imagval1, _Imagval2)) {}

	_CONSTEXPR20 Quaternion& operator=(const _Ty& _Right)
	{
		this->x = _Right;
		this->y = _Ty();
		this->z = _Ty();
		this->w = _Ty();
		return *this;
	}

	template <class _Other>
	//_DEPRECATE_NONFLOATING_HYPERCOMPLEX
	constexpr Quaternion(const Quaternion<_Other>& _Right)
	: _Mybase(_GL gl_vec4<_Ty>(static_cast<_Ty>(_Right.real()), static_cast<_Ty>(_Right.imag0()), 
		static_cast<_Ty>(_Right.imag1()), static_cast<_Ty>(_Right.imag2()))) {}

	template <class _Other>
	_CONSTEXPR20 Quaternion& operator=(const Quaternion<_Other>& _Right)
	{
		this->x = static_cast<_Ty>(_Right.real());
		this->y = static_cast<_Ty>(_Right.imag0());
		this->z = static_cast<_Ty>(_Right.imag1());
		this->w = static_cast<_Ty>(_Right.imag2());
		return *this;
	}

	// IMPLEMENTS OF BASE

	_CONSTEXPR20 void real(const _Ty& _Right){this->x = _Right;}// set real component
	_CONSTEXPR20 void imag0(const _Ty& _Right){this->y = _Right;}// set imaginary component
	_CONSTEXPR20 void imag1(const _Ty& _Right){this->z = _Right;}// set imaginary component
	_CONSTEXPR20 void imag2(const _Ty& _Right){this->w = _Right;}// set imaginary component

	_NODISCARD constexpr _Ty real() const{return this->x;}// return real component
	_NODISCARD constexpr _Ty imag0() const{return this->y;}// return imaginary component
	_NODISCARD constexpr _Ty imag1() const{return this->z;}// return imaginary component
	_NODISCARD constexpr _Ty imag2() const{return this->w;}// return imaginary component

	_CONSTEXPR20 Quaternion& operator+=(const _Ty& _Right)
	{
		this->x += (_Right);
		return *this;
	}

	_CONSTEXPR20 Quaternion& operator-=(const _Ty& _Right)
	{
		this->x -= (_Right);
		return *this;
	}

	_CONSTEXPR20 Quaternion& operator*=(const _Ty& _Right)
	{
		_GL gl_vec4<_Ty>::operator*=(_Right);
		return *this;
	}

	_CONSTEXPR20 Quaternion& operator/=(const _Ty& _Right)
	{
		_GL gl_vec4<_Ty>::operator/=(_Right);
		return *this;
	}

	_CONSTEXPR20 Quaternion& operator+=(const Quaternion& _Right)
	{
		_GL gl_vec4<_Ty>::operator+=(_Right);
		return *this;
	}

	_CONSTEXPR20 Quaternion& operator-=(const Quaternion& _Right)
	{
		_GL gl_vec4<_Ty>::operator-=(_Right);
		return *this;
	}

	_CONSTEXPR20 Quaternion& operator*=(const Quaternion& _Right)
	{
		_Ty _Rightreal  = static_cast<_Ty>(_Right.real());
		_Ty _Rightimag0 = static_cast<_Ty>(_Right.imag0());
		_Ty _Rightimag1 = static_cast<_Ty>(_Right.imag1());
		_Ty _Rightimag2 = static_cast<_Ty>(_Right.imag2());

		_Ty _TmpR = this->x * _Rightreal - this->y * _Rightimag0 - this->z * _Rightimag1 - this->w * _Rightimag2;
		_Ty _TmpI = this->x * _Rightimag0 + this->y * _Rightreal + this->z * _Rightimag2 - this->w * _Rightimag1;
		_Ty _TmpJ = this->x * _Rightimag1 - this->y * _Rightimag2 + this->z * _Rightreal + this->w * _Rightimag0;
		_Ty _TmpK = this->x * _Rightimag2 + this->y * _Rightimag1 - this->z * _Rightimag0 + this->w * _Rightreal;
		this->x = _TmpR;
		this->y = _TmpI;
		this->z = _TmpJ;
		this->w = _TmpK;
		return *this;
	}

	_CONSTEXPR20 Quaternion& operator/=(const Quaternion& _Right)
	{
		*this *= this->Inverse();
		return *this;
	}

	// Algebra

	_CONSTEXPR20 Quaternion Inverse()const
	{
		return Quaternion(this->x, -this->y, -this->z, -this->w) / 
			(cse::pow(this->x, 2) + cse::pow(this->y, 2) + cse::pow(this->z, 2) + cse::pow(this->w, 2));
	}

	// Converts
	template <class _Other>
	//_DEPRECATE_NONFLOATING_HYPERCOMPLEX
	constexpr Quaternion(const _GL gl_vec4<_Other>& _Right) : _Mybase(_Right) {}

	template <class _Other = _Ty>
	operator _GL gl_vec4<_Other>()
	{
		return _GL gl_vec4<_Other>(this->x, this->y, this->z, this->w);
	}
};

// Binary operations

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator+(const Quaternion<_Ty>& _Left, const Quaternion<_Ty>& _Right)
{
	Quaternion<_Ty> _Tmp(_Left);
	_Tmp += _Right;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator+(const Quaternion<_Ty>& _Left, const _Ty& _Right)
{
	Quaternion<_Ty> _Tmp(_Left);
	_Tmp += _Right;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator+(const _Ty& _Left, const Quaternion<_Ty>& _Right)
{
	Quaternion<_Ty> _Tmp(_Right);
	_Tmp += _Left;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator-(const Quaternion<_Ty>& _Left, const Quaternion<_Ty>& _Right)
{
	Quaternion<_Ty> _Tmp(_Left);
	_Tmp -= _Right;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator-(const Quaternion<_Ty>& _Left, const _Ty& _Right)
{
	Quaternion<_Ty> _Tmp(_Left);
	_Tmp -= _Right;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator-(const _Ty& _Left, const Quaternion<_Ty>& _Right)
{
	Quaternion<_Ty> _Tmp(_Left);
	_Tmp -= _Right;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator*(const Quaternion<_Ty>& _Left, const Quaternion<_Ty>& _Right)
{
	Quaternion<_Ty> _Tmp(_Left);
	_Tmp *= _Right;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator*(const Quaternion<_Ty>& _Left, const _Ty& _Right)
{
	Quaternion<_Ty> _Tmp(_Left);
	_Tmp *= _Right;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator*(const _Ty& _Left, const Quaternion<_Ty>& _Right)
{
	Quaternion<_Ty> _Tmp(_Right);
	_Tmp *= _Left;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator/(const Quaternion<_Ty>& _Left, const Quaternion<_Ty>& _Right)
{
	Quaternion<_Ty> _Tmp(_Left);
	_Tmp /= _Right;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator/(const Quaternion<_Ty>& _Left, const _Ty& _Right)
{
	Quaternion<_Ty> _Tmp(_Left);
	_Tmp /= _Right;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator/(const _Ty& _Left, const Quaternion<_Ty>& _Right)
{
	Quaternion<_Ty> _Tmp(_Left);
	_Tmp /= _Right;
	return _Tmp;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator+(const Quaternion<_Ty>& _Left)
{
	return _Left;
}

template <class _Ty>
_NODISCARD _CONSTEXPR20 Quaternion<_Ty> operator-(const Quaternion<_Ty>& _Left)
{
	return _Left * -1;
}

template <class _Ty>
_NODISCARD constexpr bool operator==(const Quaternion<_Ty>& _Left, const Quaternion<_Ty>& _Right)
{
	return _Left.real() == _Right.real() && _Left.imag0() == _Right.imag0() && 
		_Left.imag1() == _Right.imag1() && _Left.imag2() == _Right.imag2();
}

template <class _Ty>
_NODISCARD constexpr bool operator==(const Quaternion<_Ty>& _Left, const _Ty& _Right)
{
	return _Left.real() == _Right && _Left.imag0() == 0 && _Left.imag1() == 0 && _Left.imag2() == 0;
}

// Output

template <class _Ty, class _Elem, class _Tr>
_STD basic_ostream<_Elem, _Tr>& operator<<(_STD basic_ostream<_Elem, _Tr>& _Ostr, const Quaternion<_Ty>& _Right)
{
	const _STD ctype<_Elem>& _Ctype_fac = _STD use_facet<_STD ctype<_Elem>>(_Ostr.getloc());
	_STD basic_ostringstream<_Elem, _Tr, _STD allocator<_Elem>> _Sstr;

	_Sstr.flags(_Ostr.flags());
	_Sstr.imbue(_Ostr.getloc());
	_Sstr.precision(_Ostr.precision());
	_Sstr << _Ctype_fac.widen('(') << _Right.real() << _Ctype_fac.widen(',') << _Right.imag0() << _Ctype_fac.widen(',') 
		<< _Right.imag1() << _Ctype_fac.widen(',') << _Right.imag2() << _Ctype_fac.widen(')');

	return _Ostr << _Sstr.str();
}

inline namespace hypercomplex_literals // The algebra of quaternions is often denoted by H (Hamilton)
{
	_NODISCARD constexpr Quaternion<float64> operator""ih(long double _Val)
	{
		return Quaternion<float64>(0.0, static_cast<float64>(_Val), 0.0, 0.0);
	}

	_NODISCARD constexpr Quaternion<float64> operator""ih(unsigned long long _Val)
	{
		return Quaternion<float64>(0.0, static_cast<float64>(_Val), 0.0, 0.0);
	}

	_NODISCARD constexpr Quaternion<float64> operator""jh(long double _Val)
	{
		return Quaternion<float64>(0.0, 0.0, static_cast<float64>(_Val), 0.0);
	}

	_NODISCARD constexpr Quaternion<float64> operator""jh(unsigned long long _Val)
	{
		return Quaternion<float64>(0.0, 0.0, static_cast<float64>(_Val), 0.0);
	}

	_NODISCARD constexpr Quaternion<float64> operator""kh(long double _Val)
	{
		return Quaternion<float64>(0.0, 0.0, 0.0, static_cast<float64>(_Val));
	}

	_NODISCARD constexpr Quaternion<float64> operator""kh(unsigned long long _Val)
	{
		return Quaternion<float64>(0.0, 0.0, 0.0, static_cast<float64>(_Val));
	}
}

// Maths and Algorithms

template <class _Ty>
_NODISCARD _Ty abs(const Quaternion<_Ty>& _Left)
{
	return Length((vec4)_Left);
}

template <class _Ty>
_NODISCARD _STD array<Quaternion<_Ty>, 2> sqrt(const Quaternion<_Ty>& _Left)
{
	_Ty _Real = _Left.real();
	_GL gl_vec3<_Ty> _Imag(_Left.imag0(), _Left.imag1(), _Left.imag2());

	_Ty _R1 = sqrt((abs(_Left) + _Real) / 2.);
	_GL gl_vec3<_Ty> _I1 = normalize(_Imag) * cse::sqrt((abs(_Left) - _Real) / 2.);
	return { _GL gl_vec4<_Ty>(_R1, _I1), -_GL gl_vec4<_Ty>(_R1, _I1) };
}

// Quaternionic analysis

template <class _Ty>
_NODISCARD Quaternion<_Ty> exp(const Quaternion<_Ty>& _Left)
{
	_Ty _Real = _Left.real();
	_GL gl_vec3<_Ty> _Imag(_Left.imag0(), _Left.imag1(), _Left.imag2());

	return exp(_Real) * vec4(cos(Length(_Imag)), normalize(_Imag) * sin(Length(_Imag)));
}

template <class _Ty>
_NODISCARD Quaternion<_Ty> ln(const Quaternion<_Ty>& _Left)
{
	_Ty _Real = _Left.real();
	_GL gl_vec3<_Ty> _Imag(_Left.imag0(), _Left.imag1(), _Left.imag2());

	return vec4(ln(abs(_Left)), normalize(_Imag) * arccos(_Real / abs(_Left)));
}

template <class _Ty>
_NODISCARD Quaternion<_Ty> pow(const Quaternion<_Ty>& _Left, const _Ty& _Right)
{
	return exp(_Right * ln(_Left));
}

template <class _Ty>
_NODISCARD Quaternion<_Ty> pow(const _Ty& _Left, const Quaternion<_Ty>& _Right)
{
	return exp(_Right * ln(_Left));
}

template <class _Ty>
_NODISCARD Quaternion<_Ty> pow(const Quaternion<_Ty>& _Left, const Quaternion<_Ty>& _Right)
{
	return exp(_Right * ln(_Left));
}

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif