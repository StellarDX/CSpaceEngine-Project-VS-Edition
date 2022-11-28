// CSpaceEngine random number generator

#pragma once

#ifndef __CSE_RAND_ENGINE__
#define __CSE_RAND_ENGINE__
#include "CSE/Core.h"
#include "CSE/Object.h"

#include <array>
#include <random> // Base on STD random.
#include <vector>

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

#define _RAND_BEGIN namespace randgen {
#define _RAND_END }
#define _RAND randgen::

_CSE_BEGIN

// Additional Random models

_RAND_BEGIN

template<class _Ty = float64>
class triangular_distribution : public _STD piecewise_linear_distribution<_Ty>
{
public:
	using _Mybase     = _STD piecewise_linear_distribution<_Ty>;
	using _Mypbase    = typename _Mybase::param_type;
    using result_type = _Ty;

	triangular_distribution() {}

	triangular_distribution(_Ty _Min0, _Ty _Max0, _Ty _Peak)
	{
		_STD array<_Ty, 3> _Values{ _Min0, _Peak, _Max0 };
		_STD array<_Ty, 3> _Weights{ 0, 1, 0 };
		_Mybase::_Par = _Mybase::param_type(_Values.begin(), _Values.end(), _Weights.begin());
	}
};

template<class _Ty = float64>
class beta_distribution : public _STD _Beta_distribution<_Ty>
{
public:
	using _Mybase     = _STD _Beta_distribution<_Ty>;

	beta_distribution() : _Mybase() {}
	beta_distribution(_Ty _Ax0, _Ty _Bx0) : _Mybase(_Ax0, _Bx0) {}
};

/*template<class _Ty = float64>
class vonmises_distribution
{
	// Nothing...
};*/

/*template<class _Ty = float64>
class pareto_distribution
{
	// Nothing...
};*/

_RAND_END

// Random Engine
// reference: https://docs.python.org/3/library/random.html

template<typename _Rand_Engine>
class _CSE_Random_Engine
{
public:
	using Engine  = _Rand_Engine;
	using Seed    = uint64;

private:
	_Rand_Engine _Rd;
	Seed _Seed;

public:

	// ---------- Constructors ---------- //

	_CSE_Random_Engine() = delete;
	_CSE_Random_Engine(Seed _Xx0 = 0x1571U) : _Seed(_Xx0)
	{
		_Rd = _Rand_Engine(_Xx0);
	};

	void reset(){ _Rd = _Rand_Engine(_Seed); }

	// Bookkeeping functions

	Seed seed()const{return _Seed;}

	/// <summary>
	/// Initialize the random number generator with a new seed.
	/// </summary>
	Seed seed(Seed _New) 
	{ 
		Seed _Old = _Seed;
		_Seed = _New;
		reset();
		return _Old;
	}

	// ---------- Functions for integers ---------- //

	/// <summary>
	/// Return a randomly selected element from range(start, stop, step). 
	/// This is equivalent to choice(range(start, stop, step)), but doesn’t actually build a range object.
	/// </summary>
	/// <typeparam name="genType">Result type</typeparam>
	/// <param name="_Begin">Start</param>
	/// <param name="_End">Stop</param>
	/// <param name="_Step">Step</param>
	template<typename genType>
	genType randrange(genType _Begin, genType _End, genType _Step)
	{
		_STD vector<genType> _Range;
		for (genType i = _Begin; i < _End; i += _Step)
		{
			_Range.push_back(i);
		}
		_STD uniform_int_distribution<uint64> _Gen(0, _Range.size() - 1);
		return _Range[_Gen(_Rd)];
	}

	/// <summary>
	/// Return a random integer N in the range[a, b]
	/// </summary>
	/// <param name="_Min0">Min Value</param>
	/// <param name="_Max0">Max Value</param>
	int64 randint(int64 _Min0, int64 _Max0)
	{
		_STD uniform_int_distribution<int64> _Range(_Min0, _Max0);
		return _Range(_Rd);
	}

	// ---------- Functions for sequences ---------- //

