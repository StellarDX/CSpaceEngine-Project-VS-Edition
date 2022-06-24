#ifndef __MATHFUNCS_TEMP__
#define __MATHFUNCS_TEMP__

#include "..\headers\math\AsMath.h"

_CSE_BEGIN

/****************************************************************************************\
*                                Trigonometric functions                                 *
\****************************************************************************************/

template<typename genType> requires vecType<genType>
genType sin(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = sin(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType cos(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = cos(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType tan(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = tan(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType ctg(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = ctg(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType sec(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = sec(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType csc(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = csc(x[i]);
	}
	return x;
}

///////////////////////////////////// INVERSE ////////////////////////////////////


template<typename genType> requires vecType<genType>
genType arcsin(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = arcsin(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType arccos(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = arccos(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType arctan(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = arctan(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType arcctg(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = arcctg(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType arcsec(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = arcsec(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType arccsc(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = arccsc(x[i]);
	}
	return x;
}

/****************************************************************************************\
*                                          E X P                                         *
\****************************************************************************************/

template<typename genType> requires vecType<genType>
genType exp(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = exp(x[i]);
	}
	return x;
}

/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

template<typename genType> requires vecType<genType>
genType log(genType base, genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = log(base[i], x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType log(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = log(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType ln(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = ln(x[i]);
	}
	return x;
}

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

template<typename genType> requires vecType<genType>
genType pow(genType x, genType power)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = pow(x[i], power[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType sqrt(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = sqrt(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType inversesqrt(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = 1.0 / sqrt(x[i]);
	}
	return x;
}

template<typename genType> requires vecType<genType>
genType cbrt(genType x)
{
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = cbrt(x[i]);
	}
	return x;
}

_CSE_END

#endif