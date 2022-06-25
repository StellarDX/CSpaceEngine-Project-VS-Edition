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

_CSE_END