	/// <summary>
	/// Return a random element from the non-empty sequence.
	/// </summary>
	/// <typeparam name="Iterator">Iterator type</typeparam>
	/// <typeparam name="genType">Result type</typeparam>
	/// <param name="_Begin">Begin iterator</param>
	/// <param name="_End">End iterator</param>
	template<typename Iterator, typename genType>
	genType choice(Iterator _Begin, Iterator _End)
	{
		_STL_ASSERT(_Begin < _End, "Sequence is empty.");
		_STD vector<genType> _Range;
		for (; _Begin < _End; ++_Begin)
		{
			_Range.push_back(*_Begin);
		}
		_STD uniform_int_distribution<uint64> _Gen(0, _Range.size() - 1);
		return _Range[_Gen(_Rd)];
	}

	/// <summary>
	/// Shuffle the sequence x in place.
	/// this function is taken from STL algorithm file.
	/// </summary>
	/// <typeparam name="Iterator">Iterator of x</typeparam>
	/// <param name="_Begin">Begin iterator</param>
	/// <param name="_End">End iterator</param>
	template<typename Iterator>
	void shuffle(Iterator _Begin, Iterator _End)
	{
		_STL_ASSERT(_Begin < _End, "Sequence is empty.");
		auto _UFirst = _STD _Get_unwrapped(_Begin);
		const auto _ULast = _STD _Get_unwrapped(_End);
		if (_UFirst == _ULast) {return;}

		using _Diff = _STD _Iter_diff_t<Iterator>;
		auto _UTarget = _UFirst;
		_Diff _Target_index = 1;
		for ( ; ++_UTarget != _ULast; ++_Target_index) 
		{ // randomly place an element from [_First, _Target] at _Target
			_Diff _Off = randint(0, static_cast<_Diff>(_Target_index));
			_STL_ASSERT(0 <= _Off && _Off <= _Target_index, "Random value out of range");
			if (_Off != _Target_index)
			{ // avoid self-move-assignment
				_STD iter_swap(_UTarget, _UFirst + _Off);
			}
		}
	}

	// ---------- Real-valued distributions ---------- //

	/// <summary>
	/// Return the next random floating point number in the range [0.0, 1.0].
	/// </summary>
	float64 random()
	{
		_STD uniform_real_distribution<float64> _DefRange(0., 1.);
		return _DefRange(_Rd);
	}

	/// <summary>
	/// Return a random floating point number N in the range [a, b]
	/// </summary>
	/// <param name="_Min0">Min Value</param>
	/// <param name="_Max0">Max Value</param>
	float64 uniform(float64 _Min0, float64 _Max0)
	{
		_STD uniform_real_distribution<float64> _Range(_Min0, _Max0);
		return _Range(_Rd);
	}

	/// <summary>
	/// Return a random floating point number N in [low, high] and with the specified mode between those bounds.
	/// </summary>
	/// <param name="_Min0">Min Value, default is 0</param>
	/// <param name="_Max0">Max Value, default is 1</param>
	/// <param name="_Peak">Peak, default is median of min and max</param>
	float64 triangular(float64 _Min0, float64 _Max0, float64 _Peak)
	{
		_RAND triangular_distribution<float64> _Range(_Min0, _Max0, _Peak);
		return _Range(_Rd);
	}

	float64 triangular()
	{
		return triangular(0., 1., 0.5);
	}

	float64 triangular(float64 _Min0, float64 _Max0)
	{
		return triangular(_Min0, _Max0, (_Min0 + _Max0) / 2.);
	}

	/// <summary>
	/// Beta distribution.
	/// </summary>
	/// <param name="_Alf">Alpha, default is 1</param>
	/// <param name="_Bet">Beta, default is 1</param>
	/// <returns>Values range between 0 and 1.</returns>
	float64 beta(float64 _Alf = 1., float64 _Bet = 1.)
	{
		_RAND beta_distribution<float64> _F(_Alf, _Bet);
		return _F(_Rd);
	}

