#pragma once

//===========================================================================//
//                                                                           //
//             CSpaceEngine Procedural Star Generator Header                 //
//                                                                           //
//===========================================================================//

#ifndef _CSE_PROC_STAR_GEN
#define _CSE_PROC_STAR_GEN

#include "../Core/CSECore.h"
#include "../gl/gltypes.h"
#include "../lumine/SpTypes.h"

#include <array>

_CSE_BEGIN

using STPARS = _STD array<float64, 6>;

enum StarTableCoeffs;
const STPARS* GetStarTable(LSTARCLS::SpecClass Class, int* Tyc);

_CSE_END

#endif