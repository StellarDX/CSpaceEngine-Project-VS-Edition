#include "CSE/Core/CSEM_Adv.h"
#include "CSE/Core/CSEM_Func.h"

_CSE_BEGIN

_EXTERN_C

_Check_return_ float64 _cdecl integral(_In_ float64(*f)(float64 x), _In_ float64 a, _In_ float64 b, float64 Logn)
{
	float64 MaxSamples = pow(10, Logn);
	float64 Step = (b - a) / MaxSamples;
	float64 Area = 0;
	for (size_t i = 0; i < MaxSamples; i++)
	{
		Area += (f(a + i * Step) + f(a + (i + 1) * Step)) * Step / 2.;
	}
	return Area;
}

_END_EXTERN_C

_CSE_END