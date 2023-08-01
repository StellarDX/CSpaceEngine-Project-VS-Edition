#pragma once

#ifndef __STELLAR_COLOR__
#define __STELLAR_COLOR__

#include "CSE/Core/CSECore.h"
#include "CSE/Core/gltypes.h"
#include <sal.h>

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

#define _COLOR_BEGIN namespace color {
#define _COLOR_END }
#define _COLOR color::

_CSE_BEGIN
_COLOR_BEGIN

// GLSL style redefines of SAL macros
#ifndef _DISABLE_GLSL_DEFINES
#define in    _In_    // read-only
#define out   _Out_   // write-only
#define inout _Inout_ // write-only
#endif

// 32-bit type defines
using uint = unsigned;

/// <summary>
/// Convert RGB to Hex
/// </summary>
/// <param name="c">Color in RGB format and all components are in the range [0, 255]</param>
/// <returns>Hex format stored in an unsigned integer</returns>
uint RGBToHex(uvec3 c);

/// <summary>
/// Load color from an integer value
/// </summary>
/// <param name="hex">Hex format stored in an unsigned integer</param>
/// <returns>Color in RGB format and all components are in the range [0, 255]</returns>
uvec3 HexToRGB(uint hex);

/// <summary>
/// Convert RGB to HSV
/// </summary>
/// <param name="rgb">Color in RGB format and all components are in the range [0, 1]</param>
/// <returns>HSV format color</returns>
vec3 RGBtoHSV(vec3 rgb);

/// <summary>
/// Convert HSV to RGB
/// </summary>
/// <param name="rgb">Color in HSV format and all components are in the range [0, 1]</param>
/// <returns>RGB format color</returns>
vec3 HSVToRGB(vec3 hsv);

_COLOR_END
_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif
