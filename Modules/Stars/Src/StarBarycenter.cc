#include "CSE/Stars/StarBarycenter.h"

_CSE_BEGIN

Location::operator vec3()
{
	return vec3(RA, Dec, Dist);
}

_CSE_END
