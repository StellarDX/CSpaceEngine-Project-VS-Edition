// Math module for CSpaceEngine

#define _USE_CSE_DEFINES

#include "..\headers\math\AsMath.h"

_CSE_BEGIN

CSELog MathLog;

/****************************************************************************************\
*                                Trigonometric functions                                 *
\****************************************************************************************/

float64 radians(float64 degrees)
{
	return degrees * (CSE_PI / 180.);
}

float64 degrees(float64 radians)
{
	return radians * (180. / CSE_PI);
}

float64 sin(float64 angle)
{
	return std::sin(radians(angle));
}

float64 cos(float64 angle)
{
	return std::cos(radians(angle));
}

float64 tan(float64 angle)
{
	return std::tan(radians(angle));
}

///////////////////////////////////// EXPAND ////////////////////////////////////

// Inverse functions of sin, cos, tan

float64 ctg(float64 angle)
{
	return 1.0 / tan(angle);
}

float64 sec(float64 angle)
{
	return 1.0 / cos(angle);
}

float64 csc(float64 angle)
{
	return 1.0 / sin(angle);
}

// chords and arcs

float64 crd(float64 angle, float64 radius)
{
	return radius * 2.0 * sin(angle / 2.0);
}

float64 arc(float64 angle, float64 radius)
{
	return radius * radians(angle);
}

// versines

float64 siv(float64 angle)
{
	return 1.0 - cos(angle);
}

float64 vcs(float64 angle)
{
	return 1.0 - sin(angle);
}

float64 cvs(float64 angle)
{
	return 1.0 + cos(angle);
}

float64 cvc(float64 angle)
{
	return 1.0 + sin(angle);
}

float64 hvs(float64 angle)
{
	return siv(angle) / 2.0;
}

float64 hvc(float64 angle)
{
	return vcs(angle) / 2.0;
}

float64 hcv(float64 angle)
{
	return cvs(angle) / 2.0;
}

float64 hcc(float64 angle)
{
	return cvc(angle) / 2.0;
}

// exsecants

float64 exs(float64 angle)
{
	return sec(angle) - 1.0;
}

float64 exc(float64 angle)
{
	return csc(angle) - 1.0;
}

///////////////////////////////////// INVERSE ////////////////////////////////////

float64 arcsin(float64 x)
{
	return degrees(std::asin(x));
}

float64 arccos(float64 x)
{
	return degrees(std::acos(x));
}

float64 arctan(float64 x)
{
	return degrees(std::atan(x));
}

float64 arcctg(float64 x)
{
	return arctan(1.0 / x);
}

float64 arcsec(float64 x)
{
	return arccos(1.0 / x);
}

float64 arccsc(float64 x)
{
	return arcsin(1.0 / x);
}

float64 arccrd(float64 c, float64 r)
{
	return 2.0 * arcsin(c / (2.0 * r));
}

float64 arcsiv(float64 y)
{
	return arccos(1.0 - y);
}

float64 arcvcs(float64 y)
{
	return arccos(y - 1.0);
}

float64 arccvs(float64 y)
{
	return arcsin(1.0 - y);
}

float64 arccvc(float64 y)
{
	return arcsin(y - 1.0);
}

float64 archvs(float64 y)
{
	return arccos(1.0 - 2.0 * y);
}

float64 archvc(float64 y)
{
	return arccos(2.0 * y - 1.0);
}

float64 archcv(float64 y)
{
	return arcsin(1.0 - 2.0 * y);
}

float64 archcc(float64 y)
{
	return arcsin(2.0 * y - 1.0);
}

float64 arcexs(float64 y)
{
	return arcsec(y + 1);
}

float64 arcexc(float64 y)
{
	return arccsc(y + 1);
}

/****************************************************************************************\
*                                          E X P                                         *
\****************************************************************************************/

float64 exp(float64 x)
{
	return std::exp(x);
}

// Custom functions
// Reference: ZHANG Ying-xue, WANG Hua-jun "A fast algorithm to calculate exponential function e^x".
// http://doi.org/10.3969/j.issn.1009-3907-B.2007.02.012

/*static const float64 _EXPTAB[3][16] =
{
	// SCRAPPED...
};

float64 exptalor(float64 x, int64 NItems)
{
	// SCRAPPED...
}

float64 exp64f(float64 x)
{
	// SCRAPPED...
}*/

