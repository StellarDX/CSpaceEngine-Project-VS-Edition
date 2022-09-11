/////////////////////////////////////////////////////////////
//                                                         //
//                  CSpaceEngine Library                   //
//                                                         //
// This is the core module of CSpaceEngine,                //
// contains some data types from other language and file   //
// stream for Space Engine .sc file.                       //
//                                                         //
// Data type "vec2", "vec3", "vec4" taken from GLSL.       //
// Object class taken from export files from SpaceEngine   //
// I/O Stream by StellarDX, 2022                           //
//                                                         //
/////////////////////////////////////////////////////////////

#pragma once

#ifndef __CSE__
#define __CSE__

#include "Core/CSECore.h"

#include "gl/gltypes.h"
#include "math/AsMath.h"
#include "Core/ConstLists.h"

#include "date/Date_Time.h"

#include "stream/scstream.h"
#include "Core/CelObject.h"

#include "lumine/SpTypes.h"
#include "lumine/Lumine.h"

#endif