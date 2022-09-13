/***
*CSECore.h - definitions for internal modules and values
*
*       Copyright (C) StellarDX Astronomy.
*
*       This program is free software; you can redistribute it and/or modify
*       it under the terms of the GNU General Public License as published by
*       the Free Software Foundation; either version 2 of the License, or
*       (at your option) any later version.
*
*       This program is distributed in the hope that it will be useful,
*       but WITHOUT ANY WARRANTY; without even the implied warranty of
*       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*       GNU General Public License for more details.
*
*       You should have received a copy of the GNU General Public License along
*       with this program; If not, see <https://www.gnu.org/licenses/>.
*
****/

#pragma once

#ifndef __CSE_CORE__
#define __CSE_CORE__

#include <yvals_core.h>

#include <iostream>
#include <string>
#include <ctime>
#include <tchar.h>

#define CSE_TITLE_STRING _T("StellarDX CSpaceEngine Runtime Platform Binary")

#if defined _MSC_VER
#define COMPILER_VERSION std::string("MSC " + std::to_string(_MSC_FULL_VER) + "(" + std::to_string(_MSC_BUILD) + ")")
#elif defined __GNUG__ // CSE has stopped supporting MinGW anymore.
#define COMPILER_VERSION std::string("GPP " + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__) + "." + std::to_string(__GNUC_PATCHLEVEL__))
#endif

//#define RC_INVOKED
//#define NOWINRES
#define NOMINMAX

#define _CSE_BEGIN namespace cse {
#define _CSE_END   }
#define _CSE  cse::

#define _GL_BEGIN namespace gl {
#define _GL_END   }
#define _GL  gl::

#define _SC_BEGIN namespace sc {
#define _SC_END   }
#define _SC  sc::

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

_CSE_BEGIN

using int64              = long long;
using float64            = double;
using uint64             = unsigned long long;
using stringu8           = std::u8string;

// CG/HLSL/Unity style types, but higher precision
using lfixed             = float; // 32-bit Low precision float
using lhalf              = double; // 64-bit Medium precision float, equal to float64
using lfloat             = long double; // 96-bit High precision float

template<typename genTypeA = uint64, typename genTypeB = float64>
inline constexpr genTypeB wrtval(genTypeA Value, uint64 Bits = sizeof(genTypeA))
{
	if (Bits > sizeof(genTypeB))
	{
		throw std::runtime_error("Bytes of target is not enough.");
	}
	genTypeB Dst;
	memcpy(&Dst, &Value, Bits);
	return Dst;
}

#define NO_DATA_FLOAT_INF wrtval(POS_INF_DOUBLE)
#define NO_DATA_STRING "None"

// Logger
class CSELog
{
	std::ostream& Output = std::cout;

	inline std::string gettime()
	{
		time_t _TVAL = time(nullptr);
		tm Time;
		gmtime_s(&Time, &_TVAL);
		return std::to_string(Time.tm_hour) + ':' + std::to_string(Time.tm_min) + ':' + std::to_string(Time.tm_sec);
	}

public:
	CSELog(){}
	CSELog(std::ostream& os) : Output(os){}

	#define LogTimeStamp            true	// print time stamp into log file
	#define LogThreadStamp          true	// print thread ID  into log file

	// Level of logging while loading catalogs(IO Stream):
	// 0 - do not log
	// 1 - log errors and warnings, using min("CatalogLogLevel" in this config, "LogLevel" in the source file)
	// 2 - log everything,          using min("CatalogLogLevel" in this config, "LogLevel" in the source file)
	// 3 - log errors and warnings, ignoring "LogLevel" in the source file
	// 4 - log everything,          ignoring "LogLevel" in the source file
	#define CatalogLogLevel    2

	// Level of logging for the System files:
	// 0 - do not log
	// 1 - log errors and warnings
	// 2 - log everything
	#define SysLogLevel        1

	inline void Out(std::string Thread, std::string Type, std::string LStr, int Level, bool Catalog = false)
	{
		#if SysLogLevel > 0

		if (Catalog)
		{
			#if CatalogLogLevel == 1 || CatalogLogLevel == 2
			Level = Level < CatalogLogLevel ? Level : CatalogLogLevel;
			#elif CatalogLogLevel == 3 || CatalogLogLevel == 4
			Level = CatalogLogLevel - 2;
			#endif
		}
		else { Level = SysLogLevel; }

		if (Level == 0) { return; }

		auto LsOut = [&]()->void
		{
			#if LogTimeStamp == true
			Output << '[' << gettime() << ']' << ' ';
			#endif
			#if LogThreadStamp == true
			Output << "[" + Thread + "/" + Type + "] ";
			#endif
			Output << LStr << '\n';
			return;
		};

		if (Type == "INFO")
		{
			if (Level == 2) { LsOut(); }
		}
		else{LsOut();}

		#endif
	}
};

// Report errors
#define _CSE_LOG_ERROR(cond, except, returns)                        \
        if (cond) { /* contextually convertible to bool paranoia */ }\
		else {except; return returns;}                               \

// Report fatal errors
#define _CSE_VERIFY(cond, except)                                    \
        if (cond) { /* contextually convertible to bool paranoia */ }\
		else {throw except; exit(-1);}                               \

_CSE_END

#endif