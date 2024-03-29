﻿/***
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

#define WIN32_LEAN_AND_MEAN
#include <yvals_core.h>

#include <iostream>
#include <string>
#include <ctime>
#include <format>
#include <tchar.h>

#define CSE_TITLE_STRING     "StellarDX CSpaceEngine Runtime Platform Binary"
#define CSE_TITLE_WSTRING   L"StellarDX CSpaceEngine Runtime Platform Binary"
#define CSE_TITLE_U8STRING u8"StellarDX CSpaceEngine Runtime Platform Binary"

#if defined _MSC_VER
#define COMPILER_VERSION std::string("MSC " + std::to_string(_MSC_FULL_VER) + "(" + std::to_string(_MSC_BUILD) + ")")
#elif defined __GNUG__ // CSE has stopped supporting MinGW anymore.
#define COMPILER_VERSION std::string("GPP " + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__) + "." + std::to_string(__GNUC_PATCHLEVEL__))
#endif

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

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

_CSE_BEGIN

#include "CSE/Core/IEEE754/IEEE754.h"

using int64              = long long;
using float64            = double;
using uint64             = unsigned long long;
using stringu8           = std::u8string;

template<typename genTypeA = uint64, typename genTypeB = float64>
inline constexpr genTypeB wrtval(genTypeA Value)
{
	union _Buf 
	{genTypeA Src; genTypeB Dst;} _Buffer;
	_Buffer.Src = Value;
	return _Buffer.Dst;
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
	#define CatalogLogLevel    0

	// Level of logging for the System files:
	// 0 - do not log
	// 1 - log errors and warnings
	// 2 - log everything
	#define SysLogLevel        1

	inline void Out(std::string Thread, std::string Type, std::string LStr, int Level = SysLogLevel, bool Catalog = false)
	{
		#if SysLogLevel > 0

		if (Catalog)
		{
			#if CatalogLogLevel == 0
			return;
			#elif CatalogLogLevel == 1 || CatalogLogLevel == 2
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
		else {throw except;}                               \

_CSE_END

// IEEE754 Format output
template<>
struct std::formatter<cse::IEEE754_Dbl64, char>
{
	auto parse(std::format_parse_context& parseContext)
	{
		auto symbolsEnd = std::ranges::find(parseContext, '}');
		auto symbols = std::string_view(parseContext.begin(), symbolsEnd);
		return symbolsEnd;
	}

	auto format(cse::IEEE754_Dbl64 const& my, std::format_context& formatContext)
	{
		return std::format_to(formatContext.out(), "{}", my.x);
	}
};

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif