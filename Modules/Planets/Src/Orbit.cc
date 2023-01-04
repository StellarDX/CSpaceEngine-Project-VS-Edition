#include "CSE/Planets/Orbit.h"

_CSE_BEGIN

// StellarDL: I don't like having an all-nighter to code either, but he just keeping pilling on
// the features for this library, don't he? That's how things go here, so obviously I have to help him out... 
// Wanna keep me company?

void BinaryOrbitToKeplerianElems(OrbitParam* Par)
{
	Par->PericenterDist = Par->Separation;
	Par->Separation = NO_DATA_FLOAT_INF; // Move Separation to PericenterDist, and clear it.
	if (!isinf(Par->PositionAngle))
	{
		Par->ArgOfPericenter = Par->PositionAngle;
		Par->PositionAngle = NO_DATA_FLOAT_INF;
	}
	Par->Binary = false;
}

// Roche limits and Hill spheres

float64 RocheLimit(const Object* Primary, const Object* Companion, int Mode)
{
	switch (Mode)
	{
	case 0:
	default:
		if (isinf(Companion->Radius()) || isinf(Primary->Mass) || isinf(Companion->Mass))
		{
			throw OrbitCalculateException("too few arguments to function \"RocheLimit(Object*, Object*, int)\"");
		}

		if (isinf(Primary->Radius()))
		{
			float64 MaxRadius = max({ Companion->Dimensions.x, Companion->Dimensions.y, Companion->Dimensions.z }) / 2.;
			return MaxRadius * cbrt(2. * (Primary->Mass / Companion->Mass));
		}

		else
		{
			float64 MaxRadius = max({ Primary->Dimensions.x, Primary->Dimensions.y, Primary->Dimensions.z }) / 2.;
			vec3 AxisP = Primary->Dimensions / 2.;
			vec3 AxisC = Companion->Dimensions / 2.;
			float64 DensityP = Primary->Mass / ((4. / 3.) * CSE_PI * AxisP.x * AxisP.y * AxisP.z);
			float64 DensityC = Companion->Mass / ((4. / 3.) * CSE_PI * AxisC.x * AxisC.y * AxisC.z);
			return MaxRadius * cbrt(2. * (DensityP / DensityC));
		}

	case 1:
		if (isinf(Primary->Radius()) || isinf(Companion->Radius()) || isinf(Primary->Mass) || isinf(Companion->Mass))
		{
			throw OrbitCalculateException("too few arguments to function \"RocheLimit(Object*, Object*, int)\"");
		}

		float64 MaxRadius = max({ Primary->Dimensions.x, Primary->Dimensions.y, Primary->Dimensions.z }) / 2.;
		vec3 AxisP = Primary->Dimensions / 2.;
		vec3 AxisC = Companion->Dimensions / 2.;
		float64 DensityP = Primary->Mass / ((4. / 3.) * CSE_PI * AxisP.x * AxisP.y * AxisP.z);
		float64 DensityC = Companion->Mass / ((4. / 3.) * CSE_PI * AxisC.x * AxisC.y * AxisC.z);
		float64 PFlattening = (max(AxisC.x, AxisC.z) - AxisC.y) / max(AxisC.x, AxisC.z);
		return 2.243 * MaxRadius * cbrt(DensityP / DensityC) *
			cbrt(((1. + Companion->Mass / (3. * Primary->Mass)) + (1. / 3.) * PFlattening * (1. + Companion->Mass / Primary->Mass)) / (1. - PFlattening));
	}
}

float64 HillSphere(const Object* Primary, const Object* Companion)
{
	if (isinf(Primary->Mass) || isinf(Companion->Mass) || (isinf(Companion->Orbit.Separation) && isinf(Companion->Orbit.PericenterDist)))
	{
		throw OrbitCalculateException("too few arguments to function \"HillSphere(Object*, Object*)\"");
	}
	float64 Eccentricity, PericenterDist;
	if (isinf(Companion->Orbit.Eccentricity)) { Eccentricity = 0; }
	else { Eccentricity = Companion->Orbit.Eccentricity; }
	if (!isinf(Companion->Orbit.PericenterDist)) { PericenterDist = Companion->Orbit.PericenterDist; }
	else if (!isinf(Companion->Orbit.Separation)) { PericenterDist = Companion->Orbit.Separation * (1. - Eccentricity); }
	else { throw OrbitCalculateException("number of arguments to function \"HillSphere(Object*, Object*)\" is not match?"); }
	return PericenterDist * cbrt(Companion->Mass / (3. * Primary->Mass));
}

