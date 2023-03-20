#pragma once

#ifndef __STELLAR_COLOR__
#define __STELLAR_COLOR__

#include "CSE/Core/CSECore.h"
#include "CSE/Core/gltypes.h"

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

class _Color_RGB8 : public gl::gl_vec3<uint8_t>
{
public:
	using _Mybase = gl::gl_vec3<uint8_t>;

	_Color_RGB8() : _Mybase() {}
	_Color_RGB8(unsigned);
	_Color_RGB8(uint8_t _R, uint8_t _G, uint8_t _B) : _Mybase(_R, _G, _B) {}

	operator _Mybase() { return _Mybase(r, g, b); }

	vec3 norm()const{return vec3(r, g, b) / 255.;}
};

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif
