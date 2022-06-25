#pragma once

#ifndef __CSE_CORE__
#define __CSE_CORE__

#include <iostream>
#include <string>
#include <ctime>

#if defined _MSC_VER
#define COMPILER_VERSION std::string("MSC " + std::to_string(_MSC_FULL_VER) + "(" + std::to_string(_MSC_BUILD) + ")")
#elif defined __GNUG__ // CSE has stopped supporting MinGW anymore.
#define COMPILER_VERSION std::string("GPP " + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__) + "." + std::to_string(__GNUC_PATCHLEVEL__))
#endif

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

typedef long long            int64;
typedef double               float64;
typedef unsigned long long   uint64;

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

// Log
class CSELog
{
	std::ostream& Output = std::cout;
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

	inline std::string gettime()
	{
		time_t _TVAL = time(nullptr);
		tm Time;
		gmtime_s(&Time, &_TVAL);
		return std::to_string(Time.tm_hour) + ':' + std::to_string(Time.tm_min) + ':' + std::to_string(Time.tm_sec);
	}

	inline void Out(std::string Thread, std::string Type, std::string LStr, int Level)
	{
		#if CatalogLogLevel > 0

		#ifdef _USE_CATALOG_LOG_LEVEL
		#if CatalogLogLevel == 1 || CatalogLogLevel == 2
		Level = Level < CatalogLogLevel ? Level : CatalogLogLevel;
		#endif

		#if CatalogLogLevel == 3 || CatalogLogLevel == 4
		//#undef LogLevel
		Level = CatalogLogLevel - 2;
		#endif
		#else
		Level = SysLogLevel;
		#endif

		if (Level == 0) { return; }

		auto LsOut = [&]()->void
		{
			#if LogTimeStamp == true
			Output << '[' << gettime() << ']' << ' ';
			#endif
			#if LogThreadStamp == true
			Output << "[" + Thread + "/" + Type + "]: ";
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

_CSE_END

#endif