// Make association between 2 objects

void Addvalue(float64& Dst, float64& Src)
{
	if (Dst == NO_DATA_FLOAT_INF) { Dst = Src; }
}

void Addvalue(_STD string& Dst, _STD string& Src)
{
	if (Dst == NO_DATA_STRING) { Dst = Src; }
}

_Check_return_opt_ int __CRTDECL MakeOrbit(Object* Primary, Object* Companion, Object* ThirdGravSourse, OrbitParam Args)
{
	float64 Separation;
	if (isinf(Args.Separation) && isinf(Args.PericenterDist))
	{
		// if radiuses of the two objects are missing, throw exception.
		if (isinf(Primary->Radius())) { return 1; }
		if (isinf(Companion->Radius())) { return 2; }

		float64 PRadius = max({ Primary->Dimensions.x,  Primary->Dimensions.y, Primary->Dimensions.z });
		float64 CRadius = max({ Companion->Dimensions.x,  Companion->Dimensions.y, Companion->Dimensions.z });

		// Generate separation when diatances in arguments are missing.
		float64 MinDistance = RocheLimit(Primary, Companion, 1) + 2. * CRadius;
		float64 MaxDistance;
		if (nullptr == ThirdGravSourse){MaxDistance = LightYear / 100.;}
		else { MaxDistance = HillSphere(ThirdGravSourse, Primary) / 2; }
		if (MaxDistance < MinDistance) { return 3; }
		Separation = random.uniform(MinDistance, MaxDistance);
	}
	else
	{
		if (!isinf(Args.Separation) && isinf(Args.PericenterDist))
		{
			BinaryOrbitToKeplerianElems(&Args);
		}
		float64 MinDistance = RocheLimit(Primary, Companion, 1);
		float64 MaxDistance;
		if (nullptr == ThirdGravSourse) { MaxDistance = LightYear / 100.; }
		else { MaxDistance = HillSphere(ThirdGravSourse, Primary); }
		if (Args.PericenterDist < MinDistance || Args.PericenterDist > MaxDistance){return 3;}
		Separation = Args.PericenterDist;
	}

	Companion->ParentBody = Primary->Name[0];
	Addvalue(Companion->Orbit.AnalyticModel, Args.AnalyticModel);
	Addvalue(Companion->Orbit.RefPlane, Args.RefPlane);
	Addvalue(Companion->Orbit.Epoch, Args.Epoch);
	Addvalue(Companion->Orbit.Period, Args.Period);
	Companion->Orbit.PericenterDist = Separation;
	Addvalue(Companion->Orbit.GravParam, Args.GravParam);
	Addvalue(Companion->Orbit.Eccentricity, Args.Eccentricity);
	Addvalue(Companion->Orbit.Inclination, Args.Inclination);
	Addvalue(Companion->Orbit.AscendingNode, Args.AscendingNode);
	Addvalue(Companion->Orbit.AscNodePreces, Args.AscNodePreces);
	Addvalue(Companion->Orbit.ArgOfPericenter, Args.ArgOfPericenter);
	Addvalue(Companion->Orbit.ArgOfPeriPreces, Args.ArgOfPeriPreces);
	Addvalue(Companion->Orbit.MeanAnomaly, Args.MeanAnomaly);

	return 0;
}

_Check_return_ _STD shared_ptr<Object> __CRTDECL MakeBinary(Object* Primary, Object* Companion, Object* ThirdGravSourse, OrbitParam Args)
{
	float64 Separation;
	if (isinf(Args.Separation) && isinf(Args.PericenterDist))
	{
		// if radiuses of the two objects are missing, throw exception.
		if (isinf(Primary->Radius())) { return nullptr; }
		if (isinf(Companion->Radius())) { return nullptr; }

		float64 PRadius = max({ Primary->Dimensions.x,  Primary->Dimensions.y, Primary->Dimensions.z });
		float64 CRadius = max({ Companion->Dimensions.x,  Companion->Dimensions.y, Companion->Dimensions.z });

		// Generate separation when diatances in arguments are missing.
		float64 MinDistance = RocheLimit(Primary, Companion, 1) + 2. * CRadius;
		float64 MaxDistance;
		if (nullptr == ThirdGravSourse) { MaxDistance = LightYear / 100.; }
		else { MaxDistance = HillSphere(ThirdGravSourse, Primary) / 2; }
		if (MaxDistance < MinDistance) { return nullptr; }
		Separation = random.uniform(MinDistance, MaxDistance);
	}
	else
	{
		if (!isinf(Args.Separation) && isinf(Args.PericenterDist))
		{
			BinaryOrbitToKeplerianElems(&Args);
		}
		float64 MinDistance = RocheLimit(Primary, Companion, 1);
		float64 MaxDistance;
		if (nullptr == ThirdGravSourse) { MaxDistance = LightYear / 100.; }
		else { MaxDistance = HillSphere(ThirdGravSourse, Primary); }
		if (Args.PericenterDist < MinDistance || Args.PericenterDist > MaxDistance) { return nullptr; }
		Separation = Args.PericenterDist;
	}

	Object Barycenter =
	{
		.Type = "Barycenter",
		.Name = {Primary->Name[0] + '-' + Companion->Name[0]},

		.Orbit = Primary->Orbit
	};

	if (Primary->ParentBody != NO_DATA_STRING)
	{
		Barycenter.ParentBody = Primary->ParentBody;
	}

	Primary->ParentBody = Barycenter.Name[0];
	Companion->ParentBody = Barycenter.Name[0];

	//Addvalue(Companion->Orbit.AnalyticModel, Args.AnalyticModel);
	Addvalue(Companion->Orbit.RefPlane, Args.RefPlane);
	Addvalue(Companion->Orbit.Epoch, Args.Epoch);
	Addvalue(Companion->Orbit.Period, Args.Period);
	Companion->Orbit.PericenterDist = Separation;
	Addvalue(Companion->Orbit.GravParam, Args.GravParam);
	Addvalue(Companion->Orbit.Eccentricity, Args.Eccentricity);
	Addvalue(Companion->Orbit.Inclination, Args.Inclination);
	Addvalue(Companion->Orbit.AscendingNode, Args.AscendingNode);
	//Addvalue(Companion->Orbit.AscNodePreces, Args.AscNodePreces);
	Addvalue(Companion->Orbit.ArgOfPericenter, Args.ArgOfPericenter);
	//Addvalue(Companion->Orbit.ArgOfPeriPreces, Args.ArgOfPeriPreces);
	Addvalue(Companion->Orbit.MeanAnomaly, Args.MeanAnomaly);

	if (isinf(Companion->Orbit.ArgOfPericenter)) { Companion->Orbit.ArgOfPericenter = 0; }
	if (isinf(Companion->Orbit.MeanAnomaly)) { Companion->Orbit.MeanAnomaly = 0; }
	Primary->Orbit = Companion->Orbit;
	if (Primary->Orbit.ArgOfPericenter >= 180) { Primary->Orbit.ArgOfPericenter -= 180; }
	else { Primary->Orbit.ArgOfPericenter += 180; }

	float64 Eccentricity = 0;
	if (!isinf(Companion->Orbit.Eccentricity)) { Eccentricity = Companion->Orbit.Eccentricity; }
	float64 SemiMajorAxis = Companion->Orbit.PericenterDist / (1. - Eccentricity);
	float64 PrimarySMA = SemiMajorAxis * (Companion->Mass / (Primary->Mass + Companion->Mass));
	float64 CompanionSMA = SemiMajorAxis - PrimarySMA;
	Primary->Orbit.PericenterDist = PrimarySMA - PrimarySMA * Eccentricity;
	Companion->Orbit.PericenterDist = CompanionSMA - CompanionSMA * Eccentricity;

	return _STD make_shared<Object>(Barycenter);
}

_CSE_END
