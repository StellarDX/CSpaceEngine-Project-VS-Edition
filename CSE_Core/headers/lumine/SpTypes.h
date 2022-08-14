#pragma once

#ifndef __CSE_SP_TYPES__
#define __CSE_SP_TYPES__

#include "../Core/CSECore.h"

_CSE_BEGIN

// ----------RED GIANTS---------- //

bool IsGiant(Object Star); // III, II, I, 0
bool IsBrightGiant(Object Star); // II
bool IsSuperGiant(Object Star); // I
bool IsHyperGiant(Object Star); // 0

// ----------SUBGIANTS---------- //

bool IsSubGiant(Object Star); // IV

// ----------MAIN SEQUENCES---------- //

bool IsMainSeq(Object Star); // V

// ----------SUBDWARFS---------- //

bool IsSubDwarf(Object Star); // VI

// ----------COLORS---------- //

bool IsOType(Object Star); // O
bool IsBType(Object Star); // B
bool IsAType(Object Star); // A
bool IsFType(Object Star); // F
bool IsGType(Object Star); // G
bool IsKType(Object Star); // K
bool IsMType(Object Star); // M

// ----------WOLF RAYET---------- //

bool IsWolfRayet(Object Star); // WR

// ----------BROWN DWARFS---------- //

bool IsBrownDwarf(Object Star); // L, T, Y
bool IsLType(Object Star); // L
bool IsTType(Object Star); // T
bool IsYType(Object Star); // Y

// ----------CARBON STARS---------- //

bool IsCarbonStar(Object Star); // C, S, ...

// ----------STAR REMNANTS---------- //

bool IsWhiteDwarf(Object Star);
bool IsNeutronStar(Object Star);
bool IsBlackHole(Object Star);
bool IsStarRemnant(Object Star);

_CSE_END

#endif