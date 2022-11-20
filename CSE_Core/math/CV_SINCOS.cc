#include "../headers/math/CSEM_Func.h"

// Sine function is based on taylor series and existing formulae
// the taylor series if sine is: SUM[0, inf]((pow(-1, n) / (2 * n + 1)!) * pow(x, 2 * n + 1))
// and cosine is: SUM[0, inf]((pow(-1, n) / (2 * n)!) * pow(x, 2 * n))

_CSE_BEGIN

_Check_return_ float64 __cdecl radians(_In_ float64 _Deg)
{
	return _Deg * (CSE_PI / 180.);
}

_Check_return_ float64 __cdecl degrees(_In_ float64 _Rad)
{
	return _Rad / (CSE_PI / 180.);
}

// This table lists the sine value of (360 / 64) * n (n from 0 to 63)

static const float96 _SinCos_Tab[]
(
	+0.00000000000000000000000000000000L,
	+0.09801714032956060199419556388864L,
	+0.19509032201612826784828486847702L,
	+0.29028467725446236763619237581740L,
	+0.38268343236508977172845998403040L,
	+0.47139673682599764855638762590525L,
	+0.55557023301960222474283081394853L,
	+0.63439328416364549821517161322549L,
	+0.70710678118654752440084436210485L,
	+0.77301045336273696081090660975847L,
	+0.83146961230254523707878837761791L,
	+0.88192126434835502971275686366039L,
	+0.92387953251128675612818318939679L,
	+0.95694033573220886493579788698027L,
	+0.98078528040323044912618223613424L,
	+0.99518472667219688624483695310948L,
	+1.00000000000000000000000000000000L,
	+0.99518472667219688624483695310948L,
	+0.98078528040323044912618223613424L,
	+0.95694033573220886493579788698027L,
	+0.92387953251128675612818318939679L,
	+0.88192126434835502971275686366039L,
	+0.83146961230254523707878837761791L,
	+0.77301045336273696081090660975847L,
	+0.70710678118654752440084436210485L,
	+0.63439328416364549821517161322549L,
	+0.55557023301960222474283081394853L,
	+0.47139673682599764855638762590525L,
	+0.38268343236508977172845998403040L,
	+0.29028467725446236763619237581740L,
	+0.19509032201612826784828486847702L,
	+0.09801714032956060199419556388864L,
	-0.00000000000000000000000000000000L,
	-0.09801714032956060199419556388864L,
	-0.19509032201612826784828486847702L,
	-0.29028467725446236763619237581740L,
	-0.38268343236508977172845998403040L,
	-0.47139673682599764855638762590525L,
	-0.55557023301960222474283081394853L,
	-0.63439328416364549821517161322549L,
	-0.70710678118654752440084436210485L,
	-0.77301045336273696081090660975847L,
	-0.83146961230254523707878837761791L,
	-0.88192126434835502971275686366039L,
	-0.92387953251128675612818318939679L,
	-0.95694033573220886493579788698027L,
	-0.98078528040323044912618223613424L,
	-0.99518472667219688624483695310948L,
	-1.00000000000000000000000000000000L,
	-0.99518472667219688624483695310948L,
	-0.98078528040323044912618223613424L,
	-0.95694033573220886493579788698027L,
	-0.92387953251128675612818318939679L,
	-0.88192126434835502971275686366039L,
	-0.83146961230254523707878837761791L,
	-0.77301045336273696081090660975847L,
	-0.70710678118654752440084436210485L,
	-0.63439328416364549821517161322549L,
	-0.55557023301960222474283081394853L,
	-0.47139673682599764855638762590525L,
	-0.38268343236508977172845998403040L,
	-0.29028467725446236763619237581740L,
	-0.19509032201612826784828486847702L,
	-0.09801714032956060199419556388864L
);

