#include "CSE/StellarColor.h"

_CSE_BEGIN

_Color_RGB8::_Color_RGB8(unsigned _Hex)
{
	unsigned RedMask   = 0xFF0000U;
	unsigned GreenMask = 0x00FF00U;
	unsigned BlueMask  = 0x0000FFU;

	r = (_Hex & RedMask) >> 16;
	g = (_Hex & GreenMask) >> 8;
	b = (_Hex & BlueMask);
}

_CSE_END