	/// <summary>
	/// Exponential distribution.
	/// </summary>
	/// <param name="_Lam">Lambda, default is 1</param>
	/// <returns>Values range from 0 to positive infinity if lambd is positive, and from negative infinity to 0 if lambd is negative.</returns>
	float64 exponential(float64 _Lam = 1.)
	{
		_STD exponential_distribution<float64> _F(_Lam);
		return _F(_Rd);
	}

	/// <summary>
	/// Gamma distribution
	/// </summary>
	/// <param name="_Alf">shape parameter, default is 1</param>
	/// <param name="_Bet">scale parameter, default is 1</param>
	/// <returns></returns>
	float64 gamma(float64 _Alf = 1., float64 _Bet = 1.)
	{
		_STD gamma_distribution<float64> _F(_Alf, _Bet);
		return _F(_Rd);
	}

	float64 kgamma(float64 _K = 1., float64 _Tet = 1.)
	{
		return gamma(_K, 1. / _Tet);
	}

	/// <summary>
	/// Normal distribution, also called the Gaussian distribution.
	/// </summary>
	/// <param name="_Mean">Mean, default is 0</param>
	/// <param name="_StDev">Standard deviation, default is 1</param>
	float64 normal(float64 _Mean = 0., float64 _StDev = 1.)
	{
		_STD normal_distribution<float64> _F(_Mean, _StDev);
		return _F(_Rd);
	}

	/// <summary>
	/// Log normal distribution.
	/// </summary>
	/// <param name="_Mean">Mean, default is 0</param>
	/// <param name="_StDev">Standard deviation, default is 1</param>
	float64 lognorm(float64 _Mean = 0., float64 _StDev = 1.)
	{
		_STD lognormal_distribution<float64> _F(_Mean, _StDev);
		return _F(_Rd);
	}

	/// <summary>
	/// von Mises distribution
	/// </summary>
	/// <param name="_Mu">mean angle</param>
	/// <param name="_Kap">concentration parameter</param>
	/*float64 vonmises(float64 _Mu = 0., float64 _Kap = 1.)
	{
		vonmises_distribution<float64> _F(_Mu, _Kap);
		return _F(_Rd);
	}*/

	/// <summary>
	/// Pareto distribution
	/// </summary>
	/// <param name="_Alf">shape parameter</param>
	/*float64 pareto(float64 _Alf = 0.)
	{
		pareto_distribution<float64> _F(_Alf);
		return _F(_Rd);
	}*/

	/// <summary>
	/// Weibull distribution.
	/// </summary>
	/// <param name="_Lam">scale parameter</param>
	/// <param name="_k">shape parameter</param>
	float64 weibull(float64 _Lam = 0., float64 _k = 1.)
	{
		_STD weibull_distribution<float64> _F(_Lam, _k);
		return _F(_Rd);
	}

	/// <summary>
	/// Generate numbers in probilities
	/// </summary>
	uint64 probability(_STD initializer_list<float64> _Ilist)
	{
		_STD discrete_distribution<uint64> _Prob(_Ilist);
		return _Prob(_Rd);
	}
};

extern _CSE_Random_Engine<_STD mersenne_twister_engine
	<unsigned long long, 64, 312, 156, 31, 0xB5026F5AA96619E9ULL, 29,
	0x5555555555555555ULL, 17, 0x71D67FFFEDA60000ULL, 37, 0xFFF7EEE000000000ULL, 43, 0x5851F42D4C957F2DULL>> random;

////////// Object Generator //////////

class Object;
template<typename Engine, typename Model>
class object_generator
{
	Engine& _Eng;
	Model& _Mdl;
public:
	// types
	typedef Engine engine_type;
	typedef Model  model_type;
	typedef Object result_type;

	// construct/copy/destruct
	object_generator(Engine& e, Model& m) : _Eng(e), _Mdl(m){}

	// public member functions
	result_type operator()()
	{
		return _Mdl(_Eng);
	}

	engine_type& engine() { return _Eng; }
	model_type& objmodel() { return _Mdl; }
};

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif