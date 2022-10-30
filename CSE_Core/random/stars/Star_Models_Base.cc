#include "../../headers/random/stars.h"

_CSE_BEGIN

CSELog Log_Generator;

SPECSTR _Random_Main_Sequence_types()
{
	LSTARCLS::SpecClass _Spec = static_cast<LSTARCLS::SpecClass>
	(
		random.probability(_MAIN_SEQUENCE_WEIGHTS)
	);
	size_t _N;
	const STPARS* _Pars = GetStarTable(_Spec, &_N);

	return SPECSTR
	(
		_Spec, 
		(SPECSTR::Type)lround(random.uniform(_Pars[0][0], _Pars[_N - 1][0]) * 10.) / 10, static_cast<SPECSTR::Type>(-1),
		SPECSTR::V, static_cast<SPECSTR::LumClass>(-1)
	);
}

Object RandomMainSequenceStar()
{
	HPMainSequenceStarModel _Mdl(_Random_Main_Sequence_types());
	object_generator gen(random, _Mdl);
	return gen();
}

SPECSTR _Random_OB_Star_types()
{
	LSTARCLS::SpecClass _Spec = static_cast<LSTARCLS::SpecClass>
	(
		random.probability({1, 1})
	);
	size_t _N;
	const STPARS* _Pars = GetStarTable(_Spec, &_N);

	return SPECSTR(_Spec, (SPECSTR::Type)lround(random.uniform(_Pars[0][0], _Pars[_N - 1][0]) * 10.) / 10, -1, SPECSTR::V);
}

Object RandomOBStar()
{
	HPMainSequenceStarModel _Mdl(_Random_OB_Star_types());
	object_generator gen(random, _Mdl);
	return gen();
}

_CSE_END