/****************************************************************************************\
*                                          L O G                                         *
\****************************************************************************************/

float64 log(float64 base, float64 x)
{
	return std::log2(x) / std::log2(base);
}

float64 log(float64 x)
{
	return std::log10(x);
}

float64 ln(float64 x)
{
	return std::log(x);
}

// Custom functions

/*float64 lntalor(float64 x, int64 NItems)
{
	// SCRAPPED...
}

float64 ln64f(float64 x)
{
	// SCRAPPED...
}*/

/****************************************************************************************\
*                                    P O W E R                                           *
\****************************************************************************************/

float64 pow(float64 x, float64 power)
{
	return std::pow(x, power);
}

float64 sqrt(float64 x)
{
	return std::sqrt(x);
}

float64 inversesqrt(float64 x)
{
	return 1.0 / std::sqrt(x);
}

float64 cbrt(float64 x)
{
	return std::cbrt(x);
}

// Custom functions
/*float64 ipow64f(float64 x, float64 power)
{
	// SCRAPPED...
}*/

/* ************************************************************************** *\
   Fast cube root algorithm by Ken Turkowski
   Reference: http://www.realitypixels.com/turk/computergraphics/CubeRoot.pdf
\* ************************************************************************** */
/*float64 CubeRoot(float64 x)
{
	// SCRAPPED...
}*/

/////////////////////////////////////////////////////////////////
//                      Solving Equations                      //
/////////////////////////////////////////////////////////////////

uint64 SolvePoly(vector<float64> Coeffs, vector<complex64>& Roots, int64 p_Error, int64 MaxIterLog, complex64 Base) // The base function
{
	if (Coeffs[0] == 0 || Coeffs.empty())
	{
		throw MathException("Highest power of polynomial can't be zero.");
	}
	if (Coeffs[0] != 1)
	{
		MathLog.Out("MathFuncs", "WARNING", "Highest power of polynomial is not equal to 1, automatically nomalized.", SysLogLevel);
		float64 Base = Coeffs[0];
		for (size_t i = 0; i < Coeffs.size(); i++)
		{
			Coeffs[i] /= Base;
		}
		std::string NewCoeffs;
		NewCoeffs += '(';
		for (size_t i = 0; i < Coeffs.size(); i++)
		{
			NewCoeffs += std::to_string(Coeffs[i]);
			if (i < Coeffs.size() - 1) { NewCoeffs += ", "; }
		}
		NewCoeffs += ')';
		MathLog.Out("MathFuncs", "INFO", "New coefficients are: " + NewCoeffs, SysLogLevel);
	}

	// The clue to the method now is to combine the fixed-point iteration for P with similar iterations for Q, R, S into a simultaneous iteration for all roots.
	// Initialize p, q, r, s:
	for (size_t i = 0; i < Coeffs.size() - 1; i++)
	{
		Roots.push_back(pow(Base, i));
	}

	// Make the substitutions
	auto f = [&](complex64 x)->complex64
	{
		complex64 SIG = 0;
		for (size_t i = 0; i < Coeffs.size(); i++)
		{
			SIG += Coeffs[i] * pow(x, Coeffs.size() - i - 1);
		}
		return SIG;
	};

	auto g = [&](complex64 x, uint64 Ignore)->complex64
	{
		complex64 PI = 1;
		for (size_t i = 0; i < Roots.size(); i++)
		{
			if (i != Ignore){ PI *= x - Roots[i]; }
		}
		return PI;
	};

	uint64 it = 0;

	while (it < pow(10, MaxIterLog))
	{
		vector<float64> Diffs;
		for (size_t i = 0; i < Coeffs.size() - 1; i++)
		{
			complex64 Diff = f(Roots[i]) / g(Roots[i], i);
			Diffs.push_back(abs(Diff));
			Roots[i] -= Diff;
		}

		// Re-iterate until the numbers p, q, r, s essentially stop changing relative to the desired precision. 
		// They then have the values P, Q, R, S in some order and in the chosen precision. So the problem is solved.
		auto MaxDiff = std::max_element(Diffs.begin(), Diffs.end());
		if (-pow(10, -p_Error) < *MaxDiff && *MaxDiff < pow(10, -p_Error))
		{
			break;
		}

		++it;
	}

	return it;
}

_CSE_END