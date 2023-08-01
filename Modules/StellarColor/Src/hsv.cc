// Colorspaces conversion functions
// Reference: 
// HSL/HSV: https://en.wikipedia.org/wiki/HSL_and_HSV

#include "CSE/StellarColor.h"
#include "CSE/Core/Algorithms.h"

_CSE_BEGIN
_COLOR_BEGIN

// This file is using GLSL language but it can be compiled by C++ compiler
// Adapted for C++ and converted to double-precision math

// Hex function designed by StellarDX

uint RGBToHex(uvec3 rgb)
{
    return (uint)((rgb.r << 16) | (rgb.g << 8) | (rgb.b << 0));
}

uvec3 HexToRGB(uint hex)
{
    return uvec3((uint64)((hex >> 16) & 0xFF), (uint64)((hex >> 8) & 0xFF), (uint64)((hex >> 0) & 0xFF));
}

// HSV Functions taken from https://stackoverflow.com/questions/15095909/from-rgb-to-hsv-in-opengl-glsl
// Licenced under WTFPL, a public domain like licence:
/*
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                    Version 2, December 2004
 *
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *  0. You just DO WHAT THE FUCK YOU WANT TO.
 * 
 */

vec3 RGBtoHSV(vec3 rgb)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(rgb.b, rgb.g, K.w, K.z), vec4(rgb.g, rgb.b, K.x, K.y), step(rgb.b, rgb.g));
    vec4 q = mix(vec4(p.x, p.y, p.w, rgb.r), vec4(rgb.r, p.y, p.z, p.x), step(p.x, rgb.r));

    double d = q.x - min(q.w, q.y);
    const double e = 1.0e-10;
    return clamp(vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x), 0.0, 1.0);
}

vec3 HSVToRGB(vec3 hsv)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(vec3(hsv.x) + vec3(K.x, K.y, K.z)) * 6.0 - vec3(K.w));
    return hsv.z * mix(vec3(K.x), clamp(p - vec3(K.x), 0.0, 1.0), hsv.y);
}

_COLOR_END
_CSE_END