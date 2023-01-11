#include "CSE/Planets/Rotation.h"

_CSE_BEGIN

void Oblate(Object* Obj)
{
	if (isinf(Obj->Mass || Obj->Radius() || Obj->Rotation.RotationPeriod))
	{
		throw RotationCalculateException("too few arguments to function \"Oblate(Object*)\"");
	}
	if (Obj->Dimensions.x != Obj->Dimensions.y || Obj->Dimensions.y != Obj->Dimensions.z)
	{
		throw RotationCalculateException("Failed to oblate bracuse object is not a sphere.");
	}

	float64 AngularFreq = (2. * CSE_PI) / Obj->Rotation.RotationPeriod;
	if (AngularFreq > sqrt((GravConstant * Obj->Mass) / pow(Obj->Radius(), 3)))
	{
		throw RotationCalculateException("Spinning of this object is too fast.");
	}
	float64 Flattening = (pow(Obj->Radius(), 3) * pow(AngularFreq, 2)) / (GravConstant * Obj->Mass);
	Obj->Dimensions.y = (Obj->Radius() - Obj->Radius() * Flattening) * 2.;
}

_CSE_END