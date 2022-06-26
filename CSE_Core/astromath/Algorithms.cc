#include <cstdarg>

#include "..\headers\math\Algorithms.h"

_CSE_BEGIN

float64 abs(float64 x)
{
	return (x >= 0) ? x : -x;
}

int64 abs(int64 x)
{
	return (x >= 0) ? x : -x;
}

float64 sgn(float64 x)
{
	if (x > 0) { return 1; }
	else if (x < 0) { return -1; }
	else { return 0; }
}

int64 sgn(int64 x)
{
	if (x > 0) { return 1; }
	else if (x < 0) { return -1; }
	else { return 0; }
}

float64 floor(float64 x)
{
	return std::floor(x);
}

float64 ceil(float64 x)
{
	return std::ceil(x);
}

float64 FractionalPart(float64 x)
{
	return x - floor(x);
}

float64 mod(float64 x, float64 y)
{
	return x - y * floor(x / y);
}

float64 min(float64 x, float64 y)
{
	return x < y ? x : y;
}

float64 min(float64* x, uint64 size)
{
	float64 Min = x[0];
	for (size_t i = 1; i < size; i++)
	{
		Min = x[i] < Min ? x[i] : Min;
	}
	return Min;
}

int64 min(int64 x, int64 y)
{
	return x < y ? x : y;
}

int64 min(int64* x, uint64 size)
{
	int64 Min = x[0];
	for (size_t i = 1; i < size; i++)
	{
		Min = x[i] < Min ? x[i] : Min;
	}
	return Min;
}

uint64 min(uint64 x, uint64 y)
{
	return x < y ? x : y;
}

uint64 min(uint64* x, uint64 size)
{
	uint64 Min = x[0];
	for (size_t i = 1; i < size; i++)
	{
		Min = x[i] < Min ? x[i] : Min;
	}
	return Min;
}

float64 min(uint64 size, ...)
{
	MathLog.Out("Math-Algorithms", "WARNING", "You are using an unstable function cse::min(uint64, ...), maybe cause problems!", SysLogLevel);
	va_list args;
	va_start(args, size);

	float64 Min = va_arg(args, float64);

	for (size_t i = 1; i < size; i++)
	{
		float64 Current = va_arg(args, float64);
		Min = Current < Min ? Current : Min;
	}

	va_end(args);
	return Min;
}

float64 max(float64 x, float64 y)
{
	return x > y ? x : y;
}

float64 max(float64* x, uint64 size)
{
	float64 Max = x[0];
	for (size_t i = 1; i < size; i++)
	{
		Max = x[i] > Max ? x[i] : Max;
	}
	return Max;
}

int64 max(int64 x, int64 y)
{
	return x > y ? x : y;
}

int64 max(int64* x, uint64 size)
{
	int64 Max = x[0];
	for (size_t i = 1; i < size; i++)
	{
		Max = x[i] > Max ? x[i] : Max;
	}
	return Max;
}

uint64 max(uint64 x, uint64 y)
{
	return x > y ? x : y;
}

uint64 max(uint64* x, uint64 size)
{
	uint64 Max = x[0];
	for (size_t i = 1; i < size; i++)
	{
		Max = x[i] > Max ? x[i] : Max;
	}
	return Max;
}

float64 max(uint64 size, ...)
{
	MathLog.Out("Math-Algorithms", "WARNING", "You are using an unstable function cse::max(uint64, ...), maybe cause problems!", SysLogLevel);
	va_list args;
	va_start(args, size);

	float64 Max = va_arg(args, float64);

	for (size_t i = 1; i < size; i++)
	{
		float64 Current = va_arg(args, float64);
		Max = Current > Max ? Current : Max;
	}

	va_end(args);
	return Max;
}

float64 clamp(float64 x, float64 MinVal, float64 MaxVal)
{
	if (MinVal > MaxVal)
	{ 
		MathLog.Out("Math-Algorithms", "ERROR", "genType cse::clamp(genType x, genType MinVal, genType MaxVal) : MinVal is larger than MaxVal, return will be undefined.", SysLogLevel);
		return wrtval(Q_NAN_DOUBLE);
	}
	return min(max(x, MinVal), MaxVal);
}

int64 clamp(int64 x, int64 MinVal, int64 MaxVal)
{
	if (MinVal > MaxVal)
	{
		MathLog.Out("Math-Algorithms", "ERROR", "genIType cse::clamp(genIType x, genIType MinVal, genIType MaxVal) : MinVal is larger than MaxVal, return will be undefined.", SysLogLevel);
		return 0xFFFFFFFFFFFFFFFF;
	}
	return min(max(x, MinVal), MaxVal);
}

uint64 clamp(uint64 x, uint64 MinVal, uint64 MaxVal)
{
	if (MinVal > MaxVal)
	{
		MathLog.Out("Math-Algorithms", "ERROR", "genUType cse::clamp(genUType x, genUType MinVal, genUType MaxVal) : MinVal is larger than MaxVal, return will be undefined.", SysLogLevel);
		return 0xFFFFFFFFFFFFFFFF;
	}
	return min(max(x, MinVal), MaxVal);
}

float64 mix(float64 x, float64 y, float64 a)
{
	return x * (1. - a) + y * a;
}

float64 mix(float64 x, float64 y, bool a)
{
	return x * (1. - a) + y * a;
}

int64 mix(int64 x, int64 y, bool a)
{
	return x * (1i64 - a) + y * a;
}

uint64 mix(uint64 x, uint64 y, bool a)
{
	return x * (1ui64 - a) + y * a;
}

bool mix(bool x, bool y, bool a)
{
	return x * (1 - a) + y * a;
}

float64 step(float64 edge, float64 x)
{
	return x < edge ? 0. : 1.;
}

float64 smoothstep(float64 edge0, float64 edge1, float64 x)
{
	if (edge0 > edge1)
	{
		MathLog.Out("Math-Algorithms", "ERROR", "genType cse::smoothstep(genType edge0, genType edge1, genType x) : edge0 is larger than edge1, return will be undefined.", SysLogLevel);
		return wrtval(Q_NAN_DOUBLE);
	}
	float64 t;  /* Or genDType t; */
	t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}

float64 smoothstepAMD(float64 edge0, float64 edge1, float64 x)
{
	if (edge0 > edge1)
	{
		MathLog.Out("Math-Algorithms", "ERROR", "genType cse::smoothstepAMD(genType edge0, genType edge1, genType x) : edge0 is larger than edge1, return will be undefined.", SysLogLevel);
		return wrtval(Q_NAN_DOUBLE);
	}

	if (x < edge0)
		return 0;

	if (x >= edge1)
		return 1;

	// Scale/bias into [0..1] range
	x = (x - edge0) / (edge1 - edge0);

	return x * x * (3 - 2 * x);
}

float64 smoothstepKP(float64 edge0, float64 edge1, float64 x)
{
	if (edge0 > edge1)
	{
		MathLog.Out("Math-Algorithms", "ERROR", "genType cse::smoothstepKP(genType edge0, genType edge1, genType x) : edge0 is larger than edge1, return will be undefined.", SysLogLevel);
		return wrtval(Q_NAN_DOUBLE);
	}
	// Scale, and clamp x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	return x * x * x * (x * (x * 6 - 15) + 10);
}

float64 inverse_smoothstep(float64 x)
{
	return 0.5 - std::sin(std::asin(1.0 - 2.0 * x) / 3.0);
}

_CSE_END