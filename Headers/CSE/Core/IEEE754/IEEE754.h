#pragma once

// The IEEE754 defines taken from glibc
/* Copyright (C) 1992-2023 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef _IEEE754_H
#define _IEEE754_H 1

#ifndef _CSE
#error This file must be included in namespace "CSE"
#endif

//_EXTERN_C

// Partisions for IEEE754 standard double-precision floating point number.
// It is constructs of 1-bit for sign, 11-bit for exponential and 52-bit for fractional.
// CSpaceEngine only use 64-bit precision for calculation.
// Reference: 
union IEEE754_Dbl64
{
	double Val;
	unsigned long long Bytes;
	struct
	{
		unsigned int Mantissa1 : 32; // Lower-32 bits of fractional
		unsigned int Mantissa0 : 20; // Higher-20 bits of fractional
		unsigned int Exponent : 11;  // Exponential bits
		unsigned int Negative : 1;   // Sign bit
	};

	struct
	{
		uint32_t lsw;
		uint32_t msw;
	} parts;

	// Constructors
	IEEE754_Dbl64() {}
	IEEE754_Dbl64(double d) : Val(d) {}
	operator double() { return Val; }

	static IEEE754_Dbl64 FromBytes(unsigned long long _By)
	{
		IEEE754_Dbl64 d;
		d.Bytes = _By;
		return d;
	}

	// Generate unary operator
	#define IEEE754_UNARY_OPERATOR(operation) \
	_CONSTEXPR20 IEEE754_Dbl64& operator operation (const IEEE754_Dbl64& scaler) \
	{ \
		this->Val operation scaler.Val; \
		return *this; \
	} \

	IEEE754_UNARY_OPERATOR(+=)
	IEEE754_UNARY_OPERATOR(-=)
	IEEE754_UNARY_OPERATOR(*=)
	IEEE754_UNARY_OPERATOR(/=)

	#undef IEEE754_UNARY_OPERATOR
};
#if 0
#define IEEE754_BINARY_OPERATOR(operation) \
IEEE754_Dbl64 operator operation (IEEE754_Dbl64 const& _Ax0, IEEE754_Dbl64 const& _Bx0) \
{ \
	return IEEE754_Dbl64(_Ax0.Val operation _Bx0.Val); \
} \

IEEE754_BINARY_OPERATOR(+)
IEEE754_BINARY_OPERATOR(-)
IEEE754_BINARY_OPERATOR(*)
IEEE754_BINARY_OPERATOR(/)

#undef IEEE754_BINARY_OPERATOR
#endif
// Values only for development.(Be careful to use!!!)
// -------------------------------------------------------------------------
//      Name                 Value                 Note
// The following defines are pecular values in double format.
#define MIN_SNORM_POS_DOUBLE 0x0000000000000001 // Min. subnormal positive double
#define MAX_SNORM_DOUBLE     0x000FFFFFFFFFFFFF // Max. subnormal double
#define MIN_NORM_POS_DOUBLE  0x0010000000000000 // Min. normal positive double
#define MAX_DOUBLE           0x7FEFFFFFFFFFFFFF // Max. Double
#define POS_ZERO_DOUBLE      0x0000000000000000 // +0
#define NEG_ZERO_DOUBLE      0x8000000000000000 // -0
#define POS_INF_DOUBLE       0x7FF0000000000000 // +inf
#define NEG_INF_DOUBLE       0xFFF0000000000000 // -inf
#define S_NAN_DOUBLE         0x7FF0000000000001 // Singaling NAN
#define Q_NAN_DOUBLE         0x7FF8000000000001 // Quiet NAN
#define BIG_NAN_DOUBLE       0x7FFFFFFFFFFFFFFF // an alternative encoding of NaN

#if 0
union IEEE754_Float128 // Quaduple-presision floating point
{
	__float128 Val;
	unsigned __int128 Bytes;
	struct
	{
		uint64_t Mantissa1 : 64; // Lower-64 bits of fractional
		uint64_t Mantissa0 : 48; // Higher-48 bits of fractional
		uint64_t Exponent  : 15;  // Exponential bits
		uint64_t Negative  : 1;   // Sign bit
	};

	struct
	{
		uint64_t msw;
		uint64_t lsw;
	} parts64;
	struct
	{
		uint32_t w0, w1, w2, w3;
	} parts32;

	// Constructors
	IEEE754_Float128() {}
	IEEE754_Float128(__float128 d) : Val(d) {}
	operator __float128() { return Val; }

	static IEEE754_Float128 FromBytes(unsigned __int128 _By)
	{
		IEEE754_Float128 d;
		d.Bytes = _By;
		return d;
	}

	// Generate unary operator
	#define IEEE754_UNARY_OPERATOR(operation) \
	_CONSTEXPR20 IEEE754_Float128& operator operation (const IEEE754_Float128& scaler) \
	{ \
		this->Val operation scaler.Val; \
		return *this; \
	} \

	IEEE754_UNARY_OPERATOR(+= )
	IEEE754_UNARY_OPERATOR(-= )
	IEEE754_UNARY_OPERATOR(*= )
	IEEE754_UNARY_OPERATOR(/= )

	#undef IEEE754_UNARY_OPERATOR
};
#endif

//_END_EXTERN_C

#endif