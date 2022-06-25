#include <cstdarg>

#include "..\headers\math\Algorithms.h"

_CSE_BEGIN

//#include <cstdarg>

float64 abs(float64 x)
{
	return (x >= 0) ? x : -x;
}

float64 sgn(float64 x)
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

float64 max(float64 x, float64 y)
{
	return x > y ? x : y;
}

float64 max(uint64 size, ...)
{
	va_list args;
	va_start(args, size);

	float64 Max = va_arg(args, float64);
	int cnt = 0;
	for (size_t i = 1; i < size; i++)
	{
		float64 Current = va_arg(args, float64);
		Max = Current > Max ? Current : Max;
		cnt++;
	}
	std::cout << cnt << '\n';
	va_end(args);
	return Max;
}

_CSE_END