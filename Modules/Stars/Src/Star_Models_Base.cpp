#include "CSE/Stars/Star_Models_Base.hpp"
#include "CSE/Stars/Star_Models_Evolution.hpp"
#include "CSE/Stars/Star_Models_Classification.hpp"

_CSE_BEGIN

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

Object RandomBlueSupergiant()
{
	auto OTable = GetSGTable(LSTARCLS::SpecClass::O);
	auto BTable = GetSGTable(LSTARCLS::SpecClass::B);
	typename SPECSTR::SpecClass _SCl = (SPECSTR::SpecClass)random.randint(0, 1);
	typename SPECSTR::Type      _STy;
	if (_SCl == 0)
	{
		auto TableSize = OTable.size();
		_STy = (SPECSTR::Type)lround(random.uniform(OTable[0][1], OTable[TableSize - 2][1]) * 10.) / 10;
	}
	if (_SCl == 1)
	{
		auto TableSize = BTable.size();
		_STy = (SPECSTR::Type)lround(random.uniform(BTable[0][1], BTable[TableSize - 2][1]) * 10.) / 10;
	}
	typename SPECSTR::LumClass  _LCl = (SPECSTR::LumClass)random.randint(1, 3);

	SPECSTR _Spec(_SCl, _STy, -1, _LCl);

	SupergiantModel _Mdl(_Spec);
	object_generator gen(random, _Mdl);
	return gen();
}

Object RandomRedSupergiant()
{
	auto KTable = GetSGTable(LSTARCLS::SpecClass::K);
	auto MTable = GetSGTable(LSTARCLS::SpecClass::M);
	typename SPECSTR::SpecClass _SCl = (SPECSTR::SpecClass)random.randint(5, 6);
	typename SPECSTR::Type      _STy;
	if (_SCl == 5)
	{
		auto TableSize = KTable.size();
		_STy = (SPECSTR::Type)lround(random.uniform(KTable[0][1], KTable[TableSize - 2][1]) * 10.) / 10;
	}
	if (_SCl == 6)
	{
		auto TableSize = MTable.size();
		_STy = (SPECSTR::Type)lround(random.uniform(MTable[0][1], MTable[TableSize - 1][1]) * 10.) / 10;
	}
	typename SPECSTR::LumClass  _LCl = (SPECSTR::LumClass)random.randint(1, 3);
	 
	SPECSTR _Spec(_SCl, _STy, -1, _LCl);

	SupergiantModel _Mdl(_Spec);
	object_generator gen(random, _Mdl);
	return gen();
}

Object RandomYellowSupergiant()
{
	auto ATable = GetSGTable(LSTARCLS::SpecClass::A);
	auto FTable = GetSGTable(LSTARCLS::SpecClass::F);
	auto GTable = GetSGTable(LSTARCLS::SpecClass::G);
	typename SPECSTR::SpecClass _SCl = (SPECSTR::SpecClass)random.randint(2, 4);
	typename SPECSTR::Type      _STy;
	if (_SCl == 2)
	{
		auto TableSize = ATable.size();
		_STy = (SPECSTR::Type)lround(random.uniform(ATable[0][1], ATable[TableSize - 2][1]) * 10.) / 10;
	}
	if (_SCl == 3)
	{
		auto TableSize = FTable.size();
		_STy = (SPECSTR::Type)lround(random.uniform(FTable[0][1], FTable[TableSize - 2][1]) * 10.) / 10;
	}
	if (_SCl == 4)
	{
		auto TableSize = GTable.size();
		_STy = (SPECSTR::Type)lround(random.uniform(GTable[0][1], GTable[TableSize - 2][1]) * 10.) / 10;
	}
	typename SPECSTR::LumClass  _LCl = (SPECSTR::LumClass)random.randint(1, 3);

	SPECSTR _Spec(_SCl, _STy, -1, _LCl);

	SupergiantModel _Mdl(_Spec);
	object_generator gen(random, _Mdl);
	return gen();
}

_CSE_END