_Check_return_ float64 __cdecl sin(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return std::sin(radians(_X));
	#else

	float64 k1 = _Conf.TABSIZE / 360.; // Base on angle
	static const float96 k2 = (2 * CSE_PI) / _Conf.TABSIZE;

	// OpenCV only use the first 2 terms for calculation in 32-bif format
	// For 64-bit, maybe this number need to increase to more than 5
	static const float96 As0 = +2.7557319223985890652557319223986e-6L * k2 * k2 * k2 * k2 * k2 * k2 * k2 * k2 * k2;
	static const float96 As1 = -1.9841269841269841269841269841270e-4L * k2 * k2 * k2 * k2 * k2 * k2 * k2;
	static const float96 As2 = +0.00833333333333333333333333333333L * k2 * k2 * k2 * k2 * k2;
	static const float96 As3 = -0.16666666666666666666666666666667L * k2 * k2 * k2;
	static const float96 As4 = +1.00000000000000000000000000000000L * k2;

	static const float96 Ac0 = +2.4801587301587301587301587301587e-5L * k2 * k2 * k2 * k2 * k2 * k2 * k2 * k2;
	static const float96 Ac1 = -0.00138888888888888888888888888889L * k2 * k2 * k2 * k2 * k2 * k2;
	static const float96 Ac2 = +0.04166666666666666666666666666667L * k2 * k2 * k2 * k2;
	static const float96 Ac3 = -0.50000000000000000000000000000000L * k2 * k2;
	static const float96 Ac4 = +1.00000000000000000000000000000000L;

	// Separate number
	float64 t = _X * k1;
	int64 it = std::llround(t);
	t -= it;

	// Locate parameters
	int64 IndexS = it & (_Conf.TABSIZE - 1);
	int64 IndexC = (_Conf.TABSIZE / 4 - IndexS) & (_Conf.TABSIZE - 1);

	// Finding solution with table value and taylor series
	float64 sina = _Conf.TABLE[IndexS];
	float64 sinb = ((((As0 * t * t + As1) * t * t + As2) * t * t + As3) * t * t + As4) * t;
	float64 cosa = _Conf.TABLE[IndexC];
	float64 cosb = (((Ac0 * t * t + Ac1) * t * t + Ac2) * t * t + Ac3) * t * t + Ac4;

	// Merge data using Sum formula
	return sina * cosb + cosa * sinb;

	#endif
}

// cos(x) = sin(90 - x)

_Check_return_ float64 __cdecl cos(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return std::cos(radians(_X));
	#else

	// Almost same as the sine function, but different return.
	float64 k1 = _Conf.TABSIZE / 360.;
	static const float96 k2 = (2 * CSE_PI) / _Conf.TABSIZE;

	static const float96 As0 = +2.7557319223985890652557319223986e-6L * k2 * k2 * k2 * k2 * k2 * k2 * k2 * k2 * k2;
	static const float96 As1 = -1.9841269841269841269841269841270e-4L * k2 * k2 * k2 * k2 * k2 * k2 * k2;
	static const float96 As2 = +0.00833333333333333333333333333333L * k2 * k2 * k2 * k2 * k2;
	static const float96 As3 = -0.16666666666666666666666666666667L * k2 * k2 * k2;
	static const float96 As4 = +1.00000000000000000000000000000000L * k2;

	static const float96 Ac0 = +2.4801587301587301587301587301587e-5L * k2 * k2 * k2 * k2 * k2 * k2 * k2 * k2;
	static const float96 Ac1 = -0.00138888888888888888888888888889L * k2 * k2 * k2 * k2 * k2 * k2;
	static const float96 Ac2 = +0.04166666666666666666666666666667L * k2 * k2 * k2 * k2;
	static const float96 Ac3 = -0.50000000000000000000000000000000L * k2 * k2;
	static const float96 Ac4 = +1.00000000000000000000000000000000L;

	float64 t = _X * k1;
	int64 it = std::llround(t);
	t -= it;

	int64 IndexS = it & (_Conf.TABSIZE - 1);
	int64 IndexC = (_Conf.TABSIZE / 4 - IndexS) & (_Conf.TABSIZE - 1);

	float64 sina = _Conf.TABLE[IndexS];
	float64 sinb = ((((As0 * t * t + As1) * t * t + As2) * t * t + As3) * t * t + As4) * t;
	float64 cosa = _Conf.TABLE[IndexC];
	float64 cosb = (((Ac0 * t * t + Ac1) * t * t + Ac2) * t * t + Ac3) * t * t + Ac4;

	return cosa * cosb - sina * sinb;

	#endif
}

_Check_return_ float64 __cdecl tan(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	#if !defined(_USE_CV_FUNCTIONS) && ((defined _MSC_VER && _MSC_VER >= 1900) || defined(__EMSCRIPTEN__))
	return std::tan(radians(_X));
	#else
	return _CSE sin(_X, _Conf) / _CSE cos(_X, _Conf);
	#endif
}

_Check_return_ float64 __cdecl ctg(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return 1. / _CSE tan(_X, _Conf);
}

_Check_return_ float64 __cdecl sec(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return 1. / _CSE cos(_X, _Conf);
}

_Check_return_ float64 __cdecl csc(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return 1. / _CSE sin(_X, _Conf);
}

///////////////////////////////////// EXPAND ////////////////////////////////////

_Check_return_ float64 __cdecl crd(_In_ float64 _X, _In_ float64 _Radius, SINCOS_CONFIG _Conf)
{
	return 2. * _Radius * _CSE sin(_X / 2., _Conf);
}

_Check_return_ float64 __cdecl arc(_In_ float64 _X, _In_ float64 _Radius, SINCOS_CONFIG _Conf)
{
	return (_X * CSE_PI * _Radius) / 180.;
}

_Check_return_ float64 __cdecl siv(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return 1. - _CSE cos(_X, _Conf);
}

_Check_return_ float64 __cdecl cvs(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return 1. - _CSE sin(_X, _Conf);
}

_Check_return_ float64 __cdecl vcs(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return 1. + _CSE cos(_X, _Conf);
}

_Check_return_ float64 __cdecl cvc(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return 1. + _CSE sin(_X, _Conf);
}

_Check_return_ float64 __cdecl hvs(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return siv(_X, _Conf) / 2.;
}

_Check_return_ float64 __cdecl hcv(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return cvs(_X, _Conf) / 2.;
}

_Check_return_ float64 __cdecl hvc(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return vcs(_X, _Conf) / 2.;
}

_Check_return_ float64 __cdecl hcc(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return cvc(_X, _Conf) / 2.;
}

_Check_return_ float64 __cdecl exs(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return sec(_X, _Conf) - 1.;
}

_Check_return_ float64 __cdecl exc(_In_ float64 _X, SINCOS_CONFIG _Conf)
{
	return csc(_X, _Conf) - 1.;
}

// Complex form

_Check_return_ complex64 __cdecl sin(_In_ complex64 _X, EXP_CONFIG _Conf)
{
	return (_CSE exp(1i * _X, _Conf) - _CSE exp(-1i * _X, _Conf)) / 2.i;
}

_Check_return_ complex64 __cdecl cos(_In_ complex64 _X, EXP_CONFIG _Conf)
{
	return (_CSE exp(1i * _X, _Conf) + _CSE exp(-1i * _X, _Conf)) / 2.;
}

_Check_return_ complex64 __cdecl tan(_In_ complex64 _X, EXP_CONFIG _Conf)
{
	return _CSE sin(_X, _Conf) / _CSE cos(_X, _Conf);
}

_Check_return_ complex64 __cdecl ctg(_In_ complex64 _X, EXP_CONFIG _Conf)
{
	return complex64(1.) / _CSE tan(_X, _Conf);
}

_Check_return_ complex64 __cdecl sec(_In_ complex64 _X, EXP_CONFIG _Conf)
{
	return complex64(1.) / _CSE cos(_X, _Conf);
}

_Check_return_ complex64 __cdecl csc(_In_ complex64 _X, EXP_CONFIG _Conf)
{
	return complex64(1.) / _CSE sin(_X, _Conf);
}

_